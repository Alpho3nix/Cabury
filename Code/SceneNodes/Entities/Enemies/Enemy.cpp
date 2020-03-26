#include "Enemy.h"
#include "Player.h"
#include "UtilityFunc.h"
#include "DataTables.h"
#include <iostream>
#include "Item.h"
#include "Projectile.h"

namespace
{
	void wispsTexRects(sf::Vector2u texDim, sf::Vector2i frameDim, sf::IntRect* rects, Enemy::Type wispType)
	{
		switch (wispType)
		{
		case Enemy::FireWisp:
			rects[0] = sf::IntRect(sf::Vector2i(texDim.x - frameDim.x, texDim.y / 2), frameDim);	//idleRight
			rects[1] = sf::IntRect(sf::Vector2i(0, 0), frameDim);	//idleLeft
			rects[2] = sf::IntRect(sf::Vector2i(0, texDim.y / 2), frameDim);	//walkRight
			rects[3] = sf::IntRect(sf::Vector2i(texDim.x - frameDim.x, 0), frameDim);	//walkLeft
			rects[4] = sf::IntRect(sf::Vector2i(frameDim.x, texDim.y - 3 * frameDim.y), frameDim);	//hurtRight
			rects[5] = sf::IntRect(sf::Vector2i(texDim.x - 2 * frameDim.x, 3 * frameDim.y), frameDim);	//hurtLeft
			rects[6] = sf::IntRect(sf::Vector2i(frameDim.x, texDim.y / 2 + frameDim.y), frameDim);	//attackRight
			rects[7] = sf::IntRect(sf::Vector2i(texDim.x - 2 * frameDim.x, frameDim.y), frameDim);	//attackLeft
			rects[8] = sf::IntRect(sf::Vector2i(texDim.x - 2 * frameDim.x, texDim.y - 2 * frameDim.y), frameDim);	//deadRight
			rects[9] = sf::IntRect(sf::Vector2i(frameDim.x, texDim.y / 2 - 2 * frameDim.y), frameDim);	//deadLeft
			break;
		case Enemy::WaterWisp:
		case Enemy::WindWisp:
		case Enemy::EarthWisp:
			rects[0] = sf::IntRect(sf::Vector2i(texDim.x - frameDim.x, texDim.y / 2), frameDim);	//idleRight
			rects[1] = sf::IntRect(sf::Vector2i(0, 0), frameDim);	//idleLeft
			rects[2] = sf::IntRect(sf::Vector2i(frameDim.x, texDim.y / 2), frameDim);	//walkRight
			rects[3] = sf::IntRect(sf::Vector2i(texDim.x - 2 * frameDim.x, 0), frameDim);	//walkLeft
			rects[4] = sf::IntRect(sf::Vector2i(texDim.x - frameDim.x, texDim.y - 2 * frameDim.y), frameDim);	//hurtRight
			rects[5] = sf::IntRect(sf::Vector2i(0, 3 * frameDim.y), frameDim);	//hurtLeft
			rects[6] = sf::IntRect(sf::Vector2i(texDim.x - 3 * frameDim.x, texDim.y / 2 + frameDim.y), frameDim);	//attackRight
			rects[7] = sf::IntRect(sf::Vector2i(2 * frameDim.x, frameDim.y), frameDim);	//attackLeft
			rects[8] = sf::IntRect(sf::Vector2i(2 * frameDim.x, texDim.y - 2 * frameDim.y), frameDim);	//deadRight
			rects[9] = sf::IntRect(sf::Vector2i(3 * frameDim.x, texDim.y / 2 - 2 * frameDim.y), frameDim);	//deadLeft
			break;
		default:
			break;
		}
	}

}


