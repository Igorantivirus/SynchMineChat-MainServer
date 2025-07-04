#include "Subs/ClientBrocker.hpp"

#include "Subs/Subscribers/TgBotSubscriber.hpp"
#include "Subs/Subscribers/CrowSubscriber.hpp"

int main()
{
    Service::staticConstruct("resources\\ServerConfig.json");

    ClientBrocker brocker;
    TgBotSubscriber tg{brocker};
    CrowSubscriber crow{brocker};

    crow.asynchRun();
    tg.runNoAsynch();

    return 0;
}


// #include <iostream>
// #include <string>
// #include <set>

// #include "Services/Service.hpp"

// #include <nlohmann/json.hpp>

// // #include "Subs/ClientBrocker.hpp"

// // #include "Services/Configs/TgBotConfig.hpp"

// // #include "StringUtility.hpp"

// // #include "Subs/Subscribers/TgBotSubscriber.hpp"

// // #include "AsynchContainers/AsynchVector.hpp"

// #include "crow.h"
// // #include "crow_"

// // namespace test
// // {
// //     crow::SimpleApp app;
// //     std::mutex mtx;
// //     std::map<crow::websocket::connection *, nlohmann::json> connected_clients;

// //     void onOpen(crow::websocket::connection &conn)
// //     {
// //         Service::log.log("New client linked", crow::LogLevel::Info);
// //     }

// //     void onMessage(crow::websocket::connection &conn, const std::string &data, bool is_binary)
// //     {
// //         std::lock_guard<std::mutex> _(mtx);

// //         Service::log.log("Client message. Is binary: " + std::string(is_binary ? "Yes" : "No") + "\nMessage: " + data, crow::LogLevel::Info);
// //         conn.send_text("Message");
// //     }

// //     void onClose(crow::websocket::connection &conn, const std::string &reason)
// //     {
// //         std::lock_guard<std::mutex> _(mtx);
// //         Service::log.log("Client deleted by reason: " + reason, crow::LogLevel::Info);
// //     }

// //     void init()
// //     {
// //         CROW_WEBSOCKET_ROUTE(app, "/ws")
// //         .onopen([&](crow::websocket::connection &conn)
// //         {
// //             Service::log.log("New client linked", crow::LogLevel::Info);
// //         })
// //         .onclose([&](crow::websocket::connection &conn, const std::string &reason)
// //         {
// //             std::lock_guard<std::mutex> _(mtx);
// //             Service::log.log("Client deleted by reason: " + reason, crow::LogLevel::Info);
// //         })
// //         .onmessage([&](crow::websocket::connection &conn, const std::string &data, bool is_binary)
// //         {
// //             std::lock_guard<std::mutex> _(mtx);

// //             Service::log.log("Client message. Is binary: " + std::string(is_binary ? "Yes" : "No") + "\nMessage: " + data, crow::LogLevel::Info);
// //             conn.send_text("Message");
// //         });

// //         // CROW_WEBSOCKET_ROUTE(app, "/ws")
// //         // .onopen(&test::onOpen)
// //         // .onclose(&test::onClose)
// //         // .onmessage(&test::onMessage);

// //         // // Пример маршрута для отправки сообщений конкретному клиенту
// //         // CROW_ROUTE(app, "/send_to_user/<int>")
// //         //     .methods("POST"_method)([&](const crow::request &req, int user_id)
// //         //                             {
// //         //     auto message = req.url_params.get("message");
// //         //     if (!message) return crow::response(400, "Message is required");
// //         //     std::lock_guard<std::mutex> _(mtx);
// //         //     bool found = false;
// //         //     for (auto& [conn, data] : connected_clients) {
// //         //         if (data["user_id"] == user_id) {
// //         //             json msg = {
// //         //                 {"from", "server"},
// //         //                 {"text", message}
// //         //             };
// //         //             conn->send_text(msg.dump());
// //         //             found = true;
// //         //         }
// //         //     }
// //         //     return found ? crow::response("Message sent")
// //         //                 : crow::response(404, "User not connected"); });

// //         app.port(8080).multithreaded().run();
// //     }

// // }

// void onOpen(crow::websocket::connection &conn)
// {
//     std::cout << "Connect\n";
// }

// class Server
// {
// public:
//     Server()
//     {
//         init();
//     }

//     void run()
//     {
//         app.port(18080).multithreaded().run();
//     }

// private:
//     crow::SimpleApp app;

// private:
//     void init()
//     {
//         CROW_WEBSOCKET_ROUTE(app, "/ws")
//             .onopen([this](crow::websocket::connection &conn)
//                     { this->onOpen(conn); })
//             .onclose([this](crow::websocket::connection &conn, const std::string &reason, uint16_t)
//                      { this->onClose(conn, reason, 0); })
//             .onmessage([this](crow::websocket::connection &conn, const std::string &data, bool is_binary)
//                        { this->onMessage(conn, data, is_binary); });

//         CROW_ROUTE(app, "/")([](){ return "Hello, Crow!"; });
//     }

//     void onOpen(crow::websocket::connection &conn)
//     {
//         std::cout << "Connect\n";
//         // conn.close();
//     }

//     void onClose(crow::websocket::connection &conn, const std::string &reason, uint16_t)
//     {
//         std::cout << "On close\n";
//     }

//     void onMessage(crow::websocket::connection &conn, const std::string &data, bool is_binary)
//     {
//         if (is_binary)
//             std::cout << "Bin\n";
//         else
//             std::cout << "No bin\n";
//         conn.send_text("Hello, my friend!");
//     }
// };

// #include "AsynchContainers/AsynchMap.hpp"

// int main()
// {
//     Service::staticConstruct("resources\\ServerConfig.json");

//     AsynchSafelyMap<int, int> map;
//     map[1] = 2;
//     std::cout << map[1] << ' ' << map.safelyGet(2, 0) << '\n';

//     // Server s;
//     // s.run();

//     // crow::SimpleApp app;

//     // std::set<crow::websocket::connection *> users;
//     // std::mutex mtx; // Mutex для защиты доступа к users

//     // // WebSocket endpoint
//     // // CROW_ROUTE

//     // CROW_WEBSOCKET_ROUTE(app, "/ws")
//     //     .onopen(onOpen)
//     //     .onclose([&](crow::websocket::connection &conn, const std::string &reason, uint16_t)
//     //              { std::cout << "Close\n"; })
//     //     .onmessage([&](crow::websocket::connection & conn, const std::string &data, bool is_binary)
//     //                {
//     //             if (is_binary)
//     //                 std::cout << "Bin\n";
//     //             else
//     //                 std::cout << "No bin\n";
//     //             conn.send_text("Hello, my friend!");

//     //             });

//     // // Простое HTTP endpoint для проверки работы сервера
//     // CROW_ROUTE(app, "/")([]()
//     //                      { return "Hello, Crow!"; });

//     // // Запускаем сервер
//     // app.port(18080).multithreaded().run();

//     // test::init();

//     return 0;
// }