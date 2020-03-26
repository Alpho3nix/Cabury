#include "CommandQueue.h"

Command CommandQueue::pop()
{
	Command front = fQueue.front();
	fQueue.pop();
	return front;
}

void CommandQueue::push(const Command & command)
{
	fQueue.push(command);
}

bool CommandQueue::isEmpty() const
{
	return fQueue.empty();
}

size_t CommandQueue::size() const
{
	return fQueue.size();
}

void CommandQueue::addCommand(unsigned int category, std::function<void(SceneNode&, sf::Time)> fn)
{
	Command command;
	command.category = category;
	command.action = fn;

	push(command);
}
