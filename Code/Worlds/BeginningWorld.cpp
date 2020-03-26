#include "BeginningWorld.h"
#include "Player.h"
#include "FollowingEnemy.h"
#include "GuardEnemy.h"
#include "Answer.h"
#include <iostream>
#include "TalkingNPC.h"
#include "Item.h"
#include "Minotaur.h"
#include <fstream>
#include <sstream>
#include "Platform.h"
#include "Projectile.h"
#include "BreakableBox.h"
#include "Ladder.h"
#include "ShoppingNPC.h"
#include "Cabury.h"
#include "HitBox.h"
#include "Stack.h"

namespace ItemChanceOfDrop
{
	float BrownHelmet = 0.5f;
	float GrayHelmet = 0.2f;
	float BlueHelmet = 0.2f;
	float GoldHelmet = 0.07f;
	float RedHelmet = 0.03f;

	float BrownSword = 0.5f;
	float WhiteSword = 0.3f;
	float SilverSword = 0.1f;
	float GoldSword = 0.05f;
	float RedSword = 0.05f;

	float BrownCoin = 0.5f;
	float OrangeCoin = 0.2f;
	float SilverCoin = 0.2f;
	float GoldCoin = 0.08f;
	float BlueCoin = 0.02f;

	float GreenDiamond = 0.5f;
	float YellowDiamond = 0.2f;
	float BlueDiamond = 0.1f;
	float RedDiamond = 0.1f;
	float SilverDiamond = 0.05f;
	float PinkDiamond = 0.05f;

}

//const float viewTransiSpeed = 1627.118644f;

void BeginningWorld::getCoinsItem(std::vector<ItemType>& coins) const
{
	assert(coins.size() == 100);
	int CoinIndex = 0;
	for (int i = 0; i < int(ItemChanceOfDrop::BrownCoin * 100.f); ++i)	//brown coin
	{
		coins[i] = ItemType(Item::Type::BrownCoin_stack1, sf::IntRect(176, 80, 16, 16), [](Player& player) { player.addMoney(2); std::cout << "debug1" << std::endl;
		return true; }, fWorldBounds, -2.5f);	//hardcoded values for the texture coordinates
	}
	CoinIndex += int(ItemChanceOfDrop::BrownCoin * 100.f);

	for (int i = 0; i < int(ItemChanceOfDrop::OrangeCoin * 100.f); ++i)	//orange coin
	{
		coins[i + CoinIndex] = ItemType(Item::Type::OrangeCoin_stack1, sf::IntRect(192, 80, 16, 16), [](Player& player) { player.addMoney(4); std::cout << "debug2" << std::endl;
		return true; }, fWorldBounds, -2.7f);
	}
	CoinIndex += int(ItemChanceOfDrop::OrangeCoin * 100.f);

	for (int i = 0; i < int(ItemChanceOfDrop::SilverCoin * 100.f); ++i)	//silver coin
	{
		coins[i + CoinIndex] = ItemType(Item::Type::SilverCoin_stack1, sf::IntRect(208, 80, 16, 16), [](Player& player) { player.addMoney(10); std::cout << "debug3" << std::endl;
		return true; }, fWorldBounds, -2.7f);
	}
	CoinIndex += int(ItemChanceOfDrop::SilverCoin * 100.f);

	for (int i = 0; i < int(ItemChanceOfDrop::GoldCoin * 100.f); ++i)	//Gold coin
	{
		coins[i + CoinIndex] = ItemType(Item::Type::GoldCoin_stack1, sf::IntRect(224, 80, 16, 16), [](Player& player) { player.addMoney(15); std::cout << "debug4" << std::endl;
		return true; }, fWorldBounds, -3.2f);
	}
	CoinIndex += int(ItemChanceOfDrop::GoldCoin * 100.f);

	for (int i = 0; i < int(ItemChanceOfDrop::BlueCoin * 100.f); ++i)	//Blue coin
	{
		coins[i + CoinIndex] = ItemType(Item::Type::BlueCoin_stack1, sf::IntRect(240, 80, 16, 16), [](Player& player) { player.addMoney(20); std::cout << "debug5" << std::endl;
		return true; }, fWorldBounds, -3.2f);
	}
	
}

