#include "ValueSymbol.h"

GUI::ValueSymbol::ValueSymbol(sf::Vector2f pos, const sf::Texture & tex, const sf::Font & font, const int & value, sf::IntRect texRect)
	:fSprite(tex, texRect), fValueText(std::to_string(value), font, 16), fValue(value)
{
	fSprite.setPosition(pos);
	fValueText.setPosition(pos.x - fValueText.getLocalBounds().width - 5.f, pos.y);
}

GUI::ValueSymbol::ValueSymbol(sf::Vector2f pos, const sf::Texture & tex, const sf::Font & font, const int & value)
	: fSprite(tex), fValueText(std::to_string(value), font, 16), fValue(value)
{
	fSprite.setPosition(pos);
	fValueText.setPosition(pos.x - fValueText.getLocalBounds().width - 5.f, pos.y);
}

bool GUI::ValueSymbol::isSelectable() const
{
	return false;
}

void GUI::ValueSymbol::handleEvent(const sf::Event & e)
{
	return;
}

void GUI::ValueSymbol::update(sf::Time dt)
{
	fValueText.setString(std::to_string(fValue));
	fValueText.setPosition(fSprite.getPosition().x - fValueText.getLocalBounds().width - 5.f, fSprite.getPosition().y);
}

void GUI::ValueSymbol::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(fSprite, states);
	target.draw(fValueText, states);
}
