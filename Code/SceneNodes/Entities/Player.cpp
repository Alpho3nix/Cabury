#include "Player.h"
#include <iostream>
#include "UtilityFunc.h"
#include "Enemy.h"
#include "NonPlayerChar.h"
#include "Button.h"
#include "Bar.h"
#include "ValueSymbol.h"
#include "InventoryBox.h"
#include "Item.h"
#include "PlayerData.h"

const float Speed = 120.f;


void Player::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(fSprite, states);

	sf::View currView = target.getView();
	target.setView(target.getDefaultView());
	target.draw(fHUD);
	if (openInventory) {
		target.draw(VisualInventory);
		target.draw(Inventory);
	}
	target.setView(currView);
}

bool Player::ProcessRealTimeInput(CommandQueue & commands, sf::Time dt)
{
	input = false;
	if (sf::Keyboard::isKeyPressed(fKeyBinding[PlayerAction::MoveRight]))
	{
		fActionBinding[PlayerAction::MoveRight](commands, dt);
	}
	else if (sf::Keyboard::isKeyPressed(fKeyBinding[PlayerAction::MoveLeft]))
	{
		fActionBinding[PlayerAction::MoveLeft](commands, dt);
	}
	else if (sf::Keyboard::isKeyPressed(fKeyBinding[PlayerAction::Crouch]))
	{
		fActionBinding[PlayerAction::Crouch](commands, dt);
	}
	if (sf::Keyboard::isKeyPressed(fKeyBinding[PlayerAction::Slide]))
	{
		fActionBinding[PlayerAction::Slide](commands, dt);
	}
	if (sf::Keyboard::isKeyPressed(fKeyBinding[PlayerAction::RolledDash]))
	{
		fActionBinding[PlayerAction::RolledDash](commands, dt);
	}
	if (sf::Keyboard::isKeyPressed(fKeyBinding[PlayerAction::Attack1]))
	{
		fActionBinding[PlayerAction::Attack1](commands, dt);
	}
	if (sf::Keyboard::isKeyPressed(fKeyBinding[PlayerAction::Attack2]))
	{
		fActionBinding[PlayerAction::Attack2](commands, dt);
	}
	if (sf::Keyboard::isKeyPressed(fKeyBinding[PlayerAction::Attack3]))
	{
		fActionBinding[PlayerAction::Attack3](commands, dt);
	}
	if (sf::Keyboard::isKeyPressed(fKeyBinding[PlayerAction::Jump]))
	{
		fActionBinding[PlayerAction::Jump](commands, dt);
	}
	
	return input;
}

bool Player::isAttacking()
{
	switch (CurrentAnimation)
	{
	case AnimType::Attack1:
		fAttackDuration = Attack1_Duration;
		fAttackNumFrames = 6;
		return true;
	case AnimType::Attack2:
		fAttackDuration = Attack2_Duration;
		fAttackNumFrames = 5;
		return true;
	case AnimType::Attack3:
		fAttackDuration = Attack3_Duration;
		fAttackNumFrames = 6;
		return true;
	default:
		break;
	}
	return false;
}

void Player::HandlingAnimationAndInput(sf::Time dt, CommandQueue & commands)
{
	bool inputs = false;
	if (!fEnableInputs)
	{
		inputs = false;
	}
	else
	{
		inputs = ProcessRealTimeInput(commands, dt);
	}
	if (!isSteppingOnSth() && !AnimTimer) {
		setCurrentAnimation(Jump);
	}
	if (!inputs && !AnimTimer && isSteppingOnSth())
	{
		setCurrentAnimation(Idle);
		setVelocity(0.f, getVelocity().y);
	}

	if (fCanClimbLadder && !AnimTimer && !isSteppingOnSth()) {
		setCurrentAnimation(AnimType::ClimbLadder);
		if (!inputs) {
			setVelocity(getVelocity().x, 0.f);
		}
	}

	if (AnimTimer && AnimTimerAccu <= AnimDuration)
	{
		setVelocity(AnimVelocity(AnimTimerAccu) * dt.asSeconds());
		AnimTimerAccu += dt;
	}
	else
	{
		AnimTimer = false;
		AnimTimerAccu = sf::Time::Zero;
		fCanChangeLookingDir = true;
	}
}

void Player::die(sf::Time dt)
{
	setVelocity(0.f, getVelocity().y);
	fSprite.setColor(sf::Color::White);
	setCurrentAnimation(AnimType::Death);
	deathAccu += dt;
	if (deathAccu >= DeathDuration - DeathDuration / 5.f)
	{
		deathAccu = sf::Time::Zero;
		fIsDead = true;
	}
}

unsigned int Player::getCategory() const
{
	return Category::PlayerChar;
}

