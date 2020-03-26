#include "Cabury.h"
#include "UtilityFunc.h"
#include "Player.h"

Cabury::Cabury(int hp, TexturesHolder& textures, Player& player, CinematicState state)
	:Entity(hp), fPlayer(player), fCurrentCinematicState(state)
{
	fSprite.setTexture(textures.get(Textures::ID::Cabury));
	fSprite.setTextureRect(sf::IntRect(0, 705, 64, 64));
	setupAnims();
	Utility::CenterOrigin(fSprite);
	setCurrentAnimation(Idle);

	if (state == CinematicState::JailInKanalisation)
	{
		setLookingState(Entity::LookingLeft);
	}
}

unsigned int Cabury::getCategory() const
{
	return Category::cabury;
}

sf::FloatRect Cabury::getBoundingRect() const
{
	return Entity::getBoundingRect();
}

void Cabury::updateNode(sf::Time dt, CommandQueue& commands)
{
	Entity::updateNode(dt, commands);
	if (fIsDead)
	{
		
		fIsRemovable = true;
		
	}
	else
	{
		
		switch (fCurrentCinematicState)
		{
		case Cabury::CinematicState::BreakingBox:
			breakingBoxStateUpdate(dt);
			break;
		case Cabury::CinematicState::JailInKanalisation:
			jailInKanalisationUpdate(dt);
			break;
		case Cabury::CinematicState::BeginningAtBoss:
			beginningAtBossUpdate(dt, commands);
			break;
		case Cabury::CinematicState::EndingAtBoss:
			endingAtBossUpdate(dt);
			break;
		default:
			break;
		}
	}

	Utility::CenterOrigin(fSprite);
}


bool Cabury::isRemovable() const
{
	return fIsRemovable;
}

bool Cabury::isDestroyed() const
{
	return fIsDead;
}

bool Cabury::applyDamage(int amount)
{
	return false;
}

void Cabury::kill()
{
	fIsDead = true;
}

Cabury::CinematicState Cabury::getCinematicState() const
{
	return fCurrentCinematicState;
}

void Cabury::setCinematicState(CinematicState state)
{
	fCurrentCinematicState = state;
}

void Cabury::cageBroken()
{
	fCageBroken = true;
}

void Cabury::followPlayerBehavior(sf::Time dt)
{
	followPlayer(dt);

	if (abs(getVelocity().x) > 0.f)
	{
		if (CurrentAnimation == Idle2 || CurrentAnimation == Transition)
		{
			fBeginWaitingToSitTimer = false;
			fWaitingToSitAcc = sf::Time::Zero;
			if (CurrentAnimation == Idle2)
			{
				fAnimations[Transition + fLookingState]->setStartPoint(sf::Vector2i(fAnimations[Transition + fLookingState]->getStartPoint().x + (fAnimations[Transition + fLookingState]->getNumFrames() - 1) * 64, fAnimations[Transition + fLookingState]->getStartPoint().y));
				fAnimations[Transition + fLookingState]->setReadingDirection(-1);
				fAnimations[Transition + fLookingState]->Restart();
			}
			setCurrentAnimation(Transition);
			if (fAnimations[Transition + fLookingState]->isFinished())
			{
				setCurrentAnimation(Run);
				fAnimations[Transition + fLookingState]->setStartPoint(sf::Vector2i(6 * 64, fAnimations[Transition + fLookingState]->getStartPoint().y));
				fAnimations[Transition + fLookingState]->setReadingDirection(1);
			}
			else
			{

				setVelocity(0.f, 0.f);
			}
		}
		else {
			setCurrentAnimation(AnimType::Run);
			fBeginWaitingToSitTimer = false;
			fWaitingToSitAcc = sf::Time::Zero;
			fAnimations[Transition + fLookingState]->Restart();
			fAnimations[Transition + fLookingState]->setStartPoint(sf::Vector2i(6 * 64, fAnimations[Transition + fLookingState]->getStartPoint().y));
			fAnimations[Transition + fLookingState]->setReadingDirection(1);
		}
	}
	else
	{
		setCurrentAnimation(AnimType::Idle);
		fBeginWaitingToSitTimer = true;
	}

	if (fBeginWaitingToSitTimer)
	{
		fWaitingToSitAcc += dt;
		if (fWaitingToSitAcc >= fWaitingToSitDelay)
		{
			setCurrentAnimation(Transition);
		}

		if (fAnimations[CurrentAnimation + fLookingState]->isFinished())
		{
			setCurrentAnimation(Idle2);
		}

	}
}

