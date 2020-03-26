#pragma once
#include "NonPlayerChar.h"
#include "Container.h"

class ShoppingNPC : public NonPlayerChar
{
public:

	enum OfferType
	{
		AddingResistance,
		AddingStrength
	};

	ShoppingNPC(int hp, NonPlayerChar::Type type, State::Context context);

	void handleEvent(const sf::Event& e, CommandQueue& commands) override;
	void createMenu(CommandQueue& commands) override;
	void reset() override;

	void updateNode(sf::Time dt, CommandQueue& commands) override;

private:

	void drawSthBetween(sf::RenderTarget& target, sf::RenderStates states) const override;
	
	bool fMenuCreated = false;
	bool fInteractBuffer = false;
	GUI::Container fShopMenu;

	void setupAnims();
};