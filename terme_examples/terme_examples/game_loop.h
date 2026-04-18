#pragma once

#include <memory>

#include <terme/entities/level.h>

class GameLoop
{
    //---------------------------------------------------------- Methods
public:
    GameLoop();
private:
    std::unique_ptr<terme::Level> ShowLevelSelection();
    bool LoopSimulation(terme::Level& level);
};