void BeginningWorld::getHelmetsItem(std::vector<ItemType>& helmets) const
{
	assert(helmets.size() == 100);
	int helmetsIndex = 0;
	for (int i = 0; i < int(ItemChanceOfDrop::BrownHelmet * 100.f); ++i)	//brown helmet
	{
		helmets[i] = ItemType(Item::Type::BrownHelmet_2, sf::IntRect(64, 144, 16, 16),
			[](Player& player) { player.addResistance(int((1.f - (float(player.getResistance()) - 5.f) / float(player.getResistance())) * 100.f)); 
		return true; }, fWorldBounds, -3.7f);	//hardcoded values for the texture coordinates
	}
	helmetsIndex += int(ItemChanceOfDrop::BrownHelmet * 100.f);

	for (int i = 0; i < int(ItemChanceOfDrop::GrayHelmet * 100.f); ++i)	//Gray helmet
	{
		helmets[i + helmetsIndex] = ItemType(Item::Type::SilverHelmet_2, sf::IntRect(64 + 16, 144, 16, 16),
			[](Player& player) { player.addResistance(int((1.f - (float(player.getResistance()) - 8.f) / float(player.getResistance())) * 100.f)); 
		return true; }, fWorldBounds, -4.2f);
	}
	helmetsIndex += int(ItemChanceOfDrop::GrayHelmet * 100.f);

	for (int i = 0; i < int(ItemChanceOfDrop::BlueHelmet * 100.f); ++i)	//Blue helmet
	{
		helmets[i + helmetsIndex] = ItemType(Item::Type::BlueHelmet_2, sf::IntRect(64 + 2 * 16, 144, 16, 16), 
			[](Player& player) { player.addResistance(int((1.f - (float(player.getResistance()) - 15.f) / float(player.getResistance())) * 100.f)); 
		return true; }, fWorldBounds, -4.2f);
	}
	helmetsIndex += int(ItemChanceOfDrop::BlueHelmet * 100.f);

	for (int i = 0; i < int(ItemChanceOfDrop::GoldHelmet * 100.f); ++i)	//Gold helmet
	{
		helmets[i + helmetsIndex] = ItemType(Item::Type::YellowHelmet_2, sf::IntRect(64 + 3 * 16, 144, 16, 16), 
			[](Player& player) { player.addResistance(int((1.f - (float(player.getResistance()) - 30.f) / float(player.getResistance())) * 100.f)); 
		return true; }, fWorldBounds, -4.7f);
	}
	helmetsIndex += int(ItemChanceOfDrop::GoldHelmet * 100.f);

	for (int i = 0; i < int(ItemChanceOfDrop::RedHelmet * 100.f); ++i)	//Red helmet
	{
		helmets[i + helmetsIndex] = ItemType(Item::Type::RedHelmet_2, sf::IntRect(64 + 4 * 16, 144, 16, 16), [](Player& player) { player.addResistance(40.f); 
		return true; }, fWorldBounds, -4.7f);
	}
}

void BeginningWorld::getSwordsItem(std::vector<ItemType>& swords) const
{
	assert(swords.size() == 100);
	int swordsIndex = 0;
	for (int i = 0; i < int(ItemChanceOfDrop::BrownSword * 100.f); ++i)	//brown sword
	{
		swords[i] = ItemType(Item::Type::BrownSword_3, sf::IntRect(112, 288, 16, 16),
			[](Player& player) { player.addStrength(int((1.f - (float(player.getStrength()) - 1.f) / float(player.getStrength())) * 100.f));
		return true; }, fWorldBounds, -3.5f);	//hardcoded values for the texture coordinates
	}
	swordsIndex += int(ItemChanceOfDrop::BrownSword * 100.f);

	for (int i = 0; i < int(ItemChanceOfDrop::WhiteSword * 100.f); ++i)	//white sword
	{
		swords[i + swordsIndex] = ItemType(Item::Type::WhiteSword_3, sf::IntRect(112 + 16, 288, 16, 16),
			[](Player& player) { player.addStrength(int((1.f - (float(player.getStrength()) - 6.f) / float(player.getStrength())) * 100.f));
		return true; }, fWorldBounds, -4.f);
	}
	swordsIndex += int(ItemChanceOfDrop::WhiteSword * 100.f);

	for (int i = 0; i < int(ItemChanceOfDrop::SilverSword * 100.f); ++i)	//silver sword
	{
		swords[i + swordsIndex] = ItemType(Item::Type::SilverSword_3, sf::IntRect(112 + 2 * 16, 288, 16, 16),
			[](Player& player) { player.addStrength(int((1.f - (float(player.getStrength()) - 10.f) / float(player.getStrength())) * 100.f));
		return true; }, fWorldBounds, -4.f);
	}
	swordsIndex += int(ItemChanceOfDrop::SilverSword * 100.f);

	for (int i = 0; i < int(ItemChanceOfDrop::GoldSword * 100.f); ++i)	//gold sword
	{
		swords[i + swordsIndex] = ItemType(Item::Type::GoldSword_3, sf::IntRect(112 + 3 * 16, 288, 16, 16),
			[](Player& player) { player.addStrength(int((1.f - (float(player.getStrength()) - 30.f) / float(player.getStrength())) * 100.f)); 
		return true; }, fWorldBounds, -4.5f);
	}
	swordsIndex += int(ItemChanceOfDrop::GoldSword * 100.f);

	for (int i = 0; i < int(ItemChanceOfDrop::RedSword * 100.f); ++i)	//red sword
	{
		swords[i + swordsIndex] = ItemType(Item::Type::RedSword_3, sf::IntRect(112 + 4 * 16, 288, 16, 16), [](Player& player) { player.addStrength(50.f); 
		return true; }, fWorldBounds, -4.5f);
	}
}

