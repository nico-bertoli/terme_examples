#pragma once
#include <terme/entities/i_simulation_entity.h>
#include <terme/core/direction.h>
#include <nbkit/random_utils.h>

namespace SpaceInvaders
{
	class UfoSpawner : public terme::ISimulationEntity
	{
		//---------------------------------------------------------- Settings
	private:
		static constexpr int kMinSpawnDelay = 15;
		static constexpr int kMaxSpawnDelay = 25;

		//---------------------------------------------------------- Fields
	private:
		int xSpawnPos;
		int ySpawnPos;
		terme::Direction ufoMoveDirection;
		double lastTimeSpawned = -1;
		double nextSpawnTime = -1;

		//---------------------------------------------------------- Methods
	public:
		UfoSpawner(int xSpawnPos, int ySpawnPos, terme::Direction ufoMoveDirection);
	protected:
		void Update()override;
	private:
		void SetNextSpawnDelay();
	};
}
