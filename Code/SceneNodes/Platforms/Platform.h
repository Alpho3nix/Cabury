#pragma once
#include "Block.h"
#include "SceneNode.h"
#include <array>

class Platform : public SceneNode
{
public:
	//the width is the width of the platform and if the height is higher than 1, then it's the higher type of platform
	Platform(TexturesHolder& textures, int width, int height = 1, bool drawPlat = true);

	unsigned int getCategory() const override;
	sf::FloatRect getBoundingRect() const override;
	bool isRemovable() const override;
	bool isDestroyed() const override;

	void updateNode(sf::Time dt, CommandQueue& commands) override;
	sf::FloatRect getSize() const;

private:
	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

	std::vector<Block> fBlocks;
	bool fDrawPlatform;
};