std::vector<ItemType> BeginningWorld::getDiamonds() const
{
	//diamonds
	std::vector<ItemType> diamonds(10);
	for (int i = 0; i < 10; ++i)
	{
		diamonds[i] = ItemType(Item::Type((1 << 31) | (1 << 30) | (1 << (2 + i))), sf::IntRect(240 * ((i == 0) ? 1 : 0) + (i - ((i == 0) ? 0 : 1)) * 16, 96 + ((i == 0) ? 0 : 16), 16, 16), fWorldBounds, 0.f);
	}
	return diamonds;
}

std::vector<std::vector<ItemType>> BeginningWorld::getMobItems() const
{
	std::vector<std::vector<ItemType>> mobItems;
	size_t size = 100;
	//coins
	std::vector<ItemType> coins(size);
	getCoinsItem(coins);
	std::vector<ItemType> coins1(size);
	getCoinsItem(coins1);

	//diamonds
	//std::vector<ItemType> diamonds(size);
	//for (int i = 0; i < size; ++i)
	//{
	//	diamonds[i] = ItemType(Item::Type((1 << 31) | (1 << 30) | (1 << (2 + i))), sf::IntRect(240 * ((i == 0) ? 1 : 0) + (i - ((i == 0) ? 0 : 1)) * 16, 96 + ((i == 0) ? 0 : 16), 16, 16));
	//}

	//helmets
	std::vector<ItemType> helmets(size);
	getHelmetsItem(helmets);

	//swords
	std::vector<ItemType> swords(size);
	getSwordsItem(swords);

	//candies
	//std::vector<ItemType> candies(size);
	//for (int i = 0; i < size; ++i)
	//{
	//	candies[i] = ItemType(Item::Type((1 << 31) | (1 << (2 + i))), sf::IntRect(240 * ((i == 0) ? 1 : 0) + (i - ((i == 0) ? 0 : 1)) * 16, 64 + ((i == 0) ? 0 : 16), 16, 16));
	//}

	//books
	//std::vector<ItemType> books(size);
	//for (int i = 0; i < size; ++i)
	//{
	//	books[i] = ItemType(Item::Type((1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << (19 + i))),
	//		sf::IntRect( 240 * ((i == 0) ? 1 : 0) + (i - ((i == 0) ? 0 : 1)) * 16, 240 + ((i == 0) ? 0 : 16), 16, 16));
	//}


	mobItems.push_back(coins);
	mobItems.push_back(coins1);
	mobItems.push_back(helmets);
	mobItems.push_back(swords);
	//mobItems.push_back(diamonds);
	//mobItems.push_back(candies);
	//mobItems.push_back(books);

	return mobItems;
}

