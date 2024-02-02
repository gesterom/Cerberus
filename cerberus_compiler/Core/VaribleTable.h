#pragma once

#include "Parser.h"
#include <map>
#include <vector>

struct VariblesEntry {
	bool found = 0;
	Position pos = Position();
	IType* type = nullptr;
	llvm::AllocaInst* llvm_inst;
	std::string varibleName;
	VariblesEntry(String varibleName, IType* type, llvm::AllocaInst* llvm_inst);
	VariblesEntry();
};

typedef std::vector<std::map<std::string, VariblesEntry >> VaribleTable;
