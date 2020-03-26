#include "WorldTavernInside.h"
#include "Player.h"
#include "BackGroundNode.h"
#include "Ladder.h"
#include "TalkingNPC.h"
#include "HitBox.h"
#include "Platform.h"

WorldTavernInside::WorldTavernInside(BeginningWorld::Context context, Stack<BeginningWorld, Worlds::ID, BeginningWorld::Context>& stack)
	:BeginningWorld(context, stack)
{
	buildWorld();
}

bool WorldTavernInside::draw()
{
	BeginningWorld::draw();
	return false;
}

bool WorldTavernInside::update(sf::Time dt)
{
	return BeginningWorld::update(dt);
}

void WorldTavernInside::buildWorld()
{
	fPlayer->setPosition(fWorldView.getCenter().x - 85.f, fWorldView.getCenter().y - fPlayer->getBoundingRect().height / 2.f + 170.f);

	BackGroundNode* tavernInsideBack = new BackGroundNode(fTextures.get(Textures::ID::TavernInsideBack));
	tavernInsideBack->setPosition(fWorldView.getCenter().x - tavernInsideBack->getBoundingRect().width / 2.f, fWorldView.getCenter().y - tavernInsideBack->getBoundingRect().height / 2.f);

	Layers[Background]->attachChild(tavernInsideBack);

	TalkingNPC* bartender = new TalkingNPC(1, NonPlayerChar::Type::Mage, State::Context(fWindow, fTextures, fFonts, *fPlayerData), Question("Hey ! N'oublie pas de me rapporter\nle livre magique !"));
	bartender->setPosition(tavernInsideBack->getWorldPosition().x + 57.f + bartender->getBoundingRect().width / 2.f, tavernInsideBack->getWorldPosition().y + bartender->getBoundingRect().height / 2.f - 11.f);
	bartender->doesGravitiyApply(false);

	Layers[Interaction]->attachChild(bartender);

	BackGroundNode* tavernInsideFront = new BackGroundNode(fTextures.get(Textures::ID::TavernInsideFront));
	tavernInsideFront->setPosition(tavernInsideBack->getPosition());

	Layers[Interaction]->attachChild(tavernInsideFront);


	BackGroundNode* tavernCellar = nullptr;
	if (fPlayerData->fChangeWorld)
	{
		sf::Texture& tavernCellarTex = fTextures.get(Textures::ID::TavernCellarOpen);

		tavernCellar = new BackGroundNode(tavernCellarTex);
		tavernCellar->setPosition(tavernInsideBack->getPosition().x, tavernInsideBack->getPosition().y + tavernInsideBack->getBoundingRect().height);

		Layers[Background]->attachChild(tavernCellar);
	}
	else
	{
		sf::Texture& tavernCellarTex = fTextures.get(Textures::ID::TavernCellarClose);

		tavernCellar = new BackGroundNode(tavernCellarTex);
		tavernCellar->setPosition(tavernInsideBack->getPosition().x, tavernInsideBack->getPosition().y + tavernInsideBack->getBoundingRect().height);

		Layers[Background]->attachChild(tavernCellar);
	}


	if (fPlayerData->fChangeWorld)
	{
		HitBox* kanalisationEntry = new HitBox(sf::FloatRect(0.f, 0.f, 58, 43));
		kanalisationEntry->setPosition(5, 46);
		kanalisationEntry->fPlayerEntering = [this]()
		{
			if(fPlayer->requestEnteringWorld())
				transitionToWorld(Worlds::ID::Kanalisation);
		};

		tavernCellar->attachChild(kanalisationEntry);
	}


	Ladder* tavLadder = new Ladder(Ladder::Type::NormalLadder, fTextures, 2);
	tavLadder->setFloorPosition(tavernCellar->getWorldPosition().x, tavernCellar->getWorldPosition().y + tavernCellar->getBoundingRect().height);

	Layers[Interaction]->attachChild(tavLadder);

	Platform* tavFloor = new Platform(fTextures, 7, 1, false);
	tavFloor->setPosition(tavernCellar->getWorldPosition().x + fPlayer->getBoundingRect().width, tavernCellar->getWorldPosition().y - 20.f);

	Layers[Interaction]->attachChild(tavFloor);

	fTavernUpfloor = tavFloor->getWorldPosition().y;
	fPlayer->fFloorLevel = fFloorLevel = tavernCellar->getWorldPosition().y + tavernCellar->getBoundingRect().height - 20.f;
	fWorldBounds= sf::FloatRect(tavernInsideBack->getWorldPosition().x, tavernInsideBack->getWorldPosition().y, tavernInsideBack->getBoundingRect().width, tavernInsideBack->getBoundingRect().height * 2.f);
	fWorldView.setCenter(fWorldView.getSize().x / 2.f, fWorldView.getSize().y / 2.f);
}

void WorldTavernInside::handleMovement(sf::Time dt)
{
	while (isOutOfTheWorld(fPlayer->getBoundingRect())) {
		Utility::OutSide out = isOutOfTheWorld(fPlayer->getBoundingRect());
		if (out == Utility::Left)
			fPlayer->move(10.f * dt.asSeconds(), 0.f);
		if (out == Utility::Right)
		{
			if (fPlayer->getWorldPosition().y <= fTavernUpfloor)
			{
				if (!fStopPlayerMotion)
				{
					transitionToWorld(Worlds::ID::OutsideTavern);
					fStopPlayerMotion = true;
					break;
				}
				else
				{
					fPlayer->move(-10.f * dt.asSeconds(), 0.f);
				}
			}
			else
			{
				fPlayer->move(-10.f * dt.asSeconds(), 0.f);
			}
		}
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
