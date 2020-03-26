#pragma once
#include "Stack.h"
#include "Container.h"

class MenuState : public State
{
public:

	MenuState(State::Context context, Stack<State, States::ID, State::Context>& stack);

	bool update(sf::Time dt) override;
	bool draw() override;
	bool handleEvent(const sf::Event& e) override;

private:
	void createMenu();

	GUI::Container fMenu;

};