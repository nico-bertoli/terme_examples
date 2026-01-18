#include <terme/input_manager/input_manager.h>
#include <terme/managers/audio_manager.h>
#include <terme/core/simulation.h>
#include <terme/terminal/terminal.h>

#include "GameLoop.h"
#include "terme/config.h"
#include "EndlessRunnerLevel.h"
#include "PuzzleLevel.h"
#include "CollisionsTestLevel.h"
#include "PongLevel.h"
#include "SpaceInvadersLevel.h"
#include "SortingLayerTestLevel.h"

using std::cout;
using terme::Level;
using terme::InputManager;
using terme::Key;
using terme::Terminal;

GameLoop::GameLoop()
{
    bool returnToMainMenu = false;

    while (true)
    {
        terme::AudioManager::Instance().StopMusic();
        std::shared_ptr<Level> level = ShowLevelSelection();
        while (true)
        {
            returnToMainMenu = LoopSimulation(level);
            if (returnToMainMenu)
                break;
        }
    }
}

bool GameLoop::LoopSimulation(std::shared_ptr<Level> level)
{
    terme::Simulation::Instance().LoadLevel(level);
    while (level->IsTerminated() == false)
    {
        terme::Simulation::Instance().Step();
        if (InputManager::Instance().IsKeyPressed(Key::kEsc))
            return true;
    }
    return false;
}

std::shared_ptr<Level> GameLoop::ShowLevelSelection()
{
    Terminal::Instance().Clear();

    Terminal::Instance().SetColor(terme::color::kWhite);
    cout << "--------------------------- Games:\n";

    Terminal::Instance().SetColor(terme::color::kRed);
    cout << "1 -> space invaders\n";
    Terminal::Instance().SetColor(terme::color::kGreen);
    cout << "2 -> endless runner\n";
    Terminal::Instance().SetColor(terme::color::kCyanDark);
    cout << "3 -> puzzle game\n";
    Terminal::Instance().SetColor(terme::color::kYellow);
    cout << "4 -> pong (local multiplayer)\n";

    Terminal::Instance().SetColor(terme::color::kWhite);
#if DEBUG_MODE
    cout << "5 -> collisions test\n";
    cout << "6 -> sorting layer test\n";
#endif
    cout << "esc -> return to main menu\n\n";

    cout << "--------------------------- Controls:\n";

    cout << "Space invaders:\n";
    cout << "wasd: move, space bar: shoot\n\n";

    cout << "Bunny:\n";
    cout << "wasd: move, spacebar: jump\n\n";

    cout << "Pong:\n";
    cout << "a/d: Player 1 move, left/right arrow: Player 2 move\n\n";

    cout << "--------------------------- Credits:\n";
    cout << "Music by 'Cody O'Quinn'\n";

    while (true)
    {
        if (InputManager::Instance().IsKeyPressed(Key::kNum1))
            return std::make_shared<SpaceInvaders::SpaceInvadersLevel>();

        if (InputManager::Instance().IsKeyPressed(Key::kNum2))
            return std::make_shared<Platformer::EndlessRunnerLevel>();

        if (InputManager::Instance().IsKeyPressed(Key::kNum3))
            return std::make_shared<Platformer::PuzzleLevel>();
    
        if (InputManager::Instance().IsKeyPressed(Key::kNum4))
            return std::make_shared<Pong::PongLevel>();
        
#if DEBUG_MODE
        if (InputManager::Instance().IsKeyPressed(Key::kNum5))
            return std::make_shared<Platformer::CollisionsTestLevel>();
            
        if (InputManager::Instance().IsKeyPressed(Key::kNum6))
            return std::make_shared<Platformer::SortingLayerTestLevel>();
#endif
    }
}
