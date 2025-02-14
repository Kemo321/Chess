#pragma once
#include "ChessState.hpp"


class Heuristic 
{
public:
    Heuristic() = default;
    virtual ~Heuristic() = default;

    virtual float operator()(ChessState& state) const = 0;

};


class Heuristic1 : public Heuristic
{   
public:
    float operator()(ChessState& state) const override
    {
        float score = 0;
        for(int i = 0; i < 8; i++)
        {
            for(int j = 0; j < 8; j++)
            {
                switch (state.state[i][j])
                {
                    case 'P':
                        score += 1;
                        break;
                    case 'p':
                        score -= 1;
                        break;
                    case 'N':
                        score += 3;
                        break;
                    case 'n':
                        score -= 3;
                        break;
                    case 'B':
                        score += 3;
                        break;
                    case 'b':
                        score -= 3;
                        break;
                    case 'R':
                        score += 5;
                        break;
                    case 'r':
                        score -= 5;
                        break;
                    case 'Q':
                        score += 9;
                        break;
                    case 'q':
                        score -= 9;
                        break;
                    case 'K':
                        break;
                    case 'k':
                        break;
                    default:
                        break;
                }
                
            }
        }
        return score;
    }
};


class Heuristic2 : public Heuristic {
public:
    float operator()(ChessState& state) const override {
        float score = 0.0f;
        
        // Piece values (scaled up for integer arithmetic)
        constexpr int pawnValue   = 100;
        constexpr int knightValue = 320;
        constexpr int bishopValue = 330;
        constexpr int rookValue   = 500;
        constexpr int queenValue  = 900;
        // King is not assigned a material value here (handled separately by king safety)
        
        // Piece-square tables for White (for Black, we mirror vertically)
        static const int pawnTable[8][8] = {
            {  0,   0,   0,   0,   0,   0,   0,   0},
            { 50,  50,  50,  50,  50,  50,  50,  50},
            { 10,  10,  20,  30,  30,  20,  10,  10},
            {  5,   5,  10,  25,  25,  10,   5,   5},
            {  0,   0,   0,  20,  20,   0,   0,   0},
            {  5,  -5, -10,   0,   0, -10,  -5,   5},
            {  5,  10,  10, -20, -20,  10,  10,   5},
            {  0,   0,   0,   0,   0,   0,   0,   0}
        };

        static const int knightTable[8][8] = {
            {-50, -40, -30, -30, -30, -30, -40, -50},
            {-40, -20,   0,   0,   0,   0, -20, -40},
            {-30,   0,  10,  15,  15,  10,   0, -30},
            {-30,   5,  15,  20,  20,  15,   5, -30},
            {-30,   0,  15,  20,  20,  15,   0, -30},
            {-30,   5,  10,  15,  15,  10,   5, -30},
            {-40, -20,   0,   5,   5,   0, -20, -40},
            {-50, -40, -30, -30, -30, -30, -40, -50}
        };

        static const int bishopTable[8][8] = {
            {-20, -10, -10, -10, -10, -10, -10, -20},
            {-10,   0,   0,   0,   0,   0,   0, -10},
            {-10,   0,   5,  10,  10,   5,   0, -10},
            {-10,   5,   5,  10,  10,   5,   5, -10},
            {-10,   0,  10,  10,  10,  10,   0, -10},
            {-10,  10,  10,  10,  10,  10,  10, -10},
            {-10,   5,   0,   0,   0,   0,   5, -10},
            {-20, -10, -10, -10, -10, -10, -10, -20}
        };

        static const int rookTable[8][8] = {
            {  0,   0,   0,   0,   0,   0,   0,   0},
            {  5,  10,  10,  10,  10,  10,  10,   5},
            { -5,   0,   0,   0,   0,   0,   0,  -5},
            { -5,   0,   0,   0,   0,   0,   0,  -5},
            { -5,   0,   0,   0,   0,   0,   0,  -5},
            { -5,   0,   0,   0,   0,   0,   0,  -5},
            { -5,   0,   0,   0,   0,   0,   0,  -5},
            {  0,   0,   0,   5,   5,   0,   0,   0}
        };

        static const int queenTable[8][8] = {
            {-20, -10, -10,  -5,  -5, -10, -10, -20},
            {-10,   0,   0,   0,   0,   0,   0, -10},
            {-10,   0,   5,   5,   5,   5,   0, -10},
            { -5,   0,   5,   5,   5,   5,   0,  -5},
            {  0,   0,   5,   5,   5,   5,   0,  -5},
            {-10,   5,   5,   5,   5,   5,   0, -10},
            {-10,   0,   5,   0,   0,   0,   0, -10},
            {-20, -10, -10,  -5,  -5, -10, -10, -20}
        };

        static const int kingTable[8][8] = {
            {-30, -40, -40, -50, -50, -40, -40, -30},
            {-30, -40, -40, -50, -50, -40, -40, -30},
            {-30, -40, -40, -50, -50, -40, -40, -30},
            {-30, -40, -40, -50, -50, -40, -40, -30},
            {-20, -30, -30, -40, -40, -30, -30, -20},
            {-10, -20, -20, -20, -20, -20, -20, -10},
            { 20,  20,   0,   0,   0,   0,  20,  20},
            { 20,  30,  10,   0,   0,  10,  30,  20}
        };

        // Loop over board squares
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                char piece = state.state[i][j];
                switch(piece) {
                    case 'P':
                        score += pawnValue + pawnTable[i][j];
                        break;
                    case 'p':
                        // Mirror the table for black pieces
                        score -= pawnValue + pawnTable[7 - i][j];
                        break;
                    case 'N':
                        score += knightValue + knightTable[i][j];
                        break;
                    case 'n':
                        score -= knightValue + knightTable[7 - i][j];
                        break;
                    case 'B':
                        score += bishopValue + bishopTable[i][j];
                        break;
                    case 'b':
                        score -= bishopValue + bishopTable[7 - i][j];
                        break;
                    case 'R':
                        score += rookValue + rookTable[i][j];
                        break;
                    case 'r':
                        score -= rookValue + rookTable[7 - i][j];
                        break;
                    case 'Q':
                        score += queenValue + queenTable[i][j];
                        break;
                    case 'q':
                        score -= queenValue + queenTable[7 - i][j];
                        break;
                    case 'K':
                        // For king, we only add positional bonus (material is infinite)
                        score += kingTable[i][j];
                        break;
                    case 'k':
                        score -= kingTable[7 - i][j];
                        break;
                    default:
                        break;
                }
            }
        }

        // Mobility bonus: Evaluate the difference in legal moves
        // (Assumes state.getLegalMoves() returns moves for the current side.)
        std::vector<ChessMove> currentMoves = state.getLegalMoves();
        // Create a copy of the state with the turn switched to get opponent moves.
        ChessState opponentState = state;
        opponentState.whiteToMove = !state.whiteToMove;
        std::vector<ChessMove> opponentMoves = opponentState.getLegalMoves();

        // Weight mobility difference by a factor (e.g., 10 points per move)
        score += 10 * (static_cast<int>(currentMoves.size()) - static_cast<int>(opponentMoves.size()));

        return score;
    }
};
