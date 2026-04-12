#include "aliens_frontline.h"
#include "alien.h"
#include <nbkit/matrix.h>

#include <cstdint>

namespace SpaceInvaders
{
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

	void AliensFrontline::ReplaceDestroyedElement(Alien* destroyedAlien, const nbkit::Matrix<Alien*>& aliensGrid)
	{
		const size_t columnX = destroyedAlien->GetIndexInGridX();

		for (int y = aliensGrid.GetSizeY() - 1; y >= 0; --y)
		{
			Alien* newCandidate = aliensGrid.Get(columnX, y);
			if (newCandidate != nullptr && newCandidate != destroyedAlien)
			{
				frontLine[columnX] = newCandidate;
				return;
			}
		}

		frontLine[columnX] = nullptr;
	}

	Alien* AliensFrontline::GetRandom()
	{
		return GetAt(nbkit::random_utils::GetRandomInt(0, frontLine.size()) - 1);
	}
}