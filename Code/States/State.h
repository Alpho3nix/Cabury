#pragma once
#include "StateIdentifiers.h"
#include "ResourceIdentifiers.h"
#include "PlayerData.h"

template<typename S, typename ID, typename Context>
class Stack;

struct Player_Data;

class State
{
public:

	struct Context
	{
		Context(sf::RenderWindow& win, TexturesHolder& tex, FontsHolder& fonts, Player_Data& player);

		sf::RenderWindow* window;
		TexturesHolder* textures;
		FontsHolder* fonts;
		Player_Data* player;
	};

	State(Context context, Stack<State, States::ID, Context>& stack);
	virtual ~State();
	
	virtual bool update(sf::Time dt) = 0;
	virtual bool handleEvent(const sf::Event& e) = 0;
	virtual bool draw() = 0;

protected:

	Context getContext() const;

	void requestStackPush(States::ID state);
	void requestStackPop();
	void requestStackClear();


private:
	Context fContext;
	Stack<State, States::ID, Context>* fStack;

};