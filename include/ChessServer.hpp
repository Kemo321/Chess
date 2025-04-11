#pragma once
#include "httplib.h"
#include "Engine.hpp"


class ChessServer : public httplib::Server {
public:
    ChessServer();

    void start();
private:
    Engine engine;
};