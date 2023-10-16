#pragma once

#include "Lexer.h"

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
struct VaribleDeclaration : public IStatement {
	String type;
	String varibleName;
	std::unique_ptr< IExpression > init;
	virtual void visit(IVisitor* visitor);
};


struct IExpression {
	std::string type;//TODO change to type_id
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

struct TypeNameExpression : public IExpression
{
	String typeName;
	//String varibleName;
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
	std::vector<std::unique_ptr<IExpression>> args;
	virtual std::string toString();
	virtual void visit(IVisitor* visitor);
};

class IVisitor {
	public:
	virtual void visit(VaribleExpression*) = 0;
	virtual void visit(TypeNameExpression*) = 0;
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
};

class PrinterVisitor : public IVisitor {
	int indent_no = 0;
	std::string indent();
public:
	CompilerInterface* icompiler = nullptr;
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
};

struct Procedure {
	std::vector<String> args;
	String returnType;
	std::vector<std::unique_ptr<IStatement>> code;
};


Procedure parseProcedure(TokenizedStream* stream, CompilerInterface* context);
