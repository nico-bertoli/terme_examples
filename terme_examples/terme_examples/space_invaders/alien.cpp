#include "alien.h"
#include <terme/managers/time_manager.h>
#include <terme/managers/audio_manager.h>
#include "enemy_projectile.h"
#include <terme/core/simulation.h>

#include <memory>
#include <utility>

using terme::Direction;

namespace SpaceInvaders
{
	void Alien::OnDestroy()
	{
		Enemy::OnDestroy();
		terme::AudioManager::Instance().PlayFx("resources/sounds/space_invaders/alien_death2.wav");
		on_destroy_event.Notify(static_cast<terme::Collider*>(this));
	}

	void Alien::Shot()
	{
		std::unique_ptr<EnemyProjectile> projectile =
			std::make_unique<EnemyProjectile>(GetMidPosX(), GetPosY() - 2, Direction::kDown, kProjectileSpeed);
		terme::Simulation::Instance().TryAddEntity(std::move(projectile));
	}

	void Alien::Update()
	{
		bool show_first_anim_frame_new = terme::TimeManager::Instance().IsTimeForFirstOfTwoModels(1);
		if (is_first_animation_frame_time_ != show_first_anim_frame_new)
		{
			StepAnimation();
			is_first_animation_frame_time_ = show_first_anim_frame_new;
		}
	}
}