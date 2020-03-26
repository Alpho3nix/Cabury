#pragma once
#include "Boss.h"
class Minotaur : public Boss
{
private:

	sf::Time fJumpAttackAccu = sf::Time::Zero;
	sf::Time fJumpAttackStatic = sf::seconds(1.f);
	bool fBeginJumpAttack = false;

	void doTheJumpAttack(sf::Time dt);
	void doCloseAttack(sf::Time dt, int attack);

	sf::Time fAttack1Accu = sf::Time::Zero;
	sf::Time fAttack1Timer;

	sf::Time fAttack2Accu = sf::Time::Zero;
	sf::Time fAttack2Timer;
	bool fIsDoingCloseAttack = false;
	bool fHasDoneDamage = false;
	int random = 0;

	bool isBoosted = false;
	sf::Time BoostingAccu = sf::Time::Zero;
	sf::Time BoostingTimer = sf::seconds(5.f);

	unsigned int attacking = 0;
	sf::Time attackingAccu = sf::Time::Zero;
	sf::Time attackingDelay = sf::seconds(2.f);

	sf::Time fBoostingColorAccu = sf::Time::Zero;
	sf::Time fBoostingColorTimer = sf::seconds(0.3f);

	float fSpeed_default = 60.f;
	float fSpeed = 60.f;
	float fDamagePlayerDistance = 85.f;

	int damageAttack1_default = 50.f;
	int damageAttack2_default = 60.f;
	int damageAttack1 = 50.f;
	int damageAttack2 = 60.f;
	int fJumpAttackDamage_default = 100.f;
	int fJumpAttackDamage = 100.f;

public:
	Minotaur(int hp, TexturesHolder& textures, const FontsHolder& fonts, Player& player, std::vector<std::vector<struct ItemType>> itemTypes);
	~Minotaur();

	void updateNode(sf::Time dt, CommandQueue& commands) override;
	void JumpAttack(sf::Time dt);
	bool applyDamage(int amount) override;
	sf::FloatRect getBoundingRect() const override;
};