Enemy::Enemy(int hp, Type type, TexturesHolder& textures, const FontsHolder& fonts, Player & player, std::vector<std::vector<struct ItemType>> itemTypes, float damageDistance, int damage)
	:Entity(hp), fPlayer(player), fAnimData(), fItemTex(textures.get(Textures::ID::Items)), fItemTypes(itemTypes), minDamageDistance(damageDistance), fEnemyDamage(damage)
{

	attackOnPlayer = [](Player& player, int damage, CommandQueue& commands)
	{
		player.applyDamage(damage);
	};

	handleType(type, textures);

	setAnimations();
	setCurrentAnimation(AnimType::Idle);
	fSprite.setTexture(*fAnimData.fTexture);
	fSprite.setTextureRect(fAnimData.fTextureRects[EnemyAnim::Idle]);
	Utility::CenterOrigin(fSprite);

	fDropItemCommand.category = Category::Scene;
	fDropItemCommand.action = [this](SceneNode& node, sf::Time)
	{
		for (int i = 0; i < fItemTypes.size(); ++i)	//create items
		{
			unsigned int randomIndex = Utility::randomInt(0, fItemTypes[i].size() - 1);
			createItem(node, fItemTex, fItemTypes[i][randomIndex]);
		}

	};

	attackDuration = fAnimData.fDurations[EnemyAnim::Attack];

}

unsigned int Enemy::getCategory() const
{
	return Category::Enemies;
}

void Enemy::updateNode(sf::Time dt, CommandQueue & commands)
{

	if (isDestroyed()) {

		if (!hasSpawnedItems)
		{
			commands.push(fDropItemCommand);
			hasSpawnedItems = true;
		}

		setVelocity(0.f, getVelocity().y);
		setCurrentAnimation(AnimType::Death);
		deathAccu += dt;
		if (deathAccu >= fAnimData.fDurations[EnemyAnim::Death] - fAnimData.fDurations[EnemyAnim::Death] / float(fAnimData.fNumFrames[EnemyAnim::Death]))
		{
			deathAccu = sf::Time::Zero;
			fIsDead = true;
		}
	}
	else if(!canMove){

		setVelocity(0.f, getVelocity().y);
		notMovingAccu += dt;
		if (notMovingAccu >= notMovingDuration)
		{
			notMovingAccu = sf::Time::Zero;
			canMove = true;
			isBeingHurt = false;
			fCanAttack = false;
			fHasDoneAttack = false;
			
		}
		else
		{

			if (isBeingHurt)
			{
				setCurrentAnimation(AnimType::beingHit);
			}
			else if (fCanAttack)
			{
				setCurrentAnimation(AnimType::Attack1);
				attack(dt, commands);
			}
		}
	}


	Entity::updateNode(dt, commands);
}


bool Enemy::applyDamage(int amount)
{
	HealthPoints -= amount;
	if (HealthPoints <= 0) {
		HealthPoints = 0;
	}
	else {
		dontMove(fAnimData.fDurations[EnemyAnim::beingHit]);
		isBeingHurt = true;
	}

	return true;
}

sf::FloatRect Enemy::getBoundingRect() const
{
	return Entity::getBoundingRect();
}

bool Enemy::isRemovable() const
{
	return fIsDead;
}

void Enemy::createItem(SceneNode & node, const sf::Texture & tex, ItemType type)
{
	Item* newItem = new Item(1, fItemTex, type);
	newItem->setPosition(getWorldPosition());
	assert(&node);
	node.attachChild(newItem);
}

void Enemy::dontMove(sf::Time duration)
{
	if (canMove) {
		canMove = false;
		notMovingDuration = duration;
		notMovingAccu = sf::Time::Zero;
	}
}

void Enemy::attack(sf::Time dt, CommandQueue & commands)
{
	if (fAnimations[CurrentAnimation + fLookingState]->getCurrentFrame() == fAnimData.frameNumberToAttack && !fHasDoneAttack)
	{
		if (Utility::getDistance(getWorldPosition(), fPlayer.getWorldPosition()) <= minDamageDistance) {
			attackOnPlayer(fPlayer, fEnemyDamage, commands);
			fHasDoneAttack = true;
		}
	}
}

