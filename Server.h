#ifndef SERVER_H
#define SERVER_H

#include "Session.h"

/// <summary>
/// Class Server - asynchronous server (by boost).
/// </summary>
/// <param name="io_context">Provides the core I/O functionality.</param>
/// <param name="port">Network port.</param>
/// <param name="mode">Server operation mode (line or symbolic).</param>
class Server {
public:
    Server(boost::asio::io_context& io_context, short port, Mode mode);

private:
    void do_accept();

    tcp::acceptor _acceptor;
    std::shared_ptr<CmdHandler> _cmd_handler;
    Mode _mode;

};


#endif //SERVER_H
