#include "ExpressionManager.h"
#include <string>
#include <stack>
#include <sstream>

const std::string ExpressionManager::OPERATORS = "*|+";

NFA ExpressionManager::equivalentNFA(std::string regex) {
	return postfixEvaluate(infixToPostfix(infixFilled(regex)));
}

NFA ExpressionManager::postfixEvaluate(std::string postfixExpression) {
	std::stack<NFA> myMem;
	for (char currToken : postfixExpression) {
		if (isOperator(currToken)) {
			operate(myMem, currToken);
		} else {
			myMem.push(NFA(currToken));
		}
	}
	if (myMem.size() != 1) return NFA();
	return myMem.top();
}

std::string ExpressionManager::infixFilled(std::string orig) {
	std::string res;
	for (char c : orig) {
		if (c != ' ') res.push_back(c);
	}
	while (res.find("()") != std::string::npos) {
		res.erase(res.find("()"),2);
	}
	for (int i = 0; i < res.length(); i++) {
		if (res.at(i) == '*') {
			res.insert(i + 1, "X");
		}
	}
	for (int i = 1; i < res.length(); i++) {
		char last = res.at(i-1), curr = res.at(i);
		if (!isOperator(last) && last != '(' && !isOperator(curr) && curr != ')') {
			res.insert(i, "+");
			i++;
		}
	}
	return res;
}

std::string ExpressionManager::infixToPostfix(std::string infixExpression) {
	std::stack<char> myMem;
	std::ostringstream output;
	int currPrec = 0;
	for (char currToken : infixExpression) {
		if (isLeftBrace(currToken)) {
			myMem.push(currToken);
		} else if (isRightBrace(currToken)) {
			while (!isLeftBrace(myMem.top())) {
				output << myMem.top(); myMem.pop();
			}
			myMem.pop();
		} else if (isOperator(currToken)) {
			currPrec = precedence(currToken);
			while (!myMem.empty() && currPrec <= precedence(myMem.top())) {
				output << myMem.top(); myMem.pop();
			}
			myMem.push(currToken);
		} else {
			output << currToken;
		}
	}
	while (!myMem.empty()) {
		output << myMem.top(); myMem.pop();
	}
	return output.str();
}

bool ExpressionManager::isLeftBrace(char check) {return check == '(';}
bool ExpressionManager::isRightBrace(char check) {return check == ')';}
bool ExpressionManager::isOperator(char check) {return OPERATORS.find(check) != std::string::npos;}

int ExpressionManager::precedence(char myOp) {
	switch (myOp) {
	case ')':
		return 4;
		break;
	case '*':
		return 3;
		break;
	case '+':
		return 2;
		break;
	case '|':
		return 1;
		break;
	case '(':
		return 0;
		break;
	default:
		return -1;
		break;
	}
}

void ExpressionManager::operate(std::stack<NFA>& myMem, char myOp) {
	if (myMem.size() < 2) return;
	NFA temp2 = myMem.top(); myMem.pop();
	NFA temp1 = myMem.top(); myMem.pop();

	switch (myOp) {
	case '*':
		temp1 *= temp2;
		break;
	case '+':
		temp1 += temp2;
		break;
	case '|':
		temp1 |= temp2;
		break;
	default:
		return;
		break;
	}
	myMem.push(temp1);
}
