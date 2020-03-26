#include "SettingsState.h"
#include "UtilityFunc.h"
#include "PlayerData.h"
#include <iostream>
#include <fstream>
#include <sstream>

SettingsState::SettingsState(State::Context context, Stack<State, States::ID, State::Context>& stack)
	:State(context, stack), fGUIContainer()
{
	addButtonLabel(PlayerAction::MoveLeft, getContext().window->getSize().x / 2.f, 50.f, "Bouger a gauche", context);
	addButtonLabel(PlayerAction::MoveRight, getContext().window->getSize().x / 2.f, 120.f, "Bouger a droite", context);
	addButtonLabel(PlayerAction::Jump, getContext().window->getSize().x / 2.f, 190.f, "Sauter", context);
	addButtonLabel(PlayerAction::Crouch, getContext().window->getSize().x / 2.f, 260.f, "accroupir", context);
	addButtonLabel(PlayerAction::Slide, getContext().window->getSize().x / 2.f, 330.f, "Glisser", context);
	addButtonLabel(PlayerAction::Attack1, getContext().window->getSize().x / 2.f + 200.f, 50.f, "1ere Attaque", context);
	addButtonLabel(PlayerAction::Attack2, getContext().window->getSize().x / 2.f + 200.f, 120.f, "2nd Attaque", context);
	addButtonLabel(PlayerAction::Attack3, getContext().window->getSize().x / 2.f + 200.f, 190.f, "3eme Attaque", context);
	addButtonLabel(PlayerAction::RolledDash, getContext().window->getSize().x / 2.f + 200.f, 260.f, "Roulade", context);
	addButtonLabel(PlayerAction::OpenInventory, getContext().window->getSize().x / 2.f + 200.f, 330.f, "Ouvrir Inventaire", context);

	updateLabels();

	GUI::Button* backButton = new GUI::Button(context, sf::IntRect(139, 679, 122, 62), sf::IntRect(139, 759, 122, 62), sf::IntRect(139, 919, 122, 62), context.fonts->get(Fonts::ID::GUI));
	backButton->setPosition(80.f, 400.f);
	backButton->setText("Retour");
	backButton->setCallback([this]()
	{
		requestStackPop();
	});

	GUI::Button* defaultControlsButton = new GUI::Button(context, sf::IntRect(139, 679, 122, 62), sf::IntRect(139, 759, 122, 62), sf::IntRect(139, 919, 122, 62), context.fonts->get(Fonts::ID::GUI));
	defaultControlsButton->setPosition(240.f, 400.f);
	defaultControlsButton->setText("Par defaut");
	defaultControlsButton->setCallback([this]()
		{
			updateInputFile(true);
			updateLabels();
		});

	fGUIContainer.pack(backButton);
	fGUIContainer.pack(defaultControlsButton);
}

bool SettingsState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	sf::RectangleShape rect{ sf::Vector2f(getContext().window->getSize()) };
	rect.setFillColor(sf::Color::Black);

	window.draw(rect);
	window.draw(fGUIContainer);
	return false;
}

bool SettingsState::update(sf::Time dt)
{
	return true;
}

bool SettingsState::handleEvent(const sf::Event & e)
{

	bool isKeyBinding = false;
	for (std::size_t action = 0; action < PlayerAction::ActionCount; ++action)
	{
		if (fBindingButtons[action]->isActive())
		{
			isKeyBinding = true;
			if (e.type == sf::Event::KeyReleased)
			{
				getContext().player->assignKey(static_cast<PlayerAction::Type>(action), e.key.code);
				fBindingButtons[action]->deactivate();
			}
			break;
		}
	}

	if (isKeyBinding) {
		updateLabels();
		updateInputFile(false);
	}
	else
		fGUIContainer.handleEvent(e);


	return false;
}

void SettingsState::updateLabels()
{
	Player_Data& playerData = *getContext().player;

	for (std::size_t i = 0; i < PlayerAction::ActionCount; ++i)
	{
		sf::Keyboard::Key key = playerData.fKeyBinding[static_cast<PlayerAction::Type>(i)];
		fBindingLabels[i]->setText(Utility::toString(key));
	}
}

void SettingsState::addButtonLabel(PlayerAction::Type action, float x, float y, const std::string & text, Context context)
{
	fBindingButtons[action] = new GUI::Button(context, sf::IntRect(139, 679, 122, 62), sf::IntRect(139, 759, 122, 62), sf::IntRect(139, 919, 122, 62), context.fonts->get(Fonts::ID::GUI), 8);
	fBindingButtons[action]->setPosition(x, y);
	fBindingButtons[action]->setText(text);
	fBindingButtons[action]->setToggle(true);

	fBindingLabels[action] = new GUI::Label(context.fonts->get(Fonts::ID::Sansation), "", nullptr, 16);
	fBindingLabels[action]->setPosition(x + 100.f, y);

	fGUIContainer.pack(fBindingButtons[action]);
	fGUIContainer.pack(fBindingLabels[action]);
}

void SettingsState::updateInputFile(bool defaultInputs)
{
	std::ofstream file;
	file.open("PlayerInputs.txt");
	if (!file.is_open())
		throw std::runtime_error("failed to open PlayerInputs.txt");

	file.clear();
	if (!defaultInputs) {
		int i = 0;
		while (!file.eof() && i < PlayerAction::ActionCount)
		{
			file << i << " " << int(getContext().player->fKeyBinding[i]) << "\n";
			++i;
		}

	}
	else
	{
		Player_Data& playerD = *getContext().player;
		playerD.fKeyBinding[PlayerAction::MoveLeft] = sf::Keyboard::Q;
		playerD.fKeyBinding[PlayerAction::MoveRight] = sf::Keyboard::D;
		playerD.fKeyBinding[PlayerAction::Crouch] = sf::Keyboard::S;
		playerD.fKeyBinding[PlayerAction::Slide] = sf::Keyboard::Space;
		playerD.fKeyBinding[PlayerAction::RolledDash] = sf::Keyboard::F;
		playerD.fKeyBinding[PlayerAction::Attack1] = sf::Keyboard::A;
		playerD.fKeyBinding[PlayerAction::Attack2] = sf::Keyboard::E;
		playerD.fKeyBinding[PlayerAction::Attack3] = sf::Keyboard::R;
		playerD.fKeyBinding[PlayerAction::Jump] = sf::Keyboard::Z;
		playerD.fKeyBinding[PlayerAction::OpenInventory] = sf::Keyboard::I;

		int i = 0;
		while (!file.eof() && i < PlayerAction::ActionCount)
		{
			file << i << " " << int(playerD.fKeyBinding[i]) << "\n";

			++i;
		}
	}
	file.close();
}