void Cabury::followPlayer(sf::Time dt)
{
	float limit = 100.f;
	float x_dist = fPlayer.getWorldPosition().x - getWorldPosition().x;
	int dir = x_dist / abs(x_dist);
	float x_dist_late = fPlayer.getLastFramePos().x - getWorldPosition().x;
	if (abs(x_dist) >= limit && !fBeginFollowingLagTimer && abs(x_dist_late) < limit)
	{
		fBeginFollowingLagTimer = true;
	}
	else if (abs(x_dist) < limit)
	{
		setVelocity(0.f, getVelocity().y);
		fFollowPlayer = false;
	}
	if (fBeginFollowingLagTimer)
	{
		fFollowingLagAcc += dt;
		if (fFollowingLagAcc >= fFollowingLagDelay)
		{
			fFollowingLagAcc = sf::Time::Zero;
			fBeginFollowingLagTimer = false;
			fFollowPlayer = true;
		}
	}

	if (fFollowPlayer)
	{
		setVelocity(100.f * dt.asSeconds() * dir, 0.f);
	}
}

void Cabury::breakingBoxStateUpdate(sf::Time dt)
{
	setVelocity(100.f * dt.asSeconds(), 0.f);
	setCurrentAnimation(Run);
	if (getWorldPosition().x - getBoundingRect().width / 2.f > WIDTH)
	{
		fIsDead = true;
	}
}

void Cabury::jailInKanalisationUpdate(sf::Time dt)
{
	if (!fCageBroken) {
		setCurrentAnimation(Idle2);
	}
	else
	{
		if (CurrentAnimation != Run)
		{
			if (CurrentAnimation != Transition)
			{
				fAnimations[Transition + fLookingState]->setStartPoint(sf::Vector2i(fAnimations[Transition + fLookingState]->getStartPoint().x + (fAnimations[Transition + fLookingState]->getNumFrames() - 1) * 64, fAnimations[Transition + fLookingState]->getStartPoint().y));
				fAnimations[Transition + fLookingState]->setReadingDirection(-1);
				fAnimations[Transition + fLookingState]->Restart();
			}
			setCurrentAnimation(AnimType::Transition);

			if (fAnimations[CurrentAnimation + fLookingState]->isFinished())
			{
				setCurrentAnimation(AnimType::Run);
				setVelocity(100.f * dt.asSeconds(), 0.f);
				fAnimations[Transition + fLookingState]->setStartPoint(sf::Vector2i(6 * 64, fAnimations[Transition + fLookingState]->getStartPoint().y));
				fAnimations[Transition + fLookingState]->setReadingDirection(1);
			}
		}
		else
		{
			if (getWorldPosition().x - getSpriteBoundingRect().width / 2.f >= 4 * WIDTH)
			{
				fPlayer.ChangeToBoss(true);
				fIsRemovable = true;
			}
		}
	}
}

void Cabury::beginningAtBossUpdate(sf::Time dt, CommandQueue& commands)
{
	if (CurrentAnimation != Run)
	{
		if (CurrentAnimation != Transition)
		{
			fAnimations[Transition + fLookingState]->setStartPoint(sf::Vector2i(fAnimations[Transition + fLookingState]->getStartPoint().x + (fAnimations[Transition + fLookingState]->getNumFrames() - 1) * 64, fAnimations[Transition + fLookingState]->getStartPoint().y));
			fAnimations[Transition + fLookingState]->setReadingDirection(-1);
			fAnimations[Transition + fLookingState]->Restart();
		}
		setCurrentAnimation(AnimType::Transition);

		if (fAnimations[CurrentAnimation + fLookingState]->isFinished())
		{
			setCurrentAnimation(AnimType::Run);
			setVelocity(100.f * dt.asSeconds(), 0.f);
			fAnimations[Transition + fLookingState]->setStartPoint(sf::Vector2i(6 * 64, fAnimations[Transition + fLookingState]->getStartPoint().y));
			fAnimations[Transition + fLookingState]->setReadingDirection(1);
		}
	}
	else
	{
		if (getWorldPosition().x - getSpriteBoundingRect().width / 2.f >= WIDTH)
		{
			fPlayer.setCinematicState(Player::CinematicState::Null);
			setVelocity(0.f, 0.f);
		}
	}
}

void Cabury::endingAtBossUpdate(sf::Time dt)
{
	if (getWorldPosition().x >= WIDTH - 100.f)
	{
		setCurrentAnimation(Run);
		setVelocity(-100.f * dt.asSeconds(), 0.f);
	}
	else
	{

		setVelocity(0.f, 0.f);
		if (CurrentAnimation != Idle2 && !fNoBook) {
			setCurrentAnimation(Transition);
			if (fAnimations[CurrentAnimation + fLookingState]->isFinished())
			{
				setCurrentAnimation(Idle2);
			}
		}
		else
		{
			if (Utility::getDistance(fPlayer.getWorldPosition(), getWorldPosition()) <= 75.f && !fNoBook)
			{
				fPlayer.setCinematicState(Player::CinematicState::MinotaurEnd);
				setCurrentAnimation(Idle2WithoutBook);
				fNoBook = true;
			}
		}
	}
}

