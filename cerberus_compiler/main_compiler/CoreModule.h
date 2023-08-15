//#pragma once
//
//#include <vector> 
//#include <unordered_map>
//#include <optional>
//#include <memory>
//
//#include "String.h"
//#include "ICompilerModule.h"
//#include "Lexer.h"
//
//
//class CoreModule : public ICompilerModule
//{
//public:
//	struct TypeDefinition {
//		String name;
//		std::vector<std::pair<String, String>> types;
//	};
//	struct Varible {
//		String type;
//		String name;
//	};
//	class IStatement {
//
//	};
//	class IExpression {
//
//	};
//	enum class Operator {
//		plus,
//		minus,
//		times,
//		divide,
//		modulo,
//		assigne,
//		equal,
//		inEqual,
//		lessThen,
//		lessEqqual,
//		greaterThen,
//		greaterEqual,
//		logical_and,
//		logical_or,
//		logical_xor,
//		logical_not
//	};
//	class LiteralExpression : public IExpression {
//		_CoreModule::Lexer::Token value;
//	};
//	class OperatinExpresion : public IExpression {
//		Operator op;
//		std::optional<std::unique_ptr<IExpression>> left;
//		std::unique_ptr<IExpression>  right;
//	};
//	class ProcedureCallExpression : public IExpression {
//		String function_name;
//		std::vector<std::unique_ptr<IExpression>> arguments;
//	};
//	class ExpresionStatement : public IStatement {
//		std::unique_ptr<IExpression> expr;
//	};
//	class VaribleDeclarationStatement : public IStatement {
//		Varible var;
//		std::unique_ptr<IExpression> init;
//	};
//	class IfStatement : public IStatement {
//		std::unique_ptr<IExpression> condision;
//		std::unique_ptr<IStatement> iftrue;
//		std::optional<std::unique_ptr<IStatement>> iffalse;
//	};
//	class WhileStatement: public IStatement{
//		std::unique_ptr<IExpression> condision;
//		std::unique_ptr<IStatement> body;
//	};
//	class ReturnStatement : public IStatement {
//		std::unique_ptr<IExpression> expr;
//	};
//	class BlockStatement : public IStatement {
//		std::vector<std::unique_ptr<IStatement>> block;
//	};
//	struct ProcedureDefinition {
//		String name;
//		//std::vector<Varible> returnTypes;
//		String returnType;
//		std::vector<Varible> arguments;
//		BlockStatement body;
//	};
//	virtual const char* const ModuleName();
//	virtual const char* const Version();
//	virtual void RegisterPreambule(IModuleRepository&);
//	virtual void Init(CompilerContext& symbols);
//	virtual void RegisterSymbols(const CodeFragment&, CompilerContext& symbols);
//	virtual void DefinitionOfSymbols(const CodeFragment&, CompilerContext& symbols);
//	virtual void GenerateCode(const CodeFragment&, CompilerContext& symbols);
//	virtual void Finalize(CompilerContext& symbols);
//	virtual ~CoreModule();
//private:
//	ProcedureDefinition* parseProcedureName(const String& name, CompilerContext& context);
//	std::unordered_map<String,ProcedureDefinition*,StringHash> procedures;
//	std::unordered_map<String,TypeDefinition*,StringHash> types;
//
//	std::optional<std::unique_ptr<CoreModule::LiteralExpression>> parseLiteralExpression(const String& name, int& offset, CompilerContext& context);
//	std::optional<std::unique_ptr<CoreModule::OperatinExpresion>> parseOperatinExpresion(const String& name, int& offset, CompilerContext& context);
//	std::optional<std::unique_ptr<CoreModule::ProcedureCallExpression>> parseProcedureCallExpression(const String& name, int& offset, CompilerContext& context);
//	std::optional<std::unique_ptr<CoreModule::ExpresionStatement>> parseExpresionStatement(const String& name, int& offset, CompilerContext& context);
//	std::optional<std::unique_ptr<CoreModule::VaribleDeclarationStatement>> parseVaribleDeclarationStatement(const String& name, int& offset, CompilerContext& context);
//	std::optional<std::unique_ptr<CoreModule::IfStatement>> parseIfStatement(const String& name, int& offset, CompilerContext& context);
//	std::optional<std::unique_ptr<CoreModule::WhileStatement>> parseWhileStatement(const String& name, int& offset, CompilerContext& context);
//	std::optional<std::unique_ptr<CoreModule::ReturnStatement>> parseReturnStatement(const String& name, int& offset, CompilerContext& context);
//	std::optional<std::unique_ptr<CoreModule::IStatement>> parseStatement(const String& name, int& offset, CompilerContext& context);
//	std::optional<std::unique_ptr<CoreModule::IExpression>> parseExpression(const String& name, int& offset, CompilerContext& context);
//	CoreModule::BlockStatement parseBody(const String& name, CompilerContext& context);
//};
//
