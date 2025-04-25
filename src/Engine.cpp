#include "Engine.hpp"
#include <iostream>
#include <limits>
#include <memory>
#include <tuple>
#include <vector>
#include <algorithm>

// Define infinity constants for the search.
constexpr float NEG_INF = -1000000.0f;
constexpr float POS_INF =  1000000.0f;

Engine::Engine() : positionORM("chess.db")
{
    heuristics.emplace_back(std::make_unique<Heuristic2>());
}

Engine::~Engine()
{
    // Smart pointers in 'heuristics' handle memory automatically.
}

std::string Engine::getBestMove(const std::string& stateStr, int depth)
{
    // Retrieve the cached position from the database.
    Position position = positionORM.getPosition(stateStr);
    if (!position.fen.empty() && position.fen == stateStr && !position.best_move.empty())
    {
        std::cout << "Found position in database: " << position.best_move << std::endl;
        return position.best_move;
    }
    else
    {
        std::cout << "Position not found in database." << std::endl;
    }
    
    // If not found, compute the best move.
    ChessState state(stateStr);
    ChessMove bestMove;
    float bestScore = 0.0f;
    
    // Get best move using alpha-beta search.
    std::tie(bestMove, bestScore) = getBestMove_(state, depth);
    
    std::string computedMove = bestMove.toString();
    if (computedMove.empty())
    {
        std::cerr << "Error: Computed best move is empty." << std::endl;
        return "";
    }
    

    // Cache the computed result into the database.
    bool insertSuccess = positionORM.insertPosition({ stateStr, computedMove, bestScore });
    if (insertSuccess)
    {
        std::cout << "Inserted new position into database: " << computedMove << std::endl;
    }
    else
    {
        std::cerr << "Failed to insert position into database." << std::endl;
    }
    
    return computedMove;
}

std::pair<ChessMove, float> Engine::getBestMove_(ChessState& state, int depth)
{
    std::vector<ChessMove> legalMoves = state.getLegalMoves();
    
    // If no legal moves are available, return a default move with score zero.
    if (legalMoves.empty())
    {
        std::cerr << "No legal moves available in state." << std::endl;
        return { ChessMove(), 0.0f };
    }
    
    float bestScore = NEG_INF;
    ChessMove bestMove;
    
    // Evaluate each legal move using alpha-beta search.
    for (const auto& move : legalMoves)
    {
        state.makeMove(move);
        float score = alphabeta(state, depth - 1, NEG_INF, POS_INF, false);
        state.unmakeMove(move);
        
        if (score > bestScore)
        {
            bestScore = score;
            bestMove = move;
        }
    }
    
    return { bestMove, bestScore };
}

float Engine::alphabeta(ChessState& state, int depth, float alpha, float beta, bool maximizingPlayer)
{
    // Terminal condition: depth is zero or state is terminal.
    if (depth == 0 || state.isTerminal())
    {
        // If it's checkmate, return a very large value adjusted by whose turn it is.
        if (state.isCheckmate())
        {
            return maximizingPlayer ? -1000000.0f : 1000000.0f;
        }
        
        if (state.isStalemate())
        {
            return 0.0f;
        }

        // Evaluate the state using the heuristics.
        float score = 0.0f;
        for (const auto& heuristic : heuristics)
        {
            score += (*heuristic)(state);
        }


        return score;
    }
    
    std::vector<ChessMove> legalMoves = state.getLegalMoves();
    
    if (maximizingPlayer)
    {
        float maxEval = -1000000.0f;
        for (const ChessMove& move : legalMoves)
        {
            state.makeMove(move);
            float eval = alphabeta(state, depth - 1, alpha, beta, false);
            state.unmakeMove(move);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha)
                break;  // Beta cutoff.
        }
        return maxEval;
    }
    else
    {
        float minEval = 1000000.0f;
        for (const ChessMove& move : legalMoves)
        {
            state.makeMove(move);
            float eval = alphabeta(state, depth - 1, alpha, beta, true);
            state.unmakeMove(move);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha)
                break;  // Alpha cutoff.
        }
        return minEval;
    }
}
