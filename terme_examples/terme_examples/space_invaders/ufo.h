#pragma once
#include "enemy.h"
#include <terme/terminal/terminal.h>
#include <nbkit/random_utils.h>
#include <array>

namespace SpaceInvaders
{
	class Ufo : public Enemy
	{
		//------------------------------------------------------ Fields
	private:
		static inline const terme::Model kModel1 = terme::Model
		(
			4,
			{
				'/', '(', ')', '\\',
				'\\', '_', '_', '/'
			}
		);
		static inline constexpr std::array<size_t, 6> kPossibleScores = { 50,100,150,200,250,300 };
		static constexpr size_t kMoveSpeed = 8;

		terme::Direction move_direction_;
		//------------------------------------------------------ Methods
	public:
		Ufo(int x_pos, int y_pos, terme::Direction move_direction) : Enemy(x_pos, y_pos), move_direction_(move_direction) {};

		bool CanExitScreenSpace() const override { return true; }
		terme::TerminalColor GetColor() const override { return terme::color::kRed; }
		void InitModel() override { SetModel(kModel1); }
		size_t GetScore() const override;

	protected:
		void Update() override;
		void OnCollisionEnter(std::shared_ptr<Collider> other, terme::Direction collision_dir) override;
		terme::TerminalColor GetDestroyedParticlesColor() override { return terme::color::kRed; }
	};
}