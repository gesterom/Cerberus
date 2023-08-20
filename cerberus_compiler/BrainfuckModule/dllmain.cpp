// dllmain.cpp : Definiuje punkt wejścia dla aplikacji DLL.
#include "pch.h"

#include <iostream>

#include "../main_compiler/ModuleInterface.h"

typedef int (*event_t)();
typedef int (*moduleHandler_phase_t)(const Preambule& code, CompilerContext& context);

const size_t mem_size = 1024;
char* mem = nullptr;

String Preambule::Body_t::val() const
{
	String res;
	for (const auto& i : lines) {
		res.val += i.val;
	}
	res.pos = lines[0].pos;
	return res;
}

int initModule() {
	mem = new char[mem_size];
	memset(mem, 0, mem_size);
	return 0;
}

int phase(const Preambule& code, CompilerContext& context) {
	return 0;
}

int execute(const Preambule& code, CompilerContext& context) {
	//zliczaj nawiasy
	auto it = code.options.find("exec");
	if (it == code.options.end() or it->second != "True") return 0;
	int memIndex = 0;
	int pc = 0;
	int paramCount = 0;
	char c;
	while (pc >= 0 and pc < code.body.val().size()) {
		c = code.body.val().val[pc];
		switch (c)
		{
		case '>': ++memIndex; break;
		case '<': --memIndex; break;
		case '+': ++mem[memIndex]; break;
		case '-': --mem[memIndex]; break;
		case '.': putchar(mem[memIndex]); break;
		case ',': std::cin >> mem[memIndex]; break;
		case '[':
			paramCount = 1;
			while (pc < code.body.val().size() and mem[memIndex] == 0) {
				pc++;
				if (code.body.val().val[pc] == ']') {
					paramCount--;
					if (paramCount == 0) 
					{
						pc++;
						break;
					}
				}
				else if (code.body.val().val[pc] == '[') paramCount++;
			}
			break;
		case ']':
			paramCount = 1;
			while (pc > 0 and paramCount > 0) {
				pc--;
				if (code.body.val().val[pc] == ']') paramCount++;
				else if (code.body.val().val[pc] == '[') paramCount--;
			}
			pc--;
			break;
		default:
			break;
		}
		pc++;
	}
	return 0;
}

int finalizeModule() {
	delete[] mem;
	return 0;
}

extern "C" {
	__declspec(dllexport) int registerModule(Module_t* module, Lexer_t* lexer, Parser_t* parser) {

		module->Module_Version[0] = 1;
		module->Module_Version[1] = 0;
		module->Module_Version[2] = 0;
		module->Module_Version[3] = 0;
		module->ModuleName = "Brainfuck";
		module->ModuleLoadErrorMsg = "";
		module->initModule = initModule;
		module->phase_register_Symbols = phase;
		module->phase_define_Symbols = phase;
		module->phase_generateCode = execute;
		module->finalizeModule = finalizeModule;
		module->headnledPreambules = { "brainfuck" };
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

