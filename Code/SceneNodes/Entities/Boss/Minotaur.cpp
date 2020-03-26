#include "Minotaur.h"
#include "Player.h"
#include "Item.h"
#include "DataTables.h"
#include "UtilityFunc.h"

void Minotaur::doTheJumpAttack(sf::Time dt)
{
	if (isSteppingOnSth()) {
		fJumpAttackAccu += dt;
		if (fJumpAttackAccu >= fJumpAttackStatic)
		{
			JumpAttack(dt);
			fJumpAttackAccu = sf::Time::Zero;
		}
		else if(!fBeginJumpAttack)
		{
			setCurrentAnimation(JumpReady);
			setVelocity(0.f, 0.f);
		}
	}
	else
	{
		fJumpAttackAccu = sf::Time::Zero;
	}
}

void Minotaur::doCloseAttack(sf::Time dt, int attack)
{
	if (fIsDoingCloseAttack) {
		setVelocity(0.f, getVelocity().y);
		if (attack)	//Attack 1
		{
			fAttack2Accu = sf::Time::Zero;
			setCurrentAnimation(Attack1);
			fAttack1Accu += dt;
			if ((fAnimations[CurrentAnimation + fLookingState]->getCurrentFrame() == fAnimData.fNumFrames[BossAnim::Attack_1] - 4) && !fHasDoneDamage &&
				Utility::getDistance(getWorldPosition(), fPlayer.getWorldPosition()) <= fDamagePlayerDistance)
			{
				fPlayer.applyDamage(damageAttack1);
				fHasDoneDamage = true;
			}
			if (fAttack1Accu >= fAttack1Timer)
			{
				fAttack1Accu = sf::Time::Zero;
				fIsDoingCloseAttack = false;
				fHasDoneDamage = false;
			}
		}
		else	//Attack 2
		{
			fAttack1Accu = sf::Time::Zero;
			setCurrentAnimation(Attack2);
			fAttack2Accu += dt;
			if ((fAnimations[CurrentAnimation + fLookingState]->getCurrentFrame() == fAnimData.fNumFrames[BossAnim::Attack_2] - 3) && !fHasDoneDamage &&
				Utility::getDistance(getWorldPosition(), fPlayer.getWorldPosition()) <= fDamagePlayerDistance)
			{
				fHasDoneDamage = true;

				fPlayer.applyDamage(damageAttack2);
			}
			if (fAttack2Accu >= fAttack2Timer)
			{
				fAttack2Accu = sf::Time::Zero;
				fIsDoingCloseAttack = false;
				fHasDoneDamage = false;
			}
		}
	}
}

Minotaur::Minotaur(int hp, TexturesHolder& textures, const FontsHolder & fonts, Player & player, std::vector<std::vector<ItemType>> itemTypes)
	:Boss(hp, Boss::Minotaur, textures, fonts, player, itemTypes), fAttack1Timer(fAnimData.fDurations[BossAnim::Attack_1]), fAttack2Timer(fAnimData.fDurations[BossAnim::Attack_2])
{
}

Minotaur::~Minotaur()
{
}



