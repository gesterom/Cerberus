#pragma once


#include <vector>
#include <string>

extern const std::string seperators;
extern const std::string parentheses;

extern const std::vector<std::string> keywords;
struct Operator_t {
	std::string representation;
	int32_t precedence = 0;
	bool leftAssociativity = true;
};
extern std::vector<Operator_t> operators;
extern std::vector<std::string> prefixOperators;
extern std::vector<std::string> infixOperators;
extern std::vector<std::string> sufixOperators;