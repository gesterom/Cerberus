#include "pch.h"
#include "Emitter.h"

#include "../main_compiler/ModuleInterface.h"

std::vector<std::string> control = {
	"if",
	"while"
};

std::vector<std::string> ops = {
	"+",
	"-",
	"*",
	"/",
	"%",
	"++",
	"--",
	"dup",
	"out",
	"in",
	"drop",
	"swap",
	"size",
	"neg",
	"return"
};

const std::string OutOfBound = "End of procedure";

bool isType(TokenizedStream* stream, StackScript::TokenType type) {
	return
		stream != nullptr and
		stream->body.size() > stream->body_offset and
		stream->body[stream->body_offset].type == (uint16_t)type;
}

bool isValue(TokenizedStream* stream, std::string str) {
	return
		stream != nullptr and
		stream->body.size() > stream->body_offset and
		stream->body[stream->body_offset].val.val == str;
}

bool isExacly(TokenizedStream* stream, StackScript::TokenType type, std::string str) {
	return isType(stream, type) and isValue(stream, str);
}

void expectParanthis(TokenizedStream* stream, CompilerInterface* context, std::string param) {
	if (not isExacly(stream, StackScript::TokenType::parentheses, param)) {
		std::string msg = std::string() + "Expected Paranthis : " + param + "\nGet : " + ((stream->body_offset >= stream->body.size()) ? (OutOfBound) : (stream->body[stream->body_offset].val.val));
		context->critical_error_msg(context->context,
			CriticalErrorType::SyntaxError,
			stream->body_offset >= stream->body.size() ? stream->body[stream->body.size() - 1].val.pos : stream->body[stream->body_offset].val.pos,
			msg.c_str()
		);
	}
	stream->body_offset++;
}

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
	llvm::Value* result = Builder->CreateSub(val2, val1);

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

void Emitter::Div()
{
	llvm::Value* stack_ptr = Builder->CreateLoad(Builder->getInt32Ty(), stackPointer);
	llvm::Value* arg1 = Builder->CreateSub(stack_ptr, Builder->getInt32(1));
	llvm::Value* arg2 = Builder->CreateSub(arg1, Builder->getInt32(1));

	llvm::Value* val1 = Builder->CreateLoad(Builder->getInt32Ty(), Builder->CreateGEP(Builder->getInt32Ty(), stack, { arg1 }));
	llvm::Value* val2 = Builder->CreateLoad(Builder->getInt32Ty(), Builder->CreateGEP(Builder->getInt32Ty(), stack, { arg2 }));

	// Perform addition
	llvm::Value* result = Builder->CreateSDiv(val2, val1);

	// Store the result back to the stack
	Builder->CreateStore(result, Builder->CreateGEP(Builder->getInt32Ty(), stack, { arg2 }));

	// Update the stack pointer
	Builder->CreateStore(arg1, stackPointer);
}

void Emitter::Mod()
{
	llvm::Value* stack_ptr = Builder->CreateLoad(Builder->getInt32Ty(), stackPointer);
	llvm::Value* arg1 = Builder->CreateSub(stack_ptr, Builder->getInt32(1));
	llvm::Value* arg2 = Builder->CreateSub(arg1, Builder->getInt32(1));

	llvm::Value* val1 = Builder->CreateLoad(Builder->getInt32Ty(), Builder->CreateGEP(Builder->getInt32Ty(), stack, { arg1 }));
	llvm::Value* val2 = Builder->CreateLoad(Builder->getInt32Ty(), Builder->CreateGEP(Builder->getInt32Ty(), stack, { arg2 }));

	// Perform addition
	llvm::Value* result = Builder->CreateSRem(val2, val1);

	// Store the result back to the stack
	Builder->CreateStore(result, Builder->CreateGEP(Builder->getInt32Ty(), stack, { arg2 }));

	// Update the stack pointer
	Builder->CreateStore(arg1, stackPointer);
}

