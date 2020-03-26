#include "Projectile.h"

Projectile::Projectile(sf::Texture& tex, sf::IntRect texRect, int dir, float velocity, int damage)
	:fSprite(tex, texRect), fVelocity(velocity), fSpriteAnim(Animation(tex, sf::Vector2i(0, 0), 1)), fDamage(damage), fDirection(dir)
{
	if (dir == -1) {
		fSpriteAnim.Repeat(true);
		fSpriteAnim.setDuration(sf::seconds(0.4f));
		fSpriteAnim.setFrameSize(sf::Vector2i(texRect.width, texRect.height));
		fSpriteAnim.setNumFrames(4);
		fSpriteAnim.Restart();
	}
	else
	{
		fSpriteAnim.setStartPoint(sf::Vector2i(texRect.width, texRect.height * 2));
		fSpriteAnim.setReadingDirection(-1);
		fSpriteAnim.Repeat(true);
		fSpriteAnim.setDuration(sf::seconds(0.4f));
		fSpriteAnim.setFrameSize(sf::Vector2i(texRect.width, texRect.height));
		fSpriteAnim.setNumFrames(4);
		fSpriteAnim.Restart();
	}
}

unsigned int Projectile::getCategory() const
{
	return Category::projectile;
}

sf::FloatRect Projectile::getBoundingRect() const
{
	return getWorldTransform().transformRect(fSprite.getGlobalBounds());
}

bool Projectile::isRemovable() const
{
	return isDestroyed();
}

bool Projectile::isDestroyed() const
{
	return fIsDestroyed;
}

void Projectile::Destroy()
{
	fIsDestroyed = true;
}

int Projectile::getDamage() const
{
	return fDamage;
}

int Projectile::getDirection() const
{
	return fDirection;
}

void Projectile::setDirection(int dir)
{
	fDirection = dir;
}

void Projectile::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(fSprite, states);
}

void Projectile::updateNode(sf::Time dt, CommandQueue& commands)
{
	move(fDirection * fVelocity * dt.asSeconds(), 0.f);
	fSpriteAnim.update(dt, fSprite);
}
