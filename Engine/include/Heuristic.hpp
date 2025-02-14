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