sf::FloatRect Player::getBoundingRect() const
{
	Activity activity = whatIsPlayerDoing();
	sf::FloatRect retRect;
	sf::FloatRect defaultRect = Entity::getBoundingRect();
	switch (activity)
	{
	case Player::Activity::Run_Idle_Jump:
	case Player::Activity::ClimbingLadder:
		retRect = sf::FloatRect(defaultRect.left + defaultRect.width / 2.f - 12.f, defaultRect.top + 10.f, 25.f, defaultRect.height - 10.f);
		break;
	case Player::Activity::Crouch:
		retRect = sf::FloatRect(defaultRect.left + defaultRect.width / 2.f - 12.f, defaultRect.top + defaultRect.height / 2.f, 25.f, defaultRect.height / 2.f);
		break;
	case Player::Activity::Attacking:
		retRect = sf::FloatRect(defaultRect.left + defaultRect.width / 4.f, defaultRect.top + 10.f, defaultRect.width - defaultRect.width / 4.f, defaultRect.height - 10.f);
		break;
	case Player::Activity::Sliding:
		retRect = sf::FloatRect(defaultRect.left + 28.f, defaultRect.top + defaultRect.height - 28.f, 50.f, 28.f);
		break;
	case Player::Activity::Rolling:
		retRect = sf::FloatRect(defaultRect.left + defaultRect.width / 2.f - 12.f, defaultRect.top + defaultRect.height / 2.f - 12.f, 25.f, defaultRect.height / 2.f - 12.f);
		break;
	default:
		retRect = defaultRect;
		break;
	}

	return retRect;
}

Player::Player(State::Context context, PlayerData data)
	:Entity(context.player->fHP), fPlayerData(data), fEntitiesInRange(), fContext(context), fHUD(), Inventory(), VisualInventory(context.textures->get(Textures::ID::PlayerInventory))
	, fMoney(context.player->fMoney), fResistance(context.player->fResistance), fStrength(context.player->fStrength), inventoryBoxes()
{

	for (int i = 0; i < context.player->fInventoryBoxes.size(); ++i)
	{
		inventoryBoxes.push_back(new GUI::InventoryBox(context.player->fInventoryBoxes[i]));
	}

	fSprite.setTexture(context.textures->get(Textures::ID::PlayerAnim));
	sf::Vector2i frameSize(100, 74);
	sf::IntRect rect(sf::Vector2i(0, 0), frameSize);
	fSprite.setTextureRect(rect);
	CurrentAnimation = IdleType = Idle;

	SettingUpAnimations(frameSize);

	
	Utility::CenterOrigin(fSprite);
	setPosition(data.fSpawnPoint);

	createInventory(context.fonts->get(Fonts::ID::Sansation));
	createHUD();
	initializeAction();

	fKeyBinding = fContext.player->fKeyBinding;
}

Player::~Player()
{
}

void Player::updateNode(sf::Time dt, CommandQueue& commands)
{
	fHUD.update(dt);
	if (fCinematicState != CinematicState::Null)
	{
		switch (fCinematicState)
		{
		case Player::CinematicState::MinotaurBegin:
			updateMinotaurBeginCinematicState(dt, commands);
			break;
		case Player::CinematicState::MinotaurEnd:
			updateMinotaurEndCinematicState(dt, commands);
			break;
		default:
			break;
		}
	}
	else
	{
		if (!fCanJump)
		{
			fNotJumpAccu += dt;
			if (fNotJumpAccu >= fNotJumpTimer)
			{
				if (!isSteppingOnSth())
				{
					fNotJumpAccu = fNotJumpTimer;
				}
				else {
					if (fNotJumpAccu >= fNotJumpTimer + sf::seconds(0.1f))
					{
						fCanJump = true;
						fNotJumpAccu = sf::Time::Zero;
					}
				}
			}
		}

		if (isDestroyed())
		{
			die(dt);
		}
		else {

			HandlingAnimationAndInput(dt, commands);

			if (isBeingHurt)
			{
				beingHurt(dt);
			}

			if (isAttacking())
			{
				if (fCanAttack) {
					attackDuration = ((attackDuration == sf::Time::Zero) ? (fAttackDuration - 2.f * fAttackDuration / float(fAttackNumFrames)) : attackDuration);
					damage = CurrentAnimation;

					attack(dt, commands);

				}
				else
				{
					attackDuration = fAttackDuration - 2.f * fAttackDuration / float(fAttackNumFrames);
					attackAccu = sf::Time::Zero;
				}
			}
			else
			{
				attackDuration = sf::Time::Zero;
				attackAccu = sf::Time::Zero;
			}
		}

	}

	if (!fIsDead)
		Entity::updateNode(dt, commands);
}

