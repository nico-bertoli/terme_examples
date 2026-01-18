#include "alien.h"
#include <terme/managers/time_manager.h>
#include <terme/managers/audio_manager.h>
#include "enemy_projectile.h"
#include <terme/core/simulation.h>

using std::shared_ptr;
using terme::Direction;

namespace SpaceInvaders
{
	void Alien::OnDestroy()
	{
		Enemy::OnDestroy();
		terme::AudioManager::Instance().PlayFx("resources/sounds/space_invaders/alien_death2.wav");
		on_destroy_event.Notify(std::dynamic_pointer_cast<Collider>(shared_from_this()));
	}

	void Alien::Shot()
	{
		shared_ptr<EnemyProjectile> projectile = std::make_shared<EnemyProjectile>(GetMidPosX(), GetPosY() - 2, Direction::kDown, kProjectileSpeed);
		terme::Simulation::Instance().TryAddEntity(projectile);
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