std::vector<std::vector<struct ItemType>> BeginningWorld::getBossItems() const
{

	std::vector<std::vector<ItemType>> bossItems;
	size_t size = 100;
	//coins
	std::vector<ItemType> coins(size);
	getCoinsItem(coins);
	std::vector<ItemType> coins1(size);
	getCoinsItem(coins1);
	std::vector<ItemType> coins2(size);
	getCoinsItem(coins2);
	std::vector<ItemType> coins3(size);
	getCoinsItem(coins3);
	std::vector<ItemType> coins4(size);
	getCoinsItem(coins4);
	std::vector<ItemType> coins5(size);
	getCoinsItem(coins5);

	//helmets
	std::vector<ItemType> helmets(size);
	getHelmetsItem(helmets);
	std::vector<ItemType> helmets1(size);
	getHelmetsItem(helmets1);
	std::vector<ItemType> helmets2(size);
	getHelmetsItem(helmets2);
	std::vector<ItemType> helmets3(size);
	getHelmetsItem(helmets3);

	//swords
	std::vector<ItemType> swords(size);
	getSwordsItem(swords);
	std::vector<ItemType> swords1(size);
	getSwordsItem(swords1);
	std::vector<ItemType> swords2(size);
	getSwordsItem(swords2);
	std::vector<ItemType> swords3(size);
	getSwordsItem(swords3);

	//candies
	std::vector<ItemType> candies(7);
	for (int i = 0; i < 7; ++i)
	{
		candies[i] = ItemType(Item::Type((1 << 31) | (1 << (2 + i))), sf::IntRect(240 * ((i == 0) ? 1 : 0) + (i - ((i == 0) ? 0 : 1)) * 16, 64 + ((i == 0) ? 0 : 16), 16, 16), fWorldBounds, -3.f);
	}
	std::vector<ItemType> candies1(7);
	for (int i = 0; i < 7; ++i)
	{
		candies1[i] = ItemType(Item::Type((1 << 31) | (1 << (2 + i))), sf::IntRect(240 * ((i == 0) ? 1 : 0) + (i - ((i == 0) ? 0 : 1)) * 16, 64 + ((i == 0) ? 0 : 16), 16, 16), fWorldBounds, -3.f);
	}
	std::vector<ItemType> candies2(7);
	for (int i = 0; i < 7; ++i)
	{
		candies2[i] = ItemType(Item::Type((1 << 31) | (1 << (2 + i))), sf::IntRect(240 * ((i == 0) ? 1 : 0) + (i - ((i == 0) ? 0 : 1)) * 16, 64 + ((i == 0) ? 0 : 16), 16, 16), fWorldBounds, -3.f);
	}

	//books
	std::vector<ItemType> books(15);
	for (int i = 0; i < 15; ++i)
	{
		books[i] = ItemType(Item::Type((1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << (19 + i))),
			sf::IntRect( 240 * ((i == 0) ? 1 : 0) + (i - ((i == 0) ? 0 : 1)) * 16, 240 + ((i == 0) ? 0 : 16), 16, 16), fWorldBounds, -2.f);
	}
	std::vector<ItemType> books1(15);
	for (int i = 0; i < 15; ++i)
	{
		books1[i] = ItemType(Item::Type((1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << (19 + i))),
			sf::IntRect(240 * ((i == 0) ? 1 : 0) + (i - ((i == 0) ? 0 : 1)) * 16, 240 + ((i == 0) ? 0 : 16), 16, 16), fWorldBounds, -2.f);
	}
	std::vector<ItemType> books2(15);
	for (int i = 0; i < 15; ++i)
	{
		books2[i] = ItemType(Item::Type((1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << (19 + i))),
			sf::IntRect(240 * ((i == 0) ? 1 : 0) + (i - ((i == 0) ? 0 : 1)) * 16, 240 + ((i == 0) ? 0 : 16), 16, 16), fWorldBounds, -2.f);
	}

	bossItems.push_back(coins);
	bossItems.push_back(coins1);
	bossItems.push_back(coins2);
	bossItems.push_back(coins3);
	bossItems.push_back(coins4);
	bossItems.push_back(coins5);
	bossItems.push_back(helmets);
	bossItems.push_back(helmets1);
	bossItems.push_back(helmets2);
	bossItems.push_back(helmets3);
	bossItems.push_back(swords);
	bossItems.push_back(swords1);
	bossItems.push_back(swords2);
	bossItems.push_back(swords3);

	bossItems.push_back(candies);
	bossItems.push_back(candies1);
	bossItems.push_back(candies2);
	bossItems.push_back(books);
	bossItems.push_back(books1);
	bossItems.push_back(books2);
	return bossItems;
}

