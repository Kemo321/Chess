#pragma once
#include <utility>


class ChessMove
{
    std::pair<int, int> from;
    std::pair<int, int> to;
    int promotion;

public:
    ChessMove();
    ChessMove(int fromRow, int fromCol, int toRow, int toCol, int promotion=0);
    ChessMove(const ChessMove& other);
    ChessMove& operator=(const ChessMove& other);
    ~ChessMove();

    // Getters
    std::pair<int, int> getFrom() const;
    std::pair<int, int> getTo() const;
    int getPromotion() const;

    // Setters
    void setFrom(int from);
    void setTo(int to);
    void setPromotion(int promotion);

    bool operator==(const ChessMove& other) const;
    bool operator!=(const ChessMove& other) const;
};