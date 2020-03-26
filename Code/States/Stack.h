
#ifndef STACK_H
#define STACK_H
#include "State.h"
#include "BeginningWorld.h"
#include <vector>
#include <functional>
#include <array>
#include "WorldIdentifiers.h"
#include <map>

template<typename S, typename ID, typename Context>
class Stack : private sf::NonCopyable
{
public:

	enum Actions
	{
		Push,
		Pop,
		Clear
	};

	explicit Stack(Context context);
	~Stack();

	//So that the creation of a state is much more abstract and just a function call
	template<typename T>
	void registerState(ID state);

	void update(sf::Time dt);
	void draw();
	void handleEvent(const sf::Event& e);
	
	void pushState(ID state);
	void popState();
	void clearStates();

	bool isEmpty() const;

private:

	S* createState(ID state);
	void applyPendingChanges();

	struct PendingChange
	{
		explicit PendingChange(Actions action, ID state = ID::None)
			:action(action), stateID(state) { }

		Actions action;
		ID stateID;
	};

	Context fContext;
	std::vector<S*> fStack;
	std::vector<PendingChange> fPendingList;

	std::map<ID, std::function<S*()>> fFactories;
};

#include "Stack.inl"
#endif //STACK_H