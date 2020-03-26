#pragma once
#include "Entity.h"

class Player;

class Cabury : public Entity
{
public:

	enum class CinematicState
	{
		BreakingBox, JailInKanalisation, BeginningAtBoss, EndingAtBoss
	};

	Cabury(int hp, TexturesHolder& textures, Player& player, CinematicState state);

	unsigned int getCategory() const override;
	sf::FloatRect getBoundingRect() const override;
	
	void updateNode(sf::Time dt, CommandQueue& commands) override;

	bool isRemovable() const override;
	bool isDestroyed() const override;
	
	bool applyDamage(int amount) override;
	void kill();

	CinematicState getCinematicState() const;
	void setCinematicState(CinematicState state);

	void cageBroken();
	bool fNoBook = false;
private:
	bool fIsDead = false;
	bool fIsRemovable = false;
	bool fCageBroken = false;
	
	Player& fPlayer;

	CinematicState fCurrentCinematicState;

	void followPlayerBehavior(sf::Time dt);
	/*Following player*/
	bool fBeginFollowingLagTimer = false;
	bool fFollowPlayer = false;
	sf::Time fFollowingLagDelay = sf::seconds(0.5f);
	sf::Time fFollowingLagAcc = sf::Time::Zero;

	void followPlayer(sf::Time dt);
	/****************/

	void breakingBoxStateUpdate(sf::Time dt);
	void jailInKanalisationUpdate(sf::Time dt);
	void beginningAtBossUpdate(sf::Time dt, CommandQueue& commands);
	void endingAtBossUpdate(sf::Time dt);

	/*sit if nothing happens*/
	bool fBeginWaitingToSitTimer = false;
	sf::Time fWaitingToSitDelay = sf::seconds(4.f);
	sf::Time fWaitingToSitAcc = sf::Time::Zero;
	/***********************/

	void setupAnims();
};