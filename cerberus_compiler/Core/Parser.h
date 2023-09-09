#pragma once

#include "Lexer.h"

#include <memory>
#include <optional>

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
};

struct BinaryOperatorExpression : public IExpression {
	String op;
	std::unique_ptr<IExpression> left;
	std::unique_ptr<IExpression> righ;
};

struct PrefixOperatorExpression : public IExpression {
	String op;
	std::unique_ptr<IExpression> right;
};

struct PostfixOperatorExpression : public IExpression {
	String op;
	std::unique_ptr<IExpression> left;
};

struct ProcedureCallExpression : public IExpression {
	String name;
	std::vector<IExpression*> args;
};

struct Procedure {
	std::vector<String> args;
	String returnType;
	std::vector<std::unique_ptr<IStatement>> code;
};


Procedure parseProcedure(TokenizedStream* stream, CompilerInterface* context);
