#pragma once

#include <terme/core/direction.h>
#include <terme/entities/i_simulation_entity.h>

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
		int x_spawn_pos_;
		int y_spawn_pos_;
		terme::Direction ufo_move_direction_;
		double next_spawn_time_ = -1;

		//---------------------------------------------------------- Methods
	public:
		UfoSpawner(int x_spawn_pos, int y_spawn_pos, terme::Direction ufo_move_direction);
	protected:
		void Update() override;
	private:
		void SetNextSpawnDelay();
	};
}
