#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>
#include <utility>
#include <cstdlib>
#include <iostream>
#include <string>
#include <map>
#include "root_certificates.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = net::ssl;
using tcp = net::ip::tcp;

using HttpResponse = std::pair<std::string, std::map<std::string, std::string>>;

class HttpClient {
public:
    HttpClient(): ctx_(ssl::context::tlsv12_client), resolver_(ioc_) {
        ctx_.set_default_verify_paths();
    }

    HttpResponse get(const std::string& url, const std::map<std::string, std::string>& headers = {});
    HttpResponse post(const std::string& url, const std::map<std::string, std::string>& headers = {}, const std::string& body = {});
    HttpResponse put(const std::string& url, const std::map<std::string, std::string>& headers = {}, const std::string& body = {});
    HttpResponse patch(const std::string& url, const std::map<std::string, std::string>& headers = {}, const std::string& body = {});
    HttpResponse delete_(const std::string& url, const std::map<std::string, std::string>& headers = {});

private:
    net::io_context ioc_;
    net::ssl::context ctx_;
    tcp::resolver resolver_;

    HttpResponse request(http::verb method, const std::string& url, const std::map<std::string, std::string>& headers, const std::string& body);
    void parse_url(const std::string& url, std::string& host, std::string& target, int& port);
};

#endif // CLIENT_HPP