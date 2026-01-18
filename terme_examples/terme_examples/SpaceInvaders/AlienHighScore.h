#pragma once
#include "Alien.h"

namespace SpaceInvaders
{
	class AlienHighScore : public Alien
	{
	private:
		static inline const terme::Model kModel1 = terme::Model
		(
			4,
			{
				'/', 'O',  'o',  '\\',
				'/', '\'', '\'', '\\'
			}
		);
		static inline const terme::Model kModel2 = terme::Model
		(
			4,
			{
				'/', 'o', 'O', '\\',
				'|', '\'', '\'', '|'
			}
		);
		//------------------------------------------------------ Methods
	public:
		using Alien::Alien;

		size_t GetScore() const override { return 30; }
		terme::TerminalColor GetColor() const override { return terme::color::kMagenta; }

	protected:
		void InitModel() override
		{
			animation_model_1_ = &kModel1;
			animation_model_2_ = &kModel2;
			SetModel(kModel1);
		}
	};
}