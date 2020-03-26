#pragma once
#include "Entity.h"
#include "DataTables.h"
#include "State.h"
#include "Answer.h"
#include "Container.h"
#include "PlayerData.h"

namespace GUI
{
	class InventoryBox;
}



class Player : public Entity
{
public:

	enum class CinematicState
	{
		Null, MinotaurBegin, MinotaurEnd
	};
private:

	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

	bool ProcessRealTimeInput(CommandQueue& commands, sf::Time dt);
	std::function<sf::Vector2f(sf::Time)> AnimVelocity = [](sf::Time Accu)
	{
		return sf::Vector2f(0.f, 0.f);
	};

	CinematicState fCinematicState = CinematicState::Null;

	sf::Time IdleDuration = sf::seconds(0.9f);
	sf::Time RunDuration = sf::seconds(0.7f);
	sf::Time JumpDuration = sf::seconds(0.3f);
	sf::Time RolledDashDuration = sf::seconds(0.8f);
	sf::Time SlideDuration = sf::seconds(0.8f);
	sf::Time Attack1_Duration = sf::seconds(0.5f);
	sf::Time Attack2_Duration = sf::seconds(0.4f);
	sf::Time Attack3_Duration = sf::seconds(0.8f);
	sf::Time DeathDuration = sf::seconds(2.f);
	sf::Time ClimbingDuration = sf::seconds(0.4f);

	sf::Time fAttackDuration = sf::Time::Zero;
	size_t fAttackNumFrames = 0;
	bool isAttacking();

	sf::Time AnimDuration = sf::seconds(1.f);
	sf::Time AnimTimerAccu = sf::Time::Zero;
	bool AnimTimer = false;
	bool fCanChangeLookingDir = true;
	bool input = false;
	
	bool fCanJump = true;
	sf::Time fNotJumpTimer = sf::seconds(1.f);	//delay until the player can rejump
	sf::Time fNotJumpAccu = sf::Time::Zero;


	void HandlingAnimationAndInput(sf::Time dt, CommandQueue& commands);
	void SettingUpAnimations(sf::Vector2i frameSize);

	PlayerData fPlayerData;
	
	void attack(sf::Time dt, CommandQueue& commands);
	bool fCanAttack = false;
	sf::Time attackDuration = sf::Time::Zero;
	sf::Time attackAccu = sf::Time::Zero;
	int damage = 20;
	std::vector<Damageable*> findInRange(Category type);
	Damageable* findFirstInRange(Category type);

	void beingHurt(sf::Time dt);
	bool isBeingHurt = false;
	sf::Time hurtDuration = sf::seconds(0.3f);
	sf::Time hurtAccu = sf::Time::Zero;

	bool fIsDead = false;
	sf::Time deathAccu = sf::Time::Zero;
	void die(sf::Time dt);

	bool isInteractingNPC = false;
	State::Context fContext;

	//HUD
	GUI::Container fHUD;
	//stats
	int fStrength;
	int fResistance;
	int fMoney;

	//inventory
	sf::Sprite VisualInventory;
	GUI::Container Inventory;
	std::vector<GUI::InventoryBox*> inventoryBoxes;
	void createInventory(const sf::Font& font);

	bool openInventory = false;
	void createHUD();

	enum class Activity
	{
		Run_Idle_Jump, Crouch, Attacking, Sliding, Rolling, ClimbingLadder, ActivityCount
	};
	Activity whatIsPlayerDoing() const;

	bool fRequestEnteringWorld = false;
	bool fDamageDisable = false;
	
	bool fCanChangeToBoss = false;
	
	void updateMinotaurEndCinematicState(sf::Time dt, CommandQueue& commands);
	void updateMinotaurBeginCinematicState(sf::Time dt, CommandQueue& commands);
public:
	std::set<Damageable*> fEntitiesInRange;
	bool fCanClimbLadder = false;

	int getStrength() const;
	int getResistance() const;
	int getMoney() const;
	void addStrength(int amount);
	void addResistance(int amount);
	void addMoney(int amount);

	void ChangeToBoss(bool flag);
	bool canChangeToBoss() const;

	void setCinematicState(CinematicState state);
	CinematicState getCinematicState() const;

	bool requestEnteringWorld() const;

	unsigned int getCategory() const override;
	sf::FloatRect getBoundingRect() const override;

	Player(State::Context context, PlayerData data);
	~Player();

	void updateNode(sf::Time dt, CommandQueue& commands) override;
	void ProcessEvents(const sf::Event& e, CommandQueue& commands);
	bool applyDamage(int amount) override;
	bool isRemovable() const override;

	void addInventoryItem(class Item* item);

	bool isPlayerDead() const;

	void atRange(bool attack, Damageable& entity);
	bool areInputsEnable() const;
	void enableInputs(bool flag);

	bool canTakeDamage() const;
	void disableDamage(bool flag);

private:
	std::map < PlayerAction::Type, std::function<void(CommandQueue& commands, sf::Time dt) >> fActionBinding;
	std::map<unsigned int, sf::Keyboard::Key> fKeyBinding;

	void initializeAction();
	bool isRealTimeAction(PlayerAction::Type action);

	bool fEnableInputs = true;
	

};