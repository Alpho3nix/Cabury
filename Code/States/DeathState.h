#pragma once
#include "Stack.h"
#include "Container.h"

class DeathState : public State
{
public:
	DeathState(State::Context context, Stack<State, States::ID, State::Context>& stack);

	bool update(sf::Time dt) override;
	bool handleEvent(const sf::Event& e) override;
	bool draw() override;

private:

	void createMenu();

	GUI::Container fDeathMenu;
	
};