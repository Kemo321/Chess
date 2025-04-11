#pragma once
#include <string>
#include <sqlite3.h>

struct Position {
    std::string fen;       // 71-character board position string
    std::string best_move; // 5-character best move string
    float score;           // Score as a float
};

class PositionORM {
public:
    PositionORM(const std::string& dbPath);
    ~PositionORM();

    bool insertPosition(const Position& pos);
    Position getPosition(const std::string& name);
    bool updatePosition(const Position& pos);
    bool deletePosition(const std::string& name);

private:
    sqlite3* db;
    bool initialize(); // Creates the table if it doesn't exist
};
