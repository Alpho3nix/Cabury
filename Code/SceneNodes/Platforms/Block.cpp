#include "Block.h"
#include "UtilityFunc.h"

Block::Block(TexturesHolder& textures, Type type, int width, int height)
{
	if (width < 1 || height < 1)
	{
		throw std::runtime_error("Can not create a block with a height or a width smaller than 1");
	}

	if (width != 1)
	{
		sf::Texture& tex = (height == 1) ? textures.get(Textures::ID::RepeatedBlock_Small) : textures.get(Textures::ID::RepeatedBlock_Long);

		int rect_width = tex.getSize().x;

		tex.setRepeated(true);
		fVisualBlock.setTexture(tex);
		fVisualBlock.setTextureRect(sf::IntRect(0, 0, width * rect_width, tex.getSize().y));
		
	}
	else {
		sf::Texture& tex = textures.get(Textures::ID::Block);

		int rect_y = 0;
		int rect_x = 0;
		int rect_width = 0;
		int rect_height = 90;

		if (height == 1)
		{
			rect_y = 95;
			rect_height = 27;
		}

		switch (type)
		{
		case Block::Type::BeginBlock:
			rect_x = 0;
			rect_width = 33;
			break;
		case Block::Type::BetweenBlock:
			rect_x = 33;
			rect_width = 32;
			break;
		case Block::Type::EndBlock:
			rect_x = 65;
			rect_width = 30;
			break;
		case Block::Type::SoloBlock:
			rect_x = 95;
			rect_width = 33;
			break;
		default:
			break;
		}
		
		fVisualBlock.setTexture(tex);
		fVisualBlock.setTextureRect(sf::IntRect(rect_x, rect_y, rect_width, rect_height));
	}
	//Utility::CenterOrigin(fVisualBlock);
}

Block::Block()
	:fVisualBlock()
{
}

sf::IntRect Block::getSize() const
{
	return fVisualBlock.getTextureRect();
}


void Block::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(fVisualBlock, states);
}
