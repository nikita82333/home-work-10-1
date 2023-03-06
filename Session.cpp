#include "Session.h"

void Session::start() {
    do_read();
}

void Session::do_read() {
    auto self(shared_from_this());
    _socket.async_read_some(boost::asio::buffer(_data, max_length),
        [this, self](boost::system::error_code ec, std::size_t length) {
            if (!ec) {
                if (_mode == Mode::Symbolic) {
                    if (_data[length - 1] != '\n') {
                        _command += std::string(_data, length);
                        do_read();
                    } else {
                        if (_command.length() > 0) {
                            std::string response {_cmd_handler->execute(_command) + std::string(NEW_LINE)};
                            _command.clear();
                            do_write(response.c_str(), response.length());
                            //do_read();//asynchronous sending of response
                        }
                    }
                } else if (_mode == Mode::Line) {
                    std::string response {_cmd_handler->execute(std::string(_data, length)) + std::string(NEW_LINE)};
                    do_write(response.c_str(), response.length());
                    //do_read();//asynchronous sending of response
                }
            }
        });
}

void Session::do_write(const char* data, std::size_t length) {
    auto self(shared_from_this());
    boost::asio::async_write(_socket, boost::asio::buffer(data, length),
        [this, self](boost::system::error_code ec, std::size_t /*length*/) {
            if (!ec) {
                do_read();//synchronous sending of response
            }
        });
}
