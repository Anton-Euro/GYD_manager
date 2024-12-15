#include "client.hpp"

using namespace std;

HttpResponse HttpClient::get(const string& url, const map<string, string>& headers) {
    return request(http::verb::get, url, headers, "");
}

HttpResponse HttpClient::post(const string& url, const map<string, string>& headers, const string& body) {
    return request(http::verb::post, url, headers, body);
}

HttpResponse HttpClient::put(const string& url, const map<string, string>& headers, const string& body) {
    return request(http::verb::put, url, headers, body);
}

HttpResponse HttpClient::patch(const string& url, const map<string, string>& headers, const string& body) {
    return request(http::verb::patch, url, headers, body);
}

HttpResponse HttpClient::delete_(const string& url, const map<string, string>& headers) {
    return request(http::verb::delete_, url, headers, "");
}

HttpResponse HttpClient::request(http::verb method, const string& url, const map<string, string>& headers, const string& body) {
    try {
        std::string host, target;
        int port = 443;
        parse_url(url, host, target, port);

        ssl::stream<beast::tcp_stream> ssl_stream_(ioc_, ctx_);
        
        auto const results = resolver_.resolve(host, to_string(port));
        beast::get_lowest_layer(ssl_stream_).connect(results);

        ssl_stream_.handshake(ssl::stream_base::client);

        http::request<http::string_body> req{method, target, 11};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        for (const auto& header : headers) {
            req.set(header.first, header.second);
        }
        if (method == http::verb::post || method == http::verb::put || method == http::verb::patch) {
            req.body() = body;
            req.prepare_payload();
        }

        http::write(ssl_stream_, req);

        beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(ssl_stream_, buffer, res);

        if(res.result() == http::status::found || res.result() == http::status::moved_permanently) {
            std::string new_url = res.base()["Location"];
            return request(method, new_url, headers, body);
        }

        beast::error_code ec;
        ssl_stream_.shutdown(ec);

        map<string, string> response_headers;
        for (const auto& field : res.base()) {
            response_headers[field.name_string()] = field.value();
        }

        return {res.body(), response_headers};
    }
    catch (const std::exception& e) {
        cerr << "Error: " << e.what() << std::endl;
        return {"Error", {}};
    }
}

void HttpClient::parse_url(const string& url, string& host, string& target, int& port) {
    const std::string protocol = "https://";
    if (url.substr(0, protocol.size()) != protocol) {
        throw std::invalid_argument("URL must start with https://");
    }

    auto pos = url.find('/', protocol.size());
    if (pos == std::string::npos) {
        host = url.substr(protocol.size());
        target = "/";
    } else {
        host = url.substr(protocol.size(), pos - protocol.size());
        target = url.substr(pos);
    }

    pos = host.find(':');
    if (pos != std::string::npos) {
        port = std::stoi(host.substr(pos + 1));
        host = host.substr(0, pos);
    }
}
