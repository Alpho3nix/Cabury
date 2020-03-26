#include "BeginningWorld.h"

class Cabury;

class WorldTavern : public BeginningWorld
{
public:

	WorldTavern(BeginningWorld::Context context, Stack<BeginningWorld, Worlds::ID, BeginningWorld::Context>& stack);

	bool draw() override;
	bool update(sf::Time dt) override;

private:

	void buildWorld() override;
	void spawnNPCs();
	void handleMovement(sf::Time dt) override;

	Cabury* cabu;
	bool fCanViewMove = false;
	bool fFlag = false;
};