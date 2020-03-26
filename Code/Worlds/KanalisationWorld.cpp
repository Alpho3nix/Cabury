#include "KanalisationWorld.h"
#include "Platform.h"
#include "Projectile.h"
#include "BreakableBox.h"
#include "UtilityFunc.h"
#include "Ladder.h"
#include "ShoppingNPC.h"
#include "Cabury.h"
#include "Player.h"
#include "FollowingEnemy.h"
#include "GuardEnemy.h"
#include "Answer.h"
#include <iostream>
#include "TalkingNPC.h"
#include "Item.h"
#include "HitBox.h"
#include "Minotaur.h"
#include <sstream>
#include "Cage.h"
#include <fstream>

KanalisationWorld::KanalisationWorld(BeginningWorld::Context context, Stack<BeginningWorld, Worlds::ID, BeginningWorld::Context>& stack)
	:BeginningWorld(context, stack), guardBuffer(nullptr), boxBuffer(nullptr), cabu(nullptr),
	fWaveMessage("Vague 1", context.fonts->get(Fonts::ID::GUI), 40)
{
	std::ifstream input;
	input.open("Kanalisation.txt");
	if (!input.is_open())
	{
		throw std::runtime_error("can't open file Kanalisation.txt");
	}
	input >> fSpawnBox >> fSpawnGuard >> fSpawnCabury;
	input.close();
	
	buildWorld();
}

KanalisationWorld::~KanalisationWorld()
{
	std::ofstream output;
	output.open("Kanalisation.txt");
	if (output.is_open())
	{
		output.clear();
		if (boxBuffer)	//if the crate wasn't destroyed
		{
			output << 1 << " ";
		}
		else
		{
			output << 0 << " ";
		}

		if (guardBuffer)	//if the guard isn't dead
		{
			output << 1 << " ";
		}
		else
		{
			output << 0 << " ";
		}
		
		if (cabu)	//if cabury still is in the world
		{
			output << 1;
		}
		else
		{
			output << 0;
		}
	}

	output.close();
}

bool KanalisationWorld::update(sf::Time dt)
{
	fMobsDead = true;
	fCommands.addCommand(Category::Enemies, [this](SceneNode& node, sf::Time)
		{
			fMobsDead = false;
		});
	BeginningWorld::update(dt);
	//waveSystem(dt);
	if (guardBuffer && guardBuffer->isDestroyed())
	{
		guardBuffer = nullptr;
	}
	if (boxBuffer && boxBuffer->isDestroyed())
	{
		boxBuffer = nullptr;
	}
	return false;
}

bool KanalisationWorld::draw()
{
	BeginningWorld::draw();
	//if (fShowWaveMessage) {
	//	fWindow.draw(fWaveMessage);
	//}
	return false;
}

