#include "GuardEnemy.h"
#include "Item.h"
#include "UtilityFunc.h"
#include "Player.h"

GuardEnemy::GuardEnemy(int hp, Enemy::Type type, TexturesHolder& textures, const FontsHolder& fonts, Player& player,
	std::vector<std::vector<struct ItemType>> itemTypes, Entity::LookingState defaultLookingDir, float damageDistance, int damage, float minAgroDist, float maxAgroDist, float speed)
	:Enemy(hp, type, textures, fonts, player, itemTypes, damageDistance, damage), fMinAgroDistance(minAgroDist), fMaxAgroDistance(maxAgroDist),
	fSpeed(speed), fDefaultLookingState(defaultLookingDir), fDefaultPosition(), fFullHP(hp)
{
	setLookingState(fDefaultLookingState);
}

void GuardEnemy::setMinAgroDistance(float newDist)
{
	fMinAgroDistance = newDist;
}

float GuardEnemy::getMinAgroDistance() const
{
	return fMinAgroDistance;
}

void GuardEnemy::setMaxAgroDistance(float newDist)
{
	fMaxAgroDistance = newDist;
}

float GuardEnemy::getMaxAgroDistance() const
{
	return fMaxAgroDistance;
}

void GuardEnemy::setSpeed(float newSpeed)
{
	fSpeed = newSpeed;
}

float GuardEnemy::getSpeed() const
{
	return fSpeed;
}

void GuardEnemy::setDefaultLookingDir(Entity::LookingState newLookingDir)
{
	fDefaultLookingState = newLookingDir;
}

Entity::LookingState GuardEnemy::getDefaultLookingDir() const
{
	return fDefaultLookingState;
}

void GuardEnemy::setDefaultPosition(sf::Vector2f Pos)
{
	fDefaultPosition = Pos;
}

void GuardEnemy::setDefaultPosition(float x, float y)
{
	fDefaultPosition = sf::Vector2f(x, y);
}

sf::Vector2f GuardEnemy::getDefaultPosition() const
{
	return fDefaultPosition;
}

bool GuardEnemy::playerIsInSight()
{
	sf::Vector2f play_to_enemy = fPlayer.getWorldPosition() - getWorldPosition();
	return ((Utility::length(play_to_enemy) <= fMinAgroDistance && play_to_enemy.y <= 25.f)
		&& ((fDefaultLookingState == Entity::LookingLeft) ? (play_to_enemy.x < 0.f) : (play_to_enemy.x > 0.f)));
}

void GuardEnemy::followPlayer(sf::Time dt)
{
	sf::Vector2f playerPos = fPlayer.getWorldPosition();
	sf::Vector2f fPos = getWorldPosition();
	//go towards player
	int dir = (playerPos.x - fPos.x) / abs(playerPos.x - fPos.x);
	setCurrentAnimation(AnimType::Run);
	setVelocity(fSpeed * dir * dt.asSeconds(), getVelocity().y);

}

void GuardEnemy::updateNode(sf::Time dt, CommandQueue& commands)
{
	Enemy::updateNode(dt, commands);

	if (!fIsDead && canMove)
	{
		if (!fFollow)
		{
			if (playerIsInSight() || HealthPoints != fFullHP)
			{
				fFollow = true;
			}
		}
		else
		{
			followPlayer(dt);
			float distance = Utility::getDistance(fPlayer.getWorldPosition(), getWorldPosition());
			if (distance <= getMinDamageDistance())
			{
				canDoAttack();
			}

			if (distance >= fMaxAgroDistance)
			{
				setPosition(fDefaultPosition);
				setLookingState(fDefaultLookingState);
				fFollow = false;
				setVelocity(0.f, getVelocity().y);
				setCurrentAnimation(Idle);
			}
		}
	}
}
