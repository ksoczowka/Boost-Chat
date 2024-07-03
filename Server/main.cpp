#include "Server.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: Server.exe <port>\n";
            return 1;
        }
        boost::asio::io_context io_context;
        Server server = Server(io_context, std::atoi(argv[1]));
        io_context.run();

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << '\n';
    }

    return 0;
}