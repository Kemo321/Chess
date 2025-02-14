#include "Engine.hpp"


Engine::Engine() : positionORM("chess.db")
{
    heuristics.emplace_back(std::make_unique<Heuristic1>());

}


Engine::~Engine()
{
}


std::string Engine::getBestMove(const std::string& stateStr, int depth)
{
    // Retrieve the cached position from the database.
    Position position = positionORM.getPosition(stateStr);
    
    // If a valid position is found and its FEN matches, return the cached best move.
    if (!position.fen.empty() && position.fen == stateStr && !position.best_move.empty())
    {
        std::cout << "Found position in database: " << position.best_move << std::endl;
        return position.best_move;
    }
    
    // Compute the best move since it wasn't found in the database.
    ChessState state(stateStr);
    ChessMove bestMove;
    float bestScore = 0.0f;
    std::tie(bestMove, bestScore) = getBestMove_(state, depth);
    
    std::string computedMove = bestMove.toString();
    if (computedMove.empty())
    {
        std::cerr << "Error: Computed best move is empty." << std::endl;
        return ""; 
    }
    
    // Insert the newly computed best move into the database.
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
    float bestScore = -1000000;
    ChessMove bestMove;
    for (const ChessMove& move : legalMoves)
    {
        state.makeMove(move);
        float score = alphabeta(state, depth, -1000000, 1000000, false);
        state.unmakeMove(move);
        if (score > bestScore)
        {
            bestScore = score;
            bestMove = move;
        }
    }
    return {bestMove, bestScore};
}

float Engine::alphabeta(ChessState& state, int depth, float alpha, float beta, bool maximizingPlayer)
{
    if (depth == 0 || state.isTerminal())
    {
        float score = 0;
        for (const auto& heuristic : heuristics)
        {
            score += (*heuristic)(state);
        }
        return score;
    }

    std::vector<ChessMove> legalMoves = state.getLegalMoves();
    if (maximizingPlayer)
    {
        float maxEval = -1000000;
        for (const ChessMove& move : legalMoves)
        {
            state.makeMove(move);
            float eval = alphabeta(state, depth - 1, alpha, beta, false);
            state.unmakeMove(move);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha)
                break;
        }
        return maxEval;
    }
    else
    {
        float minEval = 1000000;
        for (const ChessMove& move : legalMoves)
        {
            state.makeMove(move);
            float eval = alphabeta(state, depth - 1, alpha, beta, true);
            state.unmakeMove(move);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha)
                break;
        }
        return minEval;
    }
}