void Cabury::setupAnims()
{
	const sf::Texture& tex = *fSprite.getTexture();
	sf::Vector2i frameSize(64, 64);

	/**********With the book**********/

	//Idle
	Animation* idle = new Animation(tex, sf::Vector2i(2 * frameSize.x, 641), -1);
	idle->Repeat(true);
	idle->setDuration(sf::seconds(0.5f));
	idle->setFrameSize(frameSize);
	idle->setNumFrames(3);
	idle->Restart();

	fAnimations[AnimType::Idle] = idle;

	//Idle facing left
	Animation* idleLeft = new Animation(tex, sf::Vector2i(2 * frameSize.x, 961), -1);
	idleLeft->Repeat(true);
	idleLeft->setDuration(sf::seconds(0.5f));
	idleLeft->setFrameSize(frameSize);
	idleLeft->setNumFrames(3);
	idleLeft->Restart();

	fAnimations[AnimType::IdleLeft] = idleLeft;

	//running 
	Animation* running = new Animation(tex, sf::Vector2i(4 * frameSize.x, 705), -1);
	running->Repeat(true);
	running->setDuration(sf::seconds(0.7f));
	running->setFrameSize(frameSize);
	running->setNumFrames(5);
	running->Restart();

	fAnimations[AnimType::Run] = running;
	//running facing left
	Animation* runningLeft = new Animation(tex, sf::Vector2i(4 * frameSize.x, 1025), -1);
	runningLeft->Repeat(true);
	runningLeft->setDuration(sf::seconds(0.7f));
	runningLeft->setFrameSize(frameSize);
	runningLeft->setNumFrames(5);
	runningLeft->Restart();

	fAnimations[AnimType::RunLeft] = runningLeft;

	//sittedLoop
	Animation* sittedLoop = new Animation(tex, sf::Vector2i(0, 769), 1);
	sittedLoop->Repeat(true);
	sittedLoop->setDuration(sf::seconds(0.5f));
	sittedLoop->setFrameSize(frameSize);
	sittedLoop->setNumFrames(3);
	sittedLoop->Restart();

	fAnimations[AnimType::Idle2] = sittedLoop;

	//sittedLoop facing left
	Animation* sittedLoopLeft = new Animation(tex, sf::Vector2i(0, 1089), 1);
	sittedLoopLeft->Repeat(true);
	sittedLoopLeft->setDuration(sf::seconds(0.5f));
	sittedLoopLeft->setFrameSize(frameSize);
	sittedLoopLeft->setNumFrames(3);
	sittedLoopLeft->Restart();

	fAnimations[AnimType::Idle2Left] = sittedLoopLeft;

	//running to sitted transition
	Animation* runToSit = new Animation(tex, sf::Vector2i(6 * frameSize.x, 833), 1);
	runToSit->Repeat(false);
	runToSit->setDuration(sf::seconds(1.5f));
	runToSit->setFrameSize(frameSize);
	runToSit->setNumFrames(12);
	runToSit->Restart();

	fAnimations[AnimType::Transition] = runToSit;

	//running to sitted transition facing left
	Animation* runToSitLeft = new Animation(tex, sf::Vector2i(6 * frameSize.x, 1153), 1);
	runToSitLeft->Repeat(false);
	runToSitLeft->setDuration(sf::seconds(1.5f));
	runToSitLeft->setFrameSize(frameSize);
	runToSitLeft->setNumFrames(12);
	runToSitLeft->Restart();

	fAnimations[AnimType::TransitionLeft] = runToSitLeft;

	//turn around attack
	Animation* turnAttack = new Animation(tex, sf::Vector2i(0, 897), 1);
	turnAttack->Repeat(false);
	turnAttack->setDuration(sf::seconds(1.5f));
	turnAttack->setFrameSize(frameSize);
	turnAttack->setNumFrames(17);
	turnAttack->Restart();

	fAnimations[AnimType::Attack1] = turnAttack;

	//turn around attack facing left
	Animation* turnAttackLeft = new Animation(tex, sf::Vector2i(0, 1217), 1);
	turnAttackLeft->Repeat(false);
	turnAttackLeft->setDuration(sf::seconds(1.5f));
	turnAttackLeft->setFrameSize(frameSize);
	turnAttackLeft->setNumFrames(17);
	turnAttackLeft->Restart();

	fAnimations[AnimType::Attack1_left] = turnAttackLeft;

	/********************************/

	/**********Without the book**********/

	//Idle
	Animation* idleWB = new Animation(tex, sf::Vector2i(2 * frameSize.x, 0), -1);
	idleWB->Repeat(true);
	idleWB->setDuration(sf::seconds(0.5f));
	idleWB->setFrameSize(frameSize);
	idleWB->setNumFrames(3);
	idleWB->Restart();

	fAnimations[AnimType::IdleWithoutBook] = idleWB;

	//Idle facing left
	Animation* idleLeftWB = new Animation(tex, sf::Vector2i(2 * frameSize.x, 321), -1);
	idleLeftWB->Repeat(true);
	idleLeftWB->setDuration(sf::seconds(0.5f));
	idleLeftWB->setFrameSize(frameSize);
	idleLeftWB->setNumFrames(3);
	idleLeftWB->Restart();

	fAnimations[AnimType::IdleWithoutBookLeft] = idleLeftWB;

	////running 
	//Animation* running = new Animation(tex, sf::Vector2i(4 * frameSize.x, 705), -1);
	//running->Repeat(true);
	//running->setDuration(sf::seconds(0.7f));
	//running->setFrameSize(frameSize);
	//running->setNumFrames(5);
	//running->Restart();

	//fAnimations[AnimType::Run] = running;
	////running facing left
	//Animation* runningLeft = new Animation(tex, sf::Vector2i(4 * frameSize.x, 1025), -1);
	//runningLeft->Repeat(true);
	//runningLeft->setDuration(sf::seconds(0.7f));
	//runningLeft->setFrameSize(frameSize);
	//runningLeft->setNumFrames(5);
	//runningLeft->Restart();

	//fAnimations[AnimType::RunLeft] = runningLeft;

	//sittedLoop
	Animation* sittedLoopWB = new Animation(tex, sf::Vector2i(0, 129), 1);
	sittedLoopWB->Repeat(true);
	sittedLoopWB->setDuration(sf::seconds(0.5f));
	sittedLoopWB->setFrameSize(frameSize);
	sittedLoopWB->setNumFrames(3);
	sittedLoopWB->Restart();

	fAnimations[AnimType::Idle2WithoutBook] = sittedLoopWB;

	//sittedLoop facing left
	Animation* sittedLoopLeftWB = new Animation(tex, sf::Vector2i(0, 449), 1);
	sittedLoopLeftWB->Repeat(true);
	sittedLoopLeftWB->setDuration(sf::seconds(0.5f));
	sittedLoopLeftWB->setFrameSize(frameSize);
	sittedLoopLeftWB->setNumFrames(3);
	sittedLoopLeftWB->Restart();

	fAnimations[AnimType::Idle2WithoutBookLeft] = sittedLoopLeftWB;

	////running to sitted transition
	//Animation* runToSit = new Animation(tex, sf::Vector2i(6 * frameSize.x, 833), 1);
	//runToSit->Repeat(false);
	//runToSit->setDuration(sf::seconds(1.5f));
	//runToSit->setFrameSize(frameSize);
	//runToSit->setNumFrames(12);
	//runToSit->Restart();

	//fAnimations[AnimType::Transition] = runToSit;

	////running to sitted transition facing left
	//Animation* runToSitLeft = new Animation(tex, sf::Vector2i(6 * frameSize.x, 1153), 1);
	//runToSitLeft->Repeat(false);
	//runToSitLeft->setDuration(sf::seconds(1.5f));
	//runToSitLeft->setFrameSize(frameSize);
	//runToSitLeft->setNumFrames(12);
	//runToSitLeft->Restart();

	//fAnimations[AnimType::TransitionLeft] = runToSitLeft;

	////turn around attack
	//Animation* turnAttack = new Animation(tex, sf::Vector2i(0, 897), 1);
	//turnAttack->Repeat(false);
	//turnAttack->setDuration(sf::seconds(1.5f));
	//turnAttack->setFrameSize(frameSize);
	//turnAttack->setNumFrames(17);
	//turnAttack->Restart();

	//fAnimations[AnimType::Attack1] = turnAttack;

	////turn around attack facing left
	//Animation* turnAttackLeft = new Animation(tex, sf::Vector2i(0, 1217), 1);
	//turnAttackLeft->Repeat(false);
	//turnAttackLeft->setDuration(sf::seconds(1.5f));
	//turnAttackLeft->setFrameSize(frameSize);
	//turnAttackLeft->setNumFrames(17);
	//turnAttackLeft->Restart();

	//fAnimations[AnimType::Attack1_left] = turnAttackLeft;

	/********************************/
}
