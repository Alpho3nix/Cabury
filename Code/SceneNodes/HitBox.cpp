#include "HitBox.h"
#include <iostream>

HitBox::HitBox(sf::FloatRect pos_size)
	:fPlayerEntering(), fHitBox(pos_size)
{
}

unsigned int HitBox::getCategory() const
{
	return Category::hitBoxes;
}

sf::FloatRect HitBox::getBoundingRect() const
{
	return getWorldTransform().transformRect(fHitBox);
}

bool HitBox::isRemovable() const
{
	return false;
}

bool HitBox::isDestroyed() const
{
	return false;
}

void HitBox::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
}

void HitBox::updateNode(sf::Time dt, CommandQueue& commands)
{
}

void HitBox::setBoundingRect(sf::FloatRect hitBox)
{
	fHitBox = hitBox;
}
