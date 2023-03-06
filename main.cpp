#include <iostream>

#include "Server.h"

int main(int argc, char* argv[]) {

    try {
        if (argc != 2 && argc != 3) {
            std::cerr << "Usage: join_server <port> [mode]" << std::endl;
            std::cerr << " - <port> - network port" << std::endl;
            std::cerr << " - [mode] - operation mode: line or symbolic (optional, symbolic by default)" << std::endl;

            return 1;
        }

        Mode mode = Mode::Symbolic;
        if (argc == 3 && std::string(argv[2]) == "line") {
            mode = Mode::Line;
        }

        boost::asio::io_context io_context;

        Server server(io_context, std::atoi(argv[1]), mode);

        io_context.run();
    } catch (const std::exception& exception) {
        std::cerr << "Exception: " << exception.what() << std::endl;
    }

    return 0;

}
