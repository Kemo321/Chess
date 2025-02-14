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
    move.setFrom({1, 2});
    EXPECT_EQ(move.getFrom(), std::make_pair(1, 2));
}

TEST(ChessMoveTest, SetTo) {
    ChessMove move;
    move.setTo({3, 4});
    EXPECT_EQ(move.getTo(), std::make_pair(3, 4));
}

TEST(ChessMoveTest, SetPromotion) {
    ChessMove move;
    move.setPromotion(5);
    EXPECT_EQ(move.getPromotion(), 5);
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

TEST(ChessMoveTest, PrintMove) {
    ChessMove move(1, 2, 3, 4, 5);
    testing::internal::CaptureStdout();
    move.printMove();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "12 -> 34\n");
}

TEST(ChessMoveTest, GetFrom) {
    ChessMove move(1, 2, 3, 4, 5);
    EXPECT_EQ(move.getFrom(), std::make_pair(1, 2));
}

TEST(ChessMoveTest, GetTo) {
    ChessMove move(1, 2, 3, 4, 5);
    EXPECT_EQ(move.getTo(), std::make_pair(3, 4));
}

TEST(ChessMoveTest, GetPromotion) {
    ChessMove move(1, 2, 3, 4, 5);
    EXPECT_EQ(move.getPromotion(), 5);
}

TEST(ChessMoveTest, SetFromAndGetFrom) {
    ChessMove move;
    move.setFrom({5, 6});
    EXPECT_EQ(move.getFrom(), std::make_pair(5, 6));
}

TEST(ChessMoveTest, SetToAndGetTo) {
    ChessMove move;
    move.setTo({7, 8});
    EXPECT_EQ(move.getTo(), std::make_pair(7, 8));
}

TEST(ChessMoveTest, SetPromotionAndGetPromotion) {
    ChessMove move;
    move.setPromotion(9);
    EXPECT_EQ(move.getPromotion(), 9);
}

TEST(ChessMoveTest, EqualityOperatorWithDifferentFrom) {
    ChessMove move1(1, 2, 3, 4, 5);
    ChessMove move2(2, 2, 3, 4, 5);
    EXPECT_FALSE(move1 == move2);
}

TEST(ChessMoveTest, EqualityOperatorWithDifferentTo) {
    ChessMove move1(1, 2, 3, 4, 5);
    ChessMove move2(1, 2, 4, 4, 5);
    EXPECT_FALSE(move1 == move2);
}

TEST(ChessMoveTest, EqualityOperatorWithDifferentPromotion) {
    ChessMove move1(1, 2, 3, 4, 5);
    ChessMove move2(1, 2, 3, 4, 6);
    EXPECT_FALSE(move1 == move2);
}

TEST(ChessMoveTest, InequalityOperatorWithSameMoves) {
    ChessMove move1(1, 2, 3, 4, 5);
    ChessMove move2(1, 2, 3, 4, 5);
    EXPECT_FALSE(move1 != move2);
}

TEST(ChessMoveTest, InequalityOperatorWithDifferentFrom) {
    ChessMove move1(1, 2, 3, 4, 5);
    ChessMove move2(2, 2, 3, 4, 5);
    EXPECT_TRUE(move1 != move2);
}

TEST(ChessMoveTest, InequalityOperatorWithDifferentTo) {
    ChessMove move1(1, 2, 3, 4, 5);
    ChessMove move2(1, 2, 4, 4, 5);
    EXPECT_TRUE(move1 != move2);
}

TEST(ChessMoveTest, InequalityOperatorWithDifferentPromotion) {
    ChessMove move1(1, 2, 3, 4, 5);
    ChessMove move2(1, 2, 3, 4, 6);
    EXPECT_TRUE(move1 != move2);
}

TEST(ChessMoveTest, SetFromWithNegativeValues) {
    ChessMove move;
    move.setFrom({-1, -2});
    EXPECT_EQ(move.getFrom(), std::make_pair(-1, -2));
}

TEST(ChessMoveTest, SetToWithNegativeValues) {
    ChessMove move;
    move.setTo({-3, -4});
    EXPECT_EQ(move.getTo(), std::make_pair(-3, -4));
}

TEST(ChessMoveTest, SetPromotionWithNegativeValue) {
    ChessMove move;
    move.setPromotion(-5);
    EXPECT_EQ(move.getPromotion(), -5);
}

TEST(ChessMoveTest, SetFromWithLargeValues) {
    ChessMove move;
    move.setFrom({1000, 2000});
    EXPECT_EQ(move.getFrom(), std::make_pair(1000, 2000));
}

TEST(ChessMoveTest, SetToWithLargeValues) {
    ChessMove move;
    move.setTo({3000, 4000});
    EXPECT_EQ(move.getTo(), std::make_pair(3000, 4000));
}

TEST(ChessMoveTest, SetPromotionWithLargeValue) {
    ChessMove move;
    move.setPromotion(5000);
    EXPECT_EQ(move.getPromotion(), 5000);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}