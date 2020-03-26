#pragma once
#include "Component.h"
#include "ResourceIdentifiers.h"

namespace GUI
{
	class Label : public Component
	{
	public:
		Label(FontsHolder& fonts, const std::string& text, sf::Texture* tex = nullptr, unsigned int charSize = 30);
		Label(sf::Font& font, const std::string& text, sf::Texture* tex = nullptr, unsigned int charSize = 30);

		bool isSelectable() const override;
		void setText(const std::string& text);

		void handleEvent(const sf::Event& e) override;
		const sf::Text& getText() const;

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		sf::Text fText;
		sf::Sprite fSprite;
	};
}