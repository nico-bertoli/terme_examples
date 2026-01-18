#include "pong_bar.h"
#include <terme/input_manager/input_manager.h>
#include <terme/core/simulation.h>
#include <terme/entities/level.h>

using terme::Direction;

namespace Pong
{
    PongBar::PongBar
    (
        int x_pos,
        int y_pos,
        size_t size_x,
        size_t size_y,
        char model_char,
        double move_speed,
        double deflect_ball_factor,
        bool is_bottom_bar
    ) :
        VariableSizeCollider(x_pos, y_pos, size_x, size_y, model_char),
        move_speed_(move_speed),
        deflect_ball_factor_(deflect_ball_factor),
        is_bottom_bar_(is_bottom_bar)
    {
    }

    void PongBar::Update()
    {
        if (terme::Simulation::Instance().GetActiveLevel()->IsGameOver())
            return;

        if ((is_bottom_bar_ && terme::InputManager::Instance().IsKeyPressed(terme::Key::kA)) || !is_bottom_bar_ && terme::InputManager::Instance().terme::InputManager::Instance().IsKeyPressed(terme::Key::kArrowLeft))
            TryMove(Direction::kLeft, move_speed_);
        else if ((is_bottom_bar_ && terme::InputManager::Instance().IsKeyPressed(terme::Key::kD)) || (!is_bottom_bar_ && terme::InputManager::Instance().IsKeyPressed(terme::Key::kArrowRight)))
            TryMove(Direction::kRight, move_speed_);
    }

}