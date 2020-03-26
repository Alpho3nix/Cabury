#pragma once
#include "SceneNode.h"

class HitBox : public SceneNode
{
public:

	HitBox(sf::FloatRect pos_size);

	unsigned int getCategory() const override;

	sf::FloatRect getBoundingRect() const override;

	bool isRemovable() const override;
	bool isDestroyed() const override;

	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	void updateNode(sf::Time dt, CommandQueue& commands);

	void setBoundingRect(sf::FloatRect hitBox);
	std::function<void()> fPlayerEntering;

private:

	sf::FloatRect fHitBox;
};