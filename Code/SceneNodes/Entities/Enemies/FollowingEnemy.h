#pragma once
#include "Enemy.h"

class FollowingEnemy : public Enemy
{
public:

	FollowingEnemy(int hp, Enemy::Type type, TexturesHolder& textures, const FontsHolder& fonts, Player& player,
		std::vector<std::vector<struct ItemType>> itemTypes, float damageDistance = 50.f, int damage = 10, float minAgroDist = 250.f, float speed = 25.f);

	void setMinAgroDistance(float newDist);
	float getMinAgroDistance() const;

	void updateNode(sf::Time dt, CommandQueue& commands) override;

private:

	bool playerIsInSight();
	void followPlayer(sf::Time dt);

	float minAgroDistance = 0.f;
	float fSpeed;
};