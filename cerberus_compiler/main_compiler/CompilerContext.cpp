#include "CompilerContext.h"

#include <iostream>

#include "Preambule.h"

const std::string criticalErrorMSG =
"\n"
"/-------------------------\\\n"
"[Encoutered Critical Error]\n"
"\\-------------------------/\n";


void CompilerContext::critical_Unexpected_NotASCII_character(Position position) {
	std::cout << criticalErrorMSG << std::endl;
	std::cout << "ERROR : unexpected ascii character at \n" << position << std::endl;
	exit(-1);
}

void CompilerContext::critical_Unexpected_Character(Position position, char c) {
	std::cout << criticalErrorMSG << std::endl;
	std::cout << "ERROR : unexpected character ( " << c << " ) at \n" << position << std::endl;
	exit(-2);
}

void CompilerContext::critical_UnMatched_parentice(Position position) {
	std::cout << criticalErrorMSG << std::endl;
	std::cout << "ERROR : unmached parenthes at \n" << position << std::endl;
	exit(-3);
}

void CompilerContext::__critical_panic(std::string filename, int line, std::string func_name)
{
	std::cout << criticalErrorMSG << std::endl;
	std::cout << "Critical Compiler Panic" << std::endl;
	std::cout << filename << std::endl;
	std::cout << line << std::endl;
	std::cout << func_name << std::endl;
	exit(-128);
}

void CompilerContext::critical_InternalError(std::string errorMsg)
{
	std::cout << "Module : " << this->moduleName << std::endl;
	std::cout << "Project : " << this->projectName << std::endl;
	std::cout << "Error : " << errorMsg << std::endl;
	exit(-4);
}

void CompilerContext::critical_InternalError(Position position, std::string errorMsg)
{
	std::cout << criticalErrorMSG << std::endl;
	std::cout << "Module : " << this->moduleName << " Position : " << position << std::endl;
	std::cout << "Critical Internal Error inside compilation module : " << this->moduleName << " at " << position << std::endl;
	std::cout << "Error: " << errorMsg << std::endl;
	exit(-4);
}

void CompilerContext::critical_syntaxError(Position position, std::string errorMsg)
{
	std::cout << criticalErrorMSG << std::endl;
	std::cout << "Module : " << this->moduleName << " Position : " << position << std::endl;
	std::cout << "Error: " << errorMsg << std::endl;
	exit(-5);
}

void CompilerContext::critical_UnableToFindModule(std::string moduleName)
{
	std::cout << criticalErrorMSG << std::endl;
	std::cout << "Compilere was unable to Find Module : " << moduleName << std::endl;
	std::cout << "Required by Project : " << projectName << std::endl;
	exit(-6);
}

void CompilerContext::critical_CoruptedModule(std::string moduleName, std::string symbolName)
{
	std::cout << criticalErrorMSG << std::endl;
	std::cout << "Compilere was unable to Find Symbol : " << symbolName << " inside Module " << moduleName << std::endl;
	std::cout << "Required by Project : " << projectName << std::endl;
	exit(-7);
}



void CompilerContext::critical_ModuleLoadError(std::string moduleName, std::string errorMsg, int errorCode)
{
	std::cout << criticalErrorMSG << std::endl;
	std::cout << "Encountered error when loading module : " << moduleName << std::endl;
	std::cout << "Error Code : " << errorCode << std::endl;
	std::cout << "Error : " << errorMsg << std::endl;
	exit(-8);
}

void CompilerContext::critical_conflict_Lexer(std::string firstModule, std::string secondModule, std::string conflictedPreambule)
{
	std::cout << criticalErrorMSG << std::endl;
	std::cout << "Conflict in included modules : " << firstModule << " and " << secondModule << std::endl;
	std::cout << "Error : Double definition of lexer for preambule : " << conflictedPreambule << std::endl;
	exit(-9);
}

void CompilerContext::critical_conflict_Parser(std::string firstModule, std::string secondModule, std::string conflictedPreambule)
{
	std::cout << criticalErrorMSG << std::endl;
	std::cout << "Conflict in included modules : " << firstModule << " and " << secondModule << std::endl;
	std::cout << "Error : Double definition of parser for preambule : " << conflictedPreambule << std::endl;
	exit(-10);
}

void CompilerContext::critical_notallowedPreambuleName(String file_data, char c)
{
	std::cout << criticalErrorMSG << std::endl;
	std::cout << "Encoutered character not allowed inside Preambule " << std::endl;
	std::cout << "Position : " << file_data.pos << std::endl;
	std::cout << "Character : [" << c << "]" << std::endl;
	exit(-11);
}
void CompilerContext::critical_Module_notAllowedPreambuleName(std::string moduleName, std::string symbolName)
{
	std::cout << criticalErrorMSG << std::endl;
	std::cout << "Encoutered error when trying to load module : " << moduleName << std::endl;
	std::cout << "Module try to register not allowed preambule : " << symbolName << std::endl;
	exit(-12);
}
void CompilerContext::critical_unrecognized_preambule(String name)
{
	std::cout << criticalErrorMSG << std::endl;
	std::cout << "Not recognized preambule" << std::endl;
	std::cout << "Preambule " << name.val << " at position " << name.pos << std::endl;
	exit(-13);
}

void CompilerContext::error_UnknownType(Position position, SymbolName name)
{
	std::cerr << "ERROR : unknown type " << name.val << " at \n" << position << std::endl;
}

void CompilerContext::LogInfo(int level, std::string msg)
{
	if (level >= this->curentLogLevel) {
		std::cout << msg;
	}
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
