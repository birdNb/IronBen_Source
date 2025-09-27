#include <iostream>
#include <thread>
#include <atomic>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <opencv2/opencv.hpp>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
using tcp = asio::ip::tcp;

// 全局变量
std::atomic<int> current_Y{0};
std::atomic<int> current_W{0};
std::atomic<int> current_P{0};
std::atomic<int> current_T{0};
std::atomic<int> current_G{0};

// 串口发送线程
void serial_sender(asio::serial_port& serial) {
    std::string last_sent;
    while (true) {
        try {
            int move_Y = current_Y.load();
            int move_Z = current_W.load();
            int pose = current_P.load() % 256;
            bool T = current_T.load();
            int G_flg = current_G.load() % 256;

            // 组合指令
            std::string command = "X0Y" + std::to_string(move_Y) +
                                  "W" + std::to_string(move_Z) +
                                  "P" + std::to_string(pose) +
                                  "T" + std::to_string(T) +
                                  "G" + std::to_string(G_flg);

            // 发送指令
            if (command != last_sent) {
                asio::write(serial, asio::buffer(command));
                last_sent = command;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        } catch (const std::exception& e) {
            std::cerr << "Serial error: " << e.what() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

// HTTP 请求处理
void handle_request(http::request<http::string_body>& req, http::response<http::string_body>& res) {
    if (req.method() == http::verb::post && req.target() == "/command") {
        // 解析 JSON 数据（简化处理）
        std::string body = req.body();
        // 这里可以使用 JSON 库（如 nlohmann/json）来解析 JSON
        // 假设 body 是 "Y=10&W=20&P=30&T=1&G=0"
        // 这里简化处理
        current_Y = 10;
        current_W = 20;
        current_P = 30;
        current_T = 1;
        current_G = 0;

        res.result(http::status::ok);
        res.body() = R"({"status": "success"})";
        res.set(http::field::content_type, "application/json");
    } else {
        res.result(http::status::not_found);
        res.body() = "Not Found";
        res.set(http::field::content_type, "text/plain");
    }
    res.prepare_payload();
}

// HTTP 服务器
void http_server(tcp::acceptor& acceptor, asio::io_context& io_context) {
    auto socket = std::make_shared<tcp::socket>(io_context);
    acceptor.async_accept(*socket, [&](boost::system::error_code ec) {
        if (!ec) {
            auto req = std::make_shared<http::request<http::string_body>>();
            auto buffer = std::make_shared<beast::flat_buffer>();  // 添加 flat_buffer

            http::async_read(*socket, *buffer, *req, [req, buffer, socket](boost::system::error_code ec, size_t) {
                if (!ec) {
                    auto res = std::make_shared<http::response<http::string_body>>();
                    handle_request(*req, *res);

                    http::async_write(*socket, *res, [socket](boost::system::error_code ec, size_t) {
                        if (!ec) {
                            socket->shutdown(tcp::socket::shutdown_send);
                        }
                    });
                }
            });
        }
        http_server(acceptor, io_context);
    });
}

int main() {
    try {
        asio::io_context io_context;

        // 串口配置
        asio::serial_port serial(io_context, "/dev/ttyTHS1");
        serial.set_option(asio::serial_port_base::baud_rate(115200));

        // 启动串口发送线程
        std::thread serial_thread(serial_sender, std::ref(serial));

        // HTTP 服务器配置
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 7500));
        http_server(acceptor, io_context);

        // 运行 IO 上下文
        io_context.run();

        serial_thread.join();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
