#include "NFA.h"
#include <sstream>
//#include <iostream>

//State::State(std::string nlabel): label(nlabel) {}

Transition::Transition(int nfrom, int nto, char nlabel) : fromState(nfrom), toState(nto), label(nlabel) {}

std::string NFA::DOTstring() const {
	const std::string DOT_PREFIX = "digraph NFA {\n\trankdir=LR;\n\tsize=\"8,5\"\n";
	std::ostringstream res;
	res << DOT_PREFIX << "\tnode [shape = doublecircle];";
	for (int as : acceptStates) {
		res << " q" << as;
	}
	res << ";\n" << "\tnode [shape = circle];\n";
	for (Transition t : transitions) {
		res << t.DOTstring();
	}
	res << "}\n";
	return res.str();
}

/*
digraph finite_state_machine {
	rankdir=LR;
	size="8,5"
	node [shape = doublecircle]; LR_0 LR_3 LR_4 LR_8;
	node [shape = circle];
	LR_0 -> LR_2 [ label = "SS(B)" ];
	LR_0 -> LR_1 [ label = "SS(S)" ];
	LR_1 -> LR_3 [ label = "S($end)" ];
	LR_2 -> LR_6 [ label = "SS(b)" ];
	LR_2 -> LR_5 [ label = "SS(a)" ];
	LR_2 -> LR_4 [ label = "S(A)" ];
	LR_5 -> LR_7 [ label = "S(b)" ];
	LR_5 -> LR_5 [ label = "S(a)" ];
	LR_6 -> LR_6 [ label = "S(b)" ];
	LR_6 -> LR_5 [ label = "S(a)" ];
	LR_7 -> LR_8 [ label = "S(b)" ];
	LR_7 -> LR_5 [ label = "S(a)" ];
	LR_8 -> LR_6 [ label = "S(b)" ];
	LR_8 -> LR_5 [ label = "S(a)" ];
}
*/

std::string Transition::DOTstring() const {
	std::ostringstream res;
	res << "\tq" << fromState << " -> q" << toState << " [ label = \"" << label << "\" ];\n";
	return res.str();
}

std::string Transition::toString() const {
	std::ostringstream res;
	res << fromState << "->" << toState << " by " << label;
	return res.str();
}

std::string NFA::toString() const {
	std::ostringstream res;
	res << "Num states:\n  " << numStates << std::endl
		<< "Start state:\n  " << startState << std::endl
		<< "Transitions:\n";
	for (Transition t : transitions) {
		res << "  " << t.toString() << std::endl;
	}
	res << "Accept states:\n";
	for (int i : acceptStates) {
		res << "  " << i << std::endl;
	}
	res << "\n\n";
	return res.str();
}

NFA::NFA(char c) : numStates(2), startState(0) {
	acceptStates.insert(1);
	transitions.insert(Transition(0, 1, c));
//	transitions.insert(Transition(1, 2, c));
}

NFA& NFA::operator|=(const NFA& rhs) {
	*this += 1;
	NFA mrhs = rhs;
	mrhs += this->numStates;
	transitions.insert(Transition(0, 1, '~'));
	transitions.insert(Transition(0, mrhs.startState, '~'));
	this->combine(mrhs);
	startState = 0;
	return *this;
}

NFA& NFA::operator*=(const NFA& rhs) {
	*this += 1;
	for (int i : acceptStates) {
		transitions.insert(Transition(i, this->startState, '~'));
	}
	transitions.insert(Transition(0, this->startState, '~'));
	startState = 0;
//	acceptStates.clear();
	acceptStates.insert(0);
	return *this;
}

NFA& NFA::operator+=(const NFA& rhs) {
	NFA mrhs = rhs;
	mrhs += this->numStates;

//std::cout << "mrhs:\n" << mrhs.toString() << std::endl;
	for (int i : acceptStates) {
		transitions.insert(Transition(i, mrhs.startState, '~'));
//std::cout << "inserting transition: " << Transition(i, mrhs.startState, '~').toString() << std::endl;
	}
	acceptStates.clear();
	this->combine(mrhs);
	return *this;
}

void NFA::combine(const NFA& rhs) {
	if (rhs.numStates > this->numStates) {
		this->numStates = rhs.numStates;
	}
/*	for (Transition t : rhs.transitions) {
		std::cout << "Should shift over the t: " << t.toString() << std::endl;
	}*/
	this->transitions.insert(rhs.transitions.begin(), rhs.transitions.end());
	this->acceptStates.insert(rhs.acceptStates.begin(), rhs.acceptStates.end());
}

NFA& NFA::operator+=(int rhs) {
	numStates += rhs;
	startState += rhs;
	std::set<Transition> nts;
	for (Transition t : transitions) {
		nts.insert(t + rhs);
	}
	transitions = nts;
	std::set<int> nas;
	for (int i : acceptStates) {
		nas.insert(i + rhs);
	}
	acceptStates = nas;
	return *this;
}

Transition Transition::operator+(int rhs) const {
	return Transition(fromState + rhs, toState + rhs, label);
}

bool operator<(const Transition lhs, const Transition rhs) {
	if (lhs.fromState < rhs.fromState) {
		return true;
	}
	if (rhs.fromState < lhs.fromState) {
		return false;
	}
	if (lhs.toState < rhs.toState) {
		return true;
	}
	if (rhs.toState < lhs.toState) {
		return false;
	}
	if (lhs.label < rhs.label) {
		return true;
	}
	if (rhs.label < lhs.label) {
		return false;
	}
	return false;
//	std::cout << "is " << lhs.toString() << " less than " << rhs.toString() << "? " << ((lhs.fromState < rhs.fromState || (lhs.toState < rhs.toState || lhs.label < rhs.label))?"yes":"no") << std::endl;
//	return lhs.fromState < rhs.fromState || (lhs.toState < rhs.toState || lhs.label < rhs.label);
}
