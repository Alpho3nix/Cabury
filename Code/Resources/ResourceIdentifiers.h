#pragma once
#include "ResourceHolder.hpp"
#define WIDTH 800
#define HEIGHT 450

namespace Fonts
{
	enum class ID
	{
		Sansation, GUI, NPCSpeach, NPCHelp
	};
}

namespace Textures
{
	enum class ID
	{
		Forest, PlayerAnim, PlayerInventory, Buttons, Slime, Goblin, Oculothorax, Ogre, EarthWisp, EarthWispProjectile, WindWisp,
		WindWispProjectile, WaterWisp, WaterWispProjectile, FireWisp, FireWispProjectile,
		Minotaur, NPC1, NPC2, Items, Block, RepeatedBlock_Long, RepeatedBlock_Small, Crate, Ladder_Normal, Ladder_Mossy,
		ShopButtons, Cabury, TavernOutside, TavernInsideBack, TavernInsideFront, TavernCellarClose, TavernCellarOpen, Kanalisation,
		BossBG_Test, Cage, MagicBook, MagicBookItem
	};
}

typedef ResourceHolder<sf::Font, Fonts::ID> FontsHolder;
typedef ResourceHolder<sf::Texture, Textures::ID> TexturesHolder;