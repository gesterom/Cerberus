#include "CompilerContext.h"

#include <iostream>

#include "CodeFragment.h"

void CompilerContext::critical_Unexpected_NotASCII_character(Position position) {
	std::cout << "ERROR : unexpected ascii character at \n" << position << std::endl;
	exit(-1);
}

void CompilerContext::critical_Unexpected_Character(Position position, char c) {
	std::cout << "ERROR : unexpected character ( " << c << " ) at \n" << position << std::endl;
	exit(-2);
}

void CompilerContext::critical_UnMatched_parentice(Position position) {
	std::cout << "ERROR : unmached parenthes at \n" << position << std::endl;
	exit(-3);
}

void CompilerContext::critical_InternalError(Position position, std::string moduleName, std::string errorMsg)
{
	std::cout << "Critical Internal Error inside compilation module : " << moduleName << " at " << position << std::endl;
	std::cout << "Error: " << errorMsg << std::endl;
	exit(-4);
}

void CompilerContext::error_UnknownType(Position position, SymbolName name)
{
	std::cerr << "ERROR : unknown type " << name.val << "at \n" << position << std::endl;
}

void CompilerContext::SymbolRepo::add(std::string symbol_type, SymbolName name)
{
	auto it = this->vals.find(symbol_type);
	if (it == this->vals.end()) {
		auto a = this->vals.emplace(symbol_type, SymbolList());
		it = a.first;
	}
	auto itt = it->second.find(name);
	if (itt == it->second.end()) {
		it->second.emplace(name, nullptr);
	}
}

void CompilerContext::SymbolRepo::define(std::string symbol_type, SymbolName name, void* ptr)
{
	auto it = this->vals.find(symbol_type);
	if (it == this->vals.end()) {
		auto a = this->vals.emplace(symbol_type, SymbolList());
		it = a.first;
	}
	auto itt = it->second.find(name);
	if (itt == it->second.end()) {
		it->second.emplace(name, ptr);
	}
	else {
		itt->second = ptr;
	}
}
