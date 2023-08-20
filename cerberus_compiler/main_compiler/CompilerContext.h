#pragma once

#include <string>
#include <unordered_map>

#include "Preambule.h"

struct Position;
class Compiler;

#define critical_panic() do{context.__critical_panic(__FILE__,__LINE__,__FUNCSIG__);}while(false)


//TODO add need to register type of symbol before using it, function will return ID of symbol_type
struct CompilerContext {
	typedef std::string SymbolType;
	typedef String SymbolName;
	typedef std::unordered_map<SymbolName, void*, StringHash> SymbolList;

	struct SymbolRepo {
		std::unordered_map<SymbolType, SymbolList> vals;
		void add(std::string, SymbolName name);
		void define(std::string, SymbolName, void*);		
	};
	SymbolRepo symbols;


	void __critical_panic(std::string filename, int line, std::string func_name);
	void critical_InternalError(std::string errorMsg);
	void critical_InternalError(Position position, std::string errorMsg);

	//Modules
	void critical_UnableToFindModule(std::string moduleName);
	void critical_CoruptedModule(std::string moduleName, std::string symbolName);
	void critical_Module_notAllowedPreambuleName(std::string moduleName, std::string symbolName);
	void critical_ModuleLoadError(std::string moduleName, std::string errorMsg, int errorCode);
	void critical_conflict_Lexer(std::string firstModule, std::string secondModule, std::string conflictedPreambule);
	void critical_conflict_Parser(std::string firstModule, std::string secondModule, std::string conflictedPreambule);

	//meta-parser
	void critical_notallowedPreambuleName(String file_data, char c);

	//parser level
	void critical_syntaxError(Position, std::string errorMsg);
	void critical_UnMatched_parentice(Position position);
	void critical_Unexpected_Character(Position position, char c);
	void critical_Unexpected_NotASCII_character(Position position);

	//semantic on level of modules
	void error_UnknownType(Position, SymbolName);

	void LogInfo(int level, std::string msg);

	//varing
	void critical_unrecognized_preambule(String name);
private:
	std::string projectName;
	std::string moduleName;
	int curentLogLevel = 0;
	friend class Compiler;
};