#include "Speach.h"

Speach::Speach(std::string * speachStrings, size_t length)
	:fSpeachStrings(speachStrings), fSpeachLength(length) {

	for (size_t i = 0; i < fSpeachLength; ++i)
	{
		fSpeachStrings[i] = speachStrings[i];
	}
}

std::string & Speach::operator[](size_t index)
{
	assert(index < fSpeachLength);
	return fSpeachStrings[index];	
}

size_t Speach::getSize() const
{
	return fSpeachLength;
}
