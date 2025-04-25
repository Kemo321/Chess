#include <gtest/gtest.h>
#include "ChessState.hpp"

class TestChessState : public ChessState {
public:
    using ChessState::ChessState;
    using ChessState::makeMove;
    using ChessState::unmakeMove;
    using ChessState::getPieceAt;
    using ChessState::isSquareAttacked;
    using ChessState::isTerminal;
    using ChessState::getLegalMoves;
    using ChessState::isInCheck;
};

const std::string initial_state = "rnbqkbnrpppppppp00000000000000000000000000000000PPPPPPPPRNBQKBNR1000000";

// initial_state is the starting position of a chess game
// rnbqkbnr
// pppppppp
// ........
// ........
// ........
// ........
// PPPPPPPP
// RNBQKBNR

class ChessStateTestFixture : public ::testing::Test {
protected:
    TestChessState chessState;

    ChessStateTestFixture() : chessState(initial_state) {}

    ~ChessStateTestFixture() override {}

    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(ChessStateTestFixture, InvalidStateString) {
    EXPECT_THROW(TestChessState("invalid_state_string"), std::invalid_argument);
}

TEST_F(ChessStateTestFixture, ValidStateString) {
    EXPECT_NO_THROW(TestChessState{initial_state});
}

TEST_F(ChessStateTestFixture, MakeMove) {
    ChessMove move(6, 4, 4, 4, 0); // e2 to e4
    chessState.makeMove(move);
    EXPECT_EQ(chessState.getPieceAt(4, 4), 'P');
    EXPECT_EQ(chessState.getPieceAt(6, 4), '0');
}

TEST_F(ChessStateTestFixture, UnmakeMove) {
    ChessMove move(6, 4, 4, 4, 0); // e2 to e4
    chessState.makeMove(move);
    chessState.unmakeMove(move);
    EXPECT_EQ(chessState.getPieceAt(6, 4), 'P');
    EXPECT_EQ(chessState.getPieceAt(4, 4), '0');
}

TEST_F(ChessStateTestFixture, PawnPromotion) {
    ChessState chessState = ChessState{"00000000P00000000000000000000000000000000000000000000000000000001000000"};

    ChessMove move(1, 0, 0, 0, 0); // a7 to a8 promoting to Queen
    chessState.makeMove(move);
    EXPECT_EQ(chessState.getPieceAt(0, 0), 'Q');
}

TEST_F(ChessStateTestFixture, KingSideCastling) {
    ChessMove move(7, 4, 7, 6, 0); // e1 to g1
    chessState.makeMove(move);
    EXPECT_EQ(chessState.getPieceAt(7, 6), 'K');
    EXPECT_EQ(chessState.getPieceAt(7, 5), 'R');
    EXPECT_EQ(chessState.getPieceAt(7, 4), '0');
    EXPECT_EQ(chessState.getPieceAt(7, 7), '0');
}

TEST_F(ChessStateTestFixture, QueenSideCastling) {
    ChessMove move(7, 4, 7, 2, 0); // e1 to c1
    chessState.makeMove(move);
    EXPECT_EQ(chessState.getPieceAt(7, 2), 'K');
    EXPECT_EQ(chessState.getPieceAt(7, 3), 'R');
    EXPECT_EQ(chessState.getPieceAt(7, 4), '0');
    EXPECT_EQ(chessState.getPieceAt(7, 0), '0');
}

TEST_F(ChessStateTestFixture, EnPassantCapture) {
    ChessMove move1(6, 4, 4, 4, 0); // e2 to e4
    ChessMove move2(1, 3, 3, 3, 0); // d7 to d5
    ChessMove move3(4, 4, 3, 3, 0); // e4 to d5 (en passant)
    chessState.makeMove(move1);
    chessState.makeMove(move2);
    chessState.makeMove(move3);
    EXPECT_EQ(chessState.getPieceAt(3, 3), 'P');
    EXPECT_EQ(chessState.getPieceAt(4, 4), '0');
    EXPECT_EQ(chessState.getPieceAt(3, 3), 'P');
}

TEST_F(ChessStateTestFixture, IsSquareAttacked) {
    EXPECT_TRUE(chessState.isSquareAttacked(5, 0, true)); 
    EXPECT_FALSE(chessState.isSquareAttacked(5, 0, false));
}

TEST_F(ChessStateTestFixture, IsTerminal) {
    EXPECT_FALSE(chessState.isTerminal());
}

TEST_F(ChessStateTestFixture, GetLegalMoves) {
    std::vector<ChessMove> legalMoves = chessState.getLegalMoves();
    EXPECT_FALSE(legalMoves.empty());
}

TEST_F(ChessStateTestFixture, IsInCheck) {
    EXPECT_FALSE(chessState.isCheckmate());
}

TEST_F(ChessStateTestFixture, isInCheckMate){
    ChessState state = ChessState{"0nbqkbnr00qqqqqq0000000000000000000000000000000000000000K00000001000000"};

    state.printBoard();
    EXPECT_TRUE(state.isSquareAttacked(7, 0, false));
    EXPECT_TRUE(state.isSquareAttacked(6, 0, false));
    EXPECT_TRUE(state.isSquareAttacked(7, 1, false));
    EXPECT_TRUE(state.isSquareAttacked(6, 1, false));
    EXPECT_FALSE(state.isSquareAttacked(0, 0, false));

    EXPECT_FALSE(state.isLegalMove(ChessMove(7, 0, 6, 0, 0)));
    EXPECT_FALSE(state.isLegalMove(ChessMove(7, 0, 6, 1, 0)));
    EXPECT_FALSE(state.isLegalMove(ChessMove(7, 0, 7, 1, 0)));


    EXPECT_TRUE(state.isTerminal());
    EXPECT_TRUE(state.getLegalMoves().empty());
}

TEST_F(ChessStateTestFixture, PawnDoubleMove) {
    ChessMove move(6, 0, 4, 0, 0); // a2 to a4
    chessState.makeMove(move);
    EXPECT_EQ(chessState.getPieceAt(4, 0), 'P');
    EXPECT_EQ(chessState.getPieceAt(6, 0), '0');
    chessState.unmakeMove(move);
    EXPECT_EQ(chessState.getPieceAt(6, 0), 'P');
    EXPECT_EQ(chessState.getPieceAt(4, 0), '0');
}

TEST_F(ChessStateTestFixture, CastlingRightsLost) {
    ChessMove kingMove(7, 4, 7, 5, 0); // Move king
    chessState.makeMove(kingMove);
    EXPECT_TRUE(chessState.whiteKingMoved);
    chessState.unmakeMove(kingMove);
    EXPECT_FALSE(chessState.whiteKingMoved);
}

TEST_F(ChessStateTestFixture, Stalemate) {
    std::string stalemateState = "00000000k00000000000000000000000000000000000000000000000K00000001000000";
    TestChessState state(stalemateState);
    EXPECT_FALSE(state.isInCheck(true));
    EXPECT_FALSE(state.isTerminal());
    EXPECT_FALSE(state.getLegalMoves().empty());
}

TEST_F(ChessStateTestFixture, PromotionToDifferentPieces) {
    std::string promotionState = "00000000P00000000000000000000000000000000000000000000000000000001000000";
    TestChessState state(promotionState);
    ChessMove rookPromotion(1, 0, 0, 0, 1); // Promote to Rook
    state.makeMove(rookPromotion);
    EXPECT_EQ(state.getPieceAt(0, 0), 'R');
    state.unmakeMove(rookPromotion);
    EXPECT_EQ(state.getPieceAt(1, 0), 'P');
}

TEST_F(ChessStateTestFixture, LegalMovesInCheck) {
    std::string checkState = "rnbqkbnrpppppppp0000000000000000000000000000q000PPPPPPPPRNBQKBNR1000000";
    TestChessState state(checkState);
    std::vector<ChessMove> legalMoves = state.getLegalMoves();
    EXPECT_FALSE(legalMoves.empty());
}

TEST_F(ChessStateTestFixture, UnmakeMoveAfterCastling) {
    ChessMove castlingMove(7, 4, 7, 6, 0); // King-side castling
    chessState.makeMove(castlingMove);
    chessState.unmakeMove(castlingMove);
    EXPECT_EQ(chessState.getPieceAt(7, 4), 'K');
    EXPECT_EQ(chessState.getPieceAt(7, 7), 'R');
    EXPECT_EQ(chessState.getPieceAt(7, 6), 'N');
}

TEST_F(ChessStateTestFixture, MultipleMovesAndUnmakes) {
    ChessMove move1(6, 4, 4, 4, 0); // e2 to e4
    ChessMove move2(1, 4, 3, 4, 0); // e7 to e5
    chessState.makeMove(move1);
    chessState.makeMove(move2);
    chessState.unmakeMove(move2);
    chessState.unmakeMove(move1);
    EXPECT_EQ(chessState.getPieceAt(6, 4), 'P');
    EXPECT_EQ(chessState.getPieceAt(1, 4), 'p');
    EXPECT_EQ(chessState.getPieceAt(4, 4), '0');
}
