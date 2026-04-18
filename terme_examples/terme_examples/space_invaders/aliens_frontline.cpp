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
		front_line_.clear();
		front_line_.reserve(size);
	}

	Alien* AliensFrontline::GetAt(size_t pos)
	{
		for (Alien* alien : front_line_)
			if (alien->GetIndexInGridX() == pos)
				return alien;
		return nullptr;
	}

	void AliensFrontline::Set(Alien* alien)
	{
		front_line_.push_back(alien);
	}

	size_t AliensFrontline::GetMinY()
	{
		size_t min = SIZE_MAX;
		for (Alien* alien : front_line_)
		{
			if (alien != nullptr && alien->GetPosY() < min)
				min = alien->GetPosY();
		}
		return min;
	}

	void AliensFrontline::ReplaceDestroyedElement(Alien* destroyed_alien, const nbkit::Matrix<Alien*>& aliens_grid)
	{
		const size_t column_x = destroyed_alien->GetIndexInGridX();

		const auto destroyed_it = std::find(front_line_.begin(), front_line_.end(), destroyed_alien);
		const bool was_on_frontline = destroyed_it != front_line_.end();

		if (was_on_frontline)
			front_line_.erase(destroyed_it);

		Alien* new_front = nullptr;
		for (int y = aliens_grid.GetSizeY() - 1; y >= 0; --y)
		{
			Alien* candidate = aliens_grid.Get(column_x, y);
			if (candidate != nullptr && candidate != destroyed_alien)
			{
				new_front = candidate;
				break;
			}
		}

		if (was_on_frontline && new_front != nullptr)
			front_line_.push_back(new_front);
	}

	bool AliensFrontline::TryGetRandom(Alien*& out_alien)
	{
		if (front_line_.empty())
			return false;

		const int last = front_line_.size() - 1;
		out_alien = front_line_[nbkit::random_utils::GetRandomInt(0, last)];

		return true;
	}
}