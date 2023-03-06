#include "Server.h"

Server::Server(boost::asio::io_context& io_context, short port, Mode mode)
        : _acceptor(io_context, tcp::endpoint(tcp::v4(), port)), _mode(mode) {
    _cmd_handler = std::make_shared<CmdHandler>();
    do_accept();
}

void Server::do_accept() {
    _acceptor.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                std::make_shared<Session>(std::move(socket), _cmd_handler, _mode)->start();
            }

            do_accept();
        });
}
