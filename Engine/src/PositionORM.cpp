#include "PositionORM.hpp"
#include <iostream>

PositionORM::PositionORM(const std::string& dbPath) {
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Failed to open database: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
    } else {
        // Create the table if it doesn't exist.
        if (!initialize()) {
            std::cerr << "Failed to initialize the database." << std::endl;
        }
    }
}

PositionORM::~PositionORM() {
    if (db)
        sqlite3_close(db);
}

bool PositionORM::initialize() {
    // SQL command to create the POSITION table if it doesn't exist
    std::string sql = "CREATE TABLE IF NOT EXISTS POSITION ("
                      "NAME CHAR(71) NOT NULL PRIMARY KEY, "
                      "BEST_MOVE CHAR(5) NOT NULL, "
                      "SCORE INT NOT NULL"
                      ");";
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error during table creation: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool PositionORM::insertPosition(const Position& pos) {
    std::string sql = "INSERT INTO POSITION (NAME, BEST_MOVE, SCORE) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare insert statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, pos.fen.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, pos.best_move.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, pos.score);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    if (!success) {
        std::cerr << "Failed to insert position: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);
    return success;
}

Position PositionORM::getPosition(const std::string& name) {
    std::string sql = "SELECT BEST_MOVE, SCORE FROM POSITION WHERE NAME = ?;";
    sqlite3_stmt* stmt;
    Position pos{name, "", 0.0f};

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* bestMove = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            double score = sqlite3_column_double(stmt, 1);
            if (bestMove) {
                pos.best_move = std::string(bestMove);
            }
            pos.score = static_cast<float>(score);
        }
    } else {
        std::cerr << "Failed to prepare getPosition statement: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);
    return pos;
}

bool PositionORM::updatePosition(const Position& pos) {
    std::string sql = "UPDATE POSITION SET BEST_MOVE = ?, SCORE = ? WHERE NAME = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, pos.best_move.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 2, pos.score);
    sqlite3_bind_text(stmt, 3, pos.fen.c_str(), -1, SQLITE_STATIC);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

bool PositionORM::deletePosition(const std::string& name) {
    std::string sql = "DELETE FROM POSITION WHERE NAME = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}
