#include "BrainFuck.h"

#include <iostream>

#include "IModuleRepository.h"
#include "SymbolRepository.h"
#include "CodeFragment.h"


void BrainFuck::GenerateCode(const CodeFragment& code, SymbolsRepository& symbols) {
	//zliczaj nawiasy
	auto it = code.options.find("exec");
	if (it == code.options.end() or it->second != "True") return;
	int memIndex = 0;
	int pc = 0;
	int paramCount = 0;
	char c;
	while (pc >= 0 and pc < code.body.val.size()) {
		c = code.body.val[pc];
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
			pc++;
			while (pc < code.body.val.size() and mem[memIndex] == 0) {
				if (code.body.val[pc] == ']') {
					paramCount--;
					if (paramCount == 0) break;
				}
				else if (code.body.val[pc] == '[') paramCount++;
				pc++;
			}
			break;
		case ']':
			paramCount = 1;
			pc--;
			while (pc > 0 and paramCount > 0) {
				if (code.body.val[pc] == ']') paramCount++;
				else if (code.body.val[pc] == '[') paramCount--;
				pc--;
			}
			break;
		default:
			break;
		}
		pc++;
	}
}

void BrainFuck::RegisterPreambule(IModuleRepository& repo) {
	repo.registerToPreambule("BrainFuck", this);
}
void BrainFuck::Init(SymbolsRepository& symbols) {
	mem = new char[1024];
	memset(mem, 0, 1024);
}
void BrainFuck::RegisterSymbols(const CodeFragment& code, SymbolsRepository& symbols) {

}
void BrainFuck::Finalize(SymbolsRepository& symbols) {
	delete[] mem;
}