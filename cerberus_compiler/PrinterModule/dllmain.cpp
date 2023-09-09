// dllmain.cpp : Definiuje punkt wejścia dla aplikacji DLL.
#include "pch.h"

#include "../main_compiler/ModuleInterface.h"

#include <iostream>

int initModule() {
	return 0;
}

int phase(const Preambule& code, CompilerInterface* context) {
	return 0;
}

std::ostream& operator<<(std::ostream& out, const Position& pos) {
	out << pos.filename << " : " << pos.line << "_" << pos.character;
	return out;
}

std::string paddToMaxSize(size_t curent, size_t max) {
	std::string res;
	while (max > 0) {
		if (curent != 0)
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
		std::cout << "\t" << op.first << "=" << op.second << " \n";
	}
	std::cout << "\npreambule : " << code.preambule_name.val << std::endl;
	std::cout << "name : " << code.name.val << std::endl;
	std::cout << "body : " << std::endl;
	for (int i = 1; i <= code.body.lines.size(); i++) {
		std::cout << paddToMaxSize(i, code.body.lines.size()) << ".";
		std::cout << code.body.lines[i - 1].val << std::endl;
	}
}

std::string typeToStr(int t) {
	switch (t) {
	case 0: return "unknown";
	case 1: return "op";
	case 2: return "id";
	case 3: return "TypeName";
	case 4: return "parentheses";
	case 5: return "keyword";
	case 6: return "string_literal";
	case 7: return "character_literal";
	case 8: return "number_literal";
	default:
		return "...";
	}
}

void printLex(const Preambule& code) {
	std::cout << "Lex::\n";
	std::cout << "Position : " << code.preambule_name.pos << std::endl;
	for (const auto& op : code.options) {
		std::cout << "\t" << op.first << "=" << op.second << " \n";
	}
	std::cout << "\npreambule : " << code.preambule_name.val << std::endl;
	std::cout << "name : " << std::endl;
	if (code.tokenizedStream == nullptr) return;
	for (auto i : code.tokenizedStream->name) {
		std::cout << i.module_id << " - " << typeToStr(i.type) << " = " << i.val.val <<" " << i.val.pos << std::endl;
	}
	std::cout << "body : " << std::endl;
	if (code.tokenizedStream == nullptr) return;
	for (auto i : code.tokenizedStream->body) {
		std::cout << i.module_id << " - " << typeToStr(i.type) << " = " << i.val.val <<" " << i.val.pos << std::endl;
	}
}

void printAST(const Preambule& code) {
	return;
}

int print_code(const Preambule& code, CompilerInterface* context) {
	auto it = code.options.find("Print");
	if (it != code.options.end() and it->second == "True") {
		std::cout << "=============================\n";
		print(code);
	}
	it = code.options.find("LexerDebug");
	if (it != code.options.end() and it->second == "True") {
		std::cout << "=============================\n";
		printLex(code);
	}
	it = code.options.find("ParserDebug");
	if (it != code.options.end() and it->second == "True") {
		std::cout << "=============================\n";
		printAST(code);
	}

	return 0;
}

extern "C" {
	__declspec(dllexport) int registerModule(ModuleInterface* module, LexerInterface* lexer, ParserInterface* parser) {

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
		module->supportedPreambules = { "brainfuck","type","procedure","atom","comment" };
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

