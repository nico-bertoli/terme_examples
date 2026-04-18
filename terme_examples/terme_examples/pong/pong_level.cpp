#include "pong_level.h"

#include "pong_ball.h"
#include "pong_bar.h"

#include <terme/core/simulation.h>
#include <terme/managers/audio_manager.h>
#include <terme/printers/ui_printer.h>

#include <memory>
#include <utility>

namespace Pong
{
	size_t PongLevel::score_player_1_ = 0;
	size_t PongLevel::score_player_2_ = 0;

	void PongLevel::LoadInSimulation()
	{
		terme::Level::LoadInSimulation();
		terme::Simulation& simulation = terme::Simulation::Instance();

		//--------------- bars general settings
		char bars_char = '#';
		int starting_pos_x = GetWorldSizeX() / 2 - 1;
		int bars_size = 8;
		double bars_move_speed = 32;
		double deflect_factor = 2.5;

		//--------------- bottom bar

		{
			std::unique_ptr<PongBar> bottom_bar = std::make_unique<PongBar>
			(
				starting_pos_x,
				GetScreenPadding(),
				bars_size,
				1,
				bars_char,
				bars_move_speed,
				deflect_factor,
				true
			);
			simulation.TryAddEntity(std::move(bottom_bar));
		}

		//--------------- top bar
		{
			std::unique_ptr<PongBar> top_bar = std::make_unique<PongBar>
			(
				starting_pos_x,
				GetWorldSizeY() - GetScreenPadding() - 1,
				bars_size,
				1,
				bars_char,
				bars_move_speed,
				deflect_factor,
				false
			);
			simulation.TryAddEntity(std::move(top_bar));
		}

		double ball_speed = 16;
		{
			std::unique_ptr<PongBall> pong_ball = std::make_unique<PongBall>(this, GetWorldSizeX() / 2, GetWorldSizeY() / 2, ball_speed);
			pong_ball->on_goal.Subscribe([this]() { OnGameOver(); });
			simulation.TryAddEntity(std::move(pong_ball));
		}
		RefreshHeader();
	}

	void PongLevel::IncreaseP1Score()
	{
		++score_player_1_;
		RefreshHeader();
	}

	void PongLevel::IncreaseP2Score()
	{
		++score_player_2_;
		RefreshHeader();
	}

	void PongLevel::RefreshHeader()
	{
		terme::Simulation::Instance().GetUIPrinter().
			PrintOnHeader(std::to_string(score_player_1_) + " - " + std::to_string(score_player_2_), 0, terme::color::kWhite);
	}

	void PongLevel::OnPostGameOverDelayEnded()
	{
		terme::Level::OnPostGameOverDelayEnded();
		Terminate();
	}

	void PongLevel::OnGameOver()
	{
		terme::Level::OnGameOver();
		terme::AudioManager::Instance().PlayFx("resources/sounds/pong/goal.wav");
	}
}