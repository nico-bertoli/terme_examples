#include "ufo_spawner.h"

#include "ufo.h"

#include <terme/core/simulation.h>
#include <terme/managers/time_manager.h>

#include <nbkit/random_utils.h>

#include <memory>
#include <utility>

using terme::Direction;
using terme::TimeManager;

namespace SpaceInvaders
{
	UfoSpawner::UfoSpawner(int x_spawn_pos, int y_spawn_pos, Direction ufo_move_direction)
		: x_spawn_pos_(x_spawn_pos), y_spawn_pos_(y_spawn_pos), ufo_move_direction_(ufo_move_direction)
	{
		SetNextSpawnDelay();
	}

	void UfoSpawner::Update()
	{
		if (TimeManager::Instance().GetTime() > next_spawn_time_)
		{
			std::unique_ptr<Ufo> ufo = std::make_unique<Ufo>(x_spawn_pos_, y_spawn_pos_, ufo_move_direction_);
			terme::Simulation::Instance().TryAddEntity(std::move(ufo));
			SetNextSpawnDelay();
		}
	}

	void UfoSpawner::SetNextSpawnDelay()
	{
		next_spawn_time_ =
			TimeManager::Instance().GetTime() +
			nbkit::random_utils::GetRandomDouble(kMinSpawnDelay, kMaxSpawnDelay);
	}
}