#include "player_tank.h"
#include "settings.h"
#include <terme/core/simulation.h>
#include <terme/input_manager/input_manager.h>
#include "player_projectile.h"
#include <terme/managers/time_manager.h>
#include <terme/entities/level.h>
#include <terme/managers/audio_manager.h>
#include "space_invaders_level.h"

#include <memory>
#include <utility>

using terme::Direction;
using terme::Model;

namespace SpaceInvaders
{
    void PlayerTank::Update()
    {
        if (terme::Simulation::Instance().GetActiveLevel().IsGameOver())
            return;

        HandleMovement();
        HandleShooting();
    }

    void PlayerTank::HandleMovement()
    {
        if (terme::InputManager::Instance().IsKeyPressed(terme::Key::kA) || terme::InputManager::Instance().IsKeyPressed(terme::Key::kArrowLeft))
            TryMove(Direction::kLeft, kMoveSpeed);
        else if (terme::InputManager::Instance().IsKeyPressed(terme::Key::kD) || terme::InputManager::Instance().IsKeyPressed(terme::Key::kArrowRight))
            TryMove(Direction::kRight, kMoveSpeed);
    }

    void PlayerTank::HandleShooting()
    {
        if (level_->IsLoadingWave())
            return;

        if (terme::InputManager::Instance().IsKeyPressed(terme::Key::kSpace))
        {
            double time = terme::TimeManager::Instance().GetTime();

            if constexpr (space_invaders::cheats::kFastFire)
            {
                if (time - last_time_shot_ > 0.08)
                    last_time_shot_ = -99;
            }

            if (time - last_time_shot_ > kShotsDelay)
            {
                last_time_shot_ = terme::TimeManager::Instance().GetTime();
                std::unique_ptr<PlayerProjectile> projectile =
                    std::make_unique<PlayerProjectile>(GetMidPosX(), GetMaxPosY() + 1, Direction::kUp, kProjectileSpeed);
                terme::Simulation::Instance().TryAddEntity(std::move(projectile));
                terme::AudioManager::Instance().PlayFx("resources/sounds/space_invaders/shot1.wav");
            }
        }
    }

    void PlayerTank::TakeDamage()
    {
        if constexpr (space_invaders::cheats::kInvincibility == false)
        {
            health_--;
            on_damage_taken.Notify(health_);

            if (health_ > 0)
                terme::AudioManager::Instance().PlayFx("resources/sounds/space_invaders/damage_taken.wav");
        }
    }
}