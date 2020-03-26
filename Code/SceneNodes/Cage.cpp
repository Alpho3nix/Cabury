#include "Cage.h"

Cage::Cage(TexturesHolder& textures)
	:fSprite(textures.get(Textures::ID::Cage)), fDestroyedAnim(textures.get(Textures::ID::Cage), sf::Vector2i(0, 0), 1)
{
	fSprite.setTextureRect(sf::IntRect(0, 0, 200, 160));

	fDestroyedAnim.setDuration(sf::seconds(0.8f));
	fDestroyedAnim.Repeat(false);
	fDestroyedAnim.setFrameSize(sf::Vector2i(200, 160));
	fDestroyedAnim.setNumFrames(4);
	fDestroyedAnim.Restart();
}

unsigned int Cage::getCategory() const
{
	return Category::cage;
}

sf::FloatRect Cage::getBoundingRect() const
{
	sf::FloatRect rect = getTransform().transformRect(fSprite.getGlobalBounds());
	return sf::FloatRect(rect.left + 37, rect.top + 22, 125, 115);
}

bool Cage::isRemovable() const
{
	return fRemovable;
}

bool Cage::isDestroyed() const
{
	return fDestroyed;
}

void Cage::destroy()
{
	fDestroyed = true;
}

void Cage::setFloorPosition(float x, float y)
{
	setPosition(sf::Vector2f(x - 37, y - 137));
}

void Cage::setFloorPosition(sf::Vector2f pos)
{
	setPosition(sf::Vector2f(pos.x - 37, pos.y - 137));
}

void Cage::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(fSprite, states);
}

void Cage::updateNode(sf::Time dt, CommandQueue& commands)
{
	if (fDestroyed) {
		fDestroyedAnim.update(dt, fSprite);

		if (fDestroyedAnim.isFinished())
		{
			fRemovable = true;
		}
	}
}
