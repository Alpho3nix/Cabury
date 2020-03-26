#include "Bar.h"
#include <cassert>

GUI::Bar::Bar(sf::FloatRect dim, sf::Color fillCol, int& defVal, float maxVal)
	:fEmptyBar(), fFillBar(), fFillColor(fillCol), fMaximumValue(maxVal), fCurrentFillingVal(defVal), fBarDimension(dim)
{
	assert(maxVal != 0.f);
	fEmptyBar.setSize(sf::Vector2f(dim.width, dim.height));
	fEmptyBar.setPosition(dim.left, dim.top);
	fEmptyBar.setFillColor(sf::Color::Color(0, 0, 0, 225));

	fFillBar.setSize(sf::Vector2f(defVal / maxVal * fEmptyBar.getSize().x, fEmptyBar.getSize().y));
	fFillBar.setPosition(fEmptyBar.getPosition());
	fFillBar.setFillColor(fillCol);
}

void GUI::Bar::setFillColor(sf::Color newCol)
{
	fFillColor = newCol;
	fFillBar.setFillColor(fFillColor);
}

sf::Color GUI::Bar::getFillColor() const
{
	return fFillColor;
}

sf::FloatRect GUI::Bar::getDimensions() const
{
	return fBarDimension;
}

int& GUI::Bar::getCurrentFillingValue() const
{
	return fCurrentFillingVal;
}

void GUI::Bar::update(sf::Time dt)
{
	fFillBar.setSize(sf::Vector2f(fCurrentFillingVal / fMaximumValue * fEmptyBar.getSize().x, fFillBar.getSize().y));
}

bool GUI::Bar::isSelectable() const
{
	return false;
}

void GUI::Bar::handleEvent(const sf::Event & e)
{
	return;
}

void GUI::Bar::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(fEmptyBar, states);
	target.draw(fFillBar, states);
}
