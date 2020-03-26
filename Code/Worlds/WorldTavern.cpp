#include "WorldTavern.h"
#include "Player.h"
#include "Item.h"
#include "Answer.h"
#include "Stack.h"
#include "TalkingNPC.h"
#include "ShoppingNPC.h"
#include "BreakableBox.h"
#include "Cabury.h"
#include <iostream>
#include "Ladder.h"
#include "HitBox.h"
#include "Platform.h"

WorldTavern::WorldTavern(BeginningWorld::Context context, Stack<BeginningWorld, Worlds::ID, BeginningWorld::Context>& stack)
	:BeginningWorld(context, stack)
{
	fWorldBounds = sf::FloatRect(0, 0, fWorldView.getSize().x * 1.5f, fWorldView.getSize().y + fWorldView.getSize().y / 2.f);
	fFloorLevel = 450.f;
	buildWorld();
}

bool WorldTavern::draw()
{
	BeginningWorld::draw();
	return false;
}

bool WorldTavern::update(sf::Time dt)
{

	if (cabu && cabu->isDestroyed())
	{
		cabu = nullptr;
		fCanViewMove = true;
	}
	return BeginningWorld::update(dt);
}

void WorldTavern::buildWorld()
{

	sf::Texture& texForest = fTextures.get(Textures::ID::Forest);
	texForest.setRepeated(true);
	BackGroundNode* forest = new BackGroundNode(texForest, sf::IntRect(fWorldBounds));
	forest->setPosition(0.f, 0.f);

	Layers[Background]->attachChild(forest);

	BackGroundNode* tavern = new BackGroundNode(fTextures.get(Textures::ID::TavernOutside));
	tavern->setPosition(200.f, fFloorLevel - tavern->getBoundingRect().height);

	Layers[Background]->attachChild(tavern);

	HitBox* tavernEntry = new HitBox(sf::FloatRect(0.f, 0.f, 83, 80));
	tavernEntry->setPosition(146.f + tavern->getWorldPosition().x, tavern->getWorldPosition().y + 171.f);
	tavernEntry->fPlayerEntering = [this]()
	{
		if(fPlayer->requestEnteringWorld())
			transitionToWorld(Worlds::ID::InsideTavern);
	};

	Layers[Interaction]->attachChild(tavernEntry);

	BreakableBox* dogBox = new BreakableBox(BreakableBox::Type::Crate, fTextures);
	dogBox->setOnDestroyedAction([this, dogBox]()
		{
			fCommands.addCommand(Category::Scene, [this, dogBox](SceneNode& node, sf::Time)
				{
					Cabury* dog = new Cabury(50, fTextures, *fPlayer, Cabury::CinematicState::BreakingBox);
					dog->setPosition(dogBox->getWorldPositionCentered().x, fFloorLevel - dog->getBoundingRect().height / 2.f);
					node.attachChild(dog);
					cabu = dog;
				});
		});
	dogBox->setPosition(tavern->getBoundingRect().left - 5.f, fFloorLevel - dogBox->getSpriteBoundingRect().height);

	Layers[Interaction]->attachChild(dogBox);


	spawnNPCs();
}

void WorldTavern::spawnNPCs()
{

	Question speach{ "Bonjour, petit !" };
	Question q1{ "Ah oui, le chien de Savaric Poudevigne.\nOui, il vient de passé, surement\npour rejoindre son maître.. " };
	Question q2{ "Si vous voulez le retrouver,\n passez par les égouts de la taverne\nque vous trouverez dans la cave,\n mais faites attention,\nil peut y avoir des monstres." };

	Answer a1_q1{ "Et comment pourrais-je le trouver ?", q2 };
	a1_q1.callback_func = [this]()
	{

		fCommands.addCommand(Category::NPC, derivedAction<NonPlayerChar>([this](NonPlayerChar& npc, sf::Time)
			{
				fPlayerData->fChangeWorld = true;
			}));
	};
	q1.addAnswer(a1_q1);
	Answer a1_speach{ "Bonjour, saurez-vous où est parti\nle chien qui vient de fuir ?", q1 };

	speach.addAnswer(a1_speach);

	TalkingNPC* NPC = new TalkingNPC(1, NonPlayerChar::Goblin, State::Context(fWindow, fTextures, fFonts, *fPlayerData), speach);
	NPC->setPosition(fWorldBounds.left + fWorldBounds.width - 325.f, fFloorLevel - NPC->getBoundingRect().height / 2.f);

	Layers[SceneLayers::Interaction]->attachChild(NPC);

}

void WorldTavern::handleMovement(sf::Time dt)
{
	if (fCanViewMove) {
		if (fPlayer->getWorldPosition().x > fWorldBounds.width / 1.5f || fFlag)
		{
			fFlag = true;
			fWorldView.setCenter(fPlayer->getWorldPosition().x, fWorldBounds.top + fWorldView.getSize().y / 2.f);

			Utility::OutSide isOut = isOutOfTheWorld(sf::FloatRect(fWorldView.getCenter().x - fWorldView.getSize().x / 2.f, fWorldView.getCenter().y - fWorldView.getSize().y / 2.f, fWorldView.getSize().x, fWorldView.getSize().y));
			if (isOut == Utility::Left)
			{
				fWorldView.setCenter(fWorldBounds.left + fWorldView.getSize().x / 2.f, fWorldView.getCenter().y);
			}
			else if (isOut == Utility::Right)
			{
				fWorldView.setCenter(fWorldBounds.left + fWorldBounds.width - fWorldView.getSize().x / 2.f, fWorldView.getCenter().y);
			}
		}

		while (isOutOfTheWorld(fPlayer->getBoundingRect())) {
			Utility::OutSide out = isOutOfTheWorld(fPlayer->getBoundingRect());
			if (out == Utility::Left)
				fPlayer->move(10.f * dt.asSeconds(), 0.f);
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
	else
	{
		while (Utility::isOutOfTheRect(fPlayer->getBoundingRect(), sf::FloatRect(fWorldBounds.left, fWorldBounds.top, fWorldBounds.width / 1.5f, fWorldBounds.height))) {
			Utility::OutSide out = Utility::isOutOfTheRect(fPlayer->getBoundingRect(), sf::FloatRect(fWorldBounds.left, fWorldBounds.top, fWorldBounds.width / 1.5f, fWorldBounds.height));
			if (out == Utility::Left)
				fPlayer->move(10.f * dt.asSeconds(), 0.f);
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

}
