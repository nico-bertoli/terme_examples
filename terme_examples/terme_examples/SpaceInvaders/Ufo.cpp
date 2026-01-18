#include "Ufo.h"
#include <nbkit/random_utils.h>
#include <terme/managers/audio_manager.h>

using terme::Model;
using terme::Direction;
using std::shared_ptr;

namespace SpaceInvaders
{
	size_t Ufo::GetScore() const
	{
		int random_index = nbkit::random_utils::GetRandomInt(0, static_cast<int>(kPossibleScores.size() - 1));
		return kPossibleScores[random_index];
	}

	void Ufo::Update()
	{
		TryMove(move_direction_, kMoveSpeed);
	}

	void Ufo::OnCollisionEnter(shared_ptr<Collider> other, Direction collision_dir)
	{
		Enemy::OnCollisionEnter(other, collision_dir);
		terme::AudioManager::Instance().PlayFx("Resources/Sounds/SpaceInvaders/UfoDefeated.wav");
	}

}