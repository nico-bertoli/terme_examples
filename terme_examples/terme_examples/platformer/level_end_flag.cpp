#include "level_end_flag.h"
#include "bunny.h"

#include <terme/entities/level.h>

using terme::Direction;
using terme::Level;
using terme::Model;

namespace Platformer
{
    LevelEndFlag::LevelEndFlag(Level* level, int x_pos, int y_pos) : Collider(x_pos, y_pos), level_(level) {}

    void LevelEndFlag::OnCollisionEnter(terme::Collider* other, Direction collision_dir)
    {
        if (dynamic_cast<Bunny*>(other))
            OnTouchBunny.Notify();
    }
}