void KanalisationWorld::buildWorld()
{
	fWorldBounds = sf::FloatRect(0.f, 0.f, 4.f * fWorldView.getSize().x, fWorldView.getSize().y);

	BackGroundNode* SceneBackground = new BackGroundNode(fTextures.get(Textures::ID::Kanalisation));
	SceneBackground->setPosition(0.f, 0.f);
	Layers[Background]->attachChild(SceneBackground);

	HitBox* tavernWorldEntry = new HitBox(sf::FloatRect(0.f, 0.f, 181, 127));
	tavernWorldEntry->setPosition(63, 214);
	tavernWorldEntry->fPlayerEntering = [this]()
	{
		if(fPlayer->requestEnteringWorld())
			transitionToWorld(Worlds::ID::InsideTavern);
	};

	SceneBackground->attachChild(tavernWorldEntry);

	spawnPlatforms();
	float mobSpawnOffset = 300.f;

	for (int i = 0; i < Utility::randomInt(4, 6); ++i)
	{
		Enemy::Type type = static_cast<Enemy::Type>(Utility::randomInt(0, 6));
		switch (type)
		{
		case Enemy::Ogre:
			spawnEnemies(1, type, 50, fTextures, fFonts, *fPlayer, getMobItems(), 500.f, 70.f, fFloorLevel, 30.f, 20, fWorldBounds.left + fWorldBounds.width - mobSpawnOffset + Utility::randomFloat(-70.f, 70.f));
			break;
		case Enemy::Slime:
			spawnEnemies(1, type, 50, fTextures, fFonts, *fPlayer, getMobItems(), 600.f, 60.f, fFloorLevel, 20.f, 15, fWorldBounds.left + fWorldBounds.width - mobSpawnOffset + Utility::randomFloat(-70.f, 70.f));
			break;
		case Enemy::Oculothorax:
			spawnEnemies(1, type, 50, fTextures, fFonts, *fPlayer, getMobItems(), 600.f, 100.f, fFloorLevel, 20.f, 15, fWorldBounds.left + fWorldBounds.width - mobSpawnOffset + Utility::randomFloat(-70.f, 70.f));
			break;
		case Enemy::FireWisp:
			spawnEnemies(1, type, 50, fTextures, fFonts, *fPlayer, getMobItems(), 700.f, 110.f, fFloorLevel, 300.f, 10, fWorldBounds.left + fWorldBounds.width - mobSpawnOffset + Utility::randomFloat(-70.f, 70.f), 215.f);
			break;
		case Enemy::WaterWisp:
			spawnEnemies(1, type, 50, fTextures, fFonts, *fPlayer, getMobItems(), 700.f, 120.f, fFloorLevel, 250.f, 10, fWorldBounds.left + fWorldBounds.width - mobSpawnOffset + Utility::randomFloat(-70.f, 70.f), 210.f);
			break;
		case Enemy::WindWisp:
			spawnEnemies(1, type, 50, fTextures, fFonts, *fPlayer, getMobItems(), 700.f, 115.f, fFloorLevel, 350.f, 10, fWorldBounds.left + fWorldBounds.width - mobSpawnOffset + Utility::randomFloat(-70.f, 70.f), 220.f);
			break;
		case Enemy::EarthWisp:
			spawnEnemies(1, type, 50, fTextures, fFonts, *fPlayer, getMobItems(), 700.f, 110.f, fFloorLevel, 300.f, 10, fWorldBounds.left + fWorldBounds.width - mobSpawnOffset + Utility::randomFloat(-70.f, 70.f), 200.f);
			break;
		default:
			break;
		}
	}

	if (fSpawnCabury)
	{
		cabu = new Cabury(50, fTextures, *fPlayer, Cabury::CinematicState::JailInKanalisation);
		cabu->setPosition(fWorldBounds.left + fWorldBounds.width - cabu->getBoundingRect().width - 200.f, fFloorLevel - cabu->getBoundingRect().height / 2.f);
		Layers[PlayerChar]->attachChild(cabu);

		Cage* cabu_cage = new Cage(fTextures);
		cabu_cage->setFloorPosition(cabu->getWorldPosition().x - cabu_cage->getBoundingRect().width / 2.f, fFloorLevel);
		Layers[PlayerChar]->attachChild(cabu_cage);

		HitBox* cage_hitbox = new HitBox(sf::FloatRect(cabu_cage->getBoundingRect()));
		cage_hitbox->fPlayerEntering = [this, cabu_cage]()
		{
			if (fMobsDead)
			{
				cabu_cage->destroy();

				cabu->cageBroken();
			}
		};

		Layers[PlayerChar]->attachChild(cage_hitbox);
	}

	Platform* little_plat2 = new Platform(fTextures, 6, 1);
	little_plat2->setPosition(fWorldBounds.left + fWorldBounds.width / 2.f - 500.f, fFloorLevel - 120.f);
	Layers[PlayerChar]->attachChild(little_plat2);

	if (fSpawnBox) {
		BreakableBox* box = new BreakableBox(BreakableBox::Crate, fTextures);
		box->setPosition(little_plat2->getWorldPosition().x, little_plat2->getWorldPosition().y - box->getSpriteBoundingRect().height);
		box->setOnDestroyedAction([this, box]()
			{
				ItemType itemType(Item::Type::BlueCoin_stack1, sf::IntRect(240, 80, 16, 16), [](Player& player) {player.addMoney(500); return true; }, fWorldBounds, 0.f);
				fCommands.addCommand(Category::Scene, [this, itemType, box](SceneNode& node, sf::Time)
					{
						Item* item = new Item(1, fTextures.get(Textures::ID::Items), itemType);
						item->setPosition(box->getWorldPositionCentered().x, box->getWorldPosition().y + box->getSpriteBoundingRect().height);
						node.attachChild(item);
					});
			});
		Layers[Interaction]->attachChild(box);

		boxBuffer = box;
	}

	Ladder* lad = new Ladder(Ladder::MossyLadder, fTextures, 2);
	lad->setFloorPosition(little_plat2->getWorldPosition().x + little_plat2->getBoundingRect().width, fFloorLevel);
	Layers[Interaction]->attachChild(lad);


	Platform* little_plat2_bis = new Platform(fTextures, 6, 1);
	little_plat2_bis->setPosition(little_plat2->getWorldPosition().x, lad->getWorldPosition().y + 2.f);
	Layers[PlayerChar]->attachChild(little_plat2_bis);

	if (fSpawnGuard) {
		GuardEnemy* guard = new GuardEnemy(50, Enemy::Oculothorax, fTextures, fFonts, *fPlayer, getMobItems(), Entity::LookingRight, 30.f, 10, 150.f, 600.f, 70.f);
		guard->setDefaultPosition(little_plat2->getWorldPosition().x + 100.f, little_plat2->getWorldPosition().y - guard->getBoundingRect().height / 2.f);
		guard->setPosition(guard->getDefaultPosition());
		Layers[PlayerChar]->attachChild(guard);

		guardBuffer = guard;
	}
}

