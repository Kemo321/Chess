#include <gtest/gtest.h>
#include "ChessMove.hpp"

TEST(ChessMoveTest, DefaultConstructor) {
    ChessMove move;
    EXPECT_EQ(move.getFrom(), std::make_pair(0, 0));
    EXPECT_EQ(move.getTo(), std::make_pair(0, 0));
    EXPECT_EQ(move.getPromotion(), 0);
}

TEST(ChessMoveTest, ParameterizedConstructor) {
    ChessMove move(1, 2, 3, 4, 5);
    EXPECT_EQ(move.getFrom(), std::make_pair(1, 2));
    EXPECT_EQ(move.getTo(), std::make_pair(3, 4));
    EXPECT_EQ(move.getPromotion(), 5);
}

TEST(ChessMoveTest, CopyConstructor) {
    ChessMove move1(1, 2, 3, 4, 5);
    ChessMove move2(move1);
    EXPECT_EQ(move2.getFrom(), std::make_pair(1, 2));
    EXPECT_EQ(move2.getTo(), std::make_pair(3, 4));
    EXPECT_EQ(move2.getPromotion(), 5);
}

TEST(ChessMoveTest, AssignmentOperator) {
    ChessMove move1(1, 2, 3, 4, 5);
    ChessMove move2;
    move2 = move1;
    EXPECT_EQ(move2.getFrom(), std::make_pair(1, 2));
    EXPECT_EQ(move2.getTo(), std::make_pair(3, 4));
    EXPECT_EQ(move2.getPromotion(), 5);
}

TEST(ChessMoveTest, SetFrom) {
    ChessMove move;
    move.setFrom(10);
    EXPECT_EQ(move.getFrom(), std::make_pair(1, 2));
}

TEST(ChessMoveTest, SetTo) {
    ChessMove move;
    move.setTo(27);
    EXPECT_EQ(move.getTo(), std::make_pair(3, 3));
}

TEST(ChessMoveTest, SetPromotion) {
    ChessMove move;
    move.setPromotion(3);
    EXPECT_EQ(move.getPromotion(), 3);
}

TEST(ChessMoveTest, EqualityOperator) {
    ChessMove move1(1, 2, 3, 4, 5);
    ChessMove move2(1, 2, 3, 4, 5);
    EXPECT_TRUE(move1 == move2);
}

TEST(ChessMoveTest, InequalityOperator) {
    ChessMove move1(1, 2, 3, 4, 5);
    ChessMove move2(1, 2, 3, 4, 6);
    EXPECT_TRUE(move1 != move2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}