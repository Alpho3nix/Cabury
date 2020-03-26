#include "MenuState.h"
#include "Button.h"

MenuState::MenuState(State::Context context, Stack<State, States::ID, State::Context>& stack)
	:State(context, stack)
{
	createMenu();
}

bool MenuState::update(sf::Time dt)
{
	return true;
}

bool MenuState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	window.draw(fMenu);
	return false;
}

bool MenuState::handleEvent(const sf::Event & e)
{
	fMenu.handleEvent(e);
	return true;
}

void MenuState::createMenu()
{
	//Play button
	GUI::Button* playButton = new GUI::Button(getContext(), sf::IntRect(419, 479, 122, 62), sf::IntRect(419, 879, 122, 62), sf::IntRect(419, 799, 122, 62), getContext().fonts->get(Fonts::ID::GUI));
	playButton->setPosition(getContext().window->getView().getSize().x / 2.f, 120.f);
	playButton->setCallback([this]()
	{
		requestStackPop();
		requestStackPush(States::ID::Game);
	});
	fMenu.pack(playButton);
	
	//Setting button
	GUI::Button* settingsButton = new GUI::Button(getContext(), sf::IntRect(0, 679, 122, 62), sf::IntRect(0, 599, 122, 62), sf::IntRect(0, 519, 122, 62), getContext().fonts->get(Fonts::ID::GUI));
	settingsButton->setPosition(getContext().window->getView().getSize().x / 2.f, playButton->getPosition().y + 70.f);
	settingsButton->setCallback([this]()
	{
		requestStackPush(States::ID::Settings);
	});
	fMenu.pack(settingsButton);

	//Quit button
	GUI::Button* quitButton = new GUI::Button(getContext(), sf::IntRect(419, 559, 122, 62), sf::IntRect(419, 399, 122, 62), sf::IntRect(319, 319, 122, 62), getContext().fonts->get(Fonts::ID::GUI));
	quitButton->setPosition(getContext().window->getView().getSize().x / 2.f, settingsButton->getPosition().y + 70.f);
	quitButton->setCallback([this]() {
		getContext().window->close();
	});
	fMenu.pack(quitButton);

}
