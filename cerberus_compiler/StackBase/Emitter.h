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

#include "../main_compiler/ModuleInterface.h"

#include <stdint.h>
namespace StackScript{
	enum class TokenType {
		uknown,
		control,
		op,
		literal,
		parentheses,
		name,
	};
}
extern std::vector<std::string> ops;
extern std::vector<std::string> control;

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

		llvm::Function* func = nullptr;


		llvm::Function* print = nullptr;
		llvm::Function* scan = nullptr;

		Emitter(
			llvm::LLVMContext* ctx = nullptr,
			llvm::Module* tm = nullptr,
			llvm::IRBuilder<>* b = nullptr
		);
		void compile(TokenizedStream* code, CompilerInterface* context);
		void PushValueonStack(llvm::Value* val);
		void Literal(uint32_t val);
		void Add();
		void Sub();
		void Mul();
		void Div();
		void Mod();
		void Rot();
		void sizeStatement();
		void clearStack();
		void Dup();
		void Drop(llvm::Value* ptr = nullptr);
		void swapStatement();
		void returnStatement();
		void out();
		void in();
		void over();
};

