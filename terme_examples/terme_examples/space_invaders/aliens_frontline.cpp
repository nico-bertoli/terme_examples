#include "aliens_frontline.h"
#include "alien.h"
#include <nbkit/matrix.h>
#include <nbkit/random_utils.h>

#include <algorithm>
#include <cassert>
#include <cstdint>

namespace SpaceInvaders
{
	void AliensFrontline::Init(size_t size)
	{
		frontLine.clear();
		frontLine.reserve(size);
	}

	Alien* AliensFrontline::GetAt(size_t pos)
	{
		for (Alien* alien : frontLine)
			if (alien->GetIndexInGridX() == pos)
				return alien;
		return nullptr;
	}

	void AliensFrontline::Set(Alien* alien)
	{
		frontLine.push_back(alien);
	}

	size_t AliensFrontline::GetMinY()
	{
		size_t min = SIZE_MAX;
		for (Alien* alien : frontLine)
		{
			if (alien != nullptr && alien->GetPosY() < min)
				min = alien->GetPosY();
		}
		return min;
	}

	void AliensFrontline::ReplaceDestroyedElement(Alien* destroyed_alien, const nbkit::Matrix<Alien*>& aliens_grid)
	{
		const size_t column_x = destroyed_alien->GetIndexInGridX();

		const auto destroyed_it = std::find(frontLine.begin(), frontLine.end(), destroyed_alien);
		const bool was_on_frontline = destroyed_it != frontLine.end();

		if (was_on_frontline)
			frontLine.erase(destroyed_it);

		Alien* newFront = nullptr;
		for (int y = aliens_grid.GetSizeY() - 1; y >= 0; --y)
		{
			Alien* candidate = aliens_grid.Get(column_x, y);
			if (candidate != nullptr && candidate != destroyed_alien)
			{
				newFront = candidate;
				break;
			}
		}

		if (was_on_frontline && newFront != nullptr)
			frontLine.push_back(newFront);
	}

	bool AliensFrontline::TryGetRandom(Alien*& out_alien)
	{
		if (frontLine.empty())
			return false;

		const int last = frontLine.size() - 1;
		out_alien = frontLine[nbkit::random_utils::GetRandomInt(0, last)];

		return true;
	}
}