void Enemy::handleType(Type type, TexturesHolder& textures)
{



	switch (type)
	{
	case Enemy::Slime:
	{
		sf::Time slimeDurations[EnemyAnim::AnimCount] = { sf::seconds(0.7f), sf::seconds(0.7f), sf::seconds(0.3f), sf::seconds(1.f), sf::seconds(0.8f) };
		sf::Texture* slimeTexture = &textures.get(Textures::ID::Slime);
		size_t slimeNumFrames[EnemyAnim::AnimCount] = { 4, 4, 3, 5, 4 };	//idle, walk, hit, attack, dead

		//frame dimension: 64 x 50, texture dimension: 512 x 300
		sf::Vector2i slimeFrameDim(64, 50);
		sf::Vector2u slimeTexDim = slimeTexture->getSize();
		sf::IntRect slimeTexRects[EnemyAnim::AnimCount2] =
		{
			sf::IntRect(sf::Vector2i(slimeTexDim.x - slimeFrameDim.x, 0), slimeFrameDim),	//IdleRight
			sf::IntRect(sf::Vector2i(0, 3 * slimeFrameDim.y), slimeFrameDim),	//IdleLeft
			sf::IntRect(sf::Vector2i(3 * slimeFrameDim.x, 0), slimeFrameDim),	//WalkRight
			sf::IntRect(sf::Vector2i(4 * slimeFrameDim.x, 3 * slimeFrameDim.y), slimeFrameDim),	//WalkLeft
			sf::IntRect(sf::Vector2i(3 * slimeFrameDim.x, slimeFrameDim.y), slimeFrameDim),	//HitRight
			sf::IntRect(sf::Vector2i(5 * slimeFrameDim.x, 4 * slimeFrameDim.y), slimeFrameDim),	//HitLeft
			sf::IntRect(sf::Vector2i(slimeTexDim.x - slimeFrameDim.x, slimeFrameDim.y), slimeFrameDim),	//AttackRight
			sf::IntRect(sf::Vector2i(0, 4 * slimeFrameDim.y), slimeFrameDim),	//AttackLeft
			sf::IntRect(sf::Vector2i(slimeTexDim.x - slimeFrameDim.x, 2 * slimeFrameDim.y), slimeFrameDim),	//DeadRight
			sf::IntRect(sf::Vector2i(0, slimeTexDim.y - slimeFrameDim.y), slimeFrameDim),	//DeadLeft
		};
		EnemyAnimationData slimeData{ slimeDurations, slimeTexRects, slimeNumFrames, slimeTexture, slimeNumFrames[EnemyAnim::Attack] - 2 };
		slimeData.fReadDir[ReadDir::Right] = -1;
		slimeData.fReadDir[ReadDir::Left] = 1;
		fAnimData = slimeData;
	}
		break;
	case Enemy::Ogre:
	{
		sf::Time ogreDurations[EnemyAnim::AnimCount] = { sf::seconds(0.5f), sf::seconds(0.7f), sf::seconds(0.3f), sf::seconds(0.8f), sf::seconds(1.5f) };
		sf::Texture* ogreTexture = &textures.get(Textures::ID::Ogre);
		size_t ogreNumFrames[EnemyAnim::AnimCount] = { 4, 6, 3, 7, 9 };	//idle, walk, hit, attack, dead

		sf::Vector2i ogreFrameDim(116, 84);
		sf::Vector2u ogreTexDim = ogreTexture->getSize();
		sf::IntRect ogreTexRects[EnemyAnim::AnimCount2] =
		{
			sf::IntRect(sf::Vector2i(ogreTexDim.x - ogreFrameDim.x, ogreTexDim.y / 2.f), ogreFrameDim),	//IdleRight
			sf::IntRect(sf::Vector2i(0, 0), ogreFrameDim),	//IdleLeft
			sf::IntRect(sf::Vector2i(3 * ogreFrameDim.x, ogreTexDim.y / 2.f), ogreFrameDim),	//WalkRight
			sf::IntRect(sf::Vector2i(4 * ogreFrameDim.x, 0), ogreFrameDim),	//WalkLeft
			sf::IntRect(sf::Vector2i(ogreTexDim.x - 2 * ogreFrameDim.x, ogreTexDim.y / 2.f + 2 * ogreFrameDim.y), ogreFrameDim),	//HitRight
			sf::IntRect(sf::Vector2i(ogreFrameDim.x, 2 * ogreFrameDim.y), ogreFrameDim),	//HitLeft
			sf::IntRect(sf::Vector2i(ogreTexDim.x - 3 * ogreFrameDim.x, ogreTexDim.y / 2.f + ogreFrameDim.y), ogreFrameDim),	//AttackRight
			sf::IntRect(sf::Vector2i(2 * ogreFrameDim.x, ogreFrameDim.y), ogreFrameDim),	//AttackLeft
			sf::IntRect(sf::Vector2i(3 * ogreFrameDim.x, ogreTexDim.y / 2.f + 2 * ogreFrameDim.y), ogreFrameDim),	//DeadRight
			sf::IntRect(sf::Vector2i(4 * ogreFrameDim.x, 2 * ogreFrameDim.y), ogreFrameDim),	//DeadLeft
		};
		EnemyAnimationData ogreData{ ogreDurations, ogreTexRects, ogreNumFrames, ogreTexture, ogreNumFrames[EnemyAnim::Attack] - 3 };
		ogreData.fReadDir[ReadDir::Right] = -1;
		ogreData.fReadDir[ReadDir::Left] = 1;
		fAnimData = ogreData;
	}
		break;
	case Enemy::Oculothorax:
	{
		sf::Time oculDurations[EnemyAnim::AnimCount] = { sf::seconds(0.5f), sf::seconds(0.7f), sf::seconds(0.3f), sf::seconds(0.7f), sf::seconds(1.5f) };
		sf::Texture* oculTexture = &textures.get(Textures::ID::Oculothorax);
		size_t oculNumFrames[EnemyAnim::AnimCount] = { 4, 4, 4, 9, 8 };	//idle, walk, hit, attack, dead

		//480 x 800 -> 80 x 80
		sf::Vector2i oculFrameDim(80, 80);
		sf::Vector2u oculTexDim = oculTexture->getSize();
		sf::IntRect oculTexRects[EnemyAnim::AnimCount2] =
		{
			sf::IntRect(sf::Vector2i(oculTexDim.x - oculFrameDim.x, 5 * oculFrameDim.y), oculFrameDim),	//IdleRight
			sf::IntRect(sf::Vector2i(0, 0), oculFrameDim),	//IdleLeft
			sf::IntRect(sf::Vector2i(oculFrameDim.x, 5 * oculFrameDim.y), oculFrameDim),	//WalkRight
			sf::IntRect(sf::Vector2i(4 * oculFrameDim.x, 0), oculFrameDim),	//WalkLeft
			sf::IntRect(sf::Vector2i(0, 7 * oculFrameDim.y), oculFrameDim),	//HitRight
			sf::IntRect(sf::Vector2i(oculTexDim.x - oculFrameDim.x, 2 * oculFrameDim.y), oculFrameDim),	//HitLeft
			sf::IntRect(sf::Vector2i(3 * oculFrameDim.x, 6 * oculFrameDim.y), oculFrameDim),	//AttackRight
			sf::IntRect(sf::Vector2i(2 * oculFrameDim.x, oculFrameDim.y), oculFrameDim),	//AttackLeft
			sf::IntRect(sf::Vector2i(2 * oculFrameDim.x, 8 * oculFrameDim.y), oculFrameDim),	//DeadRight
			sf::IntRect(sf::Vector2i(3 * oculFrameDim.x, 3 * oculFrameDim.y), oculFrameDim),	//DeadLeft
		};
		EnemyAnimationData oculData{ oculDurations, oculTexRects, oculNumFrames, oculTexture, oculNumFrames[EnemyAnim::Attack] - 5 };
		oculData.fReadDir[ReadDir::Right] = -1;
		oculData.fReadDir[ReadDir::Left] = 1;
		fAnimData = oculData;
	}
		break;
	case Enemy::FireWisp:
	{
		sf::Time wispDurations[EnemyAnim::AnimCount] = { sf::seconds(0.5f), sf::seconds(0.5f), sf::seconds(0.3f), sf::seconds(0.9f), sf::seconds(0.8f) };	//idle, walk, hurt, attack, dead
		size_t wispNumFrames[EnemyAnim::AnimCount] = { 4, 4, 3, 10, 8 };

		sf::Texture* fireWispTex = &textures.get(Textures::ID::FireWisp);
		sf::IntRect fireWispRects[EnemyAnim::AnimCount2];
		sf::Vector2u fireWispTexDim = fireWispTex->getSize();
		sf::Vector2i fireWispFrameDim(124, 116);
		wispsTexRects(fireWispTexDim, fireWispFrameDim, fireWispRects, Enemy::FireWisp);
		EnemyAnimationData fireWispData{ wispDurations, fireWispRects, wispNumFrames, fireWispTex, wispNumFrames[EnemyAnim::Attack] - 3 };
		fireWispData.fReadDir[ReadDir::Left] = 1;
		fireWispData.fReadDir[ReadDir::Right] = -1;

		fAnimData = fireWispData;
		minDamageDistance = 250.f;
		setAttackOnPlayer([this, &textures](Player& player, int damage, CommandQueue& commands)
			{
				int dir = (fPlayer.getWorldPosition().x - getWorldPosition().x) / abs(fPlayer.getWorldPosition().x - getWorldPosition().x);
				(dir == 1) ? setLookingState(LookingRight) : setLookingState(LookingLeft);
				commands.addCommand(Category::Scene, [this, &textures](SceneNode& node, sf::Time)
					{
						Projectile* proj = new Projectile(textures.get(Textures::ID::FireWispProjectile), sf::IntRect(0, 0, 64, 32), (isLookingLeft()) ? -1 : 1, 175.f, fEnemyDamage);
						proj->setPosition(getWorldPosition().x, getWorldPosition().y - getBoundingRect().height / 6.f);
						node.attachChild(proj);
					});
			});

	}
		break;
	case Enemy::WaterWisp:
	{
		sf::Time wispDurations[EnemyAnim::AnimCount] = { sf::seconds(0.5f), sf::seconds(0.5f), sf::seconds(0.3f), sf::seconds(0.9f), sf::seconds(0.8f) };	//idle, walk, hurt, attack, dead
		size_t wispNumFrames[EnemyAnim::AnimCount] = { 4, 4, 3, 10, 8 };

		sf::Texture* waterWispTex = &textures.get(Textures::ID::WaterWisp);
		sf::IntRect waterWispRects[EnemyAnim::AnimCount2];
		sf::Vector2u waterWispTexDim = waterWispTex->getSize();
		sf::Vector2i waterWispFrameDim(82, 94);
		wispsTexRects(waterWispTexDim, waterWispFrameDim, waterWispRects, Enemy::WaterWisp);
		EnemyAnimationData waterWispData{ wispDurations, waterWispRects, wispNumFrames, waterWispTex, wispNumFrames[EnemyAnim::Attack] - 3 };
		waterWispData.fReadDir[ReadDir::Left] = 1;
		waterWispData.fReadDir[ReadDir::Right] = -1;

		fAnimData = waterWispData;
		minDamageDistance = 200.f;
		setAttackOnPlayer([this, &textures](Player& player, int damage, CommandQueue& commands)
			{
				int dir = (fPlayer.getWorldPosition().x - getWorldPosition().x) / abs(fPlayer.getWorldPosition().x - getWorldPosition().x);
				(dir == 1) ? setLookingState(LookingRight) : setLookingState(LookingLeft);
				commands.addCommand(Category::Scene, [this, &textures](SceneNode& node, sf::Time)
					{
						Projectile* proj = new Projectile(textures.get(Textures::ID::WaterWispProjectile), sf::IntRect(0, 0, 64, 32), (isLookingLeft()) ? -1 : 1, 175.f, fEnemyDamage);
						proj->setPosition(getWorldPosition().x, getWorldPosition().y - getBoundingRect().height / 3.f);
						node.attachChild(proj);
					});
			});
	}
		break;
	case Enemy::WindWisp:
	{
		sf::Time wispDurations[EnemyAnim::AnimCount] = { sf::seconds(0.5f), sf::seconds(0.5f), sf::seconds(0.3f), sf::seconds(0.9f), sf::seconds(0.8f) };	//idle, walk, hurt, attack, dead
		size_t wispNumFrames[EnemyAnim::AnimCount] = { 4, 4, 3, 10, 8 };

		sf::Texture* windWispTex = &textures.get(Textures::ID::WindWisp);
		sf::IntRect windWispRects[EnemyAnim::AnimCount2];
		sf::Vector2u windWispTexDim = windWispTex->getSize();
		sf::Vector2i windWispFrameDim(90, 120);
		wispsTexRects(windWispTexDim, windWispFrameDim, windWispRects, Enemy::WindWisp);
		EnemyAnimationData windWispData{ wispDurations, windWispRects, wispNumFrames, windWispTex, wispNumFrames[EnemyAnim::Attack] - 3 };
		windWispData.fReadDir[ReadDir::Left] = 1;
		windWispData.fReadDir[ReadDir::Right] = -1;

		fAnimData = windWispData;
		minDamageDistance = 300.f;
		setAttackOnPlayer([this, &textures](Player& player, int damage, CommandQueue& commands)
			{
				int dir = (fPlayer.getWorldPosition().x - getWorldPosition().x) / abs(fPlayer.getWorldPosition().x - getWorldPosition().x);
				(dir == 1) ? setLookingState(LookingRight) : setLookingState(LookingLeft);
				commands.addCommand(Category::Scene, [this, &textures](SceneNode& node, sf::Time)
					{
						Projectile* proj = new Projectile(textures.get(Textures::ID::WindWispProjectile), sf::IntRect(0, 0, 64, 32), (isLookingLeft()) ? -1 : 1, 250.f, fEnemyDamage);
						proj->setPosition(getWorldPosition().x, getWorldPosition().y - getBoundingRect().height / 5.f);
						node.attachChild(proj);
					});
			});
	}
		break;
	case Enemy::EarthWisp:
	{
		sf::Time wispDurations[EnemyAnim::AnimCount] = { sf::seconds(0.5f), sf::seconds(0.5f), sf::seconds(0.3f), sf::seconds(0.9f), sf::seconds(0.8f) };	//idle, walk, hurt, attack, dead
		size_t wispNumFrames[EnemyAnim::AnimCount] = { 4, 4, 3, 10, 8 };

		sf::Texture* earthWispTex = &textures.get(Textures::ID::EarthWisp);
		sf::IntRect earthWispRects[EnemyAnim::AnimCount2];
		sf::Vector2u earthWispTexDim = earthWispTex->getSize();
		sf::Vector2i earthWispFrameDim(94, 116);
		wispsTexRects(earthWispTexDim, earthWispFrameDim, earthWispRects, Enemy::EarthWisp);
		EnemyAnimationData earthWispData{ wispDurations, earthWispRects, wispNumFrames, earthWispTex, wispNumFrames[EnemyAnim::Attack] - 3 };
		earthWispData.fReadDir[ReadDir::Left] = 1;
		earthWispData.fReadDir[ReadDir::Right] = -1;

		fAnimData = earthWispData;
		minDamageDistance = 225.f;
		setAttackOnPlayer([this, &textures](Player& player, int damage, CommandQueue& commands)
			{
				int dir = (fPlayer.getWorldPosition().x - getWorldPosition().x) / abs(fPlayer.getWorldPosition().x - getWorldPosition().x);
				(dir == 1) ? setLookingState(LookingRight) : setLookingState(LookingLeft);
				commands.addCommand(Category::Scene, [this, &textures](SceneNode& node, sf::Time)
					{
						Projectile* proj = new Projectile(textures.get(Textures::ID::EarthWispProjectile), sf::IntRect(0, 0, 64, 32), (isLookingLeft()) ? -1 : 1, 200.f, fEnemyDamage);
						proj->setPosition(getWorldPosition().x, getWorldPosition().y - getBoundingRect().height / 6.f);
						node.attachChild(proj);
					});
			});
	}
		break;
	default:
		throw std::logic_error("the specified enemy type doesn't exist");
		break;
	}
}

