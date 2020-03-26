#pragma once
#include "Enemy.h"

class GuardEnemy : public Enemy
{
public:
	GuardEnemy(int hp, Enemy::Type type, TexturesHolder& textures, const FontsHolder& fonts, Player& player,
		std::vector<std::vector<struct ItemType>> itemTypes, Entity::LookingState defaultLookingDir, float damageDistance = 50.f, int damage = 10, float minAgroDist = 150.f, float maxAgroDist = 700.f, float speed = 25.f);

	void updateNode(sf::Time dt, CommandQueue& commands) override;

	void setMinAgroDistance(float newDist);
	float getMinAgroDistance() const;

	void setMaxAgroDistance(float newDist);
	float getMaxAgroDistance() const;

	void setSpeed(float newSpeed);
	float getSpeed() const;

	void setDefaultLookingDir(Entity::LookingState newLookingDir);
	Entity::LookingState getDefaultLookingDir() const;

	void setDefaultPosition(sf::Vector2f Pos);
	void setDefaultPosition(float x, float y);
	sf::Vector2f getDefaultPosition() const;

private:

	float fMinAgroDistance;
	float fMaxAgroDistance;
	float fSpeed;
	bool fFollow = false;

	int fFullHP;

	sf::Vector2f fDefaultPosition;
	Entity::LookingState fDefaultLookingState;

	bool playerIsInSight();
	void followPlayer(sf::Time dt);

};