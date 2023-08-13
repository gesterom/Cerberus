#pragma once

#include <string>
#include <unordered_map>

#include "String.h"

struct Position;

struct CompilerContext {
	typedef std::string SymbolType;
	typedef String SymbolName;
	typedef std::unordered_map<SymbolName, void*,StringHash> SymbolList;

	struct SymbolRepo {
		std::unordered_map<SymbolType, SymbolList> vals;
		void add(std::string, SymbolName name);
		void define(std::string,SymbolName,void*);
	};
	SymbolRepo symbols;

	void critical_Unexpected_NotASCII_character(Position position) ;

	void critical_Unexpected_Character(Position position,char c);

	void critical_UnMatched_parentice(Position position) ;

	void critical_InternalError(Position position,std::string moduleName,std::string errorMsg);
	void critical_syntaxError(Position,std::string moduleName,std::string errorMsg);

	void error_UnknownType(Position,SymbolName);
};