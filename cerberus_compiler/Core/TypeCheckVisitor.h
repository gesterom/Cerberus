#pragma once

#include <memory>             // For std::unique_ptr
#include <map>                // For std::map
#include <string>             // For std::string
#include "Parser.h"

#include "VaribleTable.h"

class TypeCheckVisitor : public IVisitor
{

	CompilerInterface* meta_context;
	VaribleTable varibles;
	IType* lastType;
	String lastId;
	bool isArray;
	bool checkField=  false;;
public:

	SymbolTypeId procedureSymbolTypeId;
	SymbolTypeId typeSymbolTypeId;
	void check(const Preambule& code, CompilerInterface* context);
	virtual void visit(VaribleExpression*);
	virtual void visit(TypeNameExpression*);
	virtual void visit(LiteralExpression*);
	virtual void visit(PrefixOperatorExpression*);
	virtual void visit(BinaryOperatorExpression*);
	virtual void visit(PostfixOperatorExpression*);
	virtual void visit(ProcedureCallExpression*);
	virtual void visit(ArrayAcceseExpression*);
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