void Player::attack(sf::Time dt, CommandQueue & commands)
{
	attackAccu += dt;

	if (attackAccu >= attackDuration)
	{
		attackAccu = sf::Time::Zero;
		attackDuration += fAttackDuration - attackDuration;	//because attackDuration is slightly shorter than the Animation Attack duration, attackDuration is then accumulating a lag
		int dam = damage + fStrength * 0.05;
		
			std::vector<Damageable*> enemies = findInRange(Category::damagable);
			if (CurrentAnimation == Entity::AnimType::Attack3 || CurrentAnimation == Entity::AnimType::Attack3_Left)
			{
				for (Damageable* e : enemies)
				{
					e->applyDamage(dam);
				}
			}
			else
			{
				for (Damageable* e : enemies)
				{
					if (e->applyDamage(dam))
						break;
				}
			}
		fCanAttack = false;
	}
}

std::vector<Damageable*> Player::findInRange(Category type)
{
	std::vector<Damageable*> found;
	for (Damageable* ent : fEntitiesInRange)
	{
		if (ent) {
			if (ent->getCategory() & type)
			{
				found.push_back(ent);
			}
		}
	}
	return found;
}

Damageable* Player::findFirstInRange(Category type)
{
	Damageable* found = nullptr;
	for (Damageable* ent : fEntitiesInRange)
	{
		if (ent) {
			if (ent->getCategory() & type)
			{
				found = ent;
				break;
			}
		}
	}
	return found;
}

void Player::beingHurt(sf::Time dt)
{
	hurtAccu += dt;

	fSprite.setColor(sf::Color::White);
	if (hurtAccu < hurtDuration - hurtDuration / 2.f)
		fSprite.setColor(sf::Color::Red);

	if (hurtAccu >= hurtDuration)
	{
		hurtAccu = sf::Time::Zero;
		isBeingHurt = false;
	}
}

void Player::createInventory(const sf::Font& font)
{
	using namespace GUI;
	VisualInventory.setPosition(fContext.window->getDefaultView().getCenter().x - VisualInventory.getTexture()->getSize().x / 2.f, 10.f);
	Inventory.setPosition(VisualInventory.getPosition());

	if (inventoryBoxes.empty()) {
		for (int i = 0; i < 32; ++i)	//for now the inventory contains a hardCoded amount of boxes
		{
			InventoryBox* box = new InventoryBox(font);
			box->setPosition(3.f + 25.f * (i % 8) + (i % 8) * 4.f, 3.f + 25.f * int(i / 8) + int(i / 8) * 3.f);
			Inventory.pack(box);
			inventoryBoxes.push_back(box);
			fContext.player->fInventoryBoxes.push_back(*box);
		}
	}
	else
	{
		for (int i = 0; i < 32; ++i)	//for now the inventory contains a hardCoded amount of boxes
		{
			InventoryBox* box = inventoryBoxes[i];
			box->setPosition(3.f + 25.f * (i % 8) + (i % 8) * 4.f, 3.f + 25.f * int(i / 8) + int(i / 8) * 3.f);
			
			Inventory.pack(box);
		}
	}
}

void Player::createHUD()
{
	using namespace GUI;

	//create life bar
	float lifeBar_size = 100.f;
	Bar* lifeBar = new Bar(sf::FloatRect(fContext.window->getDefaultView().getSize().x - lifeBar_size - 10.f, 10.f, lifeBar_size, 15.f), sf::Color::Red, HealthPoints, fContext.player->fMaxHP);
	fHUD.pack(lifeBar);

	//create player's strength stats
	float x_statsOffset = 50.f;
	float y_statsOffset = 50.f;
	float xPos_stats = fContext.window->getDefaultView().getSize().x - x_statsOffset;
	float yPos_stats = lifeBar->getPosition().y;
	const sf::Texture& itemTex = fContext.textures->get(Textures::ID::Items);
	const sf::Font& statsFont = fContext.fonts->get(Fonts::ID::Sansation);
	ValueSymbol* strengthStats = new ValueSymbol(sf::Vector2f(xPos_stats, yPos_stats + y_statsOffset), itemTex,	statsFont, fStrength, sf::IntRect(176, 288, 16, 16));
	fHUD.pack(strengthStats);

	//create player's resistance stats
	ValueSymbol* resistanceStats = new ValueSymbol(sf::Vector2f(xPos_stats, yPos_stats + y_statsOffset * 2.f), itemTex, statsFont, fResistance, sf::IntRect(208, 144, 16, 16));
	fHUD.pack(resistanceStats);

	//create player's money stats
	ValueSymbol* money = new ValueSymbol(sf::Vector2f(xPos_stats, yPos_stats + y_statsOffset * 3.f), itemTex, statsFont, fMoney, sf::IntRect(208, 96, 16, 16));
	fHUD.pack(money);


}

Player::Activity Player::whatIsPlayerDoing() const
{
	switch (CurrentAnimation)
	{
	case AnimType::Attack1:
	case AnimType::Attack2:
	case AnimType::Attack3:
		return Activity::Attacking;
		break;
	case AnimType::Crouch:
		return Activity::Crouch;
		break;
	case AnimType::RolledDash:
		return Activity::Rolling;
		break;
	case AnimType::Slide:
		return Activity::Sliding;
		break;
	case AnimType::ClimbLadder:
		return Activity::ClimbingLadder;
		break;
	default:
		break;
	}
	return Activity::Run_Idle_Jump;
}

