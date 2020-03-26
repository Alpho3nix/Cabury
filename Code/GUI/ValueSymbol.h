#pragma once
#include "Component.h"

namespace GUI
{
	class ValueSymbol : public Component
	{
	public:
		ValueSymbol(sf::Vector2f pos, const sf::Texture& tex, const sf::Font& font, const int& value, sf::IntRect texRect);
		ValueSymbol(sf::Vector2f pos, const sf::Texture& tex, const sf::Font& font, const int& value);

		bool isSelectable() const override;
		void handleEvent(const sf::Event& e) override;
		void update(sf::Time dt) override;

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		sf::Sprite fSprite;
		sf::Text fValueText;

		const int& fValue;
	};
}