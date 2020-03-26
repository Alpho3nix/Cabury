#include "State.h"
#include "Stack.h"
#include "Player.h"
#include "PlayerData.h"

State::State(Context context, Stack<State, States::ID, Context>& stack)
	:fContext(context), fStack(&stack)
{
}

State::~State()
{
}

State::Context State::getContext() const
{
	return fContext;
}

void State::requestStackPush(States::ID state)
{
	fStack->pushState(state);
}

void State::requestStackPop()
{
	fStack->popState();
}

void State::requestStackClear()
{
	fStack->clearStates();
}

State::Context::Context(sf::RenderWindow & win, TexturesHolder & tex, FontsHolder & fonts, Player_Data& player)
	:window(&win), textures(&tex), fonts(&fonts), player(&player)
{
}