void Enemy::canDoAttack()
{
	dontMove(fAnimData.fDurations[EnemyAnim::Attack]);
	if (!fCanAttack)
		fCanAttack = true;
}

float Enemy::getMinDamageDistance() const
{
	return minDamageDistance;
}

void Enemy::setAttackOnPlayer(std::function<void(Player& player, int damage, CommandQueue& commands)> func)
{
	attackOnPlayer = func;
}

int Enemy::getDamage() const
{
	return fEnemyDamage;
}

void Enemy::setDamage(int newDamage)
{
	fEnemyDamage = newDamage;
}

void Enemy::setAnimations()
{

	//Idle when looking right
	Animation* IdleRight = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[EnemyAnim::IdleRight].left, fAnimData.fTextureRects[EnemyAnim::IdleRight].top),
		fAnimData.fReadDir[ReadDir::Right]);
	IdleRight->Repeat(true);
	IdleRight->setDuration(fAnimData.fDurations[EnemyAnim::Idle]);
	IdleRight->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[EnemyAnim::Idle].width, fAnimData.fTextureRects[EnemyAnim::Idle].height));
	IdleRight->setNumFrames(fAnimData.fNumFrames[EnemyAnim::Idle]);
	IdleRight->Restart();

	fAnimations[AnimType::Idle] = IdleRight;

	//Idle when looking left
	Animation* IdleLeft = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[EnemyAnim::IdleLeft].left, fAnimData.fTextureRects[EnemyAnim::IdleLeft].top), 
		fAnimData.fReadDir[ReadDir::Left]);
	IdleLeft->Repeat(true);
	IdleLeft->setDuration(fAnimData.fDurations[EnemyAnim::Idle]);
	IdleLeft->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[EnemyAnim::Idle].width, fAnimData.fTextureRects[EnemyAnim::Idle].height));
	IdleLeft->setNumFrames(fAnimData.fNumFrames[EnemyAnim::Idle]);
	IdleLeft->Restart();

	fAnimations[AnimType::IdleLeft] = IdleLeft;

	//Walk when looking right
	Animation* WalkRight = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[EnemyAnim::WalkRight].left, fAnimData.fTextureRects[EnemyAnim::WalkRight].top),
		fAnimData.fReadDir[ReadDir::Right]);
	WalkRight->Repeat(true);
	WalkRight->setDuration(fAnimData.fDurations[EnemyAnim::Walk]);
	WalkRight->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[EnemyAnim::Walk].width, fAnimData.fTextureRects[EnemyAnim::Walk].height));
	WalkRight->setNumFrames(fAnimData.fNumFrames[EnemyAnim::Walk]);
	WalkRight->Restart();

	fAnimations[AnimType::Run] = WalkRight;
	
	//Walk when looking left
	Animation* WalkLeft = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[EnemyAnim::WalkLeft].left, fAnimData.fTextureRects[EnemyAnim::WalkLeft].top),
		fAnimData.fReadDir[ReadDir::Left]);
	WalkLeft->Repeat(true);
	WalkLeft->setDuration(fAnimData.fDurations[EnemyAnim::Walk]);
	WalkLeft->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[EnemyAnim::Walk].width, fAnimData.fTextureRects[EnemyAnim::Walk].height));
	WalkLeft->setNumFrames(fAnimData.fNumFrames[EnemyAnim::Walk]);
	WalkLeft->Restart();

	fAnimations[RunLeft] = WalkLeft;

	//Hit when looking right
	Animation* HitRight = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[EnemyAnim::beingHitRight].left, fAnimData.fTextureRects[EnemyAnim::beingHitRight].top),
		fAnimData.fReadDir[ReadDir::Right]);
	HitRight->Repeat(false);
	HitRight->setDuration(fAnimData.fDurations[EnemyAnim::beingHit]);
	HitRight->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[EnemyAnim::beingHit].width, fAnimData.fTextureRects[EnemyAnim::beingHit].height));
	HitRight->setNumFrames(fAnimData.fNumFrames[EnemyAnim::beingHit]);
	HitRight->Restart();

	fAnimations[AnimType::beingHit] = HitRight;

	//Hit when looking left
	Animation* HitLeft = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[EnemyAnim::beingHitLeft].left, fAnimData.fTextureRects[EnemyAnim::beingHitLeft].top),
		fAnimData.fReadDir[ReadDir::Left]);
	HitLeft->Repeat(false);
	HitLeft->setDuration(fAnimData.fDurations[EnemyAnim::beingHit]);
	HitLeft->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[EnemyAnim::beingHit].width, fAnimData.fTextureRects[EnemyAnim::beingHit].height));
	HitLeft->setNumFrames(fAnimData.fNumFrames[EnemyAnim::beingHit]);
	HitLeft->Restart();

	fAnimations[AnimType::beingHitLeft] = HitLeft;

	//Attack when looking right (NOTE: not sure if it should be on repeat mode)
	Animation* AttackRight = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[EnemyAnim::AttackRight].left, fAnimData.fTextureRects[EnemyAnim::AttackRight].top), 
		fAnimData.fReadDir[ReadDir::Right]);
	AttackRight->Repeat(true);
	AttackRight->setDuration(fAnimData.fDurations[EnemyAnim::Attack]);
	AttackRight->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[EnemyAnim::Attack].width, fAnimData.fTextureRects[EnemyAnim::Attack].height));
	AttackRight->setNumFrames(fAnimData.fNumFrames[EnemyAnim::Attack]);
	AttackRight->Restart();

	fAnimations[AnimType::Attack1] = AttackRight;

	//Attack when looking left (NOTE: not sure if it should be on repeat mode)
	Animation* AttackLeft = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[EnemyAnim::AttackLeft].left, fAnimData.fTextureRects[EnemyAnim::AttackLeft].top), 
		fAnimData.fReadDir[ReadDir::Left]);
	AttackLeft->Repeat(true);
	AttackLeft->setDuration(fAnimData.fDurations[EnemyAnim::Attack]);
	AttackLeft->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[EnemyAnim::Attack].width, fAnimData.fTextureRects[EnemyAnim::Attack].height));
	AttackLeft->setNumFrames(fAnimData.fNumFrames[EnemyAnim::Attack]);
	AttackLeft->Restart();

	fAnimations[AnimType::Attack1_left] = AttackLeft;

	//Death when looking right
	Animation* DeathRight = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[EnemyAnim::DeathRight].left, fAnimData.fTextureRects[EnemyAnim::DeathRight].top), 
		fAnimData.fReadDir[ReadDir::Right]);
	DeathRight->Repeat(false);
	DeathRight->setDuration(fAnimData.fDurations[EnemyAnim::Death]);
	DeathRight->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[EnemyAnim::Death].width, fAnimData.fTextureRects[EnemyAnim::Death].height));
	DeathRight->setNumFrames(fAnimData.fNumFrames[EnemyAnim::Death]);
	DeathRight->Restart();

	fAnimations[AnimType::Death] = DeathRight;

	//Death when looking left
	Animation* DeathLeft = new Animation(*fAnimData.fTexture, sf::Vector2i(fAnimData.fTextureRects[EnemyAnim::DeathLeft].left, fAnimData.fTextureRects[EnemyAnim::DeathLeft].top), 
		fAnimData.fReadDir[ReadDir::Left]);
	DeathLeft->Repeat(false);
	DeathLeft->setDuration(fAnimData.fDurations[EnemyAnim::Death]);
	DeathLeft->setFrameSize(sf::Vector2i(fAnimData.fTextureRects[EnemyAnim::Death].width, fAnimData.fTextureRects[EnemyAnim::Death].height));
	DeathLeft->setNumFrames(fAnimData.fNumFrames[EnemyAnim::Death]);
	DeathLeft->Restart();

	fAnimations[AnimType::DeathLeft] = DeathLeft;

}