void BeginningWorld::boringStuff(sf::Time dt)
{
	fCommands.addCommand(Category::entities, derivedAction<Entity>([](Entity& ent, sf::Time)
		{
			ent.setSteppingOnSth(false);
		}));
	fCommands.addCommand(Category::NPC, derivedAction<NonPlayerChar>([](NonPlayerChar& npc, sf::Time)
		{
			npc.setBeginInteraction(false);
		}));
	fCommands.addCommand(Category::PlayerChar, derivedAction<Player>([](Player& p, sf::Time)
		{
			p.fEntitiesInRange.clear();
			p.fCanClimbLadder = false;
			p.doesGravitiyApply(true);
		}));

	while (!fCommands.isEmpty())
	{
		Command com = fCommands.pop();
		fSceneGraph.OnCommand(com, dt);
	}

	destroyProjectilesOutsideView(fCommands);
	fSceneGraph.removeDeadSceneNodes();

	handleCollisions(dt);
	fSceneGraph.updateGraph(dt, fCommands);

	handleMovement(dt);

	if (fPlayer && fPlayer->isPlayerDead())
	{
		fPlayerData->fIsDead = true;
	}

	//if (!isPlayerCentered())
	//{
	//	viewCanMove = false;
	//}
	//else
	{
		viewCanMove = true;
	}
}

BeginningWorld::Context::Context(sf::RenderWindow& win, TexturesHolder& tex, FontsHolder& fonts, Player_Data& player)
	:window(&win), textures(&tex), fonts(&fonts), player(&player)
{
}

BeginningWorld::BeginningWorld(Context context, Stack<BeginningWorld, Worlds::ID, Context>& stack)
	:fWindow(*context.window), fWorldView(fWindow.getDefaultView()), fTextures(*context.textures), fSceneGraph(), fCommands()
	, fWorldBounds(0.f, 0.f, fWorldView.getSize().x, fWorldView.getSize().y), fPlayer(nullptr), fFonts(*context.fonts)
	, fPlayerData(context.player), fFloorLevel(fWorldView.getSize().y)
	, fStack(&stack)
	, fTransitionWorldRect(fWorldView.getSize())
{
	//Utility::CenterOrigin(fWaveMessage);
	loadTextures();
	fTransitionWorldRect.setFillColor(sf::Color::Black);

	fWorldBounds = fWorldBounds;
	fWorldView.setCenter(fWorldBounds.left + fWorldView.getSize().x / 2.f, fWorldBounds.top + fWorldBounds.height / 2.f);

	fPlayerHP = context.player->fHP;
	buildWorld();
}

BeginningWorld::~BeginningWorld()
{
}

bool BeginningWorld::draw()
{
	fWindow.setView(fWorldView);
	fWindow.draw(fSceneGraph);
	if (fTransitionToWorld || fFinishTransitionToWorld)
	{
		fTransitionWorldRect.setPosition(fWorldView.getCenter().x - fWorldView.getSize().x / 2.f, fWorldView.getCenter().y - fWorldView.getSize().y / 2.f);
		fWindow.draw(fTransitionWorldRect);
	}
	return false;
}

bool BeginningWorld::update(sf::Time dt)
{
	boringStuff(dt);

	if (fTransitionToWorld)
	{
		fTransitionWorldAccu += dt;
		if (fTransitionWorldAccu >= fTransitionWorldDuration)
		{
			fTransitionWorldAccu = sf::Time::Zero;
			fTransitionToWorld = false;

			requestWorldPop();
			requestWorldPush(fWorldToTransitionTo);
			fPlayer->enableInputs(true);
			fPlayer->disableDamage(false);
		}
		else
		{
			fTransitionWorldRect.setFillColor(sf::Color::Color(0, 0, 0, 255 * fTransitionWorldAccu.asSeconds()));
			fPlayer->enableInputs(false);
			fPlayer->disableDamage(true);
		}
	}
	else if (fFinishTransitionToWorld)
	{
		fTransitionWorldAccu += dt;
		if (fTransitionWorldAccu >= fTransitionWorldDuration)
		{
			fTransitionWorldAccu = sf::Time::Zero;
			fFinishTransitionToWorld = false;
			fTransitionWorldRect.setFillColor(sf::Color::Color(0, 0, 0, 0));
			fPlayer->enableInputs(true);
			fPlayer->disableDamage(false);
		}
		else
		{
			fTransitionWorldRect.setFillColor(sf::Color::Color(0, 0, 0, 255 - 255 * fTransitionWorldAccu.asSeconds()));
			fPlayer->enableInputs(false);
			fPlayer->disableDamage(true);
		}
	}


	return false;
}

