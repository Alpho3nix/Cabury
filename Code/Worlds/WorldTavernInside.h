#pragma once
#include "BeginningWorld.h"

class WorldTavernInside : public BeginningWorld
{
public:
	WorldTavernInside(BeginningWorld::Context context, Stack<BeginningWorld, Worlds::ID, BeginningWorld::Context>& stack);


	bool draw() override;
	bool update(sf::Time dt) override;

private:


	bool fStopPlayerMotion = false;
	void buildWorld() override;
	void handleMovement(sf::Time dt) override;
	float fTavernUpfloor;
};