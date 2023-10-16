#pragma once


#include <vector>
#include <string>
#include "Operator_t.h"

extern const std::string seperators;
extern const std::string parentheses;

extern const std::vector<std::string> keywords;

extern std::vector<Operator_t> operators;
//extern std::vector<std::string> hidenOperators;
extern std::vector<std::string> prefixOperators;
extern std::vector<std::string> infixOperators;
extern std::vector<std::string> sufixOperators;