#pragma once

#pragma warning(push, 0)
#undef max
#undef min
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#pragma warning(pop)

#include <stdint.h>

class Emitter
{
	private:
		void advancePointer(llvm::Value* ptr = nullptr);
	public:
		llvm::LLVMContext* TheContext;
		llvm::Module* TheModule;
		llvm::IRBuilder<>* Builder;

		llvm::GlobalVariable* stack;
		llvm::GlobalVariable* stackPointer;

		llvm::Function* func;

		Emitter(
			llvm::LLVMContext* ctx = nullptr,
			llvm::Module* tm = nullptr,
			llvm::IRBuilder<>* b = nullptr
		);
		void PushValueonStack(llvm::Value* val);
		void Literal(uint32_t val);
		void Add();
		void Sub();
		void Mul();
		void sizeStatement();
		void ifstatement();
		void whileStatement();
		void clearStack();
		void Dup();
		void Drop(llvm::Value* ptr = nullptr);
		void swapStatement();
		void returnStatement();
};

