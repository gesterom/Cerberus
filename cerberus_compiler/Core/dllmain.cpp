// dllmain.cpp : Definiuje punkt wejścia dla aplikacji DLL.
#include "pch.h"

#include "../main_compiler/ModuleInterface.h"

#include "Lexer.h"
#include "Parser.h"

#include "Consts.h"
#include <algorithm>

int initLexer() {

	std::sort(operators.begin(), operators.end(), [](Operator_t a, Operator_t b) -> bool {
		return a.representation.size() > b.representation.size();
		});

	return 0;
}

int nopPhase() {
	return 0;
}

pointerToAST parse_code(const Preambule& code, CompilerInterface* context) {
	if (code.preambule_name.val == "procedure") {
		Procedure* res = new Procedure;
		*res = parseProcedure(code.tokenizedStream, context);
		return res;
	}
	return nullptr;
}

void freeAST(pointerToAST node) {
	delete (Procedure*)node;
}

int phase(const Preambule& code, CompilerInterface* context) {
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
	__declspec(dllexport) int registerModule(ModuleInterface* module, LexerInterface* lexer, ParserInterface* parser) {

		module->Module_Version[0] = 0;
		module->Module_Version[1] = 0;
		module->Module_Version[2] = 1;
		module->Module_Version[3] = 0;
		module->ModuleName = "Core";
		module->ModuleLoadErrorMsg = "";
		module->supportedPreambules = { "procedure","type" };
		module->initModule = nopPhase;
		module->phase_register_Symbols = phase;
		module->phase_define_Symbols = phase;
		module->phase_generateCode = phase;
		module->finalizeModule = nopPhase;
		module->destroy = nopPhase;

		lexer->supportedPreambules = { "procedure","type" };
		lexer->lex = lexer_fun;
		lexer->init = initLexer;
		lexer->destroy = nopPhase;

		parser->supportedPreambules = { "procedure","type" };
		parser->init = nopPhase;
		parser->parse_fun = parse_code;
		parser->freeAST = freeAST;
		parser->destroy = nopPhase;
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

