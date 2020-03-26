#pragma once
#include "ResourceIdentifiers.h"
#include "SceneNode.h"
#include "Animation.h"

class MagicBook : public SceneNode
{
public:
	MagicBook(const TexturesHolder& textures);

	unsigned int getCategory() const override;
	sf::FloatRect getBoundingRect() const override;

	bool isRemovable() const override;
	bool isDestroyed() const override;

	void updateNode(sf::Time dt, CommandQueue& commands) override;
	void remove();

	~MagicBook() override;
private:

	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::Sprite fSprite;

	Animation* fAnimation = nullptr;	//TODO : make this an array with 3 animations instead of one
	bool fRemovable = false;
};