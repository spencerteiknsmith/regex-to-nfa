#pragma once

#include <iostream>
#include <string>
#include <stack>

#include "NFA.h"

class ExpressionManager {
public:
	static NFA equivalentNFA(std::string regex);
private:
	static std::string infixToPostfix(std::string infixExpression);
	static NFA postfixEvaluate(std::string postfixExpression);

	static std::string infixFilled(std::string);
	static bool isLeftBrace(char check);
	static bool isRightBrace(char check);
	static bool isOperator(char check);
//	static bool isInt(std::string check);
	static int precedence(char myOp);
	static void operate(std::stack<NFA>& myMem, char myOp);
//	static bool operateNoEval(std::stack<std::string>& myMem, std::string myOp);
	static const std::string OPERATORS;
};
