#include "InventoryBox.h"
#include "Item.h"
#include <iostream>
#include "UtilityFunc.h"

GUI::InventoryBox::InventoryBox()
	:isFull(false), items(), fQuantity(0), fQuantityText()
{
}

GUI::InventoryBox::InventoryBox(const sf::Font& font)
	:isFull(false), items(), fQuantity(0), fQuantityText(std::to_string(fQuantity), font, 10)
{

	Utility::CenterOrigin(fQuantityText);

	rect.setSize(sf::Vector2f(25.f, 25.f));
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineThickness(1.f);
	rect.setOutlineColor(sf::Color::White);
	rect.setPosition(0.f, 0.f);

	fQuantityText.setPosition(rect.getSize().x - fQuantityText.getLocalBounds().width / 2.f, rect.getSize().y - fQuantityText.getLocalBounds().height / 2.f);
}

GUI::InventoryBox::InventoryBox(const InventoryBox& inventBox)
	:items(inventBox.items), fQuantityText(inventBox.fQuantityText), fQuantity(inventBox.fQuantity), rect(inventBox.rect), isFull(inventBox.isFull)
{
}

void GUI::InventoryBox::operator=(const InventoryBox& inventBox)
{
	items = inventBox.items;
	fQuantityText = inventBox.fQuantityText;
	fQuantity = inventBox.fQuantity;
	rect = inventBox.rect;
	isFull = inventBox.isFull;
}

GUI::InventoryBox::~InventoryBox()
{

	//std::cout << "HEY an inventory box is being destroyed..." << std::endl;
}

bool GUI::InventoryBox::isSelectable() const
{
	return true;
}

void GUI::InventoryBox::select()
{
	Component::select();
}

void GUI::InventoryBox::deselect()
{
	Component::deselect();
}

void GUI::InventoryBox::activate()
{
	Component::activate();
	if (!isFull) {
		deactivate();
	}
	else
	{
		//TODO: make an effect happening if the item is active
		addQuantity(-1);
		deactivate();
	}
}

void GUI::InventoryBox::deactivate()
{
	Component::deactivate();
}

void GUI::InventoryBox::handleEvent(const sf::Event & e)
{
}

bool GUI::InventoryBox::containsItem() const
{
	return isFull;
}

void GUI::InventoryBox::putItem(Item * newItem)
{
	if (!newItem)
		return;
	items.push_back(newItem);
	isFull = true;
	if(items.size() == 1)
		items[0]->setPosition(rect.getSize() / 2.f);
	newItem->fCanNotMove = true;
}

const Item * GUI::InventoryBox::getItem() const
{
	if (!items.empty()) {
		return items[0];
	}

	return nullptr;
}

void GUI::InventoryBox::addQuantity(int num)
{
	fQuantity += num;
	
	if (num < 0)
	{
		if (fQuantity < 0)
			return;

		for (int i = 0; i < -num; ++i)
		{
			delete items[items.size() - 1];
			items.pop_back();
		}
		
		if (items.empty())
		{
			isFull = false;
		}
	}

	fQuantityText.setString(std::to_string(fQuantity));
	Utility::CenterOrigin(fQuantityText);
	fQuantityText.setPosition(rect.getSize().x - fQuantityText.getLocalBounds().width / 2.f, rect.getSize().y - fQuantityText.getLocalBounds().height / 2.f);

}

int GUI::InventoryBox::getQuantity() const
{
	return fQuantity;
}

void GUI::InventoryBox::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	//temporary rectangle for the empty texture
	if(isSelected())
		target.draw(rect, states);
	if (items.size() > 0) {
		target.draw(*items[0], states);
	}
	if (fQuantity > 0)
		target.draw(fQuantityText, states);
}
