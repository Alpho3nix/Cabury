#pragma once
#include "Component.h"

class Item;

namespace GUI {

	class InventoryBox : public Component
	{
	public:
		InventoryBox();
		InventoryBox(const sf::Font& font);
		InventoryBox(const InventoryBox& inventBox);
		void operator=(const InventoryBox& inventBox);
		~InventoryBox() override;

		bool isSelectable() const override;
		void select() override;
		void deselect() override;
		void activate() override;
		void deactivate() override;
		void handleEvent(const sf::Event& e) override;

		bool containsItem() const;
		void putItem(Item* newItem);
		const Item* getItem() const;

		void addQuantity(int num);
		int getQuantity() const;

	private:

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		std::vector<Item*> items;
		sf::Text fQuantityText;
		int fQuantity;

		sf::RectangleShape rect;
		bool isFull;

	};
}