void Emitter::Rot()
{
	llvm::Value* stack_ptr = Builder->CreateLoad(Builder->getInt32Ty(), stackPointer);
	//3 2 1 -> 2 1 3
	// Load the top three values from the stack
	llvm::Value* arg1 = Builder->CreateSub(stack_ptr, Builder->getInt32(1));
	llvm::Value* val1 = Builder->CreateLoad(Builder->getInt32Ty(), Builder->CreateGEP(Builder->getInt32Ty(), stack, { arg1 }));

	llvm::Value* arg2 = Builder->CreateSub(stack_ptr, Builder->getInt32(2));
	llvm::Value* val2 = Builder->CreateLoad(Builder->getInt32Ty(), Builder->CreateGEP(Builder->getInt32Ty(), stack, { arg2 }));

	llvm::Value* arg3 = Builder->CreateSub(stack_ptr, Builder->getInt32(3));
	llvm::Value* val3 = Builder->CreateLoad(Builder->getInt32Ty(), Builder->CreateGEP(Builder->getInt32Ty(), stack, { arg3 }));

	// Rotate the top three values
	Builder->CreateStore(val2, Builder->CreateGEP(Builder->getInt32Ty(), stack, { arg3 }));
	Builder->CreateStore(val1, Builder->CreateGEP(Builder->getInt32Ty(), stack, { arg2 }));
	Builder->CreateStore(val3, Builder->CreateGEP(Builder->getInt32Ty(), stack, { arg1 }));

	// Store the result back to the stack
	//advancePointer(stack_ptr);
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

void Emitter::out()
{
	std::vector<llvm::Value*> printfArgs;
	printfArgs.push_back(Builder->CreateGlobalStringPtr("%d"));

	llvm::Value* stack_ptr = Builder->CreateLoad(Builder->getInt32Ty(), stackPointer);
	llvm::Value* arg1 = Builder->CreateSub(stack_ptr, Builder->getInt32(1));

	llvm::Value* val1 = Builder->CreateLoad(Builder->getInt32Ty(), Builder->CreateGEP(Builder->getInt32Ty(), stack, { arg1 }));
	printfArgs.push_back(val1);
	Builder->CreateCall(this->print, printfArgs);
}

void Emitter::in()
{

	auto ftm = Builder->CreateGlobalStringPtr("%d\00");

	std::vector<llvm::Value*> scanfArgsCall;
	scanfArgsCall.push_back(ftm);
	llvm::Value* stack_ptr = Builder->CreateLoad(Builder->getInt32Ty(), stackPointer);
	scanfArgsCall.push_back(Builder->CreateGEP(Builder->getInt32Ty(), stack, { stack_ptr }));

	advancePointer(stack_ptr);
}

void Emitter::over()
{
	
	llvm::Value* stack_ptr = Builder->CreateLoad(Builder->getInt32Ty(), stackPointer);
	llvm::Value* prev_stack_ptr = Builder->CreateSub(stack_ptr, Builder->getInt32(1));

	auto adr2 = Builder->CreateGEP(Builder->getInt32Ty(), stack, { prev_stack_ptr });
	auto val2 = Builder->CreateLoad(Builder->getInt32Ty(), adr2);

	llvm::Value* gep = Builder->CreateGEP(Builder->getInt32Ty(), stack, { stack_ptr });
	Builder->CreateStore(val2, gep);

	advancePointer(stack_ptr);
}

void Emitter::compile(TokenizedStream* code, CompilerInterface* context){

	while (code->body.size() > code->body_offset) {
		Token t = code->body[code->body_offset];
		if (isExacly(code, StackScript::TokenType::parentheses, "}"))
		{
			break;
		}
		if (t.type == (uint16_t)StackScript::TokenType::literal) {
			this->Literal(stoi(t.val.val));
		}
		else if(t.type == (uint16_t)StackScript::TokenType::control){
			if (t.val.val == "if") {
				code->body_offset++;

				llvm::Function* function = Builder->GetInsertBlock()->getParent();
				llvm::BasicBlock* ifBlock = llvm::BasicBlock::Create(*TheContext, "if", function);
				llvm::BasicBlock* mergeBlock;

				llvm::BasicBlock* elseBlock = llvm::BasicBlock::Create(*TheContext, "if.else", function);


				llvm::Value* stack_ptr = Builder->CreateLoad(Builder->getInt32Ty(), stackPointer);
				llvm::Value* arg1 = Builder->CreateSub(stack_ptr, Builder->getInt32(1));
				llvm::Value* cond = Builder->CreateLoad(Builder->getInt32Ty(), Builder->CreateGEP(Builder->getInt32Ty(), stack, { arg1 }));
				auto vvvv = this->Builder->CreateCmp(llvm::CmpInst::ICMP_NE, cond, Builder->getInt32(0), "cmptmp");
				Builder->CreateCondBr(vvvv, ifBlock, elseBlock);
				Builder->SetInsertPoint(ifBlock);
				
				expectParanthis(code, context,"{");
				this->compile(code,context);
				expectParanthis(code, context, "}");
				mergeBlock = llvm::BasicBlock::Create(*TheContext, "if.end", function);
				Builder->CreateBr(mergeBlock); // Jump to merge after the 'if' body
				Builder->SetInsertPoint(elseBlock);
				expectParanthis(code, context, "{");
				this->compile(code, context);
				expectParanthis(code, context, "}");
				Builder->CreateBr(mergeBlock);

				Builder->SetInsertPoint(mergeBlock);
			}
			else if (t.val.val == "while") {
				code->body_offset++;
				llvm::Function* function = Builder->GetInsertBlock()->getParent();

				// Create the blocks for the 'while' loop
				llvm::BasicBlock* conditionBlock = llvm::BasicBlock::Create(*TheContext, "while.cond", function);
				llvm::BasicBlock* loopBlock = llvm::BasicBlock::Create(*TheContext, "while.body", function);
				llvm::BasicBlock* afterLoopBlock = llvm::BasicBlock::Create(*TheContext, "while.end", function);

				// Set up the jump to the condition block
				Builder->CreateBr(conditionBlock);
				Builder->SetInsertPoint(conditionBlock);

				llvm::Value* stack_ptr = Builder->CreateLoad(Builder->getInt32Ty(), stackPointer);
				llvm::Value* arg1 = Builder->CreateSub(stack_ptr, Builder->getInt32(1));
				llvm::Value* cond = Builder->CreateLoad(Builder->getInt32Ty(), Builder->CreateGEP(Builder->getInt32Ty(), stack, { arg1 }));
				auto vvvv = this->Builder->CreateCmp(llvm::CmpInst::ICMP_NE, cond, Builder->getInt32(0), "cmptmp");
				llvm::Value* condValue = vvvv;

				// Create the conditional branch
				Builder->CreateCondBr(condValue, loopBlock, afterLoopBlock);

				// Emit code for the loop block
				//function->getBasicBlockList().push_back(loopBlock);
				Builder->SetInsertPoint(loopBlock);
				expectParanthis(code, context, "{");
				this->compile(code, context);
				expectParanthis(code, context, "}");
				Builder->CreateBr(conditionBlock); // Jump back to the condition check

				// Set insertion point for the block after the loop
				//function->getBasicBlockList().push_back(afterLoopBlock);
				Builder->SetInsertPoint(afterLoopBlock);
			}
		}
		else if (t.type == (uint16_t)StackScript::TokenType::op) {
			if (t.val.val == "size") {
				this->sizeStatement();
			}
			else if (t.val.val == "+") {
				this->Add();
			}
			else if (t.val.val == "-") {
				this->Sub();
			}
			else if (t.val.val == "*") {
				this->Mul();
			}
			else if (t.val.val == "/") {
				this->Div();
			}
			else if (t.val.val == "%") {
				this->Mod();
			}
			else if (t.val.val == "return") {
				this->returnStatement();
			}
			else if (t.val.val == "swap") {
				this->swapStatement();
			}
			else if (t.val.val == "drop") {
				this->Drop();
			}
			else if (t.val.val == "dup") {
				this->Dup();
			}
			else if (t.val.val == "in") {
				this->in();
			}
			else if (t.val.val == "out") {
				this->out();
			}
			else if (t.val.val == "rot") {
				this->Rot();
			}
			else if (t.val.val == "over") {
				this->over();
			}
			else {
				//error
			}
		}
		code->body_offset++;
	}
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
