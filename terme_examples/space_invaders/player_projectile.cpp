#include "player_projectile.h"
#include <terme/core/simulation.h>
#include "enemy.h"
#include "space_invaders_level.h"

using terme::Model;
using terme::Direction;

namespace SpaceInvaders
{
	void PlayerProjectile::OnCollisionEnter(terme::Collider* other, Direction collision_dir)
	{
		Projectile::OnCollisionEnter(other, collision_dir);
		Enemy* other_enemy = dynamic_cast<Enemy*>(other);
		if (other_enemy != nullptr)
		{
			terme::Simulation::Instance().RemoveEntity(other_enemy);
		}
	}
}
