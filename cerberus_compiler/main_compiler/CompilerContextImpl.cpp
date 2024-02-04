#include "CompilerContextImpl.h"
//
#include <iostream>
#include <string>
#include <cassert>

#include "String.h"


const std::string criticalErrorMSG =
"\n"
"/-------------------------\\\n"
"[Encoutered Critical Error]\n"
"\\-------------------------/\n";

const std::string errorMSG =
"\n<=======   Error   =======>\n";

std::string leftPading(int n, std::string a) {
	while (a.size() < n) {
		a = " " + a;
	}
	return a;
}

void CompilerContextImpl::LogInfo(uint64_t level, std::string msg)
{
	if (contextData.logLevelMask & (uint64_t)LogLevels::extendet) {
		if (level & contextData.logLevelMask & (~1))
			std::cout << "[" << leftPading(4, std::to_string(level)) << "]" << " Log : " << msg << std::endl;
	}
	else {
		if (((level & (uint64_t)LogLevels::extendet) == 0) and (level & contextData.logLevelMask))
			std::cout << "[" << leftPading(4, std::to_string(level)) << "]" << " Log : " << msg << std::endl;
	}
}
void CompilerContextImpl::LogInfo(uint64_t level, Position pos, std::string msg)
{
	if (contextData.logLevelMask & (uint64_t)LogLevels::extendet) {
		if (level & contextData.logLevelMask & (~1))
			std::cout << "[" << leftPading(4, std::to_string(level)) << "]" << pos << " Log : " << msg << std::endl;
	}
	else {
		if (((level & (uint64_t)LogLevels::extendet) == 0) and (level & contextData.logLevelMask))
			std::cout << "[" << leftPading(4, std::to_string(level)) << "]" << pos << " Log : " << msg << std::endl;
	}
}

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

void CompilerContextImpl::error_conflict_Symbol(std::string firstModule, std::string symbolName, String first, String second)
{
	std::cout << errorMSG << std::endl;
	std::cout << "Conflict in symbols"<< std::endl;
	std::cout << "Module registering : "<<firstModule<<std::endl;
	std::cout << "Symbol type : " << symbolName<<std::endl;
	std::cout << "First encounter : " << first.val<< " "<<first.pos  << std::endl;
	std::cout << "Second encounter : " << second.val << " " << second.pos << std::endl;
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

void critical_error_msg_impl(void* c, CriticalErrorType type, Position pos, const char* msg) {
	CompilerContextData* context = (CompilerContextData*)c;
	std::cout << criticalErrorMSG << std::endl;
	std::cout << "Project name : " << context->projectName << std::endl;
	std::cout << "Module Name : " << context->moduleName << std::endl;
	std::cout << "Module Reported critical error." << std::endl;
	std::cout << "Position : " << pos << std::endl;
	std::cout << "ErrorType : " << (uint32_t)type << std::endl;
	std::cout << "Error : " << msg;
	exit(-20);
}

void error_msg_impl(void* c, ErrorType type, Position pos, const char* msg) {
	CompilerContextData* context = (CompilerContextData*)c;
	std::cout << "\n<=======   Error   =======>" << std::endl;
	std::cout << "Project name : " << context->projectName << std::endl;
	std::cout << "Module Name : " << context->moduleName << std::endl;
	std::cout << "Module Reported error." << std::endl;
	std::cout << "Position : " << pos << std::endl;
	std::cout << "ErrorType : " << (uint32_t)type << std::endl;
	std::cout << "Error : " << msg;
	context->errorCount++;
}

void warning_msg_impl(void* c, WarningType type, Position pos, const char* msg) {
	CompilerContextData* context = (CompilerContextData*)c;
	std::cout << "\n<-------   Warning   ------->" << std::endl;
	std::cout << "Project name : " << context->projectName << std::endl;
	std::cout << "Module Name : " << context->moduleName << std::endl;
	std::cout << "Module Reported error." << std::endl;
	std::cout << "Position : " << pos << std::endl;
	std::cout << "ErrorType : " << (uint32_t)type << std::endl;
	std::cout << "Warning : " << msg;
}

SymbolTypeId registerSymbolType_impl(CompilerContext* context, const char* symbolTypeName, SymbolSchema schama, print_symbol_fun_t print) {

	CompilerContextData* contextData = (CompilerContextData*)context;

	SymbolTypeInfo res{};
	res.id = contextData->symbolTypeList.size();
	res.originModule = contextData->moduleName.c_str();
	res.print = print;
	res.schema = schama;
	res.symbolTypeName = symbolTypeName;
	res.found = true;
	contextData->symbolTypeList.emplace(std::make_pair(std::string(symbolTypeName), res));
	return res.id;

	//return res;
}

SymbolTypeInfo getSymbolTypeInfo_impl(CompilerContext* context, const char* symbolTypeName) {
	//context->contextData.;
	CompilerContextData* contextData = (CompilerContextData*)context;
	auto it = contextData->symbolTypeList.find(std::string(symbolTypeName));
	if (it != contextData->symbolTypeList.end()) {
		return contextData->symbolTypeList.at(std::string(symbolTypeName));
	}
	else {
		SymbolTypeInfo res;
		res.found = false;
		return res;
	}
}
SymbolTypeInfo getSymbolTypeInfoByID_impl(CompilerContext* context, SymbolTypeId id) {
	
	CompilerContextData* contextData = (CompilerContextData*)context;

	auto it = std::find_if(contextData->symbolTypeList.begin(),contextData->symbolTypeList.end(),
		[&id](const std::pair<std::string,SymbolTypeInfo>& a)->bool{
			return a.second.id == id;
		}
	);
	if(it != contextData->symbolTypeList.end()){
		return it->second;
	}
	SymbolTypeInfo res;
	res.found = false;
	return res;
}


SymbolId registerSymbolByID_impl(CompilerContext* context, SymbolTypeId id, String name) {
	CompilerContextData* contextData = (CompilerContextData*)context;
	SymbolId res = contextData->symbolTable[id].size();
	SymbolInfo info;
	info.data = nullptr;
	info.data_size = 0;
	info.found = true;
	info.id = res;
	info.originModule = contextData->moduleName.c_str();
	info.symbolName = name;
	info.typeId = id;
	auto it = std::find_if(contextData->symbolTable[id].begin(), contextData->symbolTable[id].end(),
		[&name](const std::pair<SymbolId,SymbolInfo>& a)-> bool {
			return a.second.symbolName == name;
		}
	);
	if (it != contextData->symbolTable[id].end()) {
		CompilerContextData* contextData = (CompilerContextData*)context;
		CompilerContextImpl* impl = new CompilerContextImpl();
		impl->contextData = *contextData;
		auto typeInfo = getSymbolTypeInfoByID_impl(context,id);
		impl->error_conflict_Symbol(contextData->moduleName, typeInfo.symbolTypeName , it->second.symbolName, name);
		delete impl;
	}
	contextData->symbolTable[id].insert(std::make_pair(res,info));
	return res;
}

SymbolId registerSymbol_impl(CompilerContext* context, const char* symbolTypeName, String name) {
	CompilerContextData* contextData = (CompilerContextData*)context;
	SymbolTypeId id = 0;
	for (const auto& i : contextData->symbolTypeList) {
		if (i.second.symbolTypeName == std::string(symbolTypeName)) {
			id = i.second.id;
		}
	}
	return registerSymbolByID_impl(context,id,name);
}


bool defineSymbolByID_impl(CompilerContext* context, SymbolTypeId typeId, SymbolId id, void* definition, size_t data_size) {
	CompilerContextData* contextData = (CompilerContextData*)context;
	contextData->symbolTable[typeId][id].data = definition;
	contextData->symbolTable[typeId][id].data_size = data_size;
	return true;
}

bool defineSymbol_impl(CompilerContext* context, SymbolTypeId typeId, String symbolName, void* definition, size_t data_size) {
	CompilerContextData* contextData = (CompilerContextData*)context;

	SymbolId id = -1;
	for (const auto& i : contextData->symbolTable[typeId]) {
		if (i.second.symbolName.val == symbolName.val) {
			id = i.second.id;
		}
	}
	if (id == -1) {
		id = registerSymbolByID_impl(context,typeId,symbolName);
	}
	return defineSymbolByID_impl(context,typeId,id,definition,data_size);
}


SymbolInfo findSymbolById_impl(CompilerContext* context, SymbolTypeId typeId, SymbolId id) {
	CompilerContextData* contextData = (CompilerContextData*)context;
	return contextData->symbolTable[typeId][id];
}

SymbolInfo findSymbol_impl(CompilerContext* context, SymbolTypeId typeId, const char* name) {
	CompilerContextData* contextData = (CompilerContextData*)context;
	SymbolId id = -1;
	for (const auto& i : contextData->symbolTable[typeId]) {
		if (i.second.symbolName.val == std::string(name)) {
			id = i.second.id;
		}
	}
	if(id == -1) return SymbolInfo();

	return findSymbolById_impl(context,typeId,id);
}

void log_msg(void* c, uint64_t level, const char* msg) {
	CompilerContextData* contextData = (CompilerContextData*)c;
	CompilerContextImpl* impl = new CompilerContextImpl();
	impl->contextData = *contextData;
	//LogInfo(impl,level, msg);
	impl->LogInfo(level, msg);
	delete impl;
}

void log_msg_pos(void* c, uint64_t level, Position pos, const char* msg) {
	CompilerContextData* contextData = (CompilerContextData*)c;
	CompilerContextImpl* impl = new CompilerContextImpl();
	impl->contextData = *contextData;
	//LogInfo(impl, level, msg);
	impl->LogInfo(level, pos, msg);
	delete impl;
}

CompilerContext* context;

CompilerInterface CompilerContextImpl::getInterface()
{
	CompilerInterface res;

	res.context = &(this->contextData);

	res.registerSymbolType = registerSymbolType_impl;
	res.getSymbolTypeInfo = getSymbolTypeInfo_impl;
	res.getSymbolTypeInfoByID = getSymbolTypeInfoByID_impl;

	res.registerSymbol = registerSymbol_impl;
	res.registerSymbolByID = registerSymbolByID_impl;

	res.defineSymbol = defineSymbol_impl;
	res.defineSymbolByID = defineSymbolByID_impl;


	res.findSymbol = findSymbol_impl;
	res.findSymbolById = findSymbolById_impl;

	//errorHandling and logging
	res.critical_error_msg = critical_error_msg_impl;
	res.error_msg = error_msg_impl;
	res.warning_msg = warning_msg_impl;
	res.log_msg = log_msg;
	res.log_msg_pos = log_msg_pos;

	return res;
}
