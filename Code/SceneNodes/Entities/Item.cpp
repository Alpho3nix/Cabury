#include "Item.h"
#include "Player.h"

void Item::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(fSprite, states);
}

Item::Item(int hp, const sf::Texture & tex, ItemType itemType)
	:Entity(hp), type(itemType.type), worldBounds(itemType.worldBounds)
{
	fSprite.setTexture(tex);
	fSprite.setTextureRect(itemType.texRect);
	Utility::CenterOrigin(fSprite);

	fEffectOnPlayer = 
	[this, itemType](Player& player)
	{
		if (canBePickedUp) {
			fParent->detachChild(*this);

			
			return itemType.effect(player);
		}
	};

	enableAnimation(false);

	setUpwardsVel(itemType.upwardsVel);
	double r = Utility::randomFloat(1.f, 5.f);
	double r1 = Utility::randomFloat(-5.0, -1.f);
	ratio = Utility::randomInt(0, 1) == 1 ? r : r1;
}

Item::~Item()
{
}

unsigned int Item::getCategory() const
{
	return Category::item;
}

void Item::updateNode(sf::Time dt, CommandQueue & commands)
{
	Utility::OutSide out = isOutOfTheWorld(getBoundingRect());
	if (isSteppingOnSth())
	{
		setVelocity(0.f, 0.f);
		canBePickedUp = true;
	}
	else if (out == Utility::Left || out == Utility::Right)
	{
		setVelocity(-getVelocity().x, getVelocity().y);
		fBounce = true;
	}
	else if(!fBounce)
	{
		setVelocity(ratio * 10.f * dt.asSeconds(), getVelocity().y);
	}

	Entity::updateNode(dt, commands);
		
}

void Item::canPickedUp(bool flag)
{
	canBePickedUp = flag;
}

const sf::Sprite& Item::getItemSprite() const
{
	return fSprite;
}

Item::Type Item::getType() const
{
	return type;
}

const SceneNode * Item::getParent() const
{
	return fParent;
}

Utility::OutSide Item::isOutOfTheWorld(const sf::FloatRect & pos) const
{
	return Utility::isOutOfTheRect(pos, worldBounds);
}