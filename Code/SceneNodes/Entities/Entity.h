#pragma once
#include "Damageable.h"
#include "Animation.h"


class Entity : public Damageable
{
public:
	
	enum LookingState
	{
		LookingRight, LookingLeft
	};

protected:

	/*Convention: left looking animation is odd, right looking animation is pair*/
	enum AnimType
	{
		Idle, IdleLeft, Idle2, Idle2Left, Run, RunLeft, Jump, JumpLeft, RolledDash, RolledDashLeft, beingHit, beingHitLeft,
		Crouch, CrouchLeft, Slide, SlideLeft, Attack1, Attack1_left, Attack2, Attack2_Left, Attack3, Attack3_Left, Death, DeathLeft,
		JumpReady, JumpReadyLeft, Fall, FallLeft, ClimbLadder, ClimbLadderLeft, Transition, TransitionLeft, Walk, WalkLeft, Howling, HowlingLeft, Biting, BitingLeft,
		IdleWithoutBook, IdleWithoutBookLeft, Idle2WithoutBook, Idle2WithoutBookLeft,  AnimCount
	};

	int HealthPoints;

	std::vector<Animation*> fAnimations;
	size_t CurrentAnimation;

	AnimType IdleType = Idle;
	AnimType RunLeftType = RunLeft;

	sf::Sprite fSprite;
	bool fAnimationFinished = false;

	void updateAnimations();
	void setLookingState(LookingState state);
	bool fBeginGravityTimer = false;

public:

	bool isLookingLeft() const;
	void doesGravitiyApply(bool flag);
	bool isGravityEnable() const;
	bool isAnimationEnable() const;
	void enableAnimation(bool flag);
	
private:

	bool fAnimationEnable = true;

	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	size_t stateAnimationBuffer;

	sf::Vector2f velocity = sf::Vector2f(0.f, 0.f);
	float UpwardsVelocity = 0.f;

protected:
	LookingState fLookingState = LookingRight;
	bool fCanGoUnderGround = false;

private:
	bool SteppingOnSth = false;

	sf::Time GravityAccu = sf::Time::Zero;
	bool fGravityEnable = true;

	void UpdateAndDrawAnimations(sf::Time dt, CommandQueue& commands);
	void Gravity(sf::Time dt, CommandQueue& commands);

	sf::Vector2f fLastFramePos = sf::Vector2f();
public:
	Entity(int hp);

	virtual unsigned int getCategory() const override;
	size_t getHealth() const;
	virtual void updateNode(sf::Time dt, CommandQueue& commands) override;

	void setCurrentAnimation(size_t num);
	void goUnderground(bool flag);
	bool canGoUnderground() const;
	
	virtual sf::FloatRect getBoundingRect() const override;
	sf::FloatRect getSpriteBoundingRect() const;

	void setUpwardsVel(float vel) { UpwardsVelocity = vel; }
	float getUpwardsVel() const { return UpwardsVelocity; }
	void setTextureRect(sf::IntRect texRect);

	virtual bool isRemovable() const override;
	virtual bool isDestroyed() const override;
	virtual bool applyDamage(int amount);

	sf::Vector2f getLastFramePos() const;

	void setVelocity(sf::Vector2f newVel);
	sf::Vector2f getVelocity() const;
	void setVelocity(float x, float y);
	void setSteppingOnSth(bool flag);
	bool isSteppingOnSth();

	float fFloorLevel = 450.f;

	virtual ~Entity() override;
};

