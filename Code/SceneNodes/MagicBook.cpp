#include "MagicBook.h"
#include "UtilityFunc.h"

MagicBook::MagicBook(const TexturesHolder& textures)
	:fSprite(textures.get(Textures::ID::MagicBook))
{
	const sf::Texture& tex = *fSprite.getTexture();

	fAnimation = new Animation(tex, sf::Vector2i(0, 70), 1);
	fAnimation->Repeat(true);
	fAnimation->setDuration(sf::seconds(0.5f));
	fAnimation->setFrameSize(sf::Vector2i(30, 26));
	fAnimation->setNumFrames(4);
	fAnimation->Restart();

	fSprite.setTextureRect(sf::IntRect(0, 70, 30, 26));
	Utility::CenterOrigin(fSprite);
}

unsigned int MagicBook::getCategory() const
{
	return Category::None;
}

sf::FloatRect MagicBook::getBoundingRect() const
{
	return getWorldTransform().transformRect(fSprite.getGlobalBounds());
}

bool MagicBook::isRemovable() const
{
	return fRemovable;
}

bool MagicBook::isDestroyed() const
{
	return isRemovable();
}

void MagicBook::updateNode(sf::Time dt, CommandQueue& commands)
{
	fAnimation->update(dt, fSprite);
}

void MagicBook::remove()
{
	fRemovable = true;
}

MagicBook::~MagicBook()
{
	SceneNode::~SceneNode();

	delete fAnimation;
}

void MagicBook::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(fSprite, states);
}
