#include "MinotaurWorld.h"
#include "Minotaur.h"
#include "Item.h"
#include "Platform.h"
#include "UtilityFunc.h"
#include "Player.h"
#include "Cabury.h"
#include "MagicBook.h"

MinotaurWorld::MinotaurWorld(BeginningWorld::Context context, Stack<BeginningWorld, Worlds::ID, BeginningWorld::Context>& stack)
	:BeginningWorld(context, stack)
{
	buildWorld();
}

bool MinotaurWorld::update(sf::Time dt)
{
	if (fCabu->getPosition().x >= WIDTH && !fSpawnedMinotaur)
	{
		fSpawnedMinotaur = true;
		fMinotaur = new Minotaur(200, fTextures, fFonts, *fPlayer, getBossItems());
		fMinotaur->setPosition(fWorldView.getCenter().x, 0.f);
		Layers[PlayerChar]->attachChild(fMinotaur);
	}
	
	if (fMinotaur && fMinotaur->isDestroyed())	//i.e the minotaur is dead
	{
		fCabu->setCinematicState(Cabury::CinematicState::EndingAtBoss);
		fMinotaur = nullptr;
	}

	if (fCabu->fNoBook && !fSpawnedBook)
	{
		fMagicBook = new MagicBook(fTextures);
		fMagicBook->setPosition(fCabu->getWorldPosition().x - 25.f, fCabu->getWorldPosition().y);

		Layers[PlayerChar]->attachChild(fMagicBook);
		fSpawnedBook = true;
	}

	if (fSpawnedBook && fMagicBook)
	{
		if (fCanSlide)
		{
			fBookSlidingAccu += dt;
			if (fBookSlidingAccu >= fBookSlidingTime)
			{
				fBookSlidingAccu = sf::Time::Zero;
				fCanSlide = false;
				Item* magicBookItem = new Item(1, fTextures.get(Textures::ID::MagicBookItem), ItemType(Item::Type::GoldCoin, sf::IntRect(0, 0, 18, 25), fWorldBounds, 0.f));
				magicBookItem->canPickedUp(true);
				fPlayer->addInventoryItem(magicBookItem);

				fPlayer->setCinematicState(Player::CinematicState::Null);
				fMagicBook->remove();
			}
			else
			{
				int dir = abs(fPlayer->getWorldPosition().x - fMagicBook->getWorldPosition().x) / (fPlayer->getWorldPosition().x - fMagicBook->getWorldPosition().x);
				fMagicBook->move(50.f * dir * abs(fBookSlidingTime.asSeconds() - fBookSlidingAccu.asSeconds()) * dt.asSeconds(), -100.f * abs(fBookSlidingTime.asSeconds() - fBookSlidingAccu.asSeconds()) * dt.asSeconds());
			}
		}
	}

	BeginningWorld::update(dt);
	return false;
}

bool MinotaurWorld::draw()
{
	BeginningWorld::draw();
	return false;
}

void MinotaurWorld::buildWorld()
{
	BackGroundNode* background = new BackGroundNode(fTextures.get(Textures::ID::BossBG_Test));
	Layers[Background]->attachChild(background);

	Platform* plat = new Platform(fTextures, fWorldBounds.width / fTextures.get(Textures::ID::RepeatedBlock_Small).getSize().x + 5, 2);
	plat->setPosition(fWorldBounds.left, fWorldView.getCenter().y + 150.f);

	fFloorLevel = plat->getWorldPosition().y;

	Layers[Interaction]->attachChild(plat);

	fWorldBounds = sf::FloatRect(0, 0, fWorldView.getSize().x, fWorldView.getSize().y);

	
	fCabu = new Cabury(50, fTextures, *fPlayer, Cabury::CinematicState::BeginningAtBoss);
	fCabu->setPosition(fWorldBounds.left + fWorldBounds.width - fCabu->getBoundingRect().width, fFloorLevel - fCabu->getBoundingRect().height / 2.f);
	
	Layers[PlayerChar]->attachChild(fCabu);

	fPlayer->setPosition(100.f, fFloorLevel - fPlayer->getBoundingRect().height / 2.f);
	fPlayer->setCinematicState(Player::CinematicState::MinotaurBegin);
}

void MinotaurWorld::handleMovement(sf::Time dt)
{
	while (isOutOfTheWorld(fPlayer->getBoundingRect())) {
		Utility::OutSide out = isOutOfTheWorld(fPlayer->getBoundingRect());
		if (out == Utility::Left)
		{
			transitionToWorld(Worlds::ID::Kanalisation);
			break;
		}
		if (out == Utility::Right)
			fPlayer->move(-10.f * dt.asSeconds(), 0.f);
		if (out == Utility::Top) {
			fPlayer->setUpwardsVel(0.f);
			break;
		}
		else if (out == Utility::Bottom)
		{
			break;
		}
	}
}
