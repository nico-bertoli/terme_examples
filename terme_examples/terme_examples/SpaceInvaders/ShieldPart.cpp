#include "ShieldPart.h"
#include <terme/core/simulation.h>
#include "Alien.h"
#include "Projectile.h"
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
            terme::AudioManager::Instance().PlayFx("Resources/Sounds/SpaceInvaders/ShieldBreak.wav");
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