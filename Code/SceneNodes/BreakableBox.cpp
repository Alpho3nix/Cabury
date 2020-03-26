#include "BreakableBox.h"
#include "UtilityFunc.h"

BreakableBox::BreakableBox(Type type, const TexturesHolder& textures, std::function<void()> breakAction)
	:fDestroyedAction(breakAction)
{
	handleType(type, textures);
}

bool BreakableBox::isDestroyed() const
{
	return fIsDead;
}

bool BreakableBox::isRemovable() const
{
	return fCanRemove;
}

unsigned int BreakableBox::getCategory() const
{
	return Category::breakableBox;
}

sf::FloatRect BreakableBox::getBoundingRect() const
{
	return getWorldTransform().transformRect(fBoundingRect);
}

sf::FloatRect BreakableBox::getSpriteBoundingRect() const
{
	return getWorldTransform().transformRect(fSprite.getGlobalBounds());
}

sf::Vector2f BreakableBox::getWorldPositionCentered() const
{
	return sf::Vector2f(getWorldPosition().x + getSpriteBoundingRect().width / 2.f, getWorldPosition().y + getSpriteBoundingRect().height / 2.f);
}

void BreakableBox::setOnDestroyedAction(std::function<void()> newAction)
{
	fDestroyedAction = newAction;
}

void BreakableBox::updateNode(sf::Time dt, CommandQueue& commands)
{
	if (fIsDead)
	{
		if (!fHasCalledAction)
		{
			fHasCalledAction = true;
			fDestroyedAction();
		}
		if(!fCanRemove)
			fDeathAnim.update(dt, fSprite);

		if (fDeathAnim.isFinished())
		{
			fCanRemove = true;
		}
	}
}

bool BreakableBox::applyDamage(int amount)
{
	fIsDead = true;
	return true;
}

void BreakableBox::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(fSprite, states);
}

void BreakableBox::handleType(Type type, const TexturesHolder& textures)
{
	switch (type)
	{
	case BreakableBox::Crate:
		fSprite.setTexture(textures.get(Textures::ID::Crate));
		fSprite.setTextureRect(sf::IntRect(0, 0, 70, 70));

		fDeathAnim.setTexture(textures.get(Textures::ID::Crate));
		fDeathAnim.setStartPoint(sf::Vector2i(70, 0));
		fDeathAnim.setDuration(sf::seconds(0.4f));
		fDeathAnim.setFrameSize(sf::Vector2i(70, 70));
		fDeathAnim.setNumFrames(4);
		fDeathAnim.setReadingDirection(1);
		fDeathAnim.Repeat(false);
		fDeathAnim.Restart();

		fBoundingRect = sf::FloatRect(fSprite.getGlobalBounds().left + 9, fSprite.getGlobalBounds().top + 19, 52, 51);

		break;
	default:
		throw std::logic_error("the type specified for BreakableBox doesn't exist");
		break;
	}
}
