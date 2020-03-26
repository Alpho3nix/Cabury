#include "TalkingNPC.h"
#include "Answer.h"
#include "UtilityFunc.h"
#include "Button.h"

TalkingNPC::TalkingNPC(int hp, NonPlayerChar::Type type, State::Context context, Question speach)
	:NonPlayerChar(hp, type, context), fSpeach(speach), fCurrentQuestion(speach), fDialogueBox(), fAnswerMenu()
{

	fDialogueBox.setFillColor(sf::Color::Color(0, 0, 0, 150));
}

Question TalkingNPC::getSpeach() const
{
	return fSpeach;
}

void TalkingNPC::handleEvent(const sf::Event& e, CommandQueue& commands)
{
	if (fBeginInteraction && fMenuCreated)
		fAnswerMenu.handleEvent(e);

	if (e.type == sf::Event::KeyReleased && e.key.code == sf::Keyboard::Enter)
	{
		nextQuestion(fCurrentAnswer);

		createMenu(commands);
	}
}

void TalkingNPC::createMenu(CommandQueue& commands)
{
	std::vector<Answer> answers = fCurrentQuestion.getAnswers();

	fAnswerMenu.clearContainer();
	int limitedAnswers = 4;
	if (answers.size() > limitedAnswers)
	{
		throw std::runtime_error("too many answers for one npc question");
		return;
	}
	if (answers.size() == 1 && answers[0].answer == "") {
		fCurrentAnswer = answers[0];
		return;
	}

	using namespace GUI;
	sf::IntRect selected(0, 219, 301, 202);
	sf::IntRect normal(0, 0, 301, 201);
	sf::IntRect pressed(0, 219, 301, 202);

	for (int i = 0; i < answers.size(); ++i)
	{
		Button* button = new Button(fContext, normal, selected, pressed, fContext.fonts->get(Fonts::ID::NPCSpeach));
		button->setPosition(122 * i - 210.f, -300.f);
		button->setText(answers[i].answer);
		button->setToggle(false);
		button->setCallback([this, answers, i]()
			{
				fCurrentAnswer = answers[i];

			});
		fAnswerMenu.pack(button);
	}

	fMenuCreated = true;
}

void TalkingNPC::reset()
{
	NonPlayerChar::reset();
	currentAnswer_num = 0;
	fMenuCreated = false;
	fCurrentQuestion = fSpeach;
	fAnswerMenu.clearContainer();
}

void TalkingNPC::drawSthBetween(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (currentAnswer_num != 0 && !fStopShowing) {
		target.draw(fDialogueBox, states);

		if (fBeginInteraction && fMenuCreated)
			target.draw(fAnswerMenu, states);
	}
}


void TalkingNPC::nextQuestion(Answer answer)
{
	if (!fStopInteract) {
		answer.callback_func();
		if (fText.getString() != fHelpingText) {

			fCurrentQuestion = answer.question;
		}
		if (fCurrentQuestion.getQuestion() != "") {
			fText.setString(fCurrentQuestion.getQuestion());
			fText.setCharacterSize(16);
			fText.setFont(fFonts.get(Fonts::ID::NPCSpeach));
			Utility::CenterOrigin(fText);
			fText.setPosition(0.f, -getBoundingRect().height / 2.f - fText.getGlobalBounds().height);
			
			fDialogueBox.setSize(sf::Vector2f(fText.getGlobalBounds().width * 1.5f + boxOffset, fText.getGlobalBounds().height * 1.5f + boxOffset));
			fDialogueBox.setPosition(fText.getGlobalBounds().left - boxOffset / 2.f, fText.getGlobalBounds().top - boxOffset / 2.f);
			currentAnswer_num++;
		}
		else
		{
			fText.setString("");
			fDialogueBox.setSize(sf::Vector2f(0.f, 0.f));
		}
	}
}
