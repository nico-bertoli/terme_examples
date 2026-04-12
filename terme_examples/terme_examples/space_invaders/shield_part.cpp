#include "shield_part.h"
#include <terme/core/simulation.h>
#include "alien.h"
#include "projectile.h"
#include <terme/managers/audio_manager.h>

using terme::Direction;

namespace SpaceInvaders
{
    void ShieldPart::OnCollisionEnter(terme::Collider* other, Direction collision_dir)
    {
        Collider::OnCollisionEnter(other, collision_dir);

        Projectile* other_projectile = dynamic_cast<Projectile*>(other);
        if (other_projectile != nullptr)
        {
            terme::AudioManager::Instance().PlayFx("resources/sounds/space_invaders/shield_break.wav");
            terme::Simulation::Instance().RemoveEntity(this);
            return;
        }

        Alien* other_alien = dynamic_cast<Alien*>(other);
        if (other_alien != nullptr)
        {
            terme::Simulation::Instance().RemoveEntity(this);
            return;
        }
    }
}