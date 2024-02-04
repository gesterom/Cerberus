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
#include <memory>             
#include <map>                
#include <string>             
#include "Parser.h"

#include "VaribleTable.h"

class IREmiterVisitor : public IVisitor
{

	std::map<std::string, llvm::Value*> NamedValues;
	llvm::Value* varible_address;
	llvm::Value* last;
	String lastType;
	String lastId;
	
	VaribleTable varibles;

	bool lastReturn = false;
	bool checkField = false;

	llvm::Function* strlenFunc;//= module.getFunction("strlen");
	llvm::Function* mallocFunc;//= module.getFunction("malloc");
	llvm::Function* strcpyFunc;//= module.getFunction("strcpy");
	llvm::Function* concatFunc;// = generateStringConcatenation();
	llvm::Function* printfFunc;
	llvm::Function* scanfFunc;

	llvm::Function* generateStringConcatenation();
	llvm::Value* convertCharToCharStr(llvm::Value* var_char);

public:
	CompilerInterface* meta_context;
	llvm::LLVMContext* TheContext;
	llvm::Module* TheModule;
	llvm::IRBuilder<>* Builder;
	SymbolTypeId procedureSymbolTypeId;
	SymbolTypeId typeSymbolTypeId;
	IREmiterVisitor(
		llvm::LLVMContext* TheContext = nullptr,
		llvm::Module* TheModule = nullptr,
		llvm::IRBuilder<>* Builder = nullptr
	);
	void compile(const Preambule& code, CompilerInterface* context);
	llvm::Type* newType(const Preambule& code, CompilerInterface* context);
	llvm::Function* addExternalProcedure(std::string name, const std::vector<std::pair<IType*, String>>& args, IType* ret);
	void finish();
	virtual void visit(VaribleExpression*);
	virtual void visit(TypeNameExpression*);
	virtual void visit(LiteralExpression*);
	virtual void visit(PrefixOperatorExpression*);
	virtual void visit(BinaryOperatorExpression*);
	virtual void visit(PostfixOperatorExpression*);
	virtual void visit(ProcedureCallExpression*);

	virtual void visit(ArrayLiteralExpression*);
	virtual void visit(ArrayAcceseExpression*);
	virtual void visit(ArrayAlloc*);

	virtual void visit(IfStatement*);
	virtual void visit(WhileStatement*);
	virtual void visit(ReturnStatement*);
	virtual void visit(ExpressionStatement*);
	virtual void visit(BlockStatement*);
	virtual void visit(VaribleDeclaration*);

	virtual llvm::Type* getLLVMType(IType* visitor);
	virtual llvm::Type* getLLVMType(TypeNameExpression* visitor);
	virtual llvm::Type* getLLVMType(ArrayType* visitor);
	virtual llvm::Type* getLLVMType(FunctionType* visitor);
	virtual llvm::Type* getLLVMType(PointerType* visitor);
	virtual llvm::Type* getLLVMType(StructType* visitor);
};

