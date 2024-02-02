#pragma once

#include "Lexer.h"

#ifndef nollvm

#pragma warning(push, 0)
#undef max
#undef min
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Type.h"
#pragma warning(pop)

#else// !nollvm

namespace llvm {
	template<typename T = int>
	class IRBuilder {
		T a;
	};

	typedef void Type;
}

#endif


#include <memory>
#include <optional>

#include "Operator_t.h"

struct IExpression;
struct IStatement;


class IVisitor;

struct IStatement
{
	virtual void visit(IVisitor* visitor) = 0;
};

struct IfStatement : public IStatement
{
	std::unique_ptr< IExpression > condition;
	std::unique_ptr<IStatement> ifTrue;
	std::unique_ptr<IStatement> ifFalse;
	virtual void visit(IVisitor* visitor);
};
struct WhileStatement : public IStatement
{
	std::unique_ptr< IExpression > condition;
	std::unique_ptr<IStatement> body;
	virtual void visit(IVisitor* visitor);
};
struct ReturnStatement : public IStatement
{
	std::unique_ptr< IExpression > exp;
	virtual void visit(IVisitor* visitor);
};
struct ExpressionStatement : public IStatement
{
	std::unique_ptr< IExpression > exp;
	virtual void visit(IVisitor* visitor);
};

struct BlockStatement : public IStatement {
	std::vector<std::unique_ptr<IStatement>> body;
	virtual void visit(IVisitor* visitor);
};
struct IType;

struct VaribleDeclaration : public IStatement {
	IType* type;
	String varibleName;
	std::unique_ptr< IExpression > init;
	virtual void visit(IVisitor* visitor);
};


struct IExpression {
	IType* type;//TODO change to type_id
	Position pos;
	virtual std::string toString() = 0;
	virtual void visit(IVisitor* visitor) = 0;
};

struct VaribleExpression : public IExpression
{
	String varibleName;
	virtual std::string toString();
	virtual void visit(IVisitor* visitor);
};

struct IType : public IExpression {
	virtual llvm::Type* getLLVMType(IVisitor* visitor) = 0;
	virtual IType* copy() = 0;
	virtual std::string toString() = 0;
	virtual void visit(IVisitor* visitor) = 0;
	SymbolId typeId;
	llvm::Type* llvmType = nullptr;
};

struct TypeNameExpression : public IType
{
	String typeName;
	//TypeNameExpression();
	TypeNameExpression(std::string typeName,SymbolId id = -1);
	TypeNameExpression(String typeName,SymbolId = -1);
	//String varibleName;
	virtual llvm::Type* getLLVMType(IVisitor* visitor);
	virtual IType* copy();
	virtual std::string toString();
	virtual void visit(IVisitor* visitor);
};

struct InternalType : public IType
{
	std::string typeName;
	InternalType(std::string name);
	//String varibleName;
	virtual llvm::Type* getLLVMType(IVisitor* visitor);
	virtual IType* copy();
	virtual std::string toString();
	virtual void visit(IVisitor* visitor);
};

struct ArrayType : public IType
{
	uint32_t size;
	IType* inner_type;
	bool dynamic;
	ArrayType(IType* ini);
	ArrayType(IType* ini,uint32_t size);
	//String varibleName;
	virtual llvm::Type* getLLVMType(IVisitor* visitor);
	virtual IType* copy();
	virtual std::string toString();
	virtual void visit(IVisitor* visitor);
};

struct FunctionType : public IType
{
	//String varibleName;
	String funcName;
	std::vector<IType*> args;
	IType* returnType;
	FunctionType(String name);
	virtual llvm::Type* getLLVMType(IVisitor* visitor);
	virtual IType* copy();
	virtual std::string toString();
	virtual void visit(IVisitor* visitor);
};

struct PointerType : public IType
{
	uint32_t size;
	IType* inner_type;
	PointerType(IType* type);
	//String varibleName;
	virtual llvm::Type* getLLVMType(IVisitor* visitor);
	virtual IType* copy();
	virtual std::string toString();
	virtual void visit(IVisitor* visitor);
};

struct StructType : public IType {
	uint32_t size;
	std::vector<std::pair<IType*, String> > inner_type;
	String name;
	virtual llvm::Type* getLLVMType(IVisitor* visitor);
	virtual IType* copy();
	virtual std::string toString();
	virtual void visit(IVisitor* visitor);
};
struct LiteralExpression : public IExpression
{
	String value;
	virtual std::string toString();
	virtual void visit(IVisitor* visitor);
};

