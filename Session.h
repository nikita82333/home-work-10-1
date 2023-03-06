#ifndef SESSION_H
#define SESSION_H

#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

#include "CmdHandler.h"

enum class Mode {
    Line,
    Symbolic
};

using boost::asio::ip::tcp;

/// <summary>
/// Class Session - establishes a network connection.
/// </summary>
/// <param name="socket">Network socket.</param>
/// <param name="sender">Shared pointer to CmdHandler.</param>
/// <param name="mode">Server operation mode (line or symbolic).</param>
class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket, std::shared_ptr<CmdHandler> cmd_handler, Mode mode)
            : _socket(std::move(socket)), _cmd_handler(std::move(cmd_handler)), _mode(mode) {}

    void start();

private:
    void do_read();
    void do_write(const char* data, std::size_t length);

    tcp::socket _socket;
    enum { max_length = 1024 };
    char _data[max_length];
    std::string _command;

    std::shared_ptr<CmdHandler> _cmd_handler;
    Mode _mode;

};


#endif //SESSION_H
