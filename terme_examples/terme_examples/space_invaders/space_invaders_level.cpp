#include "space_invaders_level.h"
#include <terme/core/simulation.h>
#include "alien_low_score.h"
#include "alien_mid_score.h"
#include "alien_high_score.h"
#include "player_tank.h"
#include "aliens_controller.h"
#include <terme/managers/audio_manager.h>
#include <terme/managers/persistence_manager.h>
#include <terme/printers/ui_printer.h>
#include <terme/printers/printer.h>
#include <terme/managers/time_manager.h>
#include "shield_part.h"
#include "ufo_spawner.h"
#include "alien.h"

#include <memory>
#include <string>
#include <utility>
#include <stdexcept>

using WindowPosition = terme::UIPrinter::WindowPosition;
using terme::Simulation;
using terme::Direction;
using std::string;
using std::vector;
using std::type_info;

namespace SpaceInvaders
{


	int SpaceInvadersLevel::GetWorldSizeX() const
	{
		return
			kAliensCountX * kAlienWidth
			+
			(kAliensCountX - 1) * kSpaceBetweenAliensX
			+
			2 * kAliensSpaceFromMarginsX
			+
			2 * GetScreenPadding();
	}

	void SpaceInvadersLevel::LoadInSimulation()
	{
		terme::Level::LoadInSimulation();

		wave_number_ = 1;
		score_ = 0;
		is_loading_wave_ = false;

		//------------ aliens controller
		{
			std::unique_ptr<AliensController> controller = std::make_unique<AliensController>(this);
			aliens_controller_ = controller.get();
			aliens_controller_->on_wave_completed.Subscribe([this]() { OnWaveCompleted(); });
			aliens_controller_->on_ground_touched.Subscribe([this]() { OnGameOver(); });
			Simulation::Instance().TryAddEntity(std::move(controller));
		}

		//------------ aliens, player tank, and header
		LoadAliens();
		LoadPlayerTank();
		InitHeader();

		//------------ ufo spawner
		{
			std::unique_ptr<UfoSpawner> ufo_spawner =
				std::make_unique<UfoSpawner>(GetWorldSizeX() - 5, GetWorldSizeY() - GetScreenPadding() - 2, Direction::kLeft);
			Simulation::Instance().TryAddEntity(std::move(ufo_spawner));
		}

#if !CHEAT_SPACEINV_DISABLE_SHIELDS
		//------------ shields
		vector<size_t> shields_spacing = { 9,9,10,9,9 };
		LoadShields(9, 4, shields_spacing);
#endif
	}

	void SpaceInvadersLevel::OnGameOver()
	{
		if (IsGameOver())
			return;

		terme::Level::OnGameOver();
		terme::AudioManager::Instance().StopMusic();
		terme::AudioManager::Instance().PlayFx("resources/sounds/space_invaders/game_over.wav");
	}

	void SpaceInvadersLevel::Update()
	{
		terme::Level::Update();
		LoadNewWave();
	}

	void SpaceInvadersLevel::OnPostGameOverDelayEnded()
	{
		terme::Level::OnPostGameOverDelayEnded();

		int saved_best_score = terme::PersistenceManager::LoadBestScore(GetPersistenceFilePath());
		if (score_ > saved_best_score)
			terme::PersistenceManager::SaveBestScore(GetPersistenceFilePath(), score_);

		ShowGameOverScreen(score_, saved_best_score);
		terme::AudioManager::Instance().PlayFx("resources/sounds/space_invaders/show_end_screen.wav");
	}

	void SpaceInvadersLevel::ShowGameOverScreen(int score, int best_score)
	{
		string message_ending = score > best_score ? ", new record!" : (", best: " + std::to_string(best_score));
		string message = "score: " + std::to_string(score) + message_ending;

		//center message
		string left_spacing = "";
		for (int i = 0; i < (42 - message.size()) / 2; ++i)
			left_spacing += " ";
		message = left_spacing + message;

		game_over_window_.WriteString(message, '$');

		Simulation::Instance().GetUIPrinter().PrintWindow(game_over_window_, terme::color::kWhite, WindowPosition::kCenterXCenterY);
	}

	void SpaceInvadersLevel::IncreasePlayerScore(size_t increment)
	{
		score_ += static_cast<int>(increment);
		PrintScore();
	}

	void SpaceInvadersLevel::InitHeader()
	{
		PrintScore();
		PrintWave();
	}

	void SpaceInvadersLevel::PrintScore()
	{
		string score_string = "score: " + std::to_string(score_);
		int x_pos = static_cast<int>(Simulation::Instance().GetScreenSizeX() / 2 - score_string.size() / 2);
		Simulation::Instance().GetUIPrinter().PrintOnHeader(score_string, x_pos, terme::color::kWhite);
	}

	void SpaceInvadersLevel::PrintHealth(size_t health)
	{
		int x_pos = terme::Printer::kLeftMarginSize;

		string health_str = "";
		for (int i = 1; i <= PlayerTank::kMaxHealth; i++)
			health_str += i <= health ? "<3 " : "  ";

		Simulation::Instance().GetUIPrinter().PrintOnHeader(health_str, x_pos, terme::color::kRed);
	}

