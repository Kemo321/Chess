#pragma once
#include <string>
#include <vector>
#include "ChessMove.hpp"


class ChessState {
public:
    friend class Heuristic;

    // Constructors
    explicit ChessState(const std::string& state);
    ChessState(const ChessState& other) = default;
    ChessState& operator=(const ChessState& other) = default;
    ChessState(ChessState&& other) noexcept = default;
    ChessState& operator=(ChessState&& other) noexcept = default;
    ~ChessState() = default;

    // State string functions
    bool checkIfStringIsValid(const std::string& state);

    // Move functions
    void makeMove(const ChessMove& move);
    void unmakeMove(const ChessMove& move);
    std::vector<ChessMove> getLegalMoves();


    // Game state functions
    bool isSquareAttacked(int row, int col, bool attackedByWhite) const;
    bool isCheck();
    bool isCheckmate();
    bool isStalemate();
    bool isTerminal();

    void printBoard() const;
    char getPieceAt(int row, int col) const;
    bool isInCheck(bool byWhite) const;

//private:
    // Board state
    char state[8][8]{};
    bool whiteToMove{};
    bool whiteKingMoved{}, blackKingMoved{};
    bool whiteRookAMoved{}, whiteRookBMoved{};
    bool blackRookAMoved{}, blackRookBMoved{};

    // Move history
    struct MoveRecord {
    std::pair<int, int> from, to;
    char movedPiece, capturedPiece;
    // Extra data for castling:
    bool isCastling = false;
    std::pair<int,int> rookFrom, rookTo;
    // Extra data for en passant:
    bool isEnPassant = false;
    std::pair<int,int> enPassantCapturedPos;
};
    std::vector<MoveRecord> moveHistory;

    // Pieces moves
    void getPawnMoves(int row, int col, std::vector<ChessMove>& legalMoves);
    void getKnightMoves(int row, int col, std::vector<ChessMove>& legalMoves);
    void getBishopMoves(int row, int col, std::vector<ChessMove>& legalMoves);
    void getRookMoves(int row, int col, std::vector<ChessMove>& legalMoves);
    void getQueenMoves(int row, int col, std::vector<ChessMove>& legalMoves);
    void getKingMoves(int row, int col, std::vector<ChessMove>& legalMoves);

    // Helper functions
    bool isInsideBoard(int row, int col) const;
    bool isOpponentPiece(char piece) const;
    bool isOwnPiece(char piece) const;
    bool isLegalMove(const ChessMove& move);

    // King positions
    int getWhiteKingRow() const;
    int getWhiteKingCol() const;
    int getBlackKingRow() const;
    int getBlackKingCol() const;

    // String conversion
    std::string toString() const;

};