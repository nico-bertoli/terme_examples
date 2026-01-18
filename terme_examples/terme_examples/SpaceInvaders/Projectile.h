#pragma once
#include <terme/entities/moving_straight_object.h>
#include <terme/core/simulation.h>
#include "Enemy.h"
#include "ShieldPart.h"

namespace SpaceInvaders
{
    class Projectile : public terme::MovingStraightObject
    {
        //---------------------------------------------------------- Methods
    public:
        using MovingStraightObject::MovingStraightObject;

    protected:
        bool CanExitScreenSpace() const override { return false; }
        double GetGravityScale() const override { return 0; }
        void OnCollisionEnter(std::shared_ptr<Collider> other, terme::Direction collision_dir) override
        {
            MovingStraightObject::OnCollisionEnter(other, collision_dir);
            terme::Direction collision_opposite_direction = terme::direction_utils::GetInverseDirection(collision_dir);

            std::shared_ptr<Enemy> other_enemy = std::dynamic_pointer_cast<Enemy>(other);
            if (other_enemy == nullptr)
            {
                terme::Simulation::Instance().SpawnParticles
                (
                    GetPosX(),
                    GetPosY(),
                    GetModelWidth(),
                    GetModelHeight(),
                    '*',
                    GetColor(),
                    8,     //speed
                    3,      //movement life
                    4,      //density
                    collision_opposite_direction
                );
            }

            terme::Simulation::Instance().RemoveEntity(shared_from_this());
        }
    };
}