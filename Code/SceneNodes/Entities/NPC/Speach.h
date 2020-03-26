#pragma once
#include <string>
#include <cassert>

class Speach
{
public:
	Speach(std::string* speachStrings, size_t length);

	std::string& operator[](size_t index);
	size_t getSize() const;

	~Speach() {}
private:
	std::string* fSpeachStrings;
	size_t fSpeachLength;
};