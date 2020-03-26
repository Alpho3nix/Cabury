#include "Platform.h"

Platform::Platform(TexturesHolder & textures, int width, int height, bool drawPlat)
	:fBlocks(), fDrawPlatform(drawPlat)
{
	if (width >= 3) {
		fBlocks.push_back(Block(textures, Block::Type::BeginBlock, 1, height));
		fBlocks[0].setPosition(0, 0);
		fBlocks.push_back(Block(textures, Block::Type::BetweenBlock, width - 2, height));
		fBlocks[1].setPosition(fBlocks[0].getSize().width, 0.f);
		fBlocks.push_back(Block(textures, Block::Type::EndBlock, 1, height));
		fBlocks[2].setPosition(fBlocks[0].getSize().width + fBlocks[1].getSize().width, 0);
	}
	else if (width == 2)
	{
		fBlocks.push_back(Block(textures, Block::Type::BeginBlock, 1, height));
		fBlocks[0].setPosition(0, 0);
		fBlocks.push_back(Block(textures, Block::Type::EndBlock, 1, height));
		fBlocks[1].setPosition(fBlocks[0].getSize().width, 0);
	}
	else
	{
		fBlocks.push_back(Block(textures, Block::Type::SoloBlock, 1, height));
		fBlocks[0].setPosition(0, 0);
	}
}

unsigned int Platform::getCategory() const
{
	return Category::platform;
}

sf::FloatRect Platform::getBoundingRect() const
{
	return sf::FloatRect(getWorldPosition().x, getWorldPosition().y, getSize().width, getSize().height);
}

bool Platform::isRemovable() const
{
	return false;
}

bool Platform::isDestroyed() const
{
	return false;
}

void Platform::updateNode(sf::Time dt, CommandQueue & commands)
{
}

sf::FloatRect Platform::getSize() const
{
	float size = 0.f;
	for (int i = 0; i < fBlocks.size(); ++i)
	{
		size += fBlocks[i].getSize().width;
	}
	return sf::FloatRect(0, 0, size, fBlocks[0].getSize().height);
}

void Platform::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (fDrawPlatform) {
		for (int i = 0; i < fBlocks.size(); ++i)
		{
			target.draw(fBlocks[i], states);
		}
	}
}
