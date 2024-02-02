#pragma once

#include <string>
#include <unordered_map>

#include "ModuleInterface.h"

#define critical_panic() do{context.__critical_panic(__FILE__,__LINE__,__FUNCSIG__);}while(false)

enum class LogLevels :uint64_t 
{
	extendet = 0x1,
	project = 0x2,
	modules = 0x4,
	lexer = 0x8,
	parser = 0x10,
	phase_registerSymbols = 0x20,
	phase_defineSymbols = 0x40,
	phase_generateCode = 0x80,
};

struct CompilerContextData {
	std::string projectName;
	std::string moduleName;
	uint64_t logLevelMask = 0;
	//std::unordered_map<std::string,std::unordered_map<std::string,void*>> symbolTable; //symbolType,symbolName, value
	std::unordered_map<std::string,SymbolTypeInfo> symbolTypeList;
	std::unordered_map<SymbolTypeId,std::unordered_map<SymbolId,SymbolInfo>> symbolTable;
	uint32_t errorCount = 0;
};

class CompilerContextImpl {

public:
	CompilerContextData contextData;

	CompilerInterface getInterface();

	void LogInfo(uint64_t level, std::string msg);
	void LogInfo(uint64_t level, Position pos, std::string msg);
	void __critical_panic(std::string filename, int line, std::string func_name);
	void critical_InternalError(std::string errorMsg);
	//void critical_InternalError(Position position, std::string errorMsg);

	//Modules
	void critical_UnableToFindModule(std::string moduleName);
	void critical_CoruptedModule(std::string moduleName, std::string symbolName);
	void critical_Module_notAllowedPreambuleName(std::string moduleName, std::string symbolName);
	void critical_ModuleLoadError(std::string moduleName, std::string errorMsg, int errorCode);
	void critical_conflict_Lexer(std::string firstModule, std::string secondModule, std::string conflictedPreambule);
	void critical_conflict_Parser(std::string firstModule, std::string secondModule, std::string conflictedPreambule);

	//	//waring
	void critical_unrecognized_preambule(String name);

	//meta-parser
	void critical_notallowedPreambuleName(String file_data, char c);

	////symbol table
	void error_conflict_Symbol(std::string firstModule,std::string symbolName, String first,String second);
	//SymbolTypeId registerSymbolType(const char* symbolTypeName, SymbolSchema schama, print_symbol_fun_t print);
	//SymbolTypeInfo getSymbolTypeInfo( const char* symbolTypeName);

	//SymbolId registerSymbol(SymbolTypeId, String name);
	//bool defineSymbol( SymbolTypeId typeId, SymbolId id, void* definition);
	//SymbolInfo findSymbol(SymbolTypeId typeId, const char* name);
	//SymbolInfo findSymbolById(SymbolTypeId typeId, SymbolId id);

};
