#pragma once
#include <string>
struct Operator_t {
	std::string representation;
	uint32_t precedence = 0;
	bool leftAssociativity = true;
	bool Prefix;
	bool Infix;
	bool Sufix;
};
