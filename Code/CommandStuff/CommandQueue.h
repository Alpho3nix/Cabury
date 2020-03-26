#pragma once
#include "Command.h"
#include <queue>

class CommandQueue
{
	std::queue<Command> fQueue;
public:

	Command pop();
	void push(const Command& command);
	bool isEmpty() const;
	size_t size() const;
	void addCommand(unsigned int category, std::function<void(SceneNode&, sf::Time)> fn);

};

