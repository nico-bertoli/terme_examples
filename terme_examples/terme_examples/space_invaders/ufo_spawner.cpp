#include "ufo_spawner.h"
#include <terme/managers/time_manager.h>
#include "ufo.h"
#include <terme/core/simulation.h>
#include <nbkit/random_utils.h>
#include <terme/managers/debug_manager.h>

#include <memory>
#include <utility>

using terme::Direction;
using terme::TimeManager;

namespace SpaceInvaders
{
	UfoSpawner::UfoSpawner(int xSpawnPos, int ySpawnPos, Direction ufoMoveDirection)
		: xSpawnPos(xSpawnPos), ySpawnPos(ySpawnPos), ufoMoveDirection(ufoMoveDirection)
	{
		SetNextSpawnDelay();
	}

	void UfoSpawner::Update()
	{
		if (TimeManager::Instance().GetTime() > nextSpawnTime)
		{
			std::unique_ptr<Ufo> ufo = std::make_unique<Ufo>(xSpawnPos, ySpawnPos, ufoMoveDirection);
			terme::Simulation::Instance().TryAddEntity(std::move(ufo));
			lastTimeSpawned = TimeManager::Instance().GetTime();
			SetNextSpawnDelay();
		}
	}

	void UfoSpawner::SetNextSpawnDelay()
	{
		nextSpawnTime =
			TimeManager::Instance().GetTime() +
			nbkit::random_utils::GetRandomDouble(kMinSpawnDelay, kMaxSpawnDelay);
	}
}