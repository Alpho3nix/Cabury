#pragma once
#include "BeginningWorld.h"

class Cabury;
class MagicBook;
class Minotaur;

class MinotaurWorld : public BeginningWorld
{
public:
	MinotaurWorld(BeginningWorld::Context context, Stack<BeginningWorld, Worlds::ID, BeginningWorld::Context>& stack);

	bool update(sf::Time dt) override;
	bool draw() override;
private:

	void buildWorld() override;
	void handleMovement(sf::Time dt) override;

	Cabury* fCabu = nullptr;
	Minotaur* fMinotaur = nullptr;
	MagicBook* fMagicBook = nullptr;
	bool fSpawnedMinotaur = false;
	bool fSpawnedBook = false;

	sf::Time fBookSlidingTime = sf::seconds(1.5f);
	sf::Time fBookSlidingAccu = sf::Time::Zero;
	bool fCanSlide = true;

};