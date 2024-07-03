#pragma once

#include <boost/asio.hpp>

#include <memory>
#include <set>
#include <string>

class Server : std::enable_shared_from_this<Server> {
 public:
    using tcp = boost::asio::ip::tcp;

    Server(boost::asio::io_context& io_context, const short& port);

    void acceptConnection();

    void readFromClient(std::shared_ptr<tcp::socket> client);
    void writeMessage(std::shared_ptr<tcp::socket> client, std::string message);

 private:
    tcp::acceptor acceptor_;
    boost::asio::streambuf buffer_;
    std::set<std::shared_ptr<tcp::socket>> clients_;
};