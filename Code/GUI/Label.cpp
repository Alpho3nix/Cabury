#include "Label.h"
#include "UtilityFunc.h"

GUI::Label::Label(FontsHolder & fonts, const std::string& text, sf::Texture* tex, unsigned int charSize)
	:fText(text, fonts.get(Fonts::ID::GUI), charSize), fSprite()
{
	Utility::CenterOrigin(fText);
	if (tex) {
		fSprite.setTexture(*tex);
		fText.setPosition(fSprite.getGlobalBounds().left + fSprite.getGlobalBounds().width / 2.f, fSprite.getGlobalBounds().top + fSprite.getGlobalBounds().height / 2.f);
	}
}

GUI::Label::Label(sf::Font & font, const std::string & text, sf::Texture * tex, unsigned int charSize)
	:fText(text, font, charSize), fSprite()
{
	Utility::CenterOrigin(fText);
	if (tex) {
		fSprite.setTexture(*tex);
		fText.setPosition(fSprite.getGlobalBounds().left + fSprite.getGlobalBounds().width / 2.f, fSprite.getGlobalBounds().top + fSprite.getGlobalBounds().height / 2.f);
	}
}

bool GUI::Label::isSelectable() const
{
	return false;
}

void GUI::Label::setText(const std::string & text)
{
	fText.setString(text);
	Utility::CenterOrigin(fText);
	if (fSprite.getTexture()) {
		fText.setPosition(fSprite.getGlobalBounds().left + fSprite.getGlobalBounds().width / 2.f, fSprite.getGlobalBounds().top + fSprite.getGlobalBounds().height / 2.f);
	}
}

void GUI::Label::handleEvent(const sf::Event & e)
{
}

const sf::Text & GUI::Label::getText() const
{
	return fText;
}

void GUI::Label::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	if(fSprite.getTexture())
		target.draw(fSprite, states);
	target.draw(fText, states);
}
