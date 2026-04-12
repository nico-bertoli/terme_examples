#include "enemy_projectile.h"
#include "player_tank.h"

using terme::Direction;
using terme::Model;

namespace SpaceInvaders
{
	void EnemyProjectile::OnCollisionEnter(terme::Collider* other, Direction collision_dir)
	{
		Projectile::OnCollisionEnter(other, collision_dir);

		PlayerTank* other_tank = dynamic_cast<PlayerTank*>(other);
		if (other_tank != nullptr)
			other_tank->TakeDamage();
	}
}