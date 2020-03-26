#pragma once
#include "BeginningWorld.h"

class GuardEnemy;
class BreakableBox;
class Cabury;

class KanalisationWorld : public BeginningWorld
{
public:
	KanalisationWorld(BeginningWorld::Context context, Stack<BeginningWorld, Worlds::ID, BeginningWorld::Context>& stack);
	~KanalisationWorld() override;

	bool update(sf::Time dt) override;
	bool draw() override;

private:

	void buildWorld() override;
	void handleMovement(sf::Time dt) override;

	sf::Text fWaveMessage;
	bool fShowWaveMessage = false;

	sf::Time fMessageAccu = sf::Time::Zero;
	sf::Time fMessageDelay = sf::seconds(2.f);
	unsigned int fWaveNum = 1;

	GuardEnemy* guardBuffer;
	BreakableBox* boxBuffer;
	bool fSpawnGuard;
	bool fSpawnBox;
	Cabury* cabu = nullptr;
	bool fSpawnCabury;

	bool fMobsDead = false;

	void spawnPlatforms();
	void prepareEnemies(unsigned int waveNum);
	void spawnEnemies(unsigned int amount, Enemy::Type type, int hp, TexturesHolder& textures,
		const FontsHolder& fonts, Player& player, std::vector<std::vector<ItemType>> itemTypes,
		float minAgroDist, float speed, float def_pos_y, float damageDistance, int damage = 10, float def_pos_x = 700.f, float projVelocity = 0.f);
	void spawnBoss();
	void waveSystem(sf::Time dt);
	bool fBeginMonsterWaves = false;
	bool fInWave = false;
	bool fWaveFinished = true;
	unsigned int fDifficulty = 1;


};