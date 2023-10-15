#pragma once

#include "Lexer.h"

#include <memory>
#include <optional>
#include "Consts.h"

struct IExpression;
struct IStatement;

struct IStatement
{
};

struct IfStatement : public IStatement
{
	std::unique_ptr< IExpression > condition;
	std::unique_ptr<IStatement> ifTrue;
	std::unique_ptr<IStatement> ifFalse;
};
struct WhileStatement : public IStatement
{
	std::unique_ptr< IExpression > condition;
	std::unique_ptr<IStatement> body;
};
struct ReturnStatement : public IStatement
{
	std::unique_ptr< IExpression > exp;
};
struct ExpressionStatement : public IStatement
{
	std::unique_ptr< IExpression > exp;
};

struct BlockStatement : public IStatement {
	std::vector<std::unique_ptr<IStatement>> body;
};
struct VaribleDeclaration : public IStatement {
	String type;
	String varibleName;
	std::optional< std::unique_ptr< IExpression > > init;
};

struct IExpression {
	std::string type;//TODO change to type_id
	Position pos;
	virtual std::string toString() = 0;
};



struct VaribleExpression : public IExpression
{
	String varibleName;
	virtual std::string toString();
};

struct ConstructorExpression : public IExpression
{
	String typeName;
	//String varibleName;
	virtual std::string toString();
};

struct LiteralExpression : public IExpression
{
	String value;
	virtual std::string toString();
};

struct PrefixOperatorExpression : public IExpression {
	String op;
	Operator_t op_type;
	std::unique_ptr<IExpression> right;
	virtual std::string toString();
};

struct BinaryOperatorExpression : public IExpression {
	String op;
	Operator_t op_type;
	std::unique_ptr<IExpression> left;
	std::unique_ptr<IExpression> right;
	virtual std::string toString();
};

struct PostfixOperatorExpression : public IExpression {
	String op;
	Operator_t op_type;
	std::unique_ptr<IExpression> left;
	virtual std::string toString();
};

struct ProcedureCallExpression : public IExpression {
	String name;
	std::vector<IExpression*> args;
	virtual std::string toString();
};

struct Procedure {
	std::vector<String> args;
	String returnType;
	std::vector<std::unique_ptr<IStatement>> code;
};


Procedure parseProcedure(TokenizedStream* stream, CompilerInterface* context);
