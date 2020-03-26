#include "Answer.h"


Answer::Answer(const std::string& string, Question ques)
	:question(ques), answer(string)
{
	callback_func = []() {};
}

Answer::Answer()
	:answer(), question("")
{
	callback_func = []() {};
}

Answer::Answer(const std::string & string)
	:answer(string), question("")
{
	callback_func = []() {};
}

bool Answer::operator==(const Answer & other)
{
	return this->answer == other.answer;
}
