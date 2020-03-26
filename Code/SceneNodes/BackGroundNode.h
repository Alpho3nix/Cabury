#pragma once
#include "SceneNode.h"
#include "ResourceIdentifiers.h"

class BackGroundNode : public SceneNode
{
public:
	explicit BackGroundNode(const sf::Texture& texture, sf::IntRect texCoord);
	explicit BackGroundNode(const sf::Texture& texture);

	unsigned int getCategory() const override;
	sf::FloatRect getBoundingRect() const override;
private:
	sf::Sprite fSprite;

	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
};

