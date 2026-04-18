#include "aliens_controller.h"

#include "alien.h"
#include "space_invaders_level.h"

#include <terme/entities/game_object.h>
#include <terme/entities/level.h>
#include <terme/managers/debug_manager.h>
#include <terme/managers/time_manager.h>

#include <nbkit/random_utils.h>

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
		return static_cast<double>((level_->GetWaveNumber() - 1));
	}

	void AliensController::Reset(int aliens_count_x, int aliens_count_y)
	{
		aliens_count_ = aliens_count_x * aliens_count_y;
		aliens_grid_.Resize(aliens_count_x, aliens_count_y);
		front_line_.Init(aliens_count_x);
	}

	void AliensController::Update()
	{
#if DEBUG
		string tot_boost = "tot boost: " + std::to_string(GetSpeedX());
		string eliminations_boost = " elim. multiplier:" + std::to_string(GetEliminatedAliensMultiplier());
		string wave_boost = " wave multiplier:" + std::to_string(GetWaveMultiplier());
		string debug_str = tot_boost + " | " + eliminations_boost + " | " + wave_boost;
		terme::DebugManager::Instance().PrintGenericLog(debug_str, 0);
#endif

		HandleMovement();
		HandleShooting();
	}

	void AliensController::RegisterAlien(Alien* alien, int x_pos, int y_pos)
	{
		assert(alien != nullptr);
		assert(y_pos >= 0 && size_t(y_pos) < GetAliensGridHeight());
		assert(x_pos >= 0 && size_t(x_pos) < GetAliensGridWidth());

		aliens_grid_.Get(size_t(x_pos), size_t(y_pos)) = alien;

		if (GetAliensGridHeight() > 0 && size_t(y_pos) == GetAliensGridHeight() - 1)
			front_line_.Set(alien);

		alien->on_move_.Subscribe
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
				alien_x_pos == level_->GetScreenPadding() ||
				alien_x_pos == level_->GetWorldSizeX() - level_->GetScreenPadding() - alien->GetModelWidth()
				)
				OnAliensReachMargin();
		}
	}

	void AliensController::MoveAliens(terme::Direction dir, double speed)
	{
		for (size_t y = 0; y < GetAliensGridHeight(); ++y)
			for (size_t x = 0; x < GetAliensGridWidth(); ++x)
			{
				Alien* alien = aliens_grid_.Get(x, y);
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

		if (aliens_grid_.Get(gx, gy) != alien)
			return;

		front_line_.ReplaceDestroyedElement(alien, aliens_grid_);
		aliens_grid_.Get(gx, gy) = nullptr;

		assert(aliens_count_ > 0);
		--aliens_count_;
		if (aliens_count_ == 0)
			on_wave_completed.Notify();
	}

	void AliensController::HandleShooting()
	{
		if (terme::TimeManager::Instance().GetTime() - last_shot_time_ > shot_delay_)
		{
			Alien* front_line_alien;
			if (front_line_.TryGetRandom(front_line_alien) == false)
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
			if (front_line_.GetMinY() - 1 <= SpaceInvadersLevel::kGameOverY)
				on_ground_touched.Notify();

			MoveAliens(terme::Direction::kDown, 9999);
			is_time_to_move_aliens_down_ = false;
		}
		else
			MoveAliens(x_move_direction_, GetSpeedX());
	}
}