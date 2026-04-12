#pragma once
#include <nbkit/random_utils.h>
#include <nbkit/matrix.h>
#include <vector>

namespace SpaceInvaders
{
	class Alien;

	class AliensFrontline
	{
	private:
		std::vector<Alien*> frontLine;
	public:
		void Init(size_t size) { frontLine.resize(size); }
		Alien* GetAt(size_t pos) { return frontLine.size() > 0 ? frontLine[pos] : nullptr; }
		Alien* GetRandom();
		void Set(size_t pos, Alien* alien) { frontLine[pos] = alien; }
		size_t GetMinY();
		void ReplaceDestroyedElement(Alien* destroyedAlien, const nbkit::Matrix<Alien*>& aliensGrid);
	};
}
