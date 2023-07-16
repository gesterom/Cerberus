#pragma once

#include <string>
#include <unordered_map>

struct SymbolsRepository {
	typedef std::string SymbolType;
	typedef std::string SymbolName;
	typedef std::string SymbolDefinition;
	typedef std::unordered_map<SymbolName, SymbolDefinition> SymbolList;
	std::unordered_map<SymbolType, SymbolList> symbols;
};