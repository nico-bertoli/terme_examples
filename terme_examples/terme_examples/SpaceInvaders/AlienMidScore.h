#pragma once
#include "Alien.h"

namespace SpaceInvaders
{
	class AlienMidScore : public Alien
	{
	private:
		static inline const terme::Model kModel1 = terme::Model
		(
			4,
			{
				'(', 'o', 'O', ')',
				'V', '~', '~', 'v'
			}
		);
		static inline const terme::Model kModel2 = terme::Model
		(
			4,
			{
				'(', 'O', 'o', ')',
				'v', '~', '~', 'V'
			}
		);
		//------------------------------------------------------ Methods
	public:
		using Alien::Alien;

		size_t GetScore() const override { return 20; }
		terme::TerminalColor GetColor() const override { return terme::color::kCyan; }

	protected:
		void InitModel() override
		{
			animation_model_1_ = &kModel1;
			animation_model_2_ = &kModel2;
			SetModel(kModel1);
		}
	};
}