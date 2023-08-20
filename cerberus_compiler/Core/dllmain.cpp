// dllmain.cpp : Definiuje punkt wejścia dla aplikacji DLL.
#include "pch.h"

#include "../main_compiler/ModuleInterface.h"

#include "Lexer.h"

int initModule() {
	return 0;
}



struct ASTNode {

};

pointerToAST parse_code (TokenizedStream* stream, CompilerContext& context) {
	return new ASTNode();
}

void freeAST(pointerToAST node) {
	delete node;
}

int phase(const Preambule& code, CompilerContext& context) {
	return 0;
}

//void phaseRegisterSymbols(const Preambule& code, CompilerContext& context)
//{
//	if (code.preambule == "type") {
//		auto res = new TypeDefinition();
//		res->name = parseTypeName(code, context);
//		context.symbols.add("type", res->name);
//
//		types.emplace(code.name, res);
//	}
//	else if (code.preambule == "procedure") {
//		auto def = parseProcedureName(code.name, context);
//		context.symbols.define("procedure", def->name, def);
//		procedures.emplace(code.name, def);
//	}
//}

extern "C" {
	__declspec(dllexport) int registerModule(Module_t* module, Lexer_t* lexer, Parser_t* parser) {

		module->Module_Version[0] = 0;
		module->Module_Version[1] = 0;
		module->Module_Version[2] = 1;
		module->Module_Version[3] = 0;
		module->ModuleName = "Core";
		module->ModuleLoadErrorMsg = "";
		module->headnledPreambules = { "procedure","type" };
		module->initModule = initModule;
		module->phase_register_Symbols = phase;
		module->phase_define_Symbols = phase;
		module->phase_generateCode = phase;
		module->finalizeModule = initModule;
		module->destroy = initModule;

		lexer->headnledPreambules = { "procedure","type" };
		lexer->lex = lexer_fun;
		lexer->init = initModule;
		lexer->destroy = initModule;

		parser->headnledPreambules = { "procedure","type" };
		parser->init = initModule;
		parser->parse_fun = parse_code;
		parser->freeAST = freeAST;
		parser->destroy = initModule;
		return 0;
	}
}



BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

