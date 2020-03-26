#pragma once
#include "NonPlayerChar.h"
#include "Answer.h"
#include "Container.h"


class TalkingNPC : public NonPlayerChar
{
public:
	TalkingNPC(int hp, NonPlayerChar::Type type, State::Context context, Question speach);


	void nextQuestion(Answer answer);
	Question getSpeach() const;

	void handleEvent(const sf::Event& e, CommandQueue& commands) override;
	void createMenu(CommandQueue& commands) override;
	void reset() override;

private:

	void drawSthBetween(sf::RenderTarget& target, sf::RenderStates states) const override;

	GUI::Container fAnswerMenu;
	Answer fCurrentAnswer;
	Question fSpeach;
	Question fCurrentQuestion;
	size_t currentAnswer_num = 0;

	bool fMenuCreated = false;

	sf::RectangleShape fDialogueBox;
	float boxOffset = 15.f;
};