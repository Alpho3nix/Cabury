#pragma once
#include "Entity.h"
#include "DataTables.h"

class Player;

/* Each enemy has 5 animations: idle, walk, being hit, attack and death */
class Enemy : public Entity
{
public:
	enum Type
	{
		Ogre, Slime, Oculothorax, FireWisp, WaterWisp, WindWisp, EarthWisp
	};

	Enemy(int hp, Type type, TexturesHolder& textures, const FontsHolder& fonts, Player& player, std::vector<std::vector<struct ItemType>> itemTypes, float damageDistance = 50.f, int damage = 10);

	virtual unsigned int getCategory() const override;
	virtual void updateNode(sf::Time dt, CommandQueue& commands) override;

	bool applyDamage(int amount) override;
	sf::FloatRect getBoundingRect() const override;

	bool isRemovable() const override;

	std::vector<std::vector<struct ItemType>> fItemTypes;
	void createItem(SceneNode& node, const sf::Texture& tex, struct ItemType type);
	float getMinDamageDistance() const;
	
	void setAttackOnPlayer(std::function<void(Player& player, int damage, CommandQueue& commands)> func);	//the default attack is just dealing damage to the player directly

	int getDamage() const;
	void setDamage(int newDamage);

private:

	int fEnemyDamage = 10;
	std::function<void(Player& player, int damage, CommandQueue& commands)> attackOnPlayer;

	float minDamageDistance = 50.f;

	bool isBeingHurt = false;

	sf::Time deathAccu = sf::Time::Zero;

	void dontMove(sf::Time duration);
	bool fCanAttack = false;
	bool fHasDoneAttack = false;
	sf::Time attackDuration;
	void attack(sf::Time dt, CommandQueue& commands);

	void handleType(Type type, TexturesHolder& textures);

protected:
	bool fIsDead = false;

	Player& fPlayer;
	bool canMove = true;

	void canDoAttack();
private:

	sf::Time notMovingDuration;
	sf::Time notMovingAccu = sf::Time::Zero;

	const sf::Texture& fItemTex;
	Command fDropItemCommand;
	bool hasSpawnedItems = false;

	//Animations
	void setAnimations();
	EnemyAnimationData fAnimData;
};