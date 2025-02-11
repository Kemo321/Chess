#pragma once
#include "ChessState.hpp"


class Heuristic 
{
public:
    Heuristic() = default;
    virtual ~Heuristic() = default;

    virtual float operator()(ChessState& state) const = 0;

};