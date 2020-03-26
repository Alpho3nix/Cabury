#pragma once
#include "State.h"
#include "Container.h"
#include "Button.h"
#include "Player.h"
#include "Label.h"
#include <array>

class SettingsState : public State
{
public:
	SettingsState(Context context, Stack<State, States::ID, State::Context>& stack);

	bool draw() override;
	bool update(sf::Time dt) override;
	bool handleEvent(const sf::Event& e) override;

private:

	void updateLabels();
	void addButtonLabel(PlayerAction::Type action, float x, float y, const std::string& text, Context context);
	void updateInputFile(bool defaulInputs);

	GUI::Container fGUIContainer;

	std::array<GUI::Button*, PlayerAction::ActionCount> fBindingButtons;
	std::array<GUI::Label*, PlayerAction::ActionCount> fBindingLabels;

};