bool BeginningWorld::hasAlivePlayer() const
{
	return !fPlayer->isPlayerDead();
}

bool BeginningWorld::handleEvent(const sf::Event & e)
{
	fPlayer->ProcessEvents(e, fCommands);
	return false;
}


void BeginningWorld::loadTextures()
{
	
}

void BeginningWorld::buildWorld()
{
	for (size_t i = 0; i < LayerCount; ++i)
	{
		Layers[i] = new SceneNode((i == Interaction) ? Category::Scene : Category::None);
		fSceneGraph.attachChild(Layers[i]);
	}

	fPlayer = new Player(State::Context(fWindow, fTextures, fFonts, *fPlayerData), PlayerData(sf::Vector2f(0.f, 0.f)));
	fPlayer->setPosition(sf::Vector2f(fWorldView.getCenter().x, fFloorLevel - fPlayer->getBoundingRect().height / 2.f));
	Layers[PlayerChar]->attachChild(fPlayer);

	//Cabury* cabry = new Cabury(50, fTextures, *fPlayer);
	//cabry->setPosition(fPlayer->getWorldPosition().x - 100.f, fFloorLevel - 2 * cabry->getBoundingRect().height);
/*
	Layers[PlayerChar]->attachChild(cabry);*/

	//spawnNPCs();
}

void BeginningWorld::transitionToWorld(Worlds::ID world)
{
	fTransitionToWorld = true;
	fWorldToTransitionTo = world;
}



void BeginningWorld::handleCollisions(sf::Time dt)
{
	//pair of sceneNodes colliding
	std::set<SceneNode::Pair> fCollisionPairs;
	fSceneGraph.checkSceneCollision(fSceneGraph, fCollisionPairs);
	//for each pair 
	//do something
	for (SceneNode::Pair colPair : fCollisionPairs)
	{
		if (colPair.first->getCategory() != colPair.second->getCategory())
		{
			if (isMatching(colPair, Category::entities, Category::platform))
			{
				Entity& entity = static_cast<Entity&>(*colPair.first);
				Platform& plat = static_cast<Platform&>(*colPair.second);
				assert(&entity && &plat);


				/**keep this code for a potential obstacle with this effect !!**
				float x = player.getWorldPosition().x - plat.getWorldPosition().x;
				float y = player.getWorldPosition().y - plat.getWorldPosition().y;
				float move_x = x / Utility::length(sf::Vector2f(x, y));
				float move_y = y / Utility::length(sf::Vector2f(x, y));*/
				if (entity.getWorldPosition().x < plat.getWorldPosition().x)	// the entity is at the left border of the platform
				{
					while (entity.getWorldPosition().x + entity.getBoundingRect().width / 2.f >= plat.getWorldPosition().x)
					{
						entity.move(-10.f * dt.asSeconds(), 0.f);
					}
				}
				else if (entity.getWorldPosition().x > plat.getWorldPosition().x + plat.getBoundingRect().width)	//the entity is at the right border of the platform
				{
					while (entity.getWorldPosition().x - entity.getBoundingRect().width / 2.f <= plat.getWorldPosition().x + plat.getBoundingRect().width)
					{
						entity.move(10.f * dt.asSeconds(), 0.f);
					}
				}
				else if (entity.getWorldPosition().y + entity.getSpriteBoundingRect().height / 2.f >= plat.getWorldPosition().y - 4.f &&
					entity.getWorldPosition().y + entity.getBoundingRect().height / 2.f <= plat.getWorldPosition().y + plat.getBoundingRect().height - 4.f)	//the entity is on the platform
				{
					entity.setSteppingOnSth(true);
					
					entity.setPosition(entity.getWorldPosition().x, plat.getWorldPosition().y - entity.getSpriteBoundingRect().height / 2.f + 1.f);
					
				}
				else //the entity is below the platform
				{
					entity.setUpwardsVel(0.f);
				}
			}
			if (isMatching(colPair, Category::PlayerChar, Category::damagable))
			{
				Player& player = static_cast<Player&>(*colPair.first);
				Damageable& damagable = static_cast<Boss&>(*colPair.second);
				assert(&player && &damagable);

				player.atRange(true, damagable);
			}
			else if (isMatching(colPair, Category::PlayerChar, Category::NPC))
			{
				Player& player = static_cast<Player&>(*colPair.first);
				NonPlayerChar& NPC = static_cast<NonPlayerChar&>(*colPair.second);
				assert(&player && &NPC);
				NPC.showHelpText();
				player.atRange(true, NPC);
			}
			if (isMatching(colPair, Category::projectile, Category::PlayerChar))
			{
				Projectile& proj = static_cast<Projectile&>(*colPair.first);
				Player& player = static_cast<Player&>(*colPair.second);
				assert(&proj && &player);

				player.applyDamage(proj.getDamage());
				proj.Destroy();

			}
			if (isMatching(colPair, Category::PlayerChar, Category::ladder))
			{
				Player& player = static_cast<Player&>(*colPair.first);
				assert(&player);

				player.fCanClimbLadder = true;
				player.doesGravitiyApply(false);
			}
			if (isMatching(colPair, Category::PlayerChar, Category::hitBoxes))
			{
				Player& player = static_cast<Player&>(*colPair.first);
				HitBox& hitbox = static_cast<HitBox&>(*colPair.second);
				assert(&hitbox && &player);

				hitbox.fPlayerEntering();
				
			}
			if (isMatching(colPair, Category::PlayerChar, Category::item))
			{
				Player& player = static_cast<Player&>(*colPair.first);
				Item& item = static_cast<Item&>(*colPair.second);
				assert(&player && &item);
				
				if (!item.fEffectOnPlayer(player)) {
					player.addInventoryItem(&item);
					
				}
				 
			}
			
		}
	}
}

