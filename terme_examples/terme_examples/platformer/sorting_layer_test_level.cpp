#include <terme/entities/level.h>
#include <terme/core/simulation.h>
#include "bunny.h"
#include "sorting_layer_test_object.h"
#include "sorting_layer_test_level.h"

#include <memory>
#include <utility>

namespace Platformer
{
    void SortingLayerTestLevel::LoadInSimulation()
    {
        terme::Level::LoadInSimulation();
        terme::Simulation& simulation = terme::Simulation::Instance();

        //------------ bunny
        {
            std::unique_ptr<Bunny> bunny = std::make_unique<Bunny>(9, 5);
            bunny->on_obstacle_hit.Subscribe([this]() { OnGameOver(); });
            simulation.TryAddEntity(std::move(bunny));
        }

        //------------ font object
        {
            std::unique_ptr<SortingLayerTestObject> sorting_layer_test_obj_front = std::make_unique<SortingLayerTestObject>
            (
                10,     //posx
                1,      //posy
                30,     //sizex
                5,      //sizey
                '#',    //char
                terme::color::kRed,
            200     //sorting layer
            );
            simulation.TryAddEntity(std::move(sorting_layer_test_obj_front));
        }

        //------------ back object
        {
            std::unique_ptr<SortingLayerTestObject> sorting_layer_test_obj_back = std::make_unique<SortingLayerTestObject>
            (
                60,
                1,
                30,
                5,
                '#',
                terme::color::kBlue,
                0
            );
            simulation.TryAddEntity(std::move(sorting_layer_test_obj_back));
        }
    }
}