void Player::updateMinotaurEndCinematicState(sf::Time dt, CommandQueue& commands)
{

	setCurrentAnimation(Idle);
	setVelocity(0.f, 0.f);
}

void Player::updateMinotaurBeginCinematicState(sf::Time dt, CommandQueue& commands)
{
	setCurrentAnimation(Idle);
	setVelocity(0.f, 0.f);
	
}

int Player::getStrength() const
{
	return fStrength;
}

int Player::getResistance() const
{
	return fResistance;
}

int Player::getMoney() const
{
	return fMoney;
}

void Player::addStrength(int amount)
{
	fStrength += amount;
	fContext.player->fStrength = fStrength;
}

void Player::addResistance(int amount)
{
	fResistance += amount;
	fContext.player->fResistance = fResistance;
}

void Player::addMoney(int amount)
{
	fMoney += amount;
	fContext.player->fMoney = fMoney;
}

void Player::ChangeToBoss(bool flag)
{
	fCanChangeToBoss = flag;
}

bool Player::canChangeToBoss() const
{
	return fCanChangeToBoss;
}

void Player::setCinematicState(CinematicState state)
{
	fCinematicState = state;
}

Player::CinematicState Player::getCinematicState() const
{
	return fCinematicState;
}

bool Player::requestEnteringWorld() const
{
	return fRequestEnteringWorld;
}

void Player::ProcessEvents(const sf::Event & e, CommandQueue & commands)
{
	if (fCanClimbLadder && !isSteppingOnSth())
	{
		if (e.type == sf::Event::KeyReleased && (e.key.code == fKeyBinding[PlayerAction::Jump] || e.key.code == fKeyBinding[PlayerAction::Crouch]))
		{
			fAnimations[CurrentAnimation + fLookingState]->stop();
		}
	}
	
	if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::P)
	{
		fRequestEnteringWorld = true;
	}
	else if (e.type == sf::Event::KeyReleased && e.key.code == sf::Keyboard::P)
	{
		fRequestEnteringWorld = false;
	}

	NonPlayerChar* npc = static_cast<NonPlayerChar*>(findFirstInRange(Category::NPC));
	if (npc && !npc->isInteractionPaused()) {
		if (e.type == sf::Event::KeyReleased && e.key.code == sf::Keyboard::Enter)
		{

			if (!isInteractingNPC) {
				isInteractingNPC = true;
			}
		}
		if (isInteractingNPC)
		{
			openInventory = false;
			npc->handleEvent(e, commands);
		}


	}
	else
	{
		isInteractingNPC = false;
	}

	
	if (!isInteractingNPC) {

		
		if (e.type == sf::Event::KeyPressed && e.key.code == fKeyBinding[PlayerAction::OpenInventory])
		{
			fActionBinding[PlayerAction::OpenInventory](commands, sf::seconds(0.f));
		}

		if (openInventory)
			Inventory.handleEvent(e);

	}
	
}

bool Player::applyDamage(int amount)
{
	if (CurrentAnimation == AnimType::Slide || fDamageDisable)
		return false;
	int damage = amount - fResistance * 0.05f;
	if (damage <= 0)
	{
		damage = 1;
	}
	HealthPoints -= damage;
	if (HealthPoints < 0)
		HealthPoints = 0;

	if(!isBeingHurt)
		isBeingHurt = true;


	fContext.player->fHP = HealthPoints;
	return true;
}

bool Player::isRemovable() const
{
	return false;	//if the player is dead, there's no more gameplay so we never remove it, we just push the deathScreenState
}

void Player::addInventoryItem(Item * item)
{
	if (item->getParent() != nullptr)
		return;

	bool buff = false;
	for (int i = 0; i < inventoryBoxes.size(); ++i)
	{
		if (!inventoryBoxes[i]->containsItem())
		{
			inventoryBoxes[i]->putItem(item);
			inventoryBoxes[i]->addQuantity(1);
			fContext.player->fInventoryBoxes[i].putItem(item);
			fContext.player->fInventoryBoxes[i].addQuantity(1);
			buff = true;
			break;
		}
		else if (inventoryBoxes[i]->getItem()->getItemSprite().getTextureRect() == item->getItemSprite().getTextureRect())	//if they have the same texture
		{
			inventoryBoxes[i]->addQuantity(1);
			inventoryBoxes[i]->putItem(item);
			fContext.player->fInventoryBoxes[i].putItem(item);
			fContext.player->fInventoryBoxes[i].addQuantity(1);
			buff = true;
			break;
			
		}
	}
}

bool Player::isPlayerDead() const
{
	return fIsDead;
}

void Player::atRange(bool attack, Damageable& entity)
{
	fCanAttack = attack;
	fEntitiesInRange.insert(&entity);
}

