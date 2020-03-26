#pragma once
#include "Component.h"
#include "ResourceIdentifiers.h"
#include "State.h"
#include <functional>

namespace GUI {
	
	class Button : public Component
	{
	public:
		typedef std::function<void()> Callback;
		Button(State::Context context, sf::IntRect texRectNorm, sf::IntRect texRectSelect, sf::IntRect texRectPress, sf::Font& font, unsigned int charSize = 16);
		Button(sf::IntRect texRectNorm, sf::IntRect texRectSelect, sf::IntRect texRectPress, sf::Font& font, const sf::Texture& ButtonTex);

		enum Type
		{
			Normal, Selected, Pressed
		};

		void setCallback(Callback cb);
		void setText(const std::string& text);
		void setToggle(bool flag);

		bool isSelectable() const override;
		void select() override;
		void deselect() override;
		void activate() override;
		void deactivate() override;
		void handleEvent(const sf::Event& e) override;

		sf::IntRect textureRectNormal = sf::IntRect(0, 0, 0, 0);
		sf::IntRect textureRectSelected = sf::IntRect(0, 0, 0, 0);
		sf::IntRect textureRectPressed = sf::IntRect(0, 0, 0, 0);


	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		void changeTexture(Type buttonType);

		Callback fCallback;
		sf::Sprite fSprite;
		sf::Text fText;
		bool fIsToggle;


	};
}