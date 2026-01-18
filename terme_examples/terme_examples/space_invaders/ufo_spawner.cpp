#include "ufo_spawner.h"
#include <terme/managers/time_manager.h>
#include "ufo.h"
#include <terme/core/simulation.h>
#include <nbkit/random_utils.h>
#include <terme/managers/debug_manager.h>

using terme::Direction;
using terme::TimeManager;
using std::shared_ptr;

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
			shared_ptr<Ufo> ufo = std::make_shared<Ufo>(xSpawnPos, ySpawnPos, ufoMoveDirection);
			terme::Simulation::Instance().TryAddEntity(ufo);
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