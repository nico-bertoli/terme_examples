#include "collisions_test_level.h"

#include "bunny.h"
#include "obstacle.h"
#include "obstacles_spawner.h"

#include <terme/core/simulation.h>

#include <memory>
#include <utility>
#include <vector>

using std::vector;

namespace Platformer
{
    void CollisionsTestLevel::LoadInSimulation()
    {
        terme::Level::LoadInSimulation();
        terme::Simulation& simulation = terme::Simulation::Instance();

        //------------ bunny
        {
            std::unique_ptr<Bunny> bunny = std::make_unique<Bunny>(5, 8);
            simulation.TryAddEntity(std::move(bunny));
        }

        double spawn_delay = 0.01;
        double speed = 2;

        int x_pos_right = GetWorldSizeX() - 2;

        vector<int> y_spawn_points;
        for (int i = 4; i < GetWorldSizeY() - 2; i += 2)
            y_spawn_points.push_back(i);

        //------------ obstacle spawner (right)
        {
            std::unique_ptr<ObstaclesSpawner> spawner_right = std::make_unique<ObstaclesSpawner>
            (
                x_pos_right,
                spawn_delay,
                -speed,
                y_spawn_points
            );
            simulation.TryAddEntity(std::move(spawner_right));
        }

        //------------ obstacle spawner (left)
        {
            int x_pos_left = 2;
            std::unique_ptr<ObstaclesSpawner> spawner_left = std::make_unique<ObstaclesSpawner>
            (
                x_pos_left,
                spawn_delay,
                speed,
                y_spawn_points
            );
            simulation.TryAddEntity(std::move(spawner_left));
        }
    }
}
