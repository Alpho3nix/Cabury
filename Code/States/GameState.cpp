#include "GameState.h"
#include "WorldTavernInside.h"
#include "KanalisationWorld.h"
#include "MinotaurWorld.h"
#include <fstream>
#include <sstream>

GameState::GameState(Context context, Stack<State, States::ID, State::Context>& stack)
	:State(context, stack), fWorldContext(*context.window, *context.textures, *context.fonts, *context.player), fWorldStack(fWorldContext)
{
	std::ifstream playerLife;
	playerLife.open("Player/PlayerStats.txt");
	std::string line;

	while (playerLife.is_open())
	{
		std::getline(playerLife, line);

		playerLife.close();
	}
	std::istringstream iss(line);
	iss >> fWorldContext.player->fHP;
	fWorldContext.player->fMaxHP = fWorldContext.player->fHP;
	fWorldContext.player->fChangeWorld = false;
	fWorldContext.player->fMoney = 0;
	fWorldContext.player->fResistance = 10;
	fWorldContext.player->fStrength = 10;
	fWorldContext.player->destroyItems();

	std::ofstream output;
	output.open("Kanalisation.txt");
	if (output.is_open())
	{
		output.clear();
		output << 1 << " " << 1 << " " << 1;
	}
	output.close();

	registerWorlds();
	fWorldStack.pushState(Worlds::ID::InsideTavern);
}

bool GameState::update(sf::Time dt)
{
	fWorldStack.update(dt);

	if (getContext().player->fIsDead)
	{
		requestStackPop();
		requestStackPush(States::ID::Death);
		getContext().player->fIsDead = false;
	}
	if (getContext().player->fBackToMenu)
	{
		requestStackPop();
		requestStackPush(States::ID::Title);
	}
	if (getContext().player->fGameWin)
	{
		//requestStackPop();
		//requestStackPush(States::ID::Death);
	}

	return true;
}

bool GameState::draw()
{
	fWorldStack.draw();
	return false;
}

bool GameState::handleEvent(const sf::Event & e)
{
	fWorldStack.handleEvent(e);
	if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
	{
		requestStackPush(States::ID::Pause);
	}
	return true;
}

GameState::~GameState()
{
}



void GameState::registerWorlds()
{
	fWorldStack.registerState<WorldTavern>(Worlds::ID::OutsideTavern);
	fWorldStack.registerState<WorldTavernInside>(Worlds::ID::InsideTavern);
	fWorldStack.registerState<KanalisationWorld>(Worlds::ID::Kanalisation);
	fWorldStack.registerState<MinotaurWorld>(Worlds::ID::MinotaurWorld);
}