void Minotaur::updateNode(sf::Time dt, CommandQueue & commands)
{
	Boss::updateNode(dt, commands);

	if (!fIsDead) {

		if (getVelocity().y < 0.f)
		{
			setCurrentAnimation(Entity::AnimType::Jump);
		}
		else
		{
			setCurrentAnimation(Entity::AnimType::Fall);
		}
		
		float distance = Utility::getDistance(getWorldPosition(), fPlayer.getWorldPosition());
		if (distance >= 250.f)	//the player is too far away so the minotaur jumps towards the player
		{
			doTheJumpAttack(dt);
		}
		else //the minotaur can run towards the player
		{
			if (fBeginJumpAttack && isSteppingOnSth())	//the minotaur has finished the jump attack and deals damage when landing
			{
				if (Utility::getDistance(getWorldPosition(), fPlayer.getWorldPosition()) <= 125.f)
				{
					fPlayer.applyDamage(fJumpAttackDamage);
				}
				fBeginJumpAttack = false;
			}

			if (attacking != 0)	//when the player attacks the minotaur, he has a attackingDelay seconds window in which if he continues to attack 2 times, he will trigger the minotaur's rage
			{
				attackingAccu += dt;
				if (attackingAccu >= attackingDelay)
				{
					attackingAccu = sf::Time::Zero;
					attacking = 0;
				}
			}

			if (attacking >= 3)	//the player triggered the rage
			{
				isBoosted = true;
			}

			if (isBoosted)
			{
				BoostingAccu += dt;
				fBoostingColorAccu += dt;

				if (fBoostingColorAccu >= fBoostingColorTimer)
				{
					fBoostingColorAccu = sf::Time::Zero;
					if (fSprite.getColor() == sf::Color::White)
					{
						fSprite.setColor(sf::Color::Color(0, 255, 0, 120));
					}
					else
					{
						fSprite.setColor(sf::Color::White);
					}
				}

				if (BoostingAccu >= BoostingTimer)
				{
					BoostingAccu = sf::Time::Zero;
					isBoosted = false;
					damageAttack1 = damageAttack1_default;
					damageAttack2 = damageAttack2_default;
					fJumpAttackDamage = fJumpAttackDamage_default;
					fSpeed = fSpeed_default;
					fSprite.setColor(sf::Color::White);
				}
				else
				{	//when in rage, the minotaur runs 2 times faster and his damage are increased
					damageAttack1 = float(damageAttack1_default) * 1.5f;
					damageAttack2 = float(damageAttack2_default) * 1.5f;
					fJumpAttackDamage = float(fJumpAttackDamage_default) * 1.5f;
					fSpeed = fSpeed_default * 2.f;
				}

				
			}
			else
			{
				fBoostingColorAccu = sf::Time::Zero;
			}

			if (isSteppingOnSth()) {

				if (!fIsDoingCloseAttack) {
					int dir = abs(fPlayer.getWorldPosition().x - getWorldPosition().x) / (fPlayer.getWorldPosition().x - getWorldPosition().x);
					setVelocity(fSpeed * dir * dt.asSeconds(), getVelocity().y);
					setCurrentAnimation(Run);
				}

				if (distance <= fDamagePlayerDistance)
				{
					//do randomly the first or the second attack
					if (!fIsDoingCloseAttack)
					{
						random = Utility::randomInt(0, 1);
						fIsDoingCloseAttack = true;
					}
					
				}
				doCloseAttack(dt, random);
			}
		}

	}


}

void Minotaur::JumpAttack(sf::Time dt)
{
	sf::Vector2f playerPos = fPlayer.getWorldPosition();
	sf::Vector2f pos = getWorldPosition();
	float dx = abs(playerPos.x - pos.x);
	if (dx <= 50.f)
		return;
	int dir = dx / (playerPos.x - pos.x);
	float dy = 2.f;
	float alpha = atan(4.f * dy / dx);
	float speed = sqrt((9.81f * dx) / (sin(2 * alpha)));
	setVelocity(dir * speed * cos(alpha) * dt.asSeconds(), getVelocity().y);
	setUpwardsVel(-speed * sin(alpha));

	fBeginGravityTimer = true;
	fBeginJumpAttack = true;

}

bool Minotaur::applyDamage(int amount)
{
	Boss::applyDamage(amount);
	if (!isBoosted)
	{
		attacking++;
	}
	else
	{
		attacking = 0;
	}
	return true;
}

sf::FloatRect Minotaur::getBoundingRect() const
{
	sf::FloatRect ret = Entity::getBoundingRect();
	if (isLookingLeft()) {
		ret.left += ret.width / 4.f + 20.f;
	}
	else {
		ret.left += ret.width / 4.f - 10.f;
	}
	ret.top += ret.height / 4.f;
	ret.width = ret.width * 3.f / 4.f - 70.f;
	ret.height *= 3.f / 4.f;
	return ret;
}
