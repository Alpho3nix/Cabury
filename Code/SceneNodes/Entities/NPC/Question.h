#pragma once
#include <vector>
#include <string>
#include <functional>

struct Answer;

class Question
{
public:
	Question(const std::string& question);
	~Question();

	const std::string& getQuestion() const;
	void setQuestion(const std::string& question);
	void addAnswer(const Answer& answer);

	Answer& operator[](size_t index);
	size_t AnswersNum() const;
	std::vector<Answer> getAnswers() const;

	std::function<void()> callback_func;

private:
	std::string fQuestion;
	std::vector<Answer> fAnswers;
};