void KanalisationWorld::handleMovement(sf::Time dt)
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
		if (out == Utility::Left)
			fPlayer->move(10.f * dt.asSeconds(), 0.f);
		if (out == Utility::Right)
		{
			if (fPlayer->canChangeToBoss()) {
				transitionToWorld(Worlds::ID::MinotaurWorld);
				break;
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

void KanalisationWorld::spawnPlatforms()
{
	Platform* plat = new Platform(fTextures, fWorldBounds.width / fTextures.get(Textures::ID::RepeatedBlock_Small).getSize().x, 2);
	plat->setPosition(fWorldBounds.left, fWorldView.getCenter().y + 150.f);
	Layers[PlayerChar]->attachChild(plat);

	fFloorLevel = plat->getWorldPosition().y;

	Platform* little_plat = new Platform(fTextures, 6, 1);
	little_plat->setPosition(fWorldBounds.left + 50.f, fFloorLevel - 170.f);
	Layers[PlayerChar]->attachChild(little_plat);

	Platform* little_plat1 = new Platform(fTextures, 6, 1);
	little_plat1->setPosition(fWorldBounds.left + 150.f, little_plat->getWorldPosition().y - 150.f);
	Layers[PlayerChar]->attachChild(little_plat1);

	Platform* little_plat3 = new Platform(fTextures, 6, 1);
	little_plat3->setPosition(fWorldBounds.left + fWorldBounds.width - little_plat3->getBoundingRect().width, fFloorLevel - 150.f);
	Layers[PlayerChar]->attachChild(little_plat3);

	Platform* little_plat4 = new Platform(fTextures, 6, 1);
	little_plat4->setPosition(little_plat3->getWorldPosition().x - 350.f, little_plat3->getWorldPosition().y - 150.f);
	Layers[PlayerChar]->attachChild(little_plat4);
}

void KanalisationWorld::spawnBoss()
{
	/**Minotaur**/
	//frame dimensions: 192 x 154, texture dimensions: 1152 x 2464
	sf::Vector2i MinotaurFrameDim(192, 154);

	std::vector<std::vector<ItemType>> BossItems = getBossItems();
	Minotaur* minotaur = new Minotaur(300 + 50 * (fDifficulty - 1), fTextures, fFonts, *fPlayer, BossItems);
	minotaur->setPosition(fWorldView.getCenter().x /*- fWorldView.getSize().x / 2.f + MinotaurFrameDim.x / 2.f*/, 0.f - MinotaurFrameDim.y / 2.f);

	Layers[PlayerChar]->attachChild(minotaur);

}

void KanalisationWorld::prepareEnemies(unsigned int waveNum)
{
	//TODO: put this in a .lua file or .txt file and then read from it

	/**Slimes**/
	//frame dimension: 64 x 50, texture dimension: 512 x 300
	sf::Vector2i slimeFrameDim(64, 50);
	int slimeHP = 30.f * fDifficulty;
	float slimeAgro = 600.f;
	float slimeSpeed = 30.f;
	float slime_def_pos_y = fPlayer->getWorldPosition().y + fPlayer->getSpriteBoundingRect().height / 2.f - slimeFrameDim.y / 2.f;
	float slimeDamageDist = 30.f;

	/**Ogres**/
	//928 x 840 -> 116 x 84
	sf::Vector2i ogreFrameDim(116, 84);

	int ogreHP = 50.f * fDifficulty;
	float ogreAgro = 600.f;
	float ogreSpeed = 60.f;
	float ogre_def_pos_y = fPlayer->getWorldPosition().y + fPlayer->getSpriteBoundingRect().height / 2.f - ogreFrameDim.y / 2.f;
	float ogreDamageDist = 40.f;

	/**Oculothorax**/
	//480 x 800 -> 80 x 80
	sf::Vector2i oculFrameDim(80, 80);
	float ocul_def_pos_y = fPlayer->getWorldPosition().y + fPlayer->getSpriteBoundingRect().height / 2.f - oculFrameDim.y / 2.f;

	int oculHP = 40.f * fDifficulty;
	float oculAgro = 600.f;
	float oculSpeed = 60.f;
	float oculDamageDist = 40.f;

	/**Earth Wisp**/
	//size depends on which kind of wisp
	//fire wisp : 620 x 1032 --> 124 x 86
	//water wisp : 492 x 620 --> 82 x 64
	//wind wisp : 540 x 900 --> 90 x 90
	//earth wisp : 564 x 860 --> 94 x 86
	int wispReadDirRight = -1, wispReadDirLeft = 1;
	int wispHP = 20.f * fDifficulty;

	//fireWisp
	sf::Vector2i fireWispFrameDim(124, 116);
	float fireWispAgro = 700.f;
	float fireWispSpeed = 80.f;
	float fireWispDamageDist = 250.f;
	float fireWispProjectileSpeed = 175.f;
	float fireWispDefPosY = fPlayer->getWorldPosition().y + fPlayer->getSpriteBoundingRect().height / 2.f - fireWispFrameDim.y / 2.f;

	//waterWisp
	sf::Vector2i waterWispFrameDim(82, 94);
	float waterWispAgro = 700.f;
	float waterWispSpeed = 70.f;
	float waterWispDamageDist = 200.f;
	float waterWispProjectileSpeed = 175.f;
	float waterWispDefPosY = fPlayer->getWorldPosition().y + fPlayer->getSpriteBoundingRect().height / 2.f - waterWispFrameDim.y / 2.f;

	//windWisp
	sf::Vector2i windWispFrameDim(90, 120);
	float windWispAgro = 700.f;
	float windWispSpeed = 90.f;
	float windWispDamageDist = 300.f;
	float windWispProjectileSpeed = 250.f;
	float windWispDefPosY = fPlayer->getWorldPosition().y + fPlayer->getSpriteBoundingRect().height / 2.f - windWispFrameDim.y / 2.f;

	//earthWisp
	sf::Vector2i earthWispFrameDim(94, 116);;
	float earthWispAgro = 700.f;
	float earthWispSpeed = 60.f;
	float earthWispDamageDist = 225.f;
	float earthWispProjectileSpeed = 200.f;
	float earthWispDefPosY = fPlayer->getWorldPosition().y + fPlayer->getSpriteBoundingRect().height / 2.f - earthWispFrameDim.y / 2.f;


	std::vector<std::vector<ItemType>> mobItems = getMobItems();

	switch (waveNum)
	{
	case 1:
		spawnEnemies(2, Enemy::Type::Slime, slimeHP, fTextures, fFonts, *fPlayer, mobItems, slimeAgro, slimeSpeed, slime_def_pos_y, slimeDamageDist);
		spawnEnemies(2, Enemy::Type::Oculothorax, oculHP, fTextures, fFonts, *fPlayer, mobItems, oculAgro, oculSpeed, ocul_def_pos_y, oculDamageDist);
		break;
	case 2:
		spawnEnemies(2, Enemy::Type::Slime, slimeHP, fTextures, fFonts, *fPlayer, mobItems, slimeAgro, slimeSpeed, slime_def_pos_y, slimeDamageDist);
		spawnEnemies(2, Enemy::Type::Oculothorax, oculHP, fTextures, fFonts, *fPlayer, mobItems, oculAgro, oculSpeed, ocul_def_pos_y, oculDamageDist);
		spawnEnemies(2, Enemy::Type::Ogre, ogreHP, fTextures, fFonts, *fPlayer, mobItems, ogreAgro, ogreSpeed, ogre_def_pos_y, ogreDamageDist);
		break;
	case 3:
		spawnEnemies(2, Enemy::Type::Slime, slimeHP, fTextures, fFonts, *fPlayer, mobItems, slimeAgro, slimeSpeed, slime_def_pos_y, slimeDamageDist);
		spawnEnemies(2, Enemy::Type::Oculothorax, oculHP, fTextures, fFonts, *fPlayer, mobItems, oculAgro, oculSpeed, ocul_def_pos_y, oculDamageDist);
		spawnEnemies(2, Enemy::Type::Ogre, ogreHP, fTextures, fFonts, *fPlayer, mobItems, ogreAgro, ogreSpeed, ogre_def_pos_y, ogreDamageDist);
		spawnEnemies(1, Enemy::Type::EarthWisp, wispHP, fTextures, fFonts, *fPlayer, mobItems, earthWispAgro, earthWispSpeed, earthWispDefPosY, earthWispDamageDist, 10, earthWispProjectileSpeed);
		spawnEnemies(1, Enemy::Type::FireWisp, wispHP, fTextures, fFonts, *fPlayer, mobItems, fireWispAgro, fireWispSpeed, fireWispDefPosY, fireWispDamageDist, 10, fireWispProjectileSpeed);
		spawnEnemies(1, Enemy::Type::WindWisp, wispHP, fTextures, fFonts, *fPlayer, mobItems, windWispAgro, windWispSpeed, windWispDefPosY, windWispDamageDist, 10, windWispProjectileSpeed);
		spawnEnemies(1, Enemy::Type::WaterWisp, wispHP, fTextures, fFonts, *fPlayer, mobItems, waterWispAgro, waterWispSpeed, waterWispDefPosY, waterWispDamageDist, 10, waterWispProjectileSpeed);
		break;
	default:
		break;
	}
}

void KanalisationWorld::spawnEnemies(unsigned int amount, Enemy::Type type, int hp, TexturesHolder& textures, const FontsHolder& fonts,
	Player& player, std::vector<std::vector<ItemType>> itemTypes, float minAgroDist, float speed, float def_pos_y, float damageDistance, int damage, float def_pos_x, float projVelocity)
{
	switch (type)
	{
	case Enemy::Type::Slime:
	{
		for (unsigned int i = 0; i < amount; ++i)
		{
			FollowingEnemy* slime = new FollowingEnemy(hp, type, textures, fonts, player, itemTypes, damageDistance, damage, minAgroDist, speed);
			slime->setPosition(def_pos_x + 10.f * i, def_pos_y);
			Layers[PlayerChar]->attachChild(slime);
		}
	}
	break;
	case Enemy::Type::Ogre:
	{
		for (unsigned int i = 0; i < amount; ++i)
		{
			FollowingEnemy* ogre = new FollowingEnemy(hp, type, textures, fonts, player, itemTypes, damageDistance, damage, minAgroDist, speed);
			ogre->setPosition(def_pos_x + 15.f * i, def_pos_y);
			Layers[PlayerChar]->attachChild(ogre);
		}
	}
	break;
	case Enemy::Type::Oculothorax:
	{
		for (unsigned int i = 0; i < amount; ++i)
		{
			FollowingEnemy* oculothorax = new FollowingEnemy(hp, type, textures, fonts, player, itemTypes, damageDistance, damage, minAgroDist, speed);
			oculothorax->setPosition(def_pos_x - 7.f * i, def_pos_y);
			Layers[PlayerChar]->attachChild(oculothorax);
		}
	}
	break;
	case Enemy::Type::EarthWisp:
	{
		for (unsigned int i = 0; i < amount; ++i)
		{
			FollowingEnemy* earthWisp = new FollowingEnemy(hp, type, textures, fonts, player, itemTypes, damageDistance, damage, minAgroDist, speed);
			earthWisp->setPosition(def_pos_x, def_pos_y);
			earthWisp->setAttackOnPlayer([this, earthWisp, projVelocity](Player& player, int damage, CommandQueue& commands)
				{
					commands.addCommand(Category::Scene, [this, &player, damage, earthWisp, projVelocity](SceneNode& node, sf::Time)
						{
							Projectile* proj = new Projectile(fTextures.get(Textures::ID::EarthWispProjectile), sf::IntRect(0, 0, 64, 32), (earthWisp->isLookingLeft()) ? -1 : 1, projVelocity, damage);
							proj->setPosition(earthWisp->getWorldPosition().x, earthWisp->getWorldPosition().y - earthWisp->getBoundingRect().height / 6.f);
							node.attachChild(proj);
						});
				});
			Layers[PlayerChar]->attachChild(earthWisp);
		}
	}
	break;
	case Enemy::Type::FireWisp:
	{
		for (unsigned int i = 0; i < amount; ++i)
		{
			FollowingEnemy* fireWisp = new FollowingEnemy(hp, type, textures, fonts, player, itemTypes, damageDistance, damage, minAgroDist, speed);
			fireWisp->setPosition(def_pos_x, def_pos_y);
			fireWisp->setAttackOnPlayer([this, fireWisp, projVelocity](Player& player, int damage, CommandQueue& commands)
				{
					commands.addCommand(Category::Scene, [this, &player, damage, fireWisp, projVelocity](SceneNode& node, sf::Time)
						{
							Projectile* proj = new Projectile(fTextures.get(Textures::ID::FireWispProjectile), sf::IntRect(0, 0, 64, 32), (fireWisp->isLookingLeft()) ? -1 : 1, projVelocity, damage);
							proj->setPosition(fireWisp->getWorldPosition().x, fireWisp->getWorldPosition().y - fireWisp->getBoundingRect().height / 6.f);
							node.attachChild(proj);
						});
				});
			Layers[PlayerChar]->attachChild(fireWisp);
		}
	}
	break;
	case Enemy::Type::WindWisp:
	{
		for (unsigned int i = 0; i < amount; ++i)
		{
			FollowingEnemy* windWisp = new FollowingEnemy(hp, type, textures, fonts, player, itemTypes, damageDistance, damage, minAgroDist, speed);
			windWisp->setPosition(def_pos_x, def_pos_y);
			windWisp->setAttackOnPlayer([this, windWisp, projVelocity](Player& player, int damage, CommandQueue& commands)
				{
					commands.addCommand(Category::Scene, [this, &player, damage, windWisp, projVelocity](SceneNode& node, sf::Time)
						{
							Projectile* proj = new Projectile(fTextures.get(Textures::ID::WindWispProjectile), sf::IntRect(0, 0, 64, 32), (windWisp->isLookingLeft()) ? -1 : 1, projVelocity, damage);
							proj->setPosition(windWisp->getWorldPosition().x, windWisp->getWorldPosition().y - windWisp->getBoundingRect().height / 5.f);
							node.attachChild(proj);
						});
				});
			Layers[PlayerChar]->attachChild(windWisp);
		}
	}
	break;
	case Enemy::Type::WaterWisp:
	{
		for (unsigned int i = 0; i < amount; ++i)
		{
			FollowingEnemy* waterWisp = new FollowingEnemy(hp, type, textures, fonts, player, itemTypes, damageDistance, damage, minAgroDist, speed);
			waterWisp->setPosition(def_pos_x, def_pos_y);
			waterWisp->setAttackOnPlayer([this, waterWisp, projVelocity](Player& player, int damage, CommandQueue& commands)
				{
					commands.addCommand(Category::Scene, [this, &player, damage, waterWisp, projVelocity](SceneNode& node, sf::Time)
						{
							Projectile* proj = new Projectile(fTextures.get(Textures::ID::WaterWispProjectile), sf::IntRect(0, 0, 64, 32), (waterWisp->isLookingLeft()) ? -1 : 1, projVelocity, damage);
							proj->setPosition(waterWisp->getWorldPosition().x, waterWisp->getWorldPosition().y - waterWisp->getBoundingRect().height / 3.f);
							node.attachChild(proj);
						});
				});
			Layers[PlayerChar]->attachChild(waterWisp);
		}
	}
	break;
	default:
		break;
	}
}

void KanalisationWorld::waveSystem(sf::Time dt)
{
	if (fBeginMonsterWaves)
	{
		//timer for the showing delay of the message "vague 1"
		fMessageAccu += dt;
		//when finished, spawn monsters according to the wave number
		if (fMessageAccu >= fMessageDelay) {

			fMessageAccu = sf::Time::Zero;
			fInWave = true;
			fWaveFinished = false;
			if (fWaveNum <= 3) {
				prepareEnemies(fWaveNum);
				//increase the wave number
				++fWaveNum;
			}
			else //Boss wave
			{
				spawnBoss();
				fWaveNum = -1;
			}
			//set fBeginMonsterWaves to false
			fBeginMonsterWaves = false;
			fShowWaveMessage = false;
		}
		else
		{
			if (fWaveNum <= 3) {
				fWaveMessage.setString("Vague " + std::to_string(fWaveNum));
			}
			else
			{
				fWaveMessage.setString("BOSS");
			}
			Utility::CenterOrigin(fWaveMessage);
			fWaveMessage.setPosition(fWorldView.getCenter().x, fWorldView.getCenter().y - 50.f);
			fShowWaveMessage = true;
		}
	}
	if (fInWave)
	{
		fCommands.addCommand(Category::Enemies | Category::Bosses, [this](SceneNode& node, sf::Time)
			{
				fWaveFinished = false;
			});
	}
	//then when the last monster of the wave dies, set fBeginMonsterWaves to true again
	if (fWaveFinished && fInWave)
	{
		fInWave = false;
		if (fWaveNum != -1) {
			fBeginMonsterWaves = true;
		}
		else
		{
			//request push game win state
			fPlayerData->fGameWin = true;
			fWaveMessage.setString("YOU WON");
			Utility::CenterOrigin(fWaveMessage);
			fWaveMessage.setPosition(fWorldView.getCenter().x, fWorldView.getCenter().y - 50.f);
			fShowWaveMessage = true;
		}

	}

	fWaveFinished = true;
}
