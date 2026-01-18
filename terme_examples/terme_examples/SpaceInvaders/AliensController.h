#pragma once
#include <terme/entities/i_simulation_entity.h>
#include <terme/core/direction.h>
#include <nbkit/random_utils.h>
#include "AliensFrontline.h"
#include <terme/general/vector2_int.h>
#include <nbkit/matrix.h>
#include <nbkit/event.h>
#include <terme/entities/game_object.h>

namespace terme{class GameObject;}

namespace SpaceInvaders
{	
	class Alien;
	class SpaceInvadersLevel;

	class AliensController : public terme::ISimulationEntity
	{
		//------------------------------------------------------------------- Aliens Settings
		//movement
		static constexpr double kBaseMoveSpeed = 0.5;
		static constexpr double kAllAliensEliminatedMoveSpeedIncrement = 16;
		static constexpr double kWaveNumberSpeedIncreaseFactor = 1;

		//shooting
		static constexpr double kBaseShootDelay = 1.5;
		static constexpr double kAllAliensEliminatedShootDelayReduction = 0.5;
		static constexpr double kWaveNumberShotDelayReductionFactor = 0.1;
		static constexpr double kShotsRandomness = 0.5;
		static constexpr double kMinShowDelay = 0.1;
		//------------------------------------------------------------------- Fields
	public:
		nbkit::Event<> on_wave_completed;
		nbkit::Event<> on_ground_touched;

	private:
		std::shared_ptr<SpaceInvadersLevel> level;
		nbkit::Matrix<std::weak_ptr<Alien>> aliensGrid;
		AliensFrontline frontLine;

		size_t aliens_count_ = 0;
		size_t aliens_pos_x_ = 0;
		terme::Direction x_move_direction_ = terme::Direction::kRight;
		bool is_time_to_move_aliens_down_ = false;
		double last_shot_time_ = -1;
		double shot_delay_ = 0;
		//------------------------------------------------------------------- Methods
	public:
		AliensController(std::shared_ptr<SpaceInvadersLevel> level) : level(level) {}
		;	void Reset(int aliens_count_x, int aliens_count_y);
		void Update() override;
		void RegisterAlien(std::shared_ptr<Alien> alien, int x_pos, int y_pos);

	private:
		double GetSpeedX()const;
		double GetNextShotDelay()const;
		double GetWaveMultiplier()const;
		double GetEliminatedAliensMultiplier()const;
		size_t GetAliensGridHeight() const { return aliensGrid.GetSizeY(); }
		size_t GetAliensGridWidth() const { return aliensGrid.GetSizeX(); }
		size_t GetStartingAliensCount() const { return GetAliensGridWidth() * GetAliensGridHeight(); }
		size_t GetDestroyedAliensCount() const { return GetStartingAliensCount() - aliens_count_; }
		void OnAlienMovedCallback(std::weak_ptr<terme::GameObject> alien, terme::Direction move_direction);
		void OnAlienDestroyedCallback(std::weak_ptr<terme::GameObject> alien_obj);
		void OnAliensReachMargin();
		void MoveAliens(terme::Direction dir, double speed);
		void HandleShooting();
		void HandleMovement();
	};
}