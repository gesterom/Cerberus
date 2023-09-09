#include "CompilerContextImpl.h"
//
#include <iostream>
#include <cassert>

#include "Preambule.h"


const std::string criticalErrorMSG =
"\n"
"/-------------------------\\\n"
"[Encoutered Critical Error]\n"
"\\-------------------------/\n";
void CompilerContextImpl::LogInfo(uint32_t level, std::string msg)
{
	if (level < contextData.logLevel) {
		std::cout << " Log : " << msg << std::endl;
	}
}
//
//
//void CompilerContext::critical_Unexpected_NotASCII_character(Position position) {
//	std::cout << criticalErrorMSG << std::endl;
//	std::cout << "ERROR : unexpected ascii character at \n" << position << std::endl;
//	exit(-1);
//}
//
//void CompilerContext::critical_Unexpected_Character(Position position, char c) {
//	std::cout << criticalErrorMSG << std::endl;
//	std::cout << "ERROR : unexpected character ( " << c << " ) at \n" << position << std::endl;
//	exit(-2);
//}
//
//void CompilerContext::critical_UnMatched_parentice(Position position) {
//	std::cout << criticalErrorMSG << std::endl;
//	std::cout << "ERROR : unmached parenthes at \n" << position << std::endl;
//	exit(-3);
//}
//
void CompilerContextImpl::__critical_panic(std::string filename, int line, std::string func_name)
{
	std::cout << criticalErrorMSG << std::endl;
	std::cout << "Critical Compiler Panic" << std::endl;
	std::cout << filename << std::endl;
	std::cout << line << std::endl;
	std::cout << func_name << std::endl;
	exit(-128);
}

void CompilerContextImpl::critical_InternalError(std::string errorMsg)
{
	std::cout << "Module : " << contextData.moduleName << std::endl;
	std::cout << "Project : " << contextData.projectName << std::endl;
	std::cout << "Error : " << errorMsg << std::endl;
	exit(-4);
}
//
//void CompilerContextImpl::critical_InternalError(Position position, std::string errorMsg)
//{
//	std::cout << criticalErrorMSG << std::endl;
//	std::cout << "Module : " << this->moduleName << " Position : " << position << std::endl;
//	std::cout << "Critical Internal Error inside compilation module : " << this->moduleName << " at " << position << std::endl;
//	std::cout << "Error: " << errorMsg << std::endl;
//	exit(-4);
//}

//void CompilerContext::critical_syntaxError(Position position, std::string errorMsg)
//{
//	std::cout << criticalErrorMSG << std::endl;
//	std::cout << "Module : " << this->moduleName << " Position : " << position << std::endl;
//	std::cout << "Error: " << errorMsg << std::endl;
//	exit(-5);
//}

void CompilerContextImpl::critical_UnableToFindModule(std::string moduleName)
{
	std::cout << criticalErrorMSG << std::endl;
	std::cout << "Compilere was unable to Find Module : " << moduleName << std::endl;
	std::cout << "Required by Project : " << contextData.projectName << std::endl;
	exit(-6);
}

void CompilerContextImpl::critical_CoruptedModule(std::string moduleName, std::string symbolName)
{
	std::cout << criticalErrorMSG << std::endl;
	std::cout << "Compilere was unable to Find Symbol : " << symbolName << " inside Module " << moduleName << std::endl;
	std::cout << "Required by Project : " << contextData.projectName << std::endl;
	exit(-7);
}

void CompilerContextImpl::critical_ModuleLoadError(std::string moduleName, std::string errorMsg, int errorCode)
{
	std::cout << criticalErrorMSG << std::endl;
	std::cout << "Encountered error when loading module : " << moduleName << std::endl;
	std::cout << "Error Code : " << errorCode << std::endl;
	std::cout << "Error : " << errorMsg << std::endl;
	exit(-8);
}

void CompilerContextImpl::critical_conflict_Lexer(std::string firstModule, std::string secondModule, std::string conflictedPreambule)
{
	std::cout << criticalErrorMSG << std::endl;
	std::cout << "Conflict in included modules : " << firstModule << " and " << secondModule << std::endl;
	std::cout << "Error : Double definition of lexer for preambule : " << conflictedPreambule << std::endl;
	exit(-9);
}

void CompilerContextImpl::critical_conflict_Parser(std::string firstModule, std::string secondModule, std::string conflictedPreambule)
{
	std::cout << criticalErrorMSG << std::endl;
	std::cout << "Conflict in included modules : " << firstModule << " and " << secondModule << std::endl;
	std::cout << "Error : Double definition of parser for preambule : " << conflictedPreambule << std::endl;
	exit(-10);
}