struct PrefixOperatorExpression : public IExpression {
	String op;
	Operator_t op_type;
	std::unique_ptr<IExpression> right;
	virtual std::string toString();
	virtual void visit(IVisitor* visitor);
};

struct BinaryOperatorExpression : public IExpression {
	String op;
	Operator_t op_type;
	std::unique_ptr<IExpression> left;
	std::unique_ptr<IExpression> right;
	virtual std::string toString();
	virtual void visit(IVisitor* visitor);
};

struct PostfixOperatorExpression : public IExpression {
	String op;
	Operator_t op_type;
	std::unique_ptr<IExpression> left;
	virtual std::string toString();
	virtual void visit(IVisitor* visitor);
};

struct ProcedureCallExpression : public IExpression {
	//String name;
	std::unique_ptr<IExpression> function;
	std::vector<std::unique_ptr<IExpression>> args;
	virtual std::string toString();
	virtual void visit(IVisitor* visitor);
};

struct ArrayAcceseExpression : public IExpression {
	//String name;
	std::unique_ptr<IExpression> array;
	std::unique_ptr<IExpression> args;
	virtual std::string toString();
	virtual void visit(IVisitor* visitor);
};

class IVisitor {
	public:
	virtual void visit(VaribleExpression*) = 0;
	virtual void visit(LiteralExpression*) = 0;
	virtual void visit(PrefixOperatorExpression*) = 0;
	virtual void visit(BinaryOperatorExpression*) = 0;
	virtual void visit(PostfixOperatorExpression*) = 0;
	virtual void visit(ProcedureCallExpression*) = 0;
	virtual void visit(ArrayAcceseExpression*) = 0;
	virtual void visit(IfStatement*) = 0;
	virtual void visit(WhileStatement*) = 0;
	virtual void visit(ReturnStatement*) = 0;
	virtual void visit(ExpressionStatement*) = 0;
	virtual void visit(BlockStatement*) = 0;
	virtual void visit(VaribleDeclaration*) = 0;
	virtual void visit(TypeNameExpression*) = 0;

	virtual llvm::Type* getLLVMType(IType* visitor) = 0;
	virtual llvm::Type* getLLVMType(TypeNameExpression* visitor) = 0;
	virtual llvm::Type* getLLVMType(ArrayType* visitor) = 0 ;
	virtual llvm::Type* getLLVMType(FunctionType* visitor) = 0 ;
	virtual llvm::Type* getLLVMType(PointerType* visitor) = 0 ;
	virtual llvm::Type* getLLVMType(StructType* visitor) = 0 ;
};

class PrinterVisitor : public IVisitor {
	int indent_no = 0;
	std::string indent();
public:
	CompilerInterface* icompiler = nullptr;
	virtual void visit(VaribleExpression*);
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
	virtual void visit(TypeNameExpression*);

	virtual llvm::Type* getLLVMType(IType* visitor);
	virtual llvm::Type* getLLVMType(TypeNameExpression* visitor);
	virtual llvm::Type* getLLVMType(ArrayType* visitor);
	virtual llvm::Type* getLLVMType(FunctionType* visitor);
	virtual llvm::Type* getLLVMType(PointerType* visitor);
	virtual llvm::Type* getLLVMType(StructType* visitor);
};

struct Procedure {
	std::vector<std::pair<IType*,String>> args;
	IType* returnType;
	String name;
	std::vector<std::unique_ptr<IStatement>> code;
};

class StoredASTSumType{

	public:
	enum class Type {
		Struct,
		Procedure
	};
	Type type;
	void* ptr;
	StoredASTSumType(Procedure* p);
	StoredASTSumType(IType* t);
};

void parseHeaderOffFunction(Procedure& proc, TokenizedStream* stream, CompilerInterface* context);
Procedure parseProcedure(TokenizedStream* stream, CompilerInterface* context);
StructType* parseStruct(TokenizedStream* stream, CompilerInterface* context);

bool operator!=(std::unique_ptr<IType>& left, std::unique_ptr<IType>& right) ;
bool operator==(std::unique_ptr<IType>& left, std::unique_ptr<IType>& right);