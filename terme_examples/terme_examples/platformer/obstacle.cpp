#include "obstacle.h"

#include <terme/managers/audio_manager.h>

using terme::Direction;
using terme::Model;

namespace Platformer
{
    Model Obstacle::model_ = {};

    void Obstacle::OnCollisionEnter(terme::Collider* other, Direction collision_dir)
    {
        terme::AudioManager::Instance().PlayFx("resources/sounds/platform/hit.wav", 0.2);
    }

    void Obstacle::InitModel()
    {
        if (model_.GetSizeY() == 0)
            model_ = CreateModelUsingChar('#', size_t(4), size_t(2));
        SetModel(model_);
    }

}