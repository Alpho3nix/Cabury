#include "PlayerData.h"
#include "Player.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <array>
#include <sstream>


void Player_Data::assignKey(unsigned int action, sf::Keyboard::Key key)
{
	//clearing already assigned keys that have the same actions
	for (auto itr = fKeyBinding.begin(); itr != fKeyBinding.end();)
	{
		if (itr->first == action)
		{
			fKeyBinding.erase(itr++);
		}
		else
		{
			++itr;
		}
	}

	//insert the new binding
	fKeyBinding[action] = key;
}

void Player_Data::destroyItems()
{
	for (GUI::InventoryBox& box : fInventoryBoxes)
	{
		while (box.containsItem())
		{
			box.addQuantity(-1);
		}
	}
}

Player_Data::~Player_Data()
{
	destroyItems();
}

Player_Data::Player_Data()
	:fKeyBinding(), fGameWin(false), fBackToMenu(false), fIsDead(false), fChangeWorld(false),
	fHP(0), fResistance(10), fStrength(10), fMoney(0), fInventoryBoxes()
{
	
	std::ifstream file;
	file.open("Player/PlayerInputs.txt");

	std::array<int, PlayerAction::ActionCount> keys;
	int i = 0;
	while (file.is_open() && i < PlayerAction::ActionCount)
	{
		if (file.eof())
			break;
		file >> keys[i] >> keys[i];
		
		++i;
	}
	file.close();

	std::ifstream playerLife;
	playerLife.open("Player/PlayerStats.txt");	//the data is layed out as follows : 
	std::string line;							//	MaxHP
	if (!playerLife.is_open())					//	CurrentHP
	{											//	Money
		throw std::runtime_error("Unable to"	//	Resistance
			"open file Player/PlayerStats.txt");//	Strength
	}											//
	std::getline(playerLife, line);	//Max HP
	std::istringstream iss(line);
	iss >> fMaxHP;
	{
		std::getline(playerLife, line);	//Current HP
		std::istringstream iss(line);
		iss >> fHP;
	}
	{
		std::getline(playerLife, line);	//Money
		std::istringstream iss(line);
		iss >> fMoney;
	}
	{
		std::getline(playerLife, line);	//Resistance
		std::istringstream iss(line);
		iss >> fResistance;
	}
	{
		std::getline(playerLife, line);	//Strength
		std::istringstream iss(line);
		iss >> fStrength;
	}

	for (size_t action = PlayerAction::MoveLeft; action < PlayerAction::ActionCount; ++action)
	{
		fKeyBinding[action] = static_cast<sf::Keyboard::Key>(keys[action]);
	}

	
}
