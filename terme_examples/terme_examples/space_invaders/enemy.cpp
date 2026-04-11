#include "enemy.h"
#include <terme/managers/time_manager.h>
#include <terme/core/simulation.h>
#include "space_invaders_level.h"

namespace SpaceInvaders
{
	void Enemy::OnDestroy()
	{
		SpaceInvadersLevel* level = dynamic_cast<SpaceInvadersLevel*>(&terme::Simulation::Instance().GetActiveLevel());
		assert(level != nullptr);
		level->IncreasePlayerScore(GetScore());
		terme::Simulation::Instance().SpawnParticles
		(
			GetPosX(),
			GetPosY(),
			GetModelWidth(),
			GetModelHeight(),
			'*',
			GetDestroyedParticlesColor(),
			8,	//speed
			5,	//lifetime
			1.5	//density
		);
	}
}