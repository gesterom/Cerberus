#include "pch.h"
#include "Emitter.h"

Emitter::Emitter(
	llvm::LLVMContext* ctx,
	llvm::Module* tm,
	llvm::IRBuilder<>* b
)
{
	if (ctx == nullptr) {
		TheContext = new llvm::LLVMContext();
	}
	else {
		TheContext = ctx;
	}
	if (tm == nullptr) {
		TheModule = new llvm::Module("CoreModule", *TheContext);
	}
	else {
		TheModule = tm;
	}
	if (b == nullptr) {
		Builder = new llvm::IRBuilder<>(*TheContext);
	}
	else {
		Builder = b;
	}
}

void Emitter::advancePointer(llvm::Value* ptr) {
	if (ptr == nullptr) {
		llvm::Value* ptr = Builder->CreateLoad(Builder->getInt32Ty(), stackPointer);
	}
	auto new_stack_pointer = Builder->CreateAdd(ptr, Builder->getInt32(1));
	Builder->CreateStore(new_stack_pointer, stackPointer);
}

void Emitter::Literal(uint32_t val)
{
	auto v = llvm::ConstantInt::get(*TheContext, llvm::APInt(32, val));
	llvm::Value* ptr = Builder->CreateLoad(Builder->getInt32Ty(), stackPointer);
	llvm::Value* gep = Builder->CreateGEP(Builder->getInt32Ty(), stack, { ptr });
	Builder->CreateStore(v, gep);
	advancePointer(ptr);
}

void Emitter::PushValueonStack(llvm::Value* val)
{
	llvm::Value* ptr = Builder->CreateLoad(Builder->getInt32Ty(), stackPointer);
	llvm::Value* gep = Builder->CreateGEP(Builder->getInt32Ty(), stack, { ptr });
	Builder->CreateStore(val, gep);
	advancePointer(ptr);
}

void Emitter::Add()
{
	llvm::Value* stack_ptr = Builder->CreateLoad(Builder->getInt32Ty(), stackPointer);
	llvm::Value* arg1 = Builder->CreateSub(stack_ptr, Builder->getInt32(1));
	llvm::Value* arg2 = Builder->CreateSub(arg1, Builder->getInt32(1));

	llvm::Value* val1 = Builder->CreateLoad(Builder->getInt32Ty(), Builder->CreateGEP(Builder->getInt32Ty(), stack, { arg1 }));
	llvm::Value* val2 = Builder->CreateLoad(Builder->getInt32Ty(), Builder->CreateGEP(Builder->getInt32Ty(), stack, { arg2 }));

	// Perform addition
	llvm::Value* result = Builder->CreateAdd(val1, val2);

	// Store the result back to the stack
	Builder->CreateStore(result, Builder->CreateGEP(Builder->getInt32Ty(), stack, { arg2 }));

	// Update the stack pointer
	Builder->CreateStore(arg1, stackPointer);
}

void Emitter::Drop(llvm::Value* ptr)
{
	if (ptr == nullptr) {
		ptr = Builder->CreateLoad(Builder->getInt32Ty(), stackPointer);
	}
	llvm::Value* prev_stack_ptr = Builder->CreateSub(ptr, Builder->getInt32(1));
	Builder->CreateStore(prev_stack_ptr, stackPointer);
}


void Emitter::Sub()
{
	llvm::Value* stack_ptr = Builder->CreateLoad(Builder->getInt32Ty(), stackPointer);
	llvm::Value* arg1 = Builder->CreateSub(stack_ptr, Builder->getInt32(1));
	llvm::Value* arg2 = Builder->CreateSub(arg1, Builder->getInt32(1));

	llvm::Value* val1 = Builder->CreateLoad(Builder->getInt32Ty(), Builder->CreateGEP(Builder->getInt32Ty(), stack, { arg1 }));
	llvm::Value* val2 = Builder->CreateLoad(Builder->getInt32Ty(), Builder->CreateGEP(Builder->getInt32Ty(), stack, { arg2 }));

	// Perform addition
	llvm::Value* result = Builder->CreateSub(val1, val2);

	// Store the result back to the stack
	Builder->CreateStore(result, Builder->CreateGEP(Builder->getInt32Ty(), stack, { arg2 }));

	// Update the stack pointer
	Builder->CreateStore(arg1, stackPointer);
}

