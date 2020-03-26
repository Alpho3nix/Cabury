#include "Entity.h"
#include <iostream>
#include "UtilityFunc.h"

void Entity::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(fSprite, states);
}

bool Entity::isLookingLeft() const
{
	return fLookingState;
}

void Entity::doesGravitiyApply(bool flag)
{
	fGravityEnable = flag;
}

bool Entity::isGravityEnable() const
{
	return fGravityEnable;
}

bool Entity::isAnimationEnable() const
{
	return fAnimationEnable;
}

void Entity::enableAnimation(bool flag)
{
	fAnimationEnable = flag;
}

void Entity::updateAnimations()
{
	assert(fAnimations[CurrentAnimation]);
	assert(fAnimations[CurrentAnimation + fLookingState]);
	if (CurrentAnimation + fLookingState != stateAnimationBuffer)	//we changed the animation
	{

		stateAnimationBuffer = CurrentAnimation + fLookingState;
		fAnimations[CurrentAnimation + fLookingState]->Restart();
		
		fAnimationFinished = false;
	}
	else
	{
		if (fAnimations[CurrentAnimation + fLookingState]->isFinished()) {

			CurrentAnimation = IdleType;
			fAnimations[CurrentAnimation + fLookingState]->Restart();
			fAnimationFinished = true;
		}
		else
		{
			fAnimationFinished = false;
		}
	}

	
}

void Entity::setLookingState(LookingState state)
{
	fLookingState = state;
}

void Entity::UpdateAndDrawAnimations(sf::Time dt, CommandQueue & commands)
{
	updateAnimations();
	
	if (fAnimations[CurrentAnimation + fLookingState]) {
		fAnimations[CurrentAnimation + fLookingState]->update(dt, fSprite);
	}
	
}

void Entity::Gravity(sf::Time dt, CommandQueue & commands)
{
	velocity = sf::Vector2f(velocity.x, 9.81f * GravityAccu.asSeconds() + UpwardsVelocity);

	if (!isSteppingOnSth() || fBeginGravityTimer)
	{
		GravityAccu += dt;
		if (!isSteppingOnSth())
			fBeginGravityTimer = false;
	}
	else
	{
		GravityAccu = sf::Time::Zero;
		UpwardsVelocity = 0.f;
	}

}

Entity::Entity(int hp)
	:HealthPoints(hp), fAnimations(AnimCount), fSprite(), CurrentAnimation(0), stateAnimationBuffer(0)
{
}

unsigned int Entity::getCategory() const
{
	return Category::Scene;
}

size_t Entity::getHealth() const
{
	return HealthPoints;
}

void Entity::updateNode(sf::Time dt, CommandQueue& commands)
{
	if (fAnimationEnable) {
		if (velocity.x < 0 || (velocity.x == 0.f && fLookingState == LookingLeft))
		{
			fLookingState = LookingLeft;
		}
		else
		{
			fLookingState = LookingRight;
		}


		UpdateAndDrawAnimations(dt, commands);
	}
	if (fGravityEnable) {
		Gravity(dt, commands);
	}

	fLastFramePos = getWorldPosition();
	move(velocity);
	
}

void Entity::setCurrentAnimation(size_t num)
{
	CurrentAnimation = num;
}

void Entity::goUnderground(bool flag)
{
	fCanGoUnderGround = flag;
}

bool Entity::canGoUnderground() const
{
	return fCanGoUnderGround;
}

sf::FloatRect Entity::getBoundingRect() const
{
	return getWorldTransform().transformRect(fSprite.getGlobalBounds());
}

sf::FloatRect Entity::getSpriteBoundingRect() const
{
	return getWorldTransform().transformRect(fSprite.getGlobalBounds());
}

Entity::~Entity()
{
	SceneNode::~SceneNode();
	for (Animation* anim : fAnimations)
	{
		if (anim) {
			delete anim;
		}
	}
}

void Entity::setTextureRect(sf::IntRect texRect)
{
	fSprite.setTextureRect(texRect);
	Utility::CenterOrigin(fSprite);
}

bool Entity::isRemovable() const
{
	return isDestroyed();
}

bool Entity::isDestroyed() const
{
	return HealthPoints == 0;
}

bool Entity::applyDamage(int amount)
{
	HealthPoints -= amount;
	return true;
}

sf::Vector2f Entity::getLastFramePos() const
{
	return fLastFramePos;
}

void Entity::setVelocity(sf::Vector2f newVel)
{
	velocity = newVel;
}

sf::Vector2f Entity::getVelocity() const
{
	return velocity;
}

void Entity::setVelocity(float x, float y)
{
	velocity = sf::Vector2f(x, y);
}

void Entity::setSteppingOnSth(bool flag)
{
	SteppingOnSth = flag;
}

bool Entity::isSteppingOnSth()
{
	if (getWorldPosition().y + getSpriteBoundingRect().height / 2.f >= fFloorLevel && !fCanGoUnderGround)
	{
		
		setPosition(getWorldPosition().x, fFloorLevel - fSprite.getTextureRect().height / 2.f);
		return true;
	}
	else {
		return SteppingOnSth;
	}
}
