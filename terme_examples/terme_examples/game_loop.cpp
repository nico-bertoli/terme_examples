#include <terme/settings.h>

#include "collisions_test_level.h"
#include "endless_runner_level.h"
#include "game_loop.h"
#include "pong_level.h"
#include "puzzle_level.h"
#include "sorting_layer_test_level.h"
#include "space_invaders_level.h"

#include <terme/core/simulation.h>
#include <terme/input_manager/input_manager.h>
#include <terme/managers/audio_manager.h>
#include <terme/managers/time_manager.h>
#include <terme/terminal/terminal.h>

#include <memory>

using std::cout;
using terme::InputManager;
using terme::Key;
using terme::Level;
using terme::Terminal;

GameLoop::GameLoop()
{
    bool return_to_main_menu = false;

    while (true)
    {
        terme::AudioManager::Instance().StopMusic();
        std::unique_ptr<Level> level = ShowLevelSelection();

        // user wants to quit
        if(level == nullptr)
            break;

        while (true)
        {
            return_to_main_menu = LoopSimulation(*level);
            if (return_to_main_menu)
                break;
        }
    }
}

bool GameLoop::LoopSimulation(Level& level)
{
    bool return_to_main_menu = false;
    terme::Simulation::Instance().LoadLevel(level);

    while (level.IsTerminated() == false)
    {
        terme::Simulation::Instance().Step();
        if (InputManager::Instance().IsKeyPressed(Key::kEsc))
        {
            return_to_main_menu = true;
            break;
        }
    }

    terme::Simulation::Instance().UnloadLevel();
    return return_to_main_menu;
}

std::unique_ptr<Level> GameLoop::ShowLevelSelection()
{
    double menu_show_start_time = terme::TimeManager::Instance().GetTime();

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
#if DEBUG
    cout << "5 -> collisions test\n";
    cout << "6 -> sorting layer test\n";
#endif
    cout << "esc -> return to main menu / quit\n\n";

    cout << "Controls: move: wasd, action: space bar";


    cout << "\n\n\n\n--------------------------- Credits:\n";
    cout << "Terme engine github: ";
    Terminal::Instance().SetColor(terme::color::kCyan);
    cout << "https://github.com/nico-bertoli/terme\n";
    Terminal::Instance().SetColor(terme::color::kWhite);

    cout << "Levels github: ";
    Terminal::Instance().SetColor(terme::color::kCyan);
    cout << "https://github.com/nico-bertoli/terme_examples\n";
    Terminal::Instance().SetColor(terme::color::kWhite);

    cout << "Developer: ";
    Terminal::Instance().SetColor(terme::color::kCyan);
    cout << "https://nico-bertoli.github.io/index.html\n";
    Terminal::Instance().SetColor(terme::color::kWhite);

    cout << "Music: ";
    Terminal::Instance().SetColor(terme::color::kCyan);
    cout << "https://www.youtube.com/@CodyOQuinn\n";
    Terminal::Instance().SetColor(terme::color::kWhite);

    while (true)
    {
        //prevent exiting immediatelly after exiting a game (double consecutive esc)
        if(terme::TimeManager::Instance().GetTime() - menu_show_start_time < 0.2)
            continue;

        if (InputManager::Instance().IsKeyPressed(Key::kNum1))
            return std::make_unique<SpaceInvaders::SpaceInvadersLevel>();

        if (InputManager::Instance().IsKeyPressed(Key::kNum2))
            return std::make_unique<Platformer::EndlessRunnerLevel>();

        if (InputManager::Instance().IsKeyPressed(Key::kNum3))
            return std::make_unique<Platformer::PuzzleLevel>();

        if (InputManager::Instance().IsKeyPressed(Key::kNum4))
            return std::make_unique<Pong::PongLevel>();

        if (InputManager::Instance().IsKeyPressed(Key::kEsc))
            return nullptr;

#if DEBUG
        if (InputManager::Instance().IsKeyPressed(Key::kNum5))
            return std::make_unique<Platformer::CollisionsTestLevel>();

        if (InputManager::Instance().IsKeyPressed(Key::kNum6))
            return std::make_unique<Platformer::SortingLayerTestLevel>();
#endif
    }
}
