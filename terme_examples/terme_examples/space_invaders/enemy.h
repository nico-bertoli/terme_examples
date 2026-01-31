#pragma once
#include <terme/entities/collider.h>

namespace SpaceInvaders
{
	class Enemy : public terme::Collider
	{
		//------------------------------------------------------ Fields
	public:
		using Collider::Collider;
	protected:
		bool isFirsAnimationFrameTime_;

		//------------------------------------------------------ Methods
	public:
		virtual size_t GetScore() const = 0;

	protected:
		double GetGravityScale() const override { return 0; }
		void OnDestroy() override;
		virtual terme::TerminalColor GetDestroyedParticlesColor() { return terme::color::kGreen; }
	};
}