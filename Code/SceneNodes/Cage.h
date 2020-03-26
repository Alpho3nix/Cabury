#pragma once
#include "SceneNode.h"
#include "ResourceIdentifiers.h"
#include "Animation.h"

class Cage : public SceneNode
{
public:
	Cage(TexturesHolder& textures);

	unsigned int getCategory() const override;
	sf::FloatRect getBoundingRect() const override;

	bool isRemovable() const override;
	bool isDestroyed() const override;

	void destroy();

	void setFloorPosition(float x, float y);
	void setFloorPosition(sf::Vector2f pos);
private:

	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	void updateNode(sf::Time dt, CommandQueue& commands) override;

	sf::Sprite fSprite;
	Animation fDestroyedAnim;

	bool fDestroyed = false;
	bool fRemovable = false;
};