#include "PlayerProjectile.h"
#include <terme/core/simulation.h>
#include "Enemy.h"
#include "SpaceInvadersLevel.h"

using terme::Model;
using std::shared_ptr;
using terme::Direction;

namespace SpaceInvaders
{
	void PlayerProjectile::OnCollisionEnter(shared_ptr<Collider> other, Direction collision_dir)
	{
		Projectile::OnCollisionEnter(other, collision_dir);
		shared_ptr<Enemy> other_enemy = std::dynamic_pointer_cast<Enemy>(other);
		if (other_enemy != nullptr)
		{
			terme::Simulation::Instance().RemoveEntity(other_enemy);
		}
	}
}
