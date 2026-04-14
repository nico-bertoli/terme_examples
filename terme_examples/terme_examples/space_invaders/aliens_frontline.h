#pragma once
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
		void Init(size_t size);
		Alien* GetAt(size_t pos);
		bool TryGetRandom(Alien*& out_alien);
		void Set(Alien* alien);
		size_t GetMinY();
		void ReplaceDestroyedElement(Alien* destroyedAlien, const nbkit::Matrix<Alien*>& aliensGrid);
	};
}
