#include "aliens_controller.h"
#include "alien.h"
#include <terme/entities/level.h>
#include <terme/managers/time_manager.h>
#include "space_invaders_level.h"
#include <terme/managers/debug_manager.h>
#include <nbkit/random_utils.h>
#include <terme/entities/game_object.h>
#include <cassert>

using std::string;

namespace SpaceInvaders
{
	double AliensController::GetSpeedX() const
	{
		return kBaseMoveSpeed +
			GetEliminatedAliensMultiplier() * kAllAliensEliminatedMoveSpeedIncrement +
			GetWaveMultiplier() * kWaveNumberSpeedIncreaseFactor;
	}

	double AliensController::GetNextShotDelay()const
	{
		double delay = kBaseShootDelay -
			GetEliminatedAliensMultiplier() * kAllAliensEliminatedShootDelayReduction -
			GetWaveMultiplier() * kWaveNumberShotDelayReductionFactor +
			nbkit::random_utils::GetRandomDouble(-kShotsRandomness / 2, kShotsRandomness / 2);

		return delay > kMinShowDelay ? delay : kMinShowDelay;
	}

	double AliensController::GetEliminatedAliensMultiplier()const
	{
		return static_cast<double>(GetDestroyedAliensCount()) / (GetStartingAliensCount() - 1);
	}

	double AliensController::GetWaveMultiplier()const
	{
		return static_cast<double>((level->GetWaveNumber() - 1));
	}

	void AliensController::Reset(int aliens_count_x, int aliens_count_y)
	{
		aliens_count_ = aliens_count_x * aliens_count_y;
		aliensGrid.Resize(aliens_count_x, aliens_count_y);
		frontLine.Init(aliens_count_x);
	}

	void AliensController::Update()
	{
#if DEBUG
		string totBoost = "tot boost: " + std::to_string(GetSpeedX());
		string eliminationsBoost = " elim. multiplier:" + std::to_string(GetEliminatedAliensMultiplier());
		string waveBoost = " wave multiplier:" + std::to_string(GetWaveMultiplier());
		string debugStr = totBoost + " | " + eliminationsBoost + " | " + waveBoost;
		terme::DebugManager::Instance().PrintGenericLog(debugStr, 0);
#endif

		HandleMovement();
		HandleShooting();
	}

	void AliensController::RegisterAlien(Alien* alien, int x_pos, int y_pos)
	{
		assert(alien != nullptr);
		assert(y_pos >= 0 && size_t(y_pos) < GetAliensGridHeight());
		assert(x_pos >= 0 && size_t(x_pos) < GetAliensGridWidth());

		aliensGrid.Get(size_t(x_pos), size_t(y_pos)) = alien;

		if (GetAliensGridHeight() > 0 && size_t(y_pos) == GetAliensGridHeight() - 1)
			frontLine.Set(size_t(x_pos), alien);

		alien->on_move.Subscribe
		(
			[this](terme::GameObject* alien_obj, terme::Direction dir) { OnAlienMovedCallback(alien_obj, dir); }
		);

		alien->on_destroy_event.Subscribe
		(
			[this](terme::Collider* alien_obj) { OnAlienDestroyedCallback(alien_obj); }
		);
	}

	void AliensController::OnAlienMovedCallback(terme::GameObject* alien, terme::Direction move_direction)
	{
		if (move_direction == x_move_direction_ && alien != nullptr)
		{
			int alien_x_pos = alien->GetPosX();
			if (
				alien_x_pos == level->GetScreenPadding() ||
				alien_x_pos == level->GetWorldSizeX() - level->GetScreenPadding() - alien->GetModelWidth()
				)
				OnAliensReachMargin();
		}
	}

	void AliensController::MoveAliens(terme::Direction dir, double speed)
	{
		for (size_t y = 0; y < GetAliensGridHeight(); ++y)
			for (size_t x = 0; x < GetAliensGridWidth(); ++x)
			{
				Alien* alien = aliensGrid.Get(x, y);
				if (alien)
					alien->TryMove(dir, speed);
			}
	}

	void AliensController::OnAliensReachMargin()
	{
		x_move_direction_ = terme::direction_utils::GetInverseDirection(x_move_direction_);
		is_time_to_move_aliens_down_ = true;
	}

	void AliensController::OnAlienDestroyedCallback(terme::Collider* alien_obj)
	{
		Alien* alien = dynamic_cast<Alien*>(alien_obj);
		if (alien != nullptr)
			UnregisterAlien(alien);
	}

	void AliensController::UnregisterAlien(Alien* alien)
	{
		const size_t gx = alien->GetIndexInGridX();
		const size_t gy = alien->GetIndexInGridY();

		if (aliensGrid.Get(gx, gy) != alien)
			return;

		frontLine.ReplaceDestroyedElement(alien, aliensGrid);
		aliensGrid.Get(gx, gy) = nullptr;

		assert(aliens_count_ > 0);
		--aliens_count_;
		if (aliens_count_ == 0)
			on_wave_completed.Notify();
	}

	void AliensController::HandleShooting()
	{
		if (terme::TimeManager::Instance().GetTime() - last_shot_time_ > shot_delay_)
		{
			Alien* front_line_alien = frontLine.GetRandom();
			if (front_line_alien == nullptr)
				return;
			front_line_alien->Shot();
			last_shot_time_ = terme::TimeManager::Instance().GetTime();
			shot_delay_ = GetNextShotDelay();
		}
	}

	void AliensController::HandleMovement()
	{
		if (is_time_to_move_aliens_down_)
		{
			if (frontLine.GetMinY() - 1 <= SpaceInvadersLevel::kGameOverY)
				on_ground_touched.Notify();

			MoveAliens(terme::Direction::kDown, 9999);
			is_time_to_move_aliens_down_ = false;
		}
		else
			MoveAliens(x_move_direction_, GetSpeedX());
	}
}