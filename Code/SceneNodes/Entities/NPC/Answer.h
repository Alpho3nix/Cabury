#pragma once
#include <string>
#include "Question.h"

struct Answer
{
	Answer(const std::string& string, Question ques);
	Answer();
	Answer(const std::string& string);

	bool operator==(const Answer& other);

	std::string answer;
	Question question;
	std::function<void()> callback_func;
};