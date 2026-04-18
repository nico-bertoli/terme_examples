#include "endless_runner_level.h"

#include "bunny.h"
#include "obstacles_spawner.h"

#include <terme/core/frame.h>
#include <terme/core/simulation.h>
#include <terme/entities/static_collider.h>
#include <terme/input_manager/input_manager.h>
#include <terme/managers/audio_manager.h>
#include <terme/managers/persistence_manager.h>
#include <terme/printers/simulation_printer.h>
#include <terme/printers/ui_printer.h>

#include <nbkit/random_utils.h>

#include <memory>
#include <string>
#include <utility>
#include <vector>

using std::string;
using WindowPosition = terme::UIPrinter::WindowPosition;

namespace Platformer
{
    void EndlessRunnerLevel::OnPostGameOverDelayEnded()
    {
        terme::Level::OnPostGameOverDelayEnded();
        int score = static_cast<int>(GetLevelTime());

        int saved_best_score = terme::PersistenceManager::LoadBestScore(GetPersistenceFilePath());
        if (score > saved_best_score)
            terme::PersistenceManager::SaveBestScore(GetPersistenceFilePath(), score);

        ShowGameOverScreen(score, saved_best_score);
        terme::AudioManager::Instance().PlayFx("resources/sounds/platform/show_end_screen.wav");
    }

    void EndlessRunnerLevel::ShowGameOverScreen(int score, int best_score)
    {
        //setup gameover message
        string message_ending = score > best_score ? "new record!" : ("best: " + std::to_string(best_score));
        string message = "you survived for " + std::to_string(score) + " seconds, " + message_ending;

        //center message
        string left_spacing = "";
        for (int i = 0; i < (42 - message.size()) / 2; ++i)
            left_spacing += " ";
        message = left_spacing + message;

        game_over_window_.WriteString(message, '$');

        terme::Simulation::Instance().GetUIPrinter().PrintWindow(game_over_window_, terme::color::kWhite, WindowPosition::kCenterXTopY);
    }

    void EndlessRunnerLevel::OnGameOver()
    {
        if (IsGameOver())
            return;

        terme::Level::OnGameOver();
        terme::AudioManager::Instance().StopMusic();
        terme::AudioManager::Instance().PlayFx("resources/sounds/platform/game_over.wav");
    }

    void EndlessRunnerLevel::LoadInSimulation()
    {
        terme::Level::LoadInSimulation();
        terme::Simulation& simulation = terme::Simulation::Instance();

        //----------------- bunny setup
        int bunny_starting_y = static_cast<int>(simulation.GetScreenPadding());
        std::unique_ptr<Bunny> bunny = std::make_unique<Bunny>(9, bunny_starting_y);
        bunny->on_obstacle_hit.Subscribe([this]() { OnGameOver(); });
        simulation.TryAddEntity(std::move(bunny));

        //----------------- obstacles spawner setup
        int spawner_pos_x = GetWorldSizeX() - GetScreenPadding();

        std::vector<double> min_spawn_delays
        {
            0.9,
            0.4,
            0.28,
            0.25,
            0.22,
            0.22
        };
        std::vector<double> max_spawn_delays
        {
            1,
            0.6,
            0.43,
            0.4,
            0.32,
            0.22
        };
        std::vector<double> move_speeds
        {
            -8,
            -16,
            -32,
            -40,
            -48,
            -48
        };

        double increase_intensity_every_seconds = 10;
        double stop_spawning_when_phase_changes_duration = 1;

        std::vector<int> y_spawn_points = { 4,8,12 };

        std::unique_ptr<ObstaclesSpawner> spawner = std::make_unique<ObstaclesSpawner>
        (
            spawner_pos_x,
            min_spawn_delays,
            max_spawn_delays,
            move_speeds,
            y_spawn_points,
            increase_intensity_every_seconds,
            stop_spawning_when_phase_changes_duration
        );

        simulation.TryAddEntity(std::move(spawner));
        PlayRandomMusic();
    }

    void EndlessRunnerLevel::PlayRandomMusic()
    {
        const char* random_music = kMusicFiles[nbkit::random_utils::GetRandomInt(0, static_cast<int>(kMusicFiles.size() - 1))];
        terme::AudioManager::Instance().PlayMusic(random_music);
    }

    void EndlessRunnerLevel::Update()
    {
        terme::Level::Update();
        double run_time = terme::Simulation::Instance().GetActiveLevel().GetLevelTime();
        int new_time = static_cast<int>(run_time);
        if (shown_time_ != new_time)
        {
            string header = "TIME: " + std::to_string(new_time);
            terme::Simulation::Instance().GetUIPrinter().PrintOnHeader(header, 0, terme::color::kWhite);
            shown_time_ = new_time;
        }
    }

}
