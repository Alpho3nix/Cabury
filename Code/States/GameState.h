#pragma once
#include "Stack.h"
#include "WorldTavern.h"

class GameState : public State
{
public:
	GameState(Context context, Stack<State, States::ID, State::Context>& stack);

	bool update(sf::Time dt) override;
	bool draw() override;
	bool handleEvent(const sf::Event& e) override;

	~GameState() override;
private:

	void registerWorlds();
	BeginningWorld::Context fWorldContext;
	Stack<BeginningWorld, Worlds::ID, BeginningWorld::Context> fWorldStack;

};