bool Player::areInputsEnable() const
{
	return fEnableInputs;
}

void Player::enableInputs(bool flag)
{
	fEnableInputs = flag;
}

bool Player::canTakeDamage() const
{
	return !fDamageDisable;
}

void Player::disableDamage(bool flag)
{
	fDamageDisable = flag;
}

void Player::initializeAction()
{
	fActionBinding[PlayerAction::MoveLeft] = [this](CommandQueue& commands, sf::Time dt)
	{
		if (fCanChangeLookingDir) {
			commands.addCommand(Category::PlayerChar, derivedAction<Player>([](Player& player, sf::Time dt)
			{
				if (player.fCanClimbLadder)
				{
					player.setVelocity(player.getVelocity().x, 0.f);
				}
				player.setVelocity(-Speed * dt.asSeconds(), player.getVelocity().y);
			}));
		}
		if (!AnimTimer && isSteppingOnSth()) {
			setCurrentAnimation(Run);
		}
		input = true;
		
	};

	fActionBinding[PlayerAction::MoveRight] = [this](CommandQueue& commands, sf::Time dt)
	{
		if (fCanChangeLookingDir) {
			commands.addCommand(Category::PlayerChar, derivedAction<Player>([](Player& player, sf::Time dt)
			{
				if (player.fCanClimbLadder)
				{
					player.setVelocity(player.getVelocity().x, 0.f);
				}
				player.setVelocity(Speed * dt.asSeconds(), player.getVelocity().y);
			}));
		}
		if (!AnimTimer && isSteppingOnSth()) {
			setCurrentAnimation(Run);
		}
		input = true;
	};

	fActionBinding[PlayerAction::Crouch] = [this](CommandQueue& commands, sf::Time dt)
	{
		if (fCanClimbLadder && !AnimTimer)
		{

			fAnimations[ClimbLadder + fLookingState]->start();
			this->move(0.f, 150.f * dt.asSeconds());
			setVelocity(0.f, 0.f);
			input = true;
		}
		else {
			if (!AnimTimer && isSteppingOnSth()) {
				setCurrentAnimation(Crouch);

				setVelocity(0.f, 0.f);
			}
			if (!AnimTimer && !isSteppingOnSth())
			{
				commands.addCommand(Category::PlayerChar, derivedAction<Player>([](Player& player, sf::Time dt)
					{
						player.setUpwardsVel(4.f);
					}));
			}
		}
		input = true;
	};

	fActionBinding[PlayerAction::Slide] = [this](CommandQueue& commands, sf::Time dt)
	{
		if (!AnimTimer)
		{
			AnimTimer = true;
			AnimDuration = SlideDuration;
			setCurrentAnimation(Slide);

			fCanChangeLookingDir = false;
			AnimVelocity = [this](sf::Time Accu)
			{
				return sf::Vector2f(5.f * Speed * (1.f - 2.f * isLookingLeft()) * (SlideDuration.asSeconds() - Accu.asSeconds()), getVelocity().y);
			};
		}
		input = true;
	};

	fActionBinding[PlayerAction::RolledDash] = [this](CommandQueue& commands, sf::Time dt)
	{
		if (!AnimTimer) {
			AnimTimer = true;
			fCanChangeLookingDir = false;
			AnimDuration = RolledDashDuration;
			setCurrentAnimation(RolledDash);
			if (isSteppingOnSth()) {
				AnimVelocity = [this](sf::Time Accu)
				{
					if (Accu >= 2.f * RolledDashDuration / float(fAnimations[RolledDash]->getNumFrames())) {
						return sf::Vector2f(3.f * Speed * (1.f - 2.f * isLookingLeft()), getVelocity().y);
					}
					return sf::Vector2f(0.f, getVelocity().y);
				};
			}
			else
			{
				setUpwardsVel(getUpwardsVel() - .1f * dt.asMilliseconds());
				AnimVelocity = [this](sf::Time Accu)
				{
					return sf::Vector2f(3.f * Speed * (1.f - 2.f * isLookingLeft()), getVelocity().y);
				};
			}
		}
		input = true;
	};

	fActionBinding[PlayerAction::Attack1] = [this](CommandQueue& commands, sf::Time dt)
	{
		if (!AnimTimer && isSteppingOnSth())
		{
			AnimTimer = true;
			AnimDuration = Attack1_Duration;

			setCurrentAnimation(Attack1);

			fCanChangeLookingDir = false;
			AnimVelocity = [this](sf::Time Accu)
			{
				return sf::Vector2f(0.f, getVelocity().y);
			};
		}
		input = true;
	};

	fActionBinding[PlayerAction::Attack2] = [this](CommandQueue& commands, sf::Time dt)
	{
		if (!AnimTimer && isSteppingOnSth())
		{
			AnimTimer = true;
			AnimDuration = Attack2_Duration;

			setCurrentAnimation(Attack2);

			fCanChangeLookingDir = false;
			AnimVelocity = [this](sf::Time Accu)
			{
				return sf::Vector2f(0.f, getVelocity().y);
			};
		}
		input = true;
	};

	fActionBinding[PlayerAction::Attack3] = [this](CommandQueue& commands, sf::Time dt)
	{
		if (!AnimTimer)
		{
			AnimTimer = true;
			AnimDuration = Attack3_Duration;

			setCurrentAnimation(Attack3);

			fCanChangeLookingDir = false;
			AnimVelocity = [this, dt](sf::Time Accu)
			{
				return sf::Vector2f((isSteppingOnSth() ? 0.f : getVelocity().x / dt.asSeconds()), getVelocity().y);
			};
		}
		input = true;
	};

	fActionBinding[PlayerAction::Jump] = [this](CommandQueue& commands, sf::Time dt)
	{
		if (fCanJump && !fCanClimbLadder) {
			if (!AnimTimer && isSteppingOnSth())
			{
				setCurrentAnimation(Jump);
				commands.addCommand(Category::PlayerChar, derivedAction<Player>([](Player& player, sf::Time dt)
				{
					player.setUpwardsVel(-.63f * dt.asMilliseconds());
				}));
				fCanJump = false;
				fBeginGravityTimer = true;
			}
			input = true;
		}
		if (fCanClimbLadder && !AnimTimer)
		{

			fAnimations[ClimbLadder + fLookingState]->start();
			this->move(0.f, -150.f * dt.asSeconds());
			setVelocity(0.f, 0.f);
			input = true;
		}
	};

	fActionBinding[PlayerAction::OpenInventory] = [this](CommandQueue& commands, sf::Time)
	{
		openInventory = !openInventory;
	};
}