bool BeginningWorld::isMatching(SceneNode::Pair& pair, unsigned int type1, unsigned int type2)
{
	unsigned int category1 = pair.first->getCategory();
	unsigned int category2 = pair.second->getCategory();

	if (category1 & type1 && category2 & type2)
	{
		return true;
	}
	if (category2 & type1 && category1 & type2)
	{
		std::swap(pair.first, pair.second);
		return true;
	}

	return false;
}

void BeginningWorld::handleMovement(sf::Time dt)
{
	if (viewCanMove) {
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
		if(out == Utility::Left)
			fPlayer->move(10.f * dt.asSeconds(), 0.f);
		if(out == Utility::Right)
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

void BeginningWorld::requestWorldPush(Worlds::ID world)
{
	fStack->pushState(world);
}

void BeginningWorld::requestWorldPop()
{
	fStack->popState();
}

void BeginningWorld::requestWorldClear()
{
	fStack->clearStates();
}

void BeginningWorld::destroyProjectilesOutsideView(CommandQueue& commands)
{
	Command com;
	com.category = Category::projectile;
	com.action = derivedAction<Projectile>([this](Projectile& p, sf::Time)
		{
			if(p.getWorldPosition().x > fWorldView.getCenter().x + fWorldView.getSize().x / 2.f || p.getWorldPosition().x + p.getBoundingRect().width < fWorldView.getCenter().x - fWorldView.getSize().x / 2.f)
				p.Destroy();
		});
	commands.push(com);
}

bool BeginningWorld::isPlayerCentered()
{
	if (fPlayer->getWorldPosition().x >= fWorldView.getCenter().x - 25.f && fPlayer->getWorldPosition().x <= fWorldView.getCenter().x + 25.f)
		return true;
	return false;
}

Utility::OutSide BeginningWorld::isOutOfTheWorld(const sf::FloatRect & pos) const
{
	return Utility::isOutOfTheRect(pos, fWorldBounds);
}

Utility::OutSide BeginningWorld::isOutOfTheView(const sf::FloatRect & pos) const
{
	return Utility::isOutOfTheRect(pos, sf::FloatRect(sf::Vector2f(fWorldView.getCenter().x - fWorldView.getSize().x / 2.f, fWorldView.getCenter().y - fWorldView.getCenter().y / 2.f), fWorldView.getSize()));
}
