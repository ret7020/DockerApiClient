#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


//For debug
std::ostream& writeString(std::ostream& out, std::string const& s)
{
   for ( auto ch : s )
   {
      switch (ch)
      {
         case '\'':
            out << "\\'";
            break;

         case '\"':
            out << "\\\"";
            break;

         case '\?':
            out << "\\?";
            break;

         case '\\':
            out << "\\\\";
            break;

         case '\a':
            out << "\\a";
            break;

         case '\b':
            out << "\\b";
            break;

         case '\f':
            out << "\\f";
            break;

         case '\n':
            out << "\\n";
            break;

         case '\r':
            out << "\\r";
            break;

         case '\t':
            out << "\\t";
            break;

         case '\v':
            out << "\\v";
            break;

         default:
            out << ch;
      }
   }

   return out;
}


// Sends a WebSocket message and prints the response
int main(int argc, char** argv)
{
    try
    {
        // Check command line arguments.
        if(argc != 4)
        {
            std::cerr <<
                "Usage: websocket-client-sync <host> <port> <text>\n" <<
                "Example:\n" <<
                "    websocket-client-sync echo.websocket.org 80 \"Hello, world!\"\n";
            return EXIT_FAILURE;
        }
        auto const host = argv[1];
        auto const port = argv[2];
        // auto const text = argv[3];
        auto const text = "ls\r";

        // The io_context is required for all I/O
        net::io_context ioc;

        // These objects perform our I/O
        tcp::resolver resolver{ioc};
        websocket::stream<tcp::socket> ws{ioc};

        // Look up the domain name
        auto const results = resolver.resolve(host, port);

        // Make the connection on the IP address we get from a lookup
        net::connect(ws.next_layer(), results.begin(), results.end());

        // Set a decorator to change the User-Agent of the handshake
        ws.set_option(websocket::stream_base::decorator(
            [](websocket::request_type& req)
            {
                req.set(http::field::user_agent,
                    std::string(BOOST_BEAST_VERSION_STRING) +
                        " websocket-client-coro");
            }));

        // Perform the websocket handshake
        ws.handshake(host, "/containers/2088223769d2/attach/ws?stream=1&stdout=1&stdin=0&logs=0");

        // Send the message
        for (int i = 0; i < 1; ++i){
            ws.write(net::buffer(std::string("python3 /home/code/main.py <<< '1 2'; echo 'SHTP_EXECUTION_FINISH'\r")));
            
            int exit_sequences_find = 0;
            while (exit_sequences_find <= 1){
                beast::flat_buffer buffer;
                ws.read(buffer);
                std::string buffer_processed = beast::buffers_to_string(buffer.data());
                //std::cout << buffer_processed << std::endl;
                //writeString(std::cout, buffer_processed);
                std::cout << buffer_processed;
                std::cout << "\n";
                if (buffer_processed.find("SHTP_EXECUTION_FINISH") == std::string::npos) exit_sequences_find++;
            }
            std::cout << "--TEST--";
            ws.write(net::buffer(std::string("clear\n\r")));
        }
        ws.close(websocket::close_code::normal); // Finish socket
    }
    catch(std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}