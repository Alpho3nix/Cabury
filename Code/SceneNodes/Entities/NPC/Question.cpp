#include "Question.h"
#include <cassert>
#include <string>
#include "Answer.h"

Question::Question(const std::string & question)
	:fQuestion(question), fAnswers()
{
	callback_func = []() {};
}

Question::~Question()
{
}

const std::string & Question::getQuestion() const
{
	return fQuestion;
}

void Question::setQuestion(const std::string & question)
{
	fQuestion = question;
}

void Question::addAnswer(const Answer& answer)
{
	auto found = std::find(fAnswers.begin(), fAnswers.end(), answer);
	assert(found == fAnswers.end());

	fAnswers.push_back(answer);
}

Answer& Question::operator[](size_t index)
{
	assert(index < fAnswers.size());

	return fAnswers[index];
}

size_t Question::AnswersNum() const
{
	return fAnswers.size();
}

std::vector<Answer> Question::getAnswers() const
{
	return fAnswers;
}
