#include <iostream>
#include "ExpressionManager.h"

int main(int argc, char* argv[]) {
	std::string regex = "ab*|a*b";
	if (argc >= 2) {
		regex = std::string(argv[1]);
	}
	NFA res = ExpressionManager::equivalentNFA(regex);

	std::cout << res.DOTstring() << std::endl;
	return 0;
}
