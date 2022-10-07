#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <util/trace.hpp> // debug remove me

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

namespace util
{
namespace http
{

namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http  = beast::http;  // from <boost/beast/http.hpp>
namespace net   = boost::asio;  // from <boost/asio.hpp>
namespace ssl   = net::ssl;     // from <boost/asio/ssl.hpp>
using tcp       = net::ip::tcp; // from <boost/asio/ip/tcp.hpp>

// Performs an HTTP GET and prints the response
int get(const char* host, const char* port, const char* target)
{
    try
    {
        int version = 11;

        // The io_context is required for all I/O
        net::io_context ioc;

        // The SSL context is required, and holds certificates
        ssl::context ctx(ssl::context::tlsv12_client);

        // This holds the root certificate used for verification
        // load_root_certificates(ctx);

        // Verify the remote server's certificate
        ctx.set_verify_mode(ssl::verify_none);
        // ctx.set_verify_mode(ssl::context::verify_peer);
        // boost::certify::enable_native_https_server_verification(ctx);

        // These objects perform our I/O
        tcp::resolver resolver(ioc);
        beast::ssl_stream<beast::tcp_stream> stream(ioc, ctx);

        // Set SNI Hostname (many hosts need this to handshake successfully)
        if (!SSL_set_tlsext_host_name(stream.native_handle(), host))
        {
            beast::error_code ec{static_cast<int>(::ERR_get_error()),
                                 net::error::get_ssl_category()};
            throw beast::system_error{ec};
        }

        // Look up the domain name
        auto const results = resolver.resolve(host, port);

        // Make the connection on the IP address we get from a lookup
        beast::get_lowest_layer(stream).connect(results);

        // Perform the SSL handshake
        stream.handshake(ssl::stream_base::client);

        // Set up an HTTP GET request message
        http::request<http::string_body> req{http::verb::get, target, version};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        // Send the HTTP request to the remote host
        http::write(stream, req);

        // This buffer is used for reading and must be persisted
        beast::flat_buffer buffer;

        // Declare a container to hold the response
        // http::response<http::dynamic_body> res;
        http::response<http::string_body> res;

        // Receive the HTTP response
        http::read(stream, buffer, res);

        // Write the message to standard out
        // for (auto const& field : res)
        //    std::cout << field.name() << " = " << field.value() << "\n";
        // std::cout << "Server: " << res[http::field::server];

        // std::cout << res << std::endl;
        std::stringstream ss;
        ss << res.body() << std::endl;
        trace::err(ss.str().c_str());

        // Gracefully close the stream
        beast::error_code ec;
        stream.shutdown(ec);
        if (ec == net::error::eof)
        {
            // Rationale:
            // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
            ec = {};
        }
        if (ec)
        {
            // could check for stream truncated too, and ignore (?)
            // or not close connection at all (?)
            // throw beast::system_error{ec};
        }

        // If we get here then the connection is closed gracefully
        // or we ignored some errors :)
    }
    catch (std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        std::stringstream ss;
        ss << e.what() << std::endl;
        trace::err(ss.str().c_str());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

} // namespace http
} // namespace util
