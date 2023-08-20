// dllmain.cpp : Definiuje punkt wejścia dla aplikacji DLL.
#include "pch.h"

#include "../main_compiler/ModuleInterface.h"

#include <iostream>

int initModule() {
	return 0;
}

int phase(const Preambule& code, CompilerContext& context) {
	return 0;
}

std::ostream& operator<<(std::ostream& out, const Position& pos) {
	out << pos.filename << " : " << pos.line << "_" << pos.character;
	return out;
}

std::string paddToMaxSize(int curent, int max) {
	std::string res;
	while (max > 0) {
		if(curent != 0 )
		res = (char)(curent % 10 + '0') + res;
		else res = ' ' + res;
		curent = curent / 10;
		max /= 10;
	}
	return res;
}

void print(const Preambule& code) {
	std::cout << "Position : " << code.preambule_name.pos << std::endl;
	for (const auto& op : code.options) {
		std::cout <<"\t" << op.first << "=" << op.second << " \n";
	}
	std::cout << "\npreambule : " << code.preambule_name.val << std::endl;
	std::cout << "name : " << code.name.val << std::endl;
	std::cout<<"body : "<<std::endl; 
	for (int i = 1; i <= code.body.lines.size(); i++) {
		std::cout<< paddToMaxSize (i, code.body.lines.size())<<".";
		std::cout<<code.body.lines[i-1].val<<std::endl;
	}
}

int print_code(const Preambule& code, CompilerContext& context) {
	auto it = code.options.find("Print");
	if(it == code.options.end() or it->second != "True" ) return 0;
	std::cout << "=============================\n";
	print(code);
	return 0;
}

extern "C" {
	__declspec(dllexport) int registerModule(Module_t* module, Lexer_t* lexer, Parser_t* parser) {

		module->Module_Version[0] = 1;
		module->Module_Version[1] = 0;
		module->Module_Version[2] = 0;
		module->Module_Version[3] = 0;
		module->ModuleName = "Printer";
		module->ModuleLoadErrorMsg = "";
		module->initModule = initModule;
		module->phase_register_Symbols = phase;
		module->phase_define_Symbols = phase;
		module->phase_generateCode = print_code;
		module->finalizeModule = initModule;
		module->destroy = initModule;
		module->headnledPreambules = { "brainfuck","type","procedure","atom","comment" };
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