bool Player::isRealTimeAction(PlayerAction::Type action)
{
	switch (action)
	{
	case PlayerAction::OpenInventory:
		return false;
		break;
	default:
		return true;
		break;
	}
}


void Player::SettingUpAnimations(sf::Vector2i frameSize)
{
	/************Animations***********/

	//Idle without sword out
	//Animation idle1 when looking to the right
	Animation* Idle1_Right = new Animation(*fSprite.getTexture(), sf::Vector2i(0, 0), 1);
	Idle1_Right->setDuration(IdleDuration);
	Idle1_Right->Repeat(true);
	Idle1_Right->setFrameSize(frameSize);
	Idle1_Right->setNumFrames(4);
	Idle1_Right->Restart();

	fAnimations[Idle] = Idle1_Right;

	//Animation idle1 when looking to the left
	Animation* Idle1_Left = new Animation(*fSprite.getTexture(), sf::Vector2i(fSprite.getTexture()->getSize().x - frameSize.x, fSprite.getTexture()->getSize().y / 2), -1);
	Idle1_Left->setDuration(IdleDuration);
	Idle1_Left->Repeat(true);
	Idle1_Left->setFrameSize(frameSize);
	Idle1_Left->setNumFrames(4);
	Idle1_Left->Restart();

	fAnimations[IdleLeft] = Idle1_Left;

	//Idlde with sword out
	//Animation idle2 when looking to the right
	Animation* Idle2_Right = new Animation(*fSprite.getTexture(), sf::Vector2i(fSprite.getTexture()->getSize().x - 2 * frameSize.x, 4 * frameSize.y), 1);
	Idle2_Right->setDuration(IdleDuration);
	Idle2_Right->Repeat(true);
	Idle2_Right->setFrameSize(frameSize);
	Idle2_Right->setNumFrames(4);
	Idle2_Right->Restart();

	fAnimations[Idle2] = Idle2_Right;

	//Animation idle2 when looking to the left
	Animation* Idle2_Left = new Animation(*fSprite.getTexture(), sf::Vector2i(frameSize.x, fSprite.getTexture()->getSize().y / 2 + 4 * frameSize.y), -1);
	Idle2_Left->setDuration(IdleDuration);
	Idle2_Left->Repeat(true);
	Idle2_Left->setFrameSize(frameSize);
	Idle2_Left->setNumFrames(4);
	Idle2_Left->Restart();

	fAnimations[Idle2Left] = Idle2_Left;

	//Animation Running when looking to the right
	Animation* RunRight = new Animation(*fSprite.getTexture(), sf::Vector2i(0, frameSize.y), 1);
	RunRight->setDuration(RunDuration);
	RunRight->Repeat(true);
	RunRight->setFrameSize(frameSize);
	RunRight->setNumFrames(6);
	RunRight->Restart();

	fAnimations[AnimType::Run] = RunRight;

	//Animation Running when looking to the left
	Animation* RunLeft = new Animation(*fSprite.getTexture(), sf::Vector2i(fSprite.getTexture()->getSize().x - frameSize.x, fSprite.getTexture()->getSize().y / 2 + frameSize.y), -1);
	RunLeft->setDuration(RunDuration);
	RunLeft->Repeat(true);
	RunLeft->setFrameSize(frameSize);
	RunLeft->setNumFrames(6);
	RunLeft->Restart();

	fAnimations[AnimType::RunLeft] = RunLeft;

	//Animation Jumping Right
	Animation* JumpRight = new Animation(*fSprite.getTexture(), sf::Vector2i(fSprite.getTexture()->getSize().x - 3 * frameSize.x, 9 * frameSize.y), 1);
	JumpRight->setDuration(JumpDuration);
	JumpRight->Repeat(true);
	JumpRight->setFrameSize(frameSize);
	JumpRight->setNumFrames(2);
	JumpRight->Restart();

	fAnimations[Jump] = JumpRight;

	//Animation Jumping Left
	Animation* JumpLeft = new Animation(*fSprite.getTexture(), sf::Vector2i(2 * frameSize.x, fSprite.getTexture()->getSize().y / 2 + 9 * frameSize.y), -1);
	JumpLeft->setDuration(JumpDuration);
	JumpLeft->Repeat(true);
	JumpLeft->setFrameSize(frameSize);
	JumpLeft->setNumFrames(2);
	JumpLeft->Restart();

	fAnimations[AnimType::JumpLeft] = JumpLeft;

	//Animation Climbing Ladder Right
	Animation* ClmbLadderRight = new Animation(*fSprite.getTexture(), sf::Vector2i(3 * frameSize.x, fSprite.getTexture()->getSize().y / 2 - frameSize.y), 1);
	ClmbLadderRight->setDuration(ClimbingDuration);
	ClmbLadderRight->Repeat(true);
	ClmbLadderRight->setFrameSize(frameSize);
	ClmbLadderRight->setNumFrames(4);
	ClmbLadderRight->Restart();

	fAnimations[AnimType::ClimbLadder] = ClmbLadderRight;

	//Animation Climbing Ladder Left
	Animation* ClmbLadderLeft = new Animation(*fSprite.getTexture(), sf::Vector2i(4 * frameSize.x, fSprite.getTexture()->getSize().y - frameSize.y), -1);
	ClmbLadderLeft->setDuration(ClimbingDuration);
	ClmbLadderLeft->Repeat(true);
	ClmbLadderLeft->setFrameSize(frameSize);
	ClmbLadderLeft->setNumFrames(4);
	ClmbLadderLeft->Restart();

	fAnimations[AnimType::ClimbLadderLeft] = ClmbLadderLeft;

	//Animation Roll dashing when looking to the right
	Animation* RolledDashRight = new Animation(*fSprite.getTexture(), sf::Vector2i(fSprite.getTexture()->getSize().x - 2 * frameSize.x, frameSize.y), 1);
	RolledDashRight->setDuration(RolledDashDuration);
	RolledDashRight->setFrameSize(frameSize);
	RolledDashRight->setNumFrames(8);
	RolledDashRight->Repeat(false);
	RolledDashRight->Restart();

	fAnimations[RolledDash] = RolledDashRight;

	//Animation Roll dashing when looking to the left
	Animation* RolledDashLeft = new Animation(*fSprite.getTexture(), sf::Vector2i(2 * frameSize.x, fSprite.getTexture()->getSize().y / 2 + frameSize.y), -1);
	RolledDashLeft->setDuration(RolledDashDuration);
	RolledDashLeft->setFrameSize(frameSize);
	RolledDashLeft->setNumFrames(8);
	RolledDashLeft->Repeat(false);
	RolledDashLeft->Restart();

	fAnimations[AnimType::RolledDashLeft] = RolledDashLeft;

	//Animation Crouching when looking to the right
	Animation* CrouchRight = new Animation(*fSprite.getTexture(), sf::Vector2i(fSprite.getTexture()->getSize().x / 2, 0), 1);
	CrouchRight->Repeat(true);
	CrouchRight->setDuration(IdleDuration);
	CrouchRight->setFrameSize(frameSize);
	CrouchRight->setNumFrames(4);
	CrouchRight->Restart();

	fAnimations[Crouch] = CrouchRight;

	//Animation Crouching when looking to the left
	Animation* CrouchLeft = new Animation(*fSprite.getTexture(), sf::Vector2i(fSprite.getTexture()->getSize().x / 2 - frameSize.x, fSprite.getTexture()->getSize().y / 2), -1);
	CrouchLeft->setNumFrames(4);
	CrouchLeft->Repeat(true);
	CrouchLeft->setDuration(IdleDuration);
	CrouchLeft->setFrameSize(frameSize);
	CrouchLeft->Restart();

	fAnimations[AnimType::CrouchLeft] = CrouchLeft;

	//Animation Slide Right
	Animation* SlideRight = new Animation(*fSprite.getTexture(), sf::Vector2i(0, 3 * frameSize.y), 1);
	SlideRight->setNumFrames(5);
	SlideRight->Repeat(false);
	SlideRight->setDuration(SlideDuration);
	SlideRight->setFrameSize(frameSize);
	SlideRight->Restart();

	fAnimations[Slide] = SlideRight;

	//Animation Slide Left
	Animation* SlideLeft = new Animation(*fSprite.getTexture(), sf::Vector2i(fSprite.getTexture()->getSize().x, fSprite.getTexture()->getSize().y / 2 + 3 * frameSize.y), -1);
	SlideLeft->setNumFrames(5);
	SlideLeft->Repeat(false);
	SlideLeft->setDuration(SlideDuration);
	SlideLeft->setFrameSize(frameSize);
	SlideLeft->Restart();

	fAnimations[AnimType::SlideLeft] = SlideLeft;

	//Animation Attack1 Right
	Animation* Attack1_Right = new Animation(*fSprite.getTexture(), sf::Vector2i(frameSize.x, 5 * frameSize.y), 1);
	Attack1_Right->setNumFrames(6);
	Attack1_Right->Repeat(false);
	Attack1_Right->setDuration(Attack1_Duration);
	Attack1_Right->setFrameSize(frameSize);
	Attack1_Right->Restart();

	fAnimations[Attack1] = Attack1_Right;

	//Animation Attack1 Left
	Animation* Attack1_Left = new Animation(*fSprite.getTexture(), sf::Vector2i(fSprite.getTexture()->getSize().x - frameSize.x, fSprite.getTexture()->getSize().y / 2 + 5 * frameSize.y), -1);
	Attack1_Left->setNumFrames(6);
	Attack1_Left->Repeat(false);
	Attack1_Left->setDuration(Attack1_Duration);
	Attack1_Left->setFrameSize(frameSize);
	Attack1_Left->Restart();

	fAnimations[AnimType::Attack1_left] = Attack1_Left;

	//Animation Attack2 Right
	Animation* Attack2_Right = new Animation(*fSprite.getTexture(), sf::Vector2i(0, 6 * frameSize.y), 1);
	Attack2_Right->setNumFrames(5);
	Attack2_Right->Repeat(false);
	Attack2_Right->setDuration(Attack2_Duration);
	Attack2_Right->setFrameSize(frameSize);
	Attack2_Right->Restart();

	fAnimations[Attack2] = Attack2_Right;

	//Animation Attack2 Left
	Animation* Attack2_Left = new Animation(*fSprite.getTexture(), sf::Vector2i(fSprite.getTexture()->getSize().x - frameSize.x, fSprite.getTexture()->getSize().y / 2 + 6 * frameSize.y), -1);
	Attack2_Left->setNumFrames(5);
	Attack2_Left->Repeat(false);
	Attack2_Left->setDuration(Attack2_Duration);
	Attack2_Left->setFrameSize(frameSize);
	Attack2_Left->Restart();

	fAnimations[AnimType::Attack2_Left] = Attack2_Left;

	//Animation Attack3 Right
	Animation* Attack3_Right = new Animation(*fSprite.getTexture(), sf::Vector2i(fSprite.getTexture()->getSize().x - 4 * frameSize.x, 6 * frameSize.y), 1);
	Attack3_Right->setNumFrames(6);
	Attack3_Right->Repeat(false);
	Attack3_Right->setDuration(Attack3_Duration);
	Attack3_Right->setFrameSize(frameSize);
	Attack3_Right->Restart();

	fAnimations[Attack3] = Attack3_Right;

	//Animation Attack3 Left
	Animation* Attack3_Left = new Animation(*fSprite.getTexture(), sf::Vector2i(3 * frameSize.x, fSprite.getTexture()->getSize().y / 2 + 6 * frameSize.y), -1);
	Attack3_Left->setNumFrames(6);
	Attack3_Left->Repeat(false);
	Attack3_Left->setDuration(Attack3_Duration);
	Attack3_Left->setFrameSize(frameSize);
	Attack3_Left->Restart();

	fAnimations[AnimType::Attack3_Left] = Attack3_Left;

	//Animation Death Right
	Animation* DeathRight = new Animation(*fSprite.getTexture(), sf::Vector2i(0, 8 * frameSize.y), 1);
	DeathRight->setNumFrames(5);
	DeathRight->Repeat(false);
	DeathRight->setDuration(DeathDuration);
	DeathRight->setFrameSize(frameSize);
	DeathRight->Restart();

	fAnimations[Death] = DeathRight;

	//Animation Death Left
	Animation* DeathLeft = new Animation(*fSprite.getTexture(), sf::Vector2i(fSprite.getTexture()->getSize().x, fSprite.getTexture()->getSize().y / 2 + 8 * frameSize.y), -1);
	DeathLeft->setNumFrames(5);
	DeathLeft->Repeat(false);
	DeathLeft->setDuration(DeathDuration);
	DeathLeft->setFrameSize(frameSize);
	DeathLeft->Restart();

	fAnimations[AnimType::DeathLeft] = DeathLeft;
}