void CompilerContextImpl::critical_notallowedPreambuleName(String file_data, char c)
{
	std::cout << criticalErrorMSG << std::endl;
	std::cout << "Encoutered character not allowed inside Preambule " << std::endl;
	std::cout << "Position : " << file_data.pos << std::endl;
	std::cout << "Character : [" << c << "]" << std::endl;
	exit(-11);
}
void CompilerContextImpl::critical_Module_notAllowedPreambuleName(std::string moduleName, std::string symbolName)
{
	std::cout << criticalErrorMSG << std::endl;
	std::cout << "Encoutered error when trying to load module : " << moduleName << std::endl;
	std::cout << "Module try to register not allowed preambule : " << symbolName << std::endl;
	exit(-12);
}
void CompilerContextImpl::critical_unrecognized_preambule(String name)
{
	std::cout << criticalErrorMSG << std::endl;
	std::cout << "Not recognized preambule" << std::endl;
	std::cout << "Preambule " << name.val << " at position " << name.pos << std::endl;
	exit(-13);
}
//
//void CompilerContext::error_UnknownType(Position position, SymbolName name)
//{
//	std::cerr << "ERROR : unknown type " << name.val << " at \n" << position << std::endl;
//}
//
//void CompilerContext::LogInfo(int level, std::string msg)
//{
//	if (level >= this->curentLogLevel) {
//		std::cout << msg;
//	}
//}
//
//
//
//void CompilerContext::SymbolRepo::add(std::string symbol_type, SymbolName name)
//{
//	auto it = this->vals.find(symbol_type);
//	if (it == this->vals.end()) {
//		auto a = this->vals.emplace(symbol_type, SymbolList());
//		it = a.first;
//	}
//	auto itt = it->second.find(name);
//	if (itt == it->second.end()) {
//		it->second.emplace(name, nullptr);
//	}
//}
//
//void CompilerContext::SymbolRepo::define(std::string symbol_type, SymbolName name, void* ptr)
//{
//	auto it = this->vals.find(symbol_type);
//	if (it == this->vals.end()) {
//		auto a = this->vals.emplace(symbol_type, SymbolList());
//		it = a.first;
//	}
//	auto itt = it->second.find(name);
//	if (itt == it->second.end()) {
//		it->second.emplace(name, ptr);
//	}
//	else {
//		itt->second = ptr;
//	}
//}

void critical_error_msg_impl(void* c, CriticalErrorType type, Position pos, const char* msg) {
	CompilerContextData* context = (CompilerContextData*)c;
	std::cout << criticalErrorMSG << std::endl;
	std::cout << "Project name : " << context->projectName << std::endl;
	std::cout << "Module Name : " << context->moduleName << std::endl;
	std::cout << "Module Reported critical error." << std::endl;
	std::cout << "Position : " << pos << std::endl;
	std::cout << "Error : " << msg;
	exit(-20);
}

void error_msg_impl(void* c, ErrorType type, Position pos, const char* msg) {
	CompilerContextData* context = (CompilerContextData*)c;
	std::cout << "Project name : " << context->projectName << std::endl;
	std::cout << "Module Name : " << context->moduleName << std::endl;
	std::cout << "Module Reported error." << std::endl;
	std::cout << "Position : " << pos << std::endl;
	std::cout << "Error : " << msg;
	exit(-21);
}

void _todo_() {
	std::cout << "Function not implemented inside CompilerContextImpl.cpp\n";
	exit(-100);
}

SymbolTypeId _todo_(CompilerContext* context, const char* symbolTypeName, SymbolSchema schama, print_symbol_fun_t print) {
	std::cout << "Function not implemented inside CompilerContextImpl.cpp\n";
	exit(-100);
}
SymbolTypeInfo _todo_(CompilerContext* context, const char* symbolTypeName) {
	std::cout << "Function not implemented inside CompilerContextImpl.cpp\n";
	exit(-100);
}

SymbolId _todo_(CompilerContext* context, SymbolTypeId, String name) {
	std::cout << "Function not implemented inside CompilerContextImpl.cpp\n";
	exit(-100);
}
bool _todo_(CompilerContext* context, SymbolTypeId typeId, SymbolId id, void* definition) {
	std::cout << "Function not implemented inside CompilerContextImpl.cpp\n";
	exit(-100);
}
SymbolInfo _todo_(CompilerContext* context, SymbolTypeId typeId, const char* name) {
	std::cout << "Function not implemented inside CompilerContextImpl.cpp\n";
	exit(-100);
}
SymbolInfo _todo_(CompilerContext* context, SymbolTypeId typeId, SymbolId id) {
	std::cout << "Function not implemented inside CompilerContextImpl.cpp\n";
	exit(-100);
}

//errorHandling and logging
void _todo_(CompilerContext* context, CriticalErrorType type, Position pos, const char* msg) {
	std::cout << "Function not implemented inside CompilerContextImpl.cpp\n";
	exit(-100);
}
void _todo_(CompilerContext* context, ErrorType type, Position pos, const char* msg) {
	std::cout << "Function not implemented inside CompilerContextImpl.cpp\n";
	exit(-100);
}
void _todo_(CompilerContext* context, WarningType type, Position pos, const char* msg) {
	std::cout << "Function not implemented inside CompilerContextImpl.cpp\n";
	exit(-100);
}
void _todo_(CompilerContext* context, uint16_t level, Position pos, const char* msg) {
	std::cout << "Function not implemented inside CompilerContextImpl.cpp\n";
	exit(-100);
}

CompilerInterface CompilerContextImpl::getInterface()
{
	CompilerInterface res;
	res.context = nullptr;
	res.critical_error_msg = _todo_;
	res.defineSymbol = _todo_;
	res.findSymbol = _todo_;
	res.findSymbolById = _todo_;
	res.getSymbolTypeInfo = _todo_;
	res.log_msg = _todo_;
	res.registerSymbol = _todo_;
	res.registerSymbolType = _todo_;
	res.warning_msg = _todo_;


	res.context = &(this->contextData);
	res.critical_error_msg = critical_error_msg_impl;
	res.error_msg = error_msg_impl;

	return res;
}
