#pragma once

#include <vector>

#include <nbkit/matrix.h>

namespace SpaceInvaders
{
	class Alien;

	class AliensFrontline
	{
	private:
		std::vector<Alien*> front_line_;
	public:
		void Init(size_t size);
		Alien* GetAt(size_t pos);
		bool TryGetRandom(Alien*& out_alien);
		void Set(Alien* alien);
		size_t GetMinY();
		void ReplaceDestroyedElement(Alien* destroyed_alien, const nbkit::Matrix<Alien*>& aliens_grid);
	};
}
