#pragma once
#include "ResourceHolder.hpp"
#include "InventoryBox.h"

struct Player_Data {

	Player_Data();

	std::map<unsigned int, sf::Keyboard::Key> fKeyBinding;
	void assignKey(unsigned int action, sf::Keyboard::Key key);
	
	bool fIsDead;
	bool fBackToMenu;
	bool fGameWin;
	bool fChangeWorld;

	int fHP;
	int fStrength;
	int fResistance;
	int fMoney;
	int fMaxHP;

	std::vector<GUI::InventoryBox> fInventoryBoxes;

	void destroyItems();
	~Player_Data();
};

namespace PlayerAction {
	enum Type
	{
		MoveLeft, MoveRight, Crouch, Slide, RolledDash, Attack1, Attack2, Attack3, Jump, OpenInventory, ActionCount
	};
}