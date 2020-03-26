#pragma once
#include "SceneNode.h"
#include "ResourceIdentifiers.h"

class Ladder : public SceneNode
{
public:

	enum Type
	{
		NormalLadder, MossyLadder
	};

	Ladder(Type type, TexturesHolder& textures, int height = 1);

	unsigned int getCategory() const override;
	bool isDestroyed() const override;
	bool isRemovable() const override;
	sf::FloatRect getBoundingRect() const override;

	void updateNode(sf::Time dt, CommandQueue& commands) override;
	
	sf::Vector2f getFloorPosition() const;
	void setFloorPosition(sf::Vector2f newPos);
	void setFloorPosition(float x, float y);

private:

	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::Sprite fSprite;
	int fLadderHeight;
	void handleType(Type type, TexturesHolder& textures);

};
