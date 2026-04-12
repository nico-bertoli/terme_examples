#include "puzzle_level.h"
#include <terme/core/simulation.h>
#include "bunny.h"
#include <terme/entities/static_collider.h>
#include "automatic_door.h"
#include "pressure_plate.h"
#include "level_end_flag.h"
#include <terme/managers/audio_manager.h>
#include "pushable_object.h"

#include <memory>
#include <utility>

using StaticCollider = terme::StaticCollider;

namespace Platformer
{
    void PuzzleLevel::LoadInSimulation()
    {
        terme::Level::LoadInSimulation();
        terme::Simulation& simulation = terme::Simulation::Instance();

        //------------ bunny
        {
            std::unique_ptr<Bunny> bunny = std::make_unique<Bunny>(5, 8);
            simulation.TryAddEntity(std::move(bunny));
        }

        //------------ left platform
        {
            std::unique_ptr<StaticCollider> platform = std::make_unique<StaticCollider>(24, 11, 12, 1, '#');
            simulation.TryAddEntity(std::move(platform));
        }

        //------------ right flag platform
        {
            std::unique_ptr<StaticCollider> flag_platform = std::make_unique<StaticCollider>(63, 15, 16, 1, '#');
            simulation.TryAddEntity(std::move(flag_platform));
        }

        //------------ flag
        {
            std::unique_ptr<LevelEndFlag> flag = std::make_unique<LevelEndFlag>(this, 70, 16);
            flag->OnTouchBunny.Subscribe([this]() { OnGameOver(); });
            simulation.TryAddEntity(std::move(flag));
        }

        //------------ automatic door
        std::unique_ptr<AutomaticDoor> automatic_door_entity = std::make_unique<AutomaticDoor>(60, 4, 2, 6, '|', 4);
        AutomaticDoor* automatic_door = automatic_door_entity.get();
        simulation.TryAddEntity(std::move(automatic_door_entity));

        //------------ automatic door container
        {
            std::unique_ptr<StaticCollider> automatic_door_top_right = std::make_unique<StaticCollider>(62, 10, 1, 18, '#');
            simulation.TryAddEntity(std::move(automatic_door_top_right));
        }

        {
            std::unique_ptr<StaticCollider> automatic_door_top_left = std::make_unique<StaticCollider>(59, 10, 1, 18, '#');
            simulation.TryAddEntity(std::move(automatic_door_top_left));
        }

        //------------ pressure plate left
        {
            std::unique_ptr<PressurePlate> pressure_plate_1 = std::make_unique<PressurePlate>(14, 4, 8);
            pressure_plate_1->on_press.Subscribe
            (
                [this]()
                {
                    std::unique_ptr<PushableObject> pushable_obj_1 = std::make_unique<PushableObject>(32, GetWorldSizeY() - 4);
                    terme::Simulation::Instance().TryAddEntity(std::move(pushable_obj_1));
                }
            );
            simulation.TryAddEntity(std::move(pressure_plate_1));
        }

        //------------ pressure plate center
        {
            std::unique_ptr<PressurePlate> open_door_pressure_plate_left = std::make_unique<PressurePlate>(37, 4, 8);
            PressurePlate* plate_left = open_door_pressure_plate_left.get();
            plate_left->on_press.Subscribe([automatic_door]() { automatic_door->AddEnergySource(); });
            plate_left->on_release.Subscribe([automatic_door]() { automatic_door->RemoveEnergySource(); });
            simulation.TryAddEntity(std::move(open_door_pressure_plate_left));
        }

        //------------ pressure plate right
        {
            std::unique_ptr<PressurePlate> open_door_pressure_plate_right = std::make_unique<PressurePlate>(86, 4, 8);
            PressurePlate* plate_right = open_door_pressure_plate_right.get();
            plate_right->on_press.Subscribe([automatic_door]() { automatic_door->AddEnergySource(); });
            plate_right->on_release.Subscribe([automatic_door]() { automatic_door->RemoveEnergySource(); });
            simulation.TryAddEntity(std::move(open_door_pressure_plate_right));
        }
    }

    void PuzzleLevel::OnGameOver()
    {
        terme::Level::OnGameOver();
        terme::AudioManager::Instance().PlayFx("resources/sounds/platform/level_won.wav");
    }

    void PuzzleLevel::OnPostGameOverDelayEnded()
    {
        terme::Level::OnPostGameOverDelayEnded();
        Terminate();
    }
}