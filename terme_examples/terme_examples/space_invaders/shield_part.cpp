#include "shield_part.h"
#include <terme/core/simulation.h>
#include "alien.h"
#include "projectile.h"
#include <terme/managers/audio_manager.h>

using std::shared_ptr;
using terme::Direction;

namespace SpaceInvaders
{
    void ShieldPart::OnCollisionEnter(shared_ptr<Collider>other, Direction collision_dir)
    {
        Collider::OnCollisionEnter(other, collision_dir);

        shared_ptr<Projectile> other_projectile = std::dynamic_pointer_cast<Projectile>(other);
        if (other_projectile != nullptr)
        {
            terme::AudioManager::Instance().PlayFx("resources/sounds/space_invaders/shield_break.wav");
            terme::Simulation::Instance().RemoveEntity(shared_from_this());
            return;
        }

        shared_ptr<Alien> other_alien = std::dynamic_pointer_cast<Alien>(other);
        if (other_alien != nullptr)
        {
            terme::Simulation::Instance().RemoveEntity(shared_from_this());
            return;
        }
    }
}