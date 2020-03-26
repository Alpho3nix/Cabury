#pragma once
#include "Entity.h"
#include "DataTables.h"

class Player;

class Boss : public Entity
{
public:

	enum Type
	{
		Minotaur
	};

protected:

	Player& fPlayer;
	BossAnimationData fAnimData;
	void setAnimations();

	const sf::Texture& fItemTex;
	Command fDropItemCommand;
	std::vector<std::vector<struct ItemType>> fItemTypes;

	bool fIsDead = false;
	bool fRemovable = false;
	bool fHasDroppedItems = false;

	sf::Time fHurtAccu = sf::Time::Zero;
	sf::Time fHurtTimer = sf::seconds(0.2f);
	bool fIsBeingHurt = false;

	bool isTakingDamage() const;

	void handleType(Type type, TexturesHolder& textures);
public:

	Boss(int hp, Type type, TexturesHolder& textures, const FontsHolder& fonts, Player& player, std::vector<std::vector<struct ItemType>> itemTypes);
	~Boss();

	virtual void updateNode(sf::Time dt, CommandQueue& commands) override;
	void createItem(SceneNode& node, const sf::Texture& tex, struct ItemType item);
	virtual bool applyDamage(int amount) override;

	unsigned int getCategory() const override;
	bool isRemovable() const override;
};