#include "Stack.h"
#include <array>
#include "PlayerData.h"
#include "UtilityFunc.h"

template<typename S, typename ID, typename Context>
Stack<S, ID, Context>::Stack(Context context)
	:fContext(context)
	, fFactories()
	, fStack()
	, fPendingList()
{
}

template<typename S, typename ID, typename Context>
Stack<S, ID, Context>::~Stack()
{
	for (S* s : fStack)
	{
		delete s;
	}
}

template<typename S, typename ID, typename Context>
void Stack<S, ID, Context>::update(sf::Time dt)
{
	//Iterate from top to bottom and stop as soon as update() returns false
	for (auto itr = fStack.rbegin(); itr != fStack.rend(); ++itr)
	{
		if (!(*itr)->update(dt))
		{
			break;
		}
	}

	applyPendingChanges();
}

template<typename S, typename ID, typename Context>
void Stack<S, ID, Context>::draw()
{
	for (S* state : fStack)
	{
		state->draw();
	}
}

template<typename S, typename ID, typename Context>
void Stack<S, ID, Context>::handleEvent(const sf::Event & e)
{
	//Iterate from top to bottom and stop as soon as handleEvent() returns false
	for (auto itr = fStack.rbegin(); itr != fStack.rend(); ++itr)
	{
		if (!(*itr)->handleEvent(e))
		{
			break;
		}
	}

	applyPendingChanges();
}

template<typename S, typename ID, typename Context>
void Stack<S, ID, Context>::pushState(ID state)
{
	fPendingList.push_back(PendingChange(Push, state));
}

template<typename S, typename ID, typename Context>
void Stack<S, ID, Context>::popState()
{
	fPendingList.push_back(PendingChange(Pop));
}

template<typename S, typename ID, typename Context>
void Stack<S, ID, Context>::clearStates()
{
	fPendingList.push_back(PendingChange(Clear));
}

template<typename S, typename ID, typename Context>
bool Stack<S, ID, Context>::isEmpty() const
{
	return fStack.empty();
}

template<typename S, typename ID, typename Context>
S* Stack<S, ID, Context>::createState(ID state)
{
	auto found = fFactories.find(state);
	assert(found != fFactories.end());

	return found->second();
}

template<typename S, typename ID, typename Context>
void Stack<S, ID, Context>::applyPendingChanges()
{
	for (PendingChange change : fPendingList)
	{
		switch (change.action)
		{
		case Push:
			fStack.push_back(createState(change.stateID));
			break;
		case Pop:
			delete fStack.back();
			fStack.pop_back();
			break;
		case Clear:
			
			for (S* st : fStack)
			{
				delete st;
			}
			fStack.clear();
			break;
		default:
			break;
		}
	}

	fPendingList.clear();
}


template<typename S, typename ID, typename Context>
template<typename T>
void Stack<S, ID, Context>::registerState(ID state)
{
	fFactories[state] = [this]()
	{
		return new T(fContext, *this);
	};
}