	void SpaceInvadersLevel::PrintWave()
	{
		terme::UIPrinter ui_printer = Simulation::Instance().GetUIPrinter();
		string wave_str = "wave: " + std::to_string(wave_number_);

		int wave_string_pos_x = static_cast<int>(ui_printer.GetMaxTerminalX() - wave_str.length());
		Simulation::Instance().GetUIPrinter().PrintOnHeader(wave_str, wave_string_pos_x, terme::color::kWhite);
	}


	void SpaceInvadersLevel::LoadPlayerTank()
	{
		int x_pos = GetWorldSizeX() / 2;
		int y_pos = GetScreenPadding();
		std::unique_ptr<PlayerTank> player_tank = std::make_unique<PlayerTank>(x_pos, y_pos, this);
		PlayerTank* tank = player_tank.get();
		tank->on_damage_taken.Subscribe([this](size_t health) { OnPlayerTakesDamage(health); });
		PrintHealth(tank->GetHealth());
		Simulation::Instance().TryAddEntity(std::move(player_tank));
	}

	void SpaceInvadersLevel::OnPlayerTakesDamage(size_t remaining_health)
	{
		PrintHealth(remaining_health);
		if (remaining_health == 0)
			OnGameOver();
	}

	void SpaceInvadersLevel::LoadAliens()
	{
		aliens_controller_->Reset(kAliensCountX, kAliensCountY);
		int y_pos = GetWorldSizeY() - GetScreenPadding() - kAliensSpaceFromTopMargin - kAlienHeight;
		int y_index = 0;
		while (y_index < kAliensCountY)
		{
			AddAliensRowToSimulation(y_pos, y_index);
			y_pos -= (kAlienHeight + kSpaceBetweenAliensY);
			++y_index;
		}
	}

	const type_info& SpaceInvadersLevel::GetAlienTypeForRow(int y_index)
	{
		if (y_index == 0)
			return typeid(AlienHighScore);
		else if (y_index < 3)
			return typeid(AlienMidScore);
		else
			return typeid(AlienLowScore);
	}

	void SpaceInvadersLevel::AddAliensRowToSimulation(int y_pos, int y_index)
	{
		int x_pos = GetScreenPadding() + kAliensSpaceFromMarginsX;
		const type_info& alien_type = GetAlienTypeForRow(y_index);
		int x_index = 0;
		while (x_index < kAliensCountX)
		{
			std::unique_ptr<Alien> alien = CreateAlienOfType(alien_type, x_pos, y_pos, x_index, y_index);
			Alien* raw = alien.get();
			Simulation::Instance().TryAddEntity(std::move(alien));
			aliens_controller_->RegisterAlien(raw, x_index, y_index);

			++x_index;
			x_pos += kAlienWidth + kSpaceBetweenAliensX;
		}
	}

	std::unique_ptr<Alien> SpaceInvadersLevel::CreateAlienOfType(const type_info& alien_type, int x_pos, int y_pos, int x_index, int y_index)
	{
		if (alien_type == typeid(AlienHighScore))
			return std::make_unique<AlienHighScore>(x_pos, y_pos, x_index, y_index);
		else if (alien_type == typeid(AlienMidScore))
			return std::make_unique<AlienMidScore>(x_pos, y_pos, x_index, y_index);
		else if (alien_type == typeid(AlienLowScore))
			return std::make_unique<AlienLowScore>(x_pos, y_pos, x_index, y_index);
		else
			throw std::invalid_argument("invalid alien type receiveds");
	}

	void SpaceInvadersLevel::LoadShields(int y_pos, size_t count, vector<size_t> spacing)
	{
		for (int i = 0; i < count; ++i)
		{
			size_t shield_spacing = 0;
			for (int j = 0; j <= i; ++j)
				shield_spacing += spacing[j];

			int x = static_cast<int>(GetScreenPadding() + shield_spacing + kShieldSizeX * i);
			LoadShield(x, y_pos);
		}
	}

	void SpaceInvadersLevel::LoadShield(int x_pos, int y_pos)
	{
		for (int y = 0; y < kShieldSizeY; ++y)
		{
			for (int x = 0; x < kShieldSizeX; ++x)
			{
				// bottom hole
				if (y == 0 && x > 1 && x < kShieldSizeX - 2)
					continue;
				//smooth angles
				if (y == kShieldSizeY - 1 && (x == 0 || x == kShieldSizeX - 1))
					continue;

				std::unique_ptr<ShieldPart> shield_part = std::make_unique<ShieldPart>(x_pos + x, y_pos + y);
				Simulation::Instance().TryAddEntity(std::move(shield_part));
			}
		}
	}

	void SpaceInvadersLevel::OnWaveCompleted()
	{
		terme::AudioManager::Instance().PlayFx("resources/sounds/space_invaders/wave_complete.wav");
		is_loading_wave_ = true;
		wave_number_++;
		PrintWave();
		started_loading_wave_time_ = terme::TimeManager::Instance().GetTime();
	}

	void SpaceInvadersLevel::LoadNewWave()
	{
		if (is_loading_wave_ && terme::TimeManager::Instance().GetTime() - started_loading_wave_time_ > kLoadWaveTime)
		{
			LoadAliens();
			is_loading_wave_ = false;
		}
	}
}