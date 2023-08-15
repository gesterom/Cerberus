#pragma once
#include <stdint.h>

#include "Preambule.h"
#include "CompilerContext.h"

struct Module_t;
struct Lexer_t;
struct Parser_t;

struct CompilerContext;

typedef int (*event_t)();
typedef int (*moduleHandler_phase_t)(const Preambule& code, CompilerContext& context);

struct Module_t
{
	uint32_t Struct_Version = 0 ;
	uint8_t Module_Version[4] = {0};
	const char* ModuleName = nullptr;
	const char* ModuleLoadErrorMsg = nullptr;
	std::vector<std::string> headnledPreambules;
	event_t initModule = nullptr;
	moduleHandler_phase_t phase_register_Symbols = nullptr;
	moduleHandler_phase_t phase_define_Symbols = nullptr;
	moduleHandler_phase_t phase_generateCode = nullptr;
	event_t finalizeModule = nullptr;
	event_t destroy = nullptr;
};

typedef TokenizedStream* (*lexer_fun_t)(const Preambule& code);

struct Lexer_t {
	std::vector<std::string> headnledPreambules;
	lexer_fun_t lex = nullptr;
	event_t init = nullptr;
	event_t destroy = nullptr;
};

typedef void* pointerToAST;
typedef pointerToAST(*parse_fun_t)(TokenizedStream*);
typedef void (*freeAST_fun_t)(pointerToAST);

struct Parser_t {
	std::vector<std::string> headnledPreambules;
	parse_fun_t parse_fun = nullptr;
	freeAST_fun_t freeAST = nullptr;
	event_t init = nullptr;;
	event_t destroy = nullptr;
};

typedef int (*registerModule_fun_t)(Module_t*, Lexer_t* lexer, Parser_t* parser);


//change CompilerContext to struct with function fields as interface;