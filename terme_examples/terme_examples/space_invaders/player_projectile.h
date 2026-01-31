#pragma once
#include "projectile.h"

namespace SpaceInvaders
{
	class PlayerProjectile : public Projectile
	{
		//---------------------------------------------------------- Fields
	private:
		static inline const terme::Model kModel = terme::Model(1, { '|' });

		//---------------------------------------------------------- Methodss
	public:
		using Projectile::Projectile;

	protected:
		terme::TerminalColor GetColor() const override { return terme::color::kGreen; }
		void InitModel() override { SetModel(kModel); }
		void OnCollisionEnter(std::shared_ptr<Collider> other, terme::Direction collision_dir) override;
	};
}