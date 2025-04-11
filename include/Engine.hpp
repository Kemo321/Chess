#pragma once
#include "Heuristic.hpp"
#include "PositionORM.hpp"
#include <memory>


class Engine
{
public:
    Engine();
    ~Engine();
    std::string getBestMove(const std::string& state, int depth);

private:
    std::pair<ChessMove, float> getBestMove_(ChessState& state, int depth);
    float alphabeta(ChessState& state, int depth, float alpha, float beta, bool maximizingPlayer);

    std::vector<std::unique_ptr<Heuristic>> heuristics;

    PositionORM positionORM;
};