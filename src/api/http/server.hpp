#ifndef SERVER_HPP
#define SERVER_HPP

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>
#include <string>
#include <regex>
#include <chrono>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

class HttpServer;

class HttpSession : public std::enable_shared_from_this<HttpSession> {
public:
    HttpSession(tcp::socket socket, HttpServer& server);
    void start();

private:
    tcp::socket socket_;
    beast::flat_buffer buffer_;
    http::request<http::dynamic_body> req_;
    http::response<http::dynamic_body> responce_;
    net::steady_timer deadline_{
        socket_.get_executor(), std::chrono::seconds(300)
    };
    HttpServer& server_;

    void do_read();
    void handle_request();
    void do_write();
    void check_deadline();
};


class HttpServer {
    friend class HttpSession;
public:
    HttpServer(std::string address, unsigned short port);
    void start();
    void stop();
    std::string get_code();
    

private:
    net::io_context ioc_;
    tcp::socket socket_;
    tcp::endpoint endpoint_;
    tcp::acceptor acceptor_;
    std::string code_;

    void do_accept();
};

#endif // SERVER_HPP