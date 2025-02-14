#include "ChessMove.hpp"


ChessMove::ChessMove() : from({0, 0}), to({0, 0}), promotion(0) {}

ChessMove::ChessMove(const ChessMove& other) : from(other.from), to(other.to), promotion(other.promotion) {}

ChessMove::ChessMove(int fromRow, int fromCol, int toRow, int toCol, int promotion) : from({fromRow, fromCol}), to({toRow, toCol}), promotion(promotion) {}

ChessMove& ChessMove::operator=(const ChessMove& other)
{
    from = other.from;
    to = other.to;
    promotion = other.promotion;
    return *this;
}

ChessMove::~ChessMove() {}

std::pair<int, int> ChessMove::getFrom() const
{
    return from;
}

std::pair<int, int> ChessMove::getTo() const
{
    return to;
}

int ChessMove::getPromotion() const
{
    return promotion;
}

void ChessMove::setFrom(std::pair<int, int> from)
{
    this->from = from;
}

void ChessMove::setTo(std::pair<int, int> to)
{
    this->to = to;
}

void ChessMove::setPromotion(int promotion)
{
    this->promotion = promotion;
}

bool ChessMove::operator==(const ChessMove& other) const
{
    return from == other.from && to == other.to && promotion == other.promotion;
}

bool ChessMove::operator!=(const ChessMove& other) const
{
    return !(*this == other);
}

void ChessMove::printMove() const
{
    std::cout << from.first << from.second << " -> " << to.first << to.second << "\n";
}

std::string ChessMove::toString() const
{
    return std::to_string(from.first) + std::to_string(from.second) + std::to_string(to.first) + std::to_string(to.second)
    + std::to_string(promotion);
}