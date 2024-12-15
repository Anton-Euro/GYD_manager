#include "server.hpp"

HttpSession::HttpSession(tcp::socket socket, HttpServer& server): socket_(std::move(socket)), server_(server) {};

void HttpSession::start() {
    do_read();
    check_deadline();
}


void HttpSession::do_read() {
    auto self = shared_from_this();
    http::async_read(socket_, buffer_, req_,
        [self](beast::error_code ec, std::size_t bytes_transferred) {
            if (!ec) {
                self->handle_request();
                self->do_write();
            } else {
                std::cerr << "Read error: " << ec.message() << std::endl;
            }
        }
    );
}

void HttpSession::handle_request() {
    std::string target = std::string(req_.target());

    responce_.version(req_.version());
    responce_.keep_alive(false);
    responce_.result(http::status::ok);
    responce_.set(http::field::content_type, "text/plain");
    beast::ostream(responce_.body()) << "Congratulations! You have linked google drive to GYDManager.";

    std::regex query_regex(R"(code=([\w/-]+))");
    std::smatch match;
    if (std::regex_search(target, match, query_regex) && match.size() == 2) {
        std::string code = match[1].str();
        server_.code_ = code;
    } else {
        std::cout << "Invalid query parameters!" << std::endl;
    }
}

void HttpSession::do_write() {
    auto self = shared_from_this();
    
    responce_.content_length(responce_.body().size());

    http::async_write(socket_, responce_,
        [self](beast::error_code ec, std::size_t)
        {
            self->socket_.shutdown(tcp::socket::shutdown_send, ec);
            self->deadline_.cancel();
            if (ec) {
                std::cerr << "Shutdown error: " << ec.message() << std::endl;
            }
            
        }
    );
}

void HttpSession::check_deadline() {
    auto self = shared_from_this();

    deadline_.async_wait(
        [self](beast::error_code ec)
        {
            if(!ec) {
                self->socket_.close(ec);
            }
        });
}




HttpServer::HttpServer(std::string address, unsigned short port): ioc_(), socket_(ioc_), endpoint_(net::ip::make_address("127.0.0.1"), port), acceptor_(ioc_) {
    beast::error_code ec;

    acceptor_.open(endpoint_.protocol(), ec);
    if (ec) {
        throw "Open error";
    }

    acceptor_.bind(endpoint_, ec);
    if (ec) {
        throw "Bind error";
    }

    acceptor_.listen(net::socket_base::max_listen_connections, ec);
    if (ec) {
        throw "Listen error";
    }
    
    do_accept();
}

void HttpServer::start() {
    ioc_.run();
}

void HttpServer::stop() {
    ioc_.stop();
}

void HttpServer::do_accept() {
    acceptor_.async_accept(socket_,
        [this](beast::error_code ec) {
            if (!ec) {
                std::make_shared<HttpSession>(std::move(socket_), *this)->start();
            }
        }
    );
}

std::string HttpServer::get_code() {
    return code_;
}