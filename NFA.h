#pragma once

#include <set>
//#include <map>
#include <string>

/*struct State {
	State(std::string str = "");
	std::string label;
};*/

struct Transition {
	Transition(int, int, char c = ' ');
	int fromState;
	int toState;
	char label;
	Transition operator+(int) const;
	friend bool operator<(const Transition, const Transition);
	std::string toString() const;
	std::string DOTstring() const;
};

class NFA {
public:
	NFA& operator*=(const NFA& rhs);
	NFA& operator+=(const NFA& rhs);
	NFA& operator|=(const NFA& rhs);

	NFA(char c = 'x');

	std::string toString() const;
	std::string DOTstring() const;
private:
//	std::map<int, State> states;
//	std::set<int> states;
	int numStates;
	std::set<Transition> transitions;
	int startState;
	std::set<int> acceptStates;

	void combine(const NFA&);
	NFA& operator+=(int);
};
