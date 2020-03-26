#include "BackGroundNode.h"
#include <iostream>

BackGroundNode::BackGroundNode(const sf::Texture & texture, sf::IntRect texCoord)
	:fSprite(texture, texCoord)
{
}

BackGroundNode::BackGroundNode(const sf::Texture& texture)
	:fSprite(texture)
{
}

unsigned int BackGroundNode::getCategory() const
{
	return Category::None;
}

sf::FloatRect BackGroundNode::getBoundingRect() const
{
	return getWorldTransform().transformRect(fSprite.getGlobalBounds());
}

void BackGroundNode::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(fSprite, states);
}