void Emitter::Mul()
{
	llvm::Value* stack_ptr = Builder->CreateLoad(Builder->getInt32Ty(), stackPointer);
	llvm::Value* arg1 = Builder->CreateSub(stack_ptr, Builder->getInt32(1));
	llvm::Value* arg2 = Builder->CreateSub(arg1, Builder->getInt32(1));

	llvm::Value* val1 = Builder->CreateLoad(Builder->getInt32Ty(), Builder->CreateGEP(Builder->getInt32Ty(), stack, { arg1 }));
	llvm::Value* val2 = Builder->CreateLoad(Builder->getInt32Ty(), Builder->CreateGEP(Builder->getInt32Ty(), stack, { arg2 }));

	// Perform addition
	llvm::Value* result = Builder->CreateMul(val1, val2);

	// Store the result back to the stack
	Builder->CreateStore(result, Builder->CreateGEP(Builder->getInt32Ty(), stack, { arg2 }));

	// Update the stack pointer
	Builder->CreateStore(arg1, stackPointer);
}

void Emitter::Dup()
{
	llvm::Value* stack_ptr = Builder->CreateLoad(Builder->getInt32Ty(), stackPointer);
	llvm::Value* arg1 = Builder->CreateSub(stack_ptr, Builder->getInt32(1));

	llvm::Value* val1 = Builder->CreateLoad(Builder->getInt32Ty(), Builder->CreateGEP(Builder->getInt32Ty(), stack, { arg1 }));

	llvm::Value* gep = Builder->CreateGEP(Builder->getInt32Ty(), stack, { stack_ptr });
	Builder->CreateStore(val1, gep);

	// Store the result back to the stack
	advancePointer(stack_ptr);
}

void Emitter::ifstatement()
{
}

void Emitter::whileStatement()
{

}

void Emitter::sizeStatement()
{
	llvm::Value* stack_ptr = Builder->CreateLoad(Builder->getInt32Ty(), stackPointer);
	// Store the current value of the stack pointer at the same memory location
	Builder->CreateStore(stack_ptr, Builder->CreateGEP(Builder->getInt32Ty(), stack, { stack_ptr }));
	advancePointer(stack_ptr);
}

void Emitter::swapStatement()
{
	// Store the current value of the stack pointer at the same memory location
	llvm::Value* stack_ptr = Builder->CreateLoad(Builder->getInt32Ty(), stackPointer);
	llvm::Value* prev_stack_ptr = Builder->CreateSub(stack_ptr, Builder->getInt32(1));

	auto adr1 = Builder->CreateGEP(Builder->getInt32Ty(), stack, { stack_ptr });
	auto adr2 = Builder->CreateGEP(Builder->getInt32Ty(), stack, { prev_stack_ptr });

	auto val1 = Builder->CreateLoad(Builder->getInt32Ty(), adr1);
	auto val2 = Builder->CreateLoad(Builder->getInt32Ty(), adr2);

	Builder->CreateStore(val1,adr2);
	Builder->CreateStore(val2, adr1);
}


void Emitter::clearStack() {
	auto val = llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 0));
	Builder->CreateStore(val, stackPointer);
}


void Emitter::returnStatement()
{
	llvm::Value* stack_ptr = Builder->CreateLoad(Builder->getInt32Ty(), stackPointer);
	llvm::Value* stack_1 = Builder->CreateSub(stack_ptr, Builder->getInt32(1));
	llvm::Value* returnValue = Builder->CreateLoad(Builder->getInt32Ty(), Builder->CreateGEP(Builder->getInt32Ty(), stack, { stack_1 }));

	Builder->CreateRet(returnValue);
}
