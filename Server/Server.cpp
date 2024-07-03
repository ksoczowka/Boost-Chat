#include "Server.hpp"

Server::Server(boost::asio::io_context& io_context, const short& port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {

    acceptConnection();
}

void Server::acceptConnection() {
    acceptor_.async_accept([&](boost::system::error_code ec, tcp::socket socket){
        if(!ec) {
            auto client = std::make_shared<tcp::socket>(std::move(socket));
            clients_.insert(client);
        }
        acceptConnection();
    });
}

void Server::readFromClient(std::shared_ptr<tcp::socket> client) {
    auto self(shared_from_this());
    boost::asio::async_read_until(*client, buffer_, '\n', [this, self, client](boost::system::error_code ec, std::size_t lenght){
        if(!ec) {
            std::string message;
            std::istream is(&buffer_);
            std::getline(is, message);
            for (auto& c : clients_) {
                if(c != client) {
                    writeMessage(c, message);
                }
                readFromClient(client);
            }
        } else {
            clients_.erase(client);
        }
    });
}

void Server::writeMessage(std::shared_ptr<tcp::socket> client, std::string message) {
    auto self(shared_from_this());
    auto msg = std::make_shared<std::string>(message + '\n');

    boost::asio::async_write(*client, boost::asio::buffer(*msg), [this, self, msg, client](boost::system::error_code ec, std::size_t lenght){
        if(ec) {
            clients_.erase(client);
        }
    });
}
