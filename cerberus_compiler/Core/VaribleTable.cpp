#include "pch.h"
#include "VaribleTable.h"

VariblesEntry::VariblesEntry(String varibleName,IType* type, llvm::AllocaInst* llvm_inst)
{
	this->found = true;
	this->pos = varibleName.pos;
	this->varibleName = varibleName.val;
	this->type = type;
	this->llvm_inst = llvm_inst;
}

VariblesEntry::VariblesEntry()
{
	this->found = false;
	this->pos = Position();
	this->llvm_inst = nullptr;
	this->type = nullptr;
	this->varibleName = "ERROR";
}
