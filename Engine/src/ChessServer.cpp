#include "ChessServer.hpp"



ChessServer::ChessServer() 
{
    // POST request to get the best move
    Post("/getBestMove", [&](const httplib::Request& req, httplib::Response& res) {
        std::cout << "Received request: " << req.body << std::endl;

        if (req.body.empty()) {
            res.status = 400;
            res.set_content("Bad Request: Empty input", "text/plain");
            return;
        }

        if (req.body.size() != 71) {
            res.status = 400;
            res.set_content("Bad Request: Invalid input", "text/plain");
            return;
        }

        try {
            std::string bestMove = engine.getBestMove(req.body, 5); // Compute before responding
            std::cout << "Best move: " << bestMove << std::endl;
            res.set_content(bestMove, "text/plain");
        } catch (const std::exception& e) {
            std::cerr << "Exception: " << e.what() << std::endl;
            res.status = 500;
            res.set_content("Internal Server Error", "text/plain");
        }
    });

    // Health check endpoint
    Get("/health", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("Server is running!", "text/plain");
    });
}

void ChessServer::start() 
{
    std::cout << "Server is running on port 8080..." << std::endl;
    listen("0.0.0.0", 8080);
}