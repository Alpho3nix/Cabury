#include "DeathState.h"
#include "Button.h"
#include "Label.h"

DeathState::DeathState(State::Context context, Stack<State, States::ID, State::Context> & stack)
	:State(context, stack), fDeathMenu()
{
	createMenu();
}

bool DeathState::update(sf::Time dt)
{
	return false;
}

bool DeathState::handleEvent(const sf::Event & e)
{
	fDeathMenu.handleEvent(e);
	return true;
}

bool DeathState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	sf::RectangleShape rect;
	rect.setSize(window.getView().getSize());
	rect.setFillColor(sf::Color::Color(0, 0, 0, 150));

	window.draw(rect);
	window.draw(fDeathMenu);
	return false;
}

void DeathState::createMenu()
{
	using namespace GUI;

	//Restart button
	Button* restartButton = new Button(getContext(), sf::IntRect(319, 239, 122, 62), sf::IntRect(319, 159, 122, 62), sf::IntRect(319, 79, 122, 62), getContext().fonts->get(Fonts::ID::GUI));
	restartButton->setPosition(getContext().window->getView().getCenter().x, 120.f);
	restartButton->setCallback([this]()
	{
		requestStackClear();
		requestStackPush(States::ID::Game);
	});
	fDeathMenu.pack(restartButton);

	//Exit to main menu button
	Button* exitButton = new Button(getContext(), sf::IntRect(279, 439, 122, 62), sf::IntRect(279, 519, 122, 62), sf::IntRect(419, 719, 122, 62), getContext().fonts->get(Fonts::ID::GUI));
	exitButton->setPosition(getContext().window->getView().getSize().x / 2.f, restartButton->getPosition().y + 70.f);
	exitButton->setCallback([this]()
	{
		requestStackClear();
		requestStackPush(States::ID::Title);
	});
	fDeathMenu.pack(exitButton);

	Label* text = new Label(*getContext().fonts, "Game Over");
	text->setPosition(restartButton->getPosition().x, 20.f);

	fDeathMenu.pack(text);
}
