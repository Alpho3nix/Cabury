#include "PauseState.h"
#include "Button.h"

PauseState::PauseState(State::Context context, Stack<State, States::ID, State::Context>& stack)
	:State(context, stack), fPauseMenu()
{
	createMenu();
}

bool PauseState::update(sf::Time dt)
{
	return false;
}

bool PauseState::handleEvent(const sf::Event & e)
{
	fPauseMenu.handleEvent(e);
	if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
	{
		requestStackPop();
	}
	return false;
}

bool PauseState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	//background blur
	sf::RectangleShape rect;
	rect.setSize(window.getView().getSize());
	rect.setFillColor(sf::Color(0, 0, 0, 100));

	window.draw(rect);
	window.draw(fPauseMenu);
	return true;
}

void PauseState::createMenu()
{
	using namespace GUI;
	//Resume button
	Button* resumeButton = new Button(getContext(), sf::IntRect(319, 0, 122, 62), sf::IntRect(279, 919, 122, 62), sf::IntRect(279, 839, 122, 62), getContext().fonts->get(Fonts::ID::GUI));
	resumeButton->setPosition(getContext().window->getView().getSize().x / 2.f, 150.f);
	resumeButton->setCallback([this]()
	{
		requestStackPop();
	});
	fPauseMenu.pack(resumeButton);

	//Restart button
	Button* restartButton = new Button(getContext(), sf::IntRect(319, 239, 122, 62), sf::IntRect(319, 159, 122, 62), sf::IntRect(319, 79, 122, 62), getContext().fonts->get(Fonts::ID::GUI));
	restartButton->setPosition(getContext().window->getView().getCenter().x, resumeButton->getPosition().y + 70.f);
	restartButton->setCallback([this]()
	{
		requestStackClear();
		requestStackPush(States::ID::Game);
	});
	fPauseMenu.pack(restartButton);

	//Exit to main menu button
	Button* exitButton = new Button(getContext(), sf::IntRect(279, 439, 122, 62), sf::IntRect(279, 519, 122, 62), sf::IntRect(419, 719, 122, 62), getContext().fonts->get(Fonts::ID::GUI));
	exitButton->setPosition(getContext().window->getView().getSize().x / 2.f, restartButton->getPosition().y + 70.f);
	exitButton->setCallback([this]()
	{
		requestStackClear();
		requestStackPush(States::ID::Title);
	});
	fPauseMenu.pack(exitButton);

	Button* saveButton = new Button(getContext(), sf::IntRect(139, 679, 122, 62), sf::IntRect(139, 759, 122, 62), sf::IntRect(139, 919, 122, 62), getContext().fonts->get(Fonts::ID::GUI));
	saveButton->setToggle(false);
	saveButton->setText("Save");
	saveButton->setPosition(WIDTH - 70.f, HEIGHT - 50.f);
	saveButton->setCallback([this]()
		{
			//save
		});
	fPauseMenu.pack(saveButton);
}
