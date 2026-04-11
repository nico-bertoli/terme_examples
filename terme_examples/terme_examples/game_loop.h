#pragma once

#include "terme/entities/level.h"
#include <memory>

class GameLoop
{
    //---------------------------------------------------------- Methods
public:
    GameLoop();
private:
    std::unique_ptr<terme::Level> ShowLevelSelection();
    bool LoopSimulation(terme::Level& level);
};