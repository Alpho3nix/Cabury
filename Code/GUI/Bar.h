#pragma once
#include "Component.h"

namespace GUI
{
	class Bar : public Component
	{
	public:
		Bar(sf::FloatRect dim, sf::Color fillCol, int& defVal, float maxVal);

		void setFillColor(sf::Color newCol);
		sf::Color getFillColor() const;

		sf::FloatRect getDimensions() const;
		int& getCurrentFillingValue() const;

		void update(sf::Time dt) override;

		bool isSelectable() const override;
		void handleEvent(const sf::Event& e) override;
	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		sf::RectangleShape fEmptyBar;
		sf::RectangleShape fFillBar;

		sf::Color fFillColor;
		sf::FloatRect fBarDimension;
		float fMaximumValue;
		int& fCurrentFillingVal;
	};
}