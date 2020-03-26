#include "Component.h"

GUI::Component::Component()
	:fSelected(false), fActive(false)
{
}

GUI::Component::~Component()
{
}

bool GUI::Component::isSelected() const
{
	return fSelected;
}

void GUI::Component::select()
{
	fSelected = true;
}

void GUI::Component::deselect()
{
	fSelected = false;
}

bool GUI::Component::isActive() const
{
	return fActive;
}

void GUI::Component::activate()
{
	fActive = true;
}

void GUI::Component::deactivate()
{
	fActive = false;
}

void GUI::Component::update(sf::Time dt)
{
	return;
}
