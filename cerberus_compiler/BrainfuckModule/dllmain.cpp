// dllmain.cpp : Definiuje punkt wejścia dla aplikacji DLL.
#include "pch.h"

#include <iostream>

#include "../main_compiler/ModuleInterface.h"

typedef int (*event_t)();
typedef int (*moduleHandler_phase_t)(const Preambule& code, CompilerContext& context);

int initModule() {
	return 0;
}

int phase(const Preambule& code, CompilerContext& context) {
	return 0;
}

extern "C" {
	__declspec(dllexport) int registerModule(Module_t* module,Lexer_t* lexer, Parser_t* parser) {

		module->Module_Version[0] = 1;
		module->Module_Version[1] = 0;
		module->Module_Version[2] = 0;
		module->Module_Version[3] = 0;
		module->ModuleName = "Brainfuck";
		module->ModuleLoadErrorMsg = "";
		module->initModule = initModule;
		module->phase_register_Symbols = phase;
		module->phase_define_Symbols = phase;
		module->phase_generateCode = phase;
		module->finalizeModule = initModule;
		module->headnledPreambules = {"brainfuck"};
		module->destroy = initModule;
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

