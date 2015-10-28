#include "common.h"
#include "dns.hpp"

static void RunServer();
static void RunClient();

#define DEFAULT_PORT 13131

int main(int argc, char *argv[])
{
    std::cout << (char)(37) << " " << (char)(165) << " " << (char)(59) << (char)(66) << " " << (uint)'k' << std::endl;

    if (argc < 2) {
        std::cout << "Few arguments!" << std::endl;
        return -1;
    }

    std::string mode = argv[1];

    if (mode == "-client") {
        RunClient();
    }

    if (mode == "-server") {
        RunServer();
    }

    std::cout << "Unrecognized mode!" << std::endl;
    return -1;
}

static void RunClient()
{
    DnsQuerySender sender("192.168.30.201", DEFAULT_PORT);
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
    DnsQueryReceiver receiver(DEFAULT_PORT);

    while(true) {
        DnsQuery query = receiver.ReceiveQuery();
        std::cout << "Received" << std::endl;

        std::cout << query.HostName() << std::endl;
    }

}
