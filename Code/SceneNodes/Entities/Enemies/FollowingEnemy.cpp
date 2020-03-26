#include "FollowingEnemy.h"
#include "UtilityFunc.h"
#include "Player.h"
#include "Item.h"

FollowingEnemy::FollowingEnemy(int hp, Enemy::Type type, TexturesHolder& textures, const FontsHolder& fonts, Player& player,
	std::vector<std::vector<struct ItemType>> itemTypes, float damageDistance, int damage, float minAgroDist, float speed)
	:Enemy(hp, type, textures, fonts, player, itemTypes, damageDistance, damage), minAgroDistance(minAgroDist), fSpeed(speed)
{
}


void FollowingEnemy::setMinAgroDistance(float newDist)
{
	minAgroDistance = newDist;
}

float FollowingEnemy::getMinAgroDistance() const
{
	return minAgroDistance;
}

void FollowingEnemy::updateNode(sf::Time dt, CommandQueue& commands)
{
	Enemy::updateNode(dt, commands);

	if (!fIsDead && canMove)
	{
		followPlayer(dt);
		if (Utility::getDistance(getWorldPosition(), fPlayer.getWorldPosition()) <= getMinDamageDistance())
		{
			canDoAttack();
		}
	}
}

bool FollowingEnemy::playerIsInSight()
{
	if (Utility::getDistance(getWorldPosition(), fPlayer.getWorldPosition()) <= minAgroDistance)
	{
		return true;
	}

	return false;
}

void FollowingEnemy::followPlayer(sf::Time dt)
{
	if (playerIsInSight())
	{
		sf::Vector2f playerPos = fPlayer.getWorldPosition();
		sf::Vector2f fPos = getWorldPosition();
		//go towards player
		int dir = (playerPos.x - fPos.x) / abs(playerPos.x - fPos.x);
		setCurrentAnimation(AnimType::Run);
		setVelocity(fSpeed * dir * dt.asSeconds(), getVelocity().y);

	}
	else
	{
		setCurrentAnimation(AnimType::Idle);
		setVelocity(0.f, getVelocity().y);
	}
}
