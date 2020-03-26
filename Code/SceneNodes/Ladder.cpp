#include "Ladder.h"

Ladder::Ladder(Type type, TexturesHolder& textures, int height)
	:fLadderHeight(height), fSprite()
{
	handleType(type, textures);
}

unsigned int Ladder::getCategory() const
{
	return Category::ladder;
}

bool Ladder::isDestroyed() const
{
	return false;
}

bool Ladder::isRemovable() const
{
	return false;
}

sf::FloatRect Ladder::getBoundingRect() const
{
	return getWorldTransform().transformRect(fSprite.getGlobalBounds());
}

void Ladder::updateNode(sf::Time dt, CommandQueue& commands)
{
}

sf::Vector2f Ladder::getFloorPosition() const
{
	return sf::Vector2f(getWorldPosition().x, getWorldPosition().y + fSprite.getGlobalBounds().height);
}

void Ladder::setFloorPosition(sf::Vector2f newPos)
{
	setPosition(newPos.x, newPos.y - fSprite.getGlobalBounds().height);
}

void Ladder::setFloorPosition(float x, float y)
{
	setPosition(x, y - fSprite.getGlobalBounds().height);
}

void Ladder::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(fSprite, states);
}

void Ladder::handleType(Type type, TexturesHolder& textures)
{
	switch (type)
	{
	case Ladder::NormalLadder:
	{
		sf::Texture& tex = textures.get(Textures::ID::Ladder_Normal);

		if (fLadderHeight > 1)
		{
			tex.setRepeated(true);
			fSprite.setTexture(tex);
			fSprite.setTextureRect(sf::IntRect(0, 0, tex.getSize().x, fLadderHeight * tex.getSize().y));
		}
		else
		{
			fSprite.setTexture(tex);
		}
	}
		break;
	case Ladder::MossyLadder:
	{
		sf::Texture& tex = textures.get(Textures::ID::Ladder_Mossy);

		if (fLadderHeight > 1)
		{
			tex.setRepeated(true);
			fSprite.setTexture(tex);
			fSprite.setTextureRect(sf::IntRect(0, 0, tex.getSize().x, fLadderHeight * tex.getSize().y));
		}
		else
		{
			fSprite.setTexture(tex);
		}
	}
		break;
	default:
		throw std::logic_error("the Ladder type provided doesn't exist");
		break;
	}
}
