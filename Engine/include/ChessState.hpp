#pragma once
#include <string>
#include <vector>
#include "ChessMove.hpp"


class ChessState {
public:
    explicit ChessState(const std::string& state);

    void makeMove(const ChessMove& move);
    void unmakeMove(const ChessMove& move);
    std::vector<ChessMove> getLegalMoves() const;
    bool isSquareAttacked(int row, int col, bool byWhite) const;
    void updateGameState();
    bool isCheckmate() const;
    bool isStalemate() const;
    bool isDraw() const;
    void printBoard() const;
    bool isTerminal() const;

    char getPieceAt(int row, int col) const;
    bool isInCheck(bool byWhite) const;

private:
    char state[8][8]{};
    bool whiteToMove{};
    bool whiteKingMoved{}, blackKingMoved{};
    bool whiteRookAMoved{}, whiteRookBMoved{};
    bool blackRookAMoved{}, blackRookBMoved{};
    bool whiteKingInCheck{}, blackKingInCheck{};
    bool whiteKingInCheckmate{}, blackKingInCheckmate{};
    bool whiteKingInStalemate{}, blackKingInStalemate{};
    bool whiteKingInDraw{}, blackKingInDraw{};

    struct MoveRecord {
        std::pair<int, int> from, to;
        char movedPiece, capturedPiece;
    };

    std::vector<MoveRecord> moveHistory;

    bool isInsideBoard(int row, int col) const;
    bool isOpponentPiece(char piece) const;
    bool isOwnPiece(char piece) const;
    bool isLegalMove(const ChessMove& move) const;
};