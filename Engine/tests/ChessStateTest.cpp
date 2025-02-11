#include <gtest/gtest.h>
#include "ChessState.hpp"
#include "ChessMove.hpp"

// Test basic move generation
TEST(ChessStateTest, PawnMoves) {
    ChessState state("0000000000000000PPPPPPPP00000000000000000000000000000000pppppppp0000000000000000");
    auto moves = state.getLegalMoves();
    EXPECT_GT(moves.size(), 0);
}

// Test move execution
TEST(ChessStateTest, MakeMove) {
    ChessState state("rnbqkbnrpppppppp00000000000000000000000000000000PPPPPPPPRNBQKBNR1");
    ChessMove move(6, 4, 4, 4); // Pawn e2 to e4
    state.makeMove(move);
    EXPECT_EQ(state.getPieceAt(4, 4), 'P');
    EXPECT_EQ(state.getPieceAt(6, 4), '0');
}

// Test castling
TEST(ChessStateTest, Castling) {
    ChessState state("rnbqkbnrpppppppp00000000000000000000000000000000PPPPPPPPRNBQKBNR1");
    ChessMove move(7, 4, 7, 6); // Short castling (shouldn't be legal yet but we're testing the move execution)
    state.makeMove(move);
    EXPECT_TRUE(state.getPieceAt(7, 6) == 'K');
    EXPECT_TRUE(state.getPieceAt(7, 5) == 'R');
}

// Test pawn promotion
TEST(ChessStateTest, PawnPromotion) {
    ChessState state("rnbqkbnrpppppppp00000000000000000000000000000000PPPPPPPPRNBQKBNR1");
    ChessMove move(1, 0, 7, 0, 0); // Promotion of a white pawn
    state.makeMove(move);
    EXPECT_EQ(state.getPieceAt(7, 0), 'q');
}

// Test en passant
TEST(ChessStateTest, EnPassant) {
    ChessState state("rnbqkbnrpppppppp00000000000000000000000000000000PPPPPPPPRNBQKBNR1");
    ChessMove move1(6, 4, 4, 4); // e2 to e4
    ChessMove move2(1, 3, 3, 3); // d7 to d5
    ChessMove move3(4, 4, 3, 3); // En passant
    state.makeMove(move1);
    state.makeMove(move2);
    state.makeMove(move3);
    EXPECT_EQ(state.getPieceAt(3, 3), 'P');
    EXPECT_EQ(state.getPieceAt(3, 4), '0');
}

// Test check detection
TEST(ChessStateTest, CheckDetection) {
    ChessState state("rnbqkbnrpppppppp00000000000000000000000000000000PPPPPPPPRNBQKBNR1");
    EXPECT_FALSE(state.isInCheck(true)); // White king not in check
}