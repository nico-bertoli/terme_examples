#pragma once

#include "terme/entities/level.h"

class GameLoop
{
    //---------------------------------------------------------- Methods
public:
    GameLoop();
private:
    std::shared_ptr<terme::Level> ShowLevelSelection();
    bool LoopSimulation(std::shared_ptr<terme::Level> level);
};