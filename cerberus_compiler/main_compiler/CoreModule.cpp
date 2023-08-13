#include "CoreModule.h"

#include "IModuleRepository.h"
#include "CodeFragment.h"
#include "CompilerContext.h"
#include "String.h"
#include <iostream>
#include <stdint.h>

inline const std::string moduleName = "Core";


void CoreModule::RegisterPreambule(IModuleRepository& repo) {
	repo.registerToPreambule("type", this);
	repo.registerToPreambule("procedure", this);
}

void CoreModule::Init(CompilerContext& context) {}

bool lowercase(char c) {
	return 'a' <= c and 'z' >= c;
}

bool digit(char c) {
	return '0' <= c and '9' >= c;
}

bool NameAlowedCharacters(char c) {
	return (c >= 'a' and c <= 'z') or
		(c >= 'A' and c <= 'Z') or
		(c >= '0' and c <= '9') or
		(c == '_');
}

enum class ParseProcedureName {
	procedureName,
	parentethisLeft,
	ArgumentType,
	ArgumentName,
	coma,
	returnType,
	end,
};

std::string ErrorMsgInvalidTypeNameChar(char c) {
	return std::string("Character ") + c + " is not allowed inside Type Name";
}
std::string ErrorMsgInvalidVaribleNameChar(char c) {
	return std::string("Character ") + c + " is not allowed inside varible name";
}

CoreModule::ProcedureDefinition* CoreModule::parseProcedureName(const String& procedureHead, CompilerContext& context) {
	ParseProcedureName mode = ParseProcedureName::procedureName;
	ProcedureDefinition* def = new ProcedureDefinition();
	Varible t;
	def->name.pos = procedureHead.pos;
	for (int i = 0; i < procedureHead.val.size(); i++) {
		char c = procedureHead.val[i];
		if (mode == ParseProcedureName::procedureName and c != '(' and not isspace(c)) {
			def->name.val += c;
		}
		else if (mode == ParseProcedureName::procedureName and c == '(') {
			mode = ParseProcedureName::ArgumentType;
		}
		else if (mode == ParseProcedureName::procedureName and isspace(c) and def->name.val.empty()) {
			mode = ParseProcedureName::parentethisLeft;
		}
		else if (mode == ParseProcedureName::parentethisLeft and c == '(') {
			mode = ParseProcedureName::ArgumentType;
		}
		else if (mode == ParseProcedureName::parentethisLeft and not isspace(c))
		{
			context.critical_Unexpected_Character(moveCursor(procedureHead, i), c);
		}
		else if (mode == ParseProcedureName::ArgumentType and not isspace(c) and NameAlowedCharacters(c)) {
			if (t.type.val.empty()) {
				t.type.pos = moveCursor(procedureHead, i);
			}
			t.type += c;
		}
		else if (mode == ParseProcedureName::ArgumentType and not isspace(c) and not NameAlowedCharacters(c)) {
			context.critical_syntaxError(moveCursor(procedureHead, i), "CoreModule", ErrorMsgInvalidTypeNameChar(c));
		}
		else if (mode == ParseProcedureName::ArgumentType and isspace(c)) {
			mode = ParseProcedureName::ArgumentName;
		}
		else if (mode == ParseProcedureName::ArgumentName and not isspace(c) and NameAlowedCharacters(c)) {
			if (t.name.val.empty()) {
				t.name.pos = moveCursor(procedureHead, i);
			}
			t.name += c;
		}
		else if (mode == ParseProcedureName::ArgumentName and not isspace(c) and c == ',') {
			mode = ParseProcedureName::ArgumentType;
			def->arguments.push_back(t);
			t = Varible();
		}
		else if (mode == ParseProcedureName::ArgumentName and not isspace(c) and c == ')') {
			def->arguments.push_back(t);
			t = Varible();
			mode = ParseProcedureName::returnType;
		}
		else if (mode == ParseProcedureName::ArgumentType and not isspace(c) and not NameAlowedCharacters(c)) {
			context.critical_syntaxError(moveCursor(procedureHead, i), "CoreModule", ErrorMsgInvalidTypeNameChar(c));
		}
		else if (mode == ParseProcedureName::returnType and not isspace(c)) {
			if (def->returnType.val.empty()) {
				def->returnType.pos = moveCursor(procedureHead, i);
			}
			def->returnType += c;
		}
		else if (mode == ParseProcedureName::returnType and isspace(c) and not def->returnType.val.empty()) {
			mode = ParseProcedureName::end;
		}
		else if (mode == ParseProcedureName::end and not isspace(c)) {
			context.critical_syntaxError(moveCursor(procedureHead, i), "CoreModule", ErrorMsgInvalidTypeNameChar(c));
		}
	}
	return def;
}

CompilerContext::SymbolName parseTypeName(const CodeFragment& code, CompilerContext& context) {
	CompilerContext::SymbolName type;
	type.pos = code.name.pos;
	for (int i = 0; i < code.name.val.size(); i++) {
		if (isspace(code.name.val[i])) {
			for (int j = i; j < code.name.val.size(); j++) {
				if (not isspace(code.name.val[j])) context.critical_Unexpected_Character(moveCursor(code.name, i), code.name.val[i]);
			}
			break;
		}
		if (i == 0 and (lowercase(code.name.val[i]) or digit(code.name.val[i]))) {
			context.critical_syntaxError(moveCursor(code.name, i), "CoreModule", ErrorMsgInvalidTypeNameChar(code.name.val[i]));
		}
		else if (not NameAlowedCharacters(code.name.val[i])) {
			context.critical_Unexpected_Character(moveCursor(code.name, i), code.name.val[i]);
		}
		type += code.name.val[i];
	}
	return type;
}

void CoreModule::RegisterSymbols(const CodeFragment& code, CompilerContext& context)
{
	if (code.preambule == "type") {
		auto res = new TypeDefinition();
		res->name = parseTypeName(code, context);
		context.symbols.add("type", res->name);

		types.emplace(code.name, res);
	}
	else if (code.preambule == "procedure") {
		auto def = parseProcedureName(code.name, context);
		context.symbols.define("procedure", def->name, def);
		procedures.emplace(code.name, def);
	}
}
std::string printTypeDefinition(CoreModule::TypeDefinition* type) {
	if (type == nullptr) {
		return "null";
	}
	std::string res = "{";
	for (auto i : type->types) {
		res += i.first.val + ":" + i.second.val + ";";
	}
	return res + "}";
}

enum class TypeParserMode {
	TypeName,
	FieldName,
	Semicolon,
};

std::string lefttrim(std::string str) {
	std::string res;
	size_t i = 0;
	while (isspace(str[i]))i++;
	for (; i < str.size(); i++) {
		res += str[i];
	}
	return res;
}

std::string righttrim(std::string str) {
	std::string res;
	int64_t i = str.size() - 1;
	while (isspace(str[i]))i--;
	for (; i >= 0; i--) {
		res = str[i] + res;
	}
	return res;
}

String trim(String str) {
	String res;
	res.pos = str.pos;
	res.val = lefttrim(righttrim(str.val));
	return res;
}

std::optional<std::unique_ptr<CoreModule::IStatement>> CoreModule::parseStatement(const String& name, int& offset, CompilerContext& context) {
	auto r = parseExpresionStatement(name,offset,context);
	if (r.has_value()) {
		std::unique_ptr<CoreModule::IStatement> statement = std::move(*r);
		return std::optional<std::unique_ptr<CoreModule::IStatement>>(std::move(statement));
	}
	return std::optional<std::unique_ptr<CoreModule::IStatement>>{};
}

std::optional<std::unique_ptr<CoreModule::IExpression>> CoreModule::parseExpression(const String& name, int& offset, CompilerContext& context)
{
	return std::optional<std::unique_ptr<CoreModule::IExpression>>();
}

void CoreModule::DefinitionOfSymbols(const CodeFragment& code, CompilerContext& context)
{
	if (code.preambule == "type") {
		TypeParserMode mode = TypeParserMode::TypeName;
		TypeDefinition* def = new TypeDefinition();
		CompilerContext::SymbolName typeName;
		CompilerContext::SymbolName fieldName;

		for (int i = 0; i < code.body.val.size(); i++) {
			char c = code.body.val[i];
			if (not NameAlowedCharacters(c) and not isspace(c) and c != ';') context.critical_Unexpected_Character(moveCursor(code.body, i), c);

			if (mode == TypeParserMode::TypeName and typeName.val != "" and not isspace(c) and c != ';') {
				typeName.pos = moveCursor(code.body, i);
				typeName.val += c;
			}
			else if (mode == TypeParserMode::TypeName and not isspace(c) and c != ';') {
				typeName.val += c;
			}
			else if (mode == TypeParserMode::TypeName and isspace(c) and typeName.val != "") {
				mode = TypeParserMode::FieldName;
			}
			else if (mode == TypeParserMode::FieldName and not isspace(c) and typeName.val != "" and c != ';') {
				fieldName.pos = moveCursor(code.body, i);
				fieldName.val += c;
			}
			else if (mode == TypeParserMode::FieldName and not isspace(c) and c != ';') {
				fieldName.val += c;
			}
			else if (mode == TypeParserMode::FieldName and isspace(c) and fieldName.val != "") {
				mode = TypeParserMode::Semicolon;
			}
			else if ((mode == TypeParserMode::Semicolon or mode == TypeParserMode::FieldName) and c == ';') {

				auto it = context.symbols.vals.find("type");
				if (it == context.symbols.vals.end()) context.critical_InternalError(moveCursor(code.body, i), "CoreModule", "Type Symbole Table was uninizilied");
				auto itt = it->second.find(typeName);
				if (itt == it->second.end()) { context.error_UnknownType(typeName.pos, typeName); }

				def->types.emplace_back(typeName, fieldName);

				mode = TypeParserMode::TypeName;
				typeName = CompilerContext::SymbolName();
				fieldName = CompilerContext::SymbolName();
			}
			else if (mode == TypeParserMode::Semicolon and c != ';') {
				context.critical_Unexpected_Character(moveCursor(code.body, i), c);
			}
			context.symbols.define("type", trim(code.name), def);
		}
	}
	else if (code.preambule == "procedure") {
		auto proc = procedures.find(code.name)->second;
		std::cout << "Procedure : " << proc->name.val << std::endl;
		for (auto& i : proc->arguments) {
			std::cout << "\tArgument : " << i.type.val << " " << i.name.val << std::endl;
		}
		std::cout << "\tReturn Type: " << proc->returnType.val << std::endl;
		parseBody(code.body, context);
	}
}

void CoreModule::GenerateCode(const CodeFragment&, CompilerContext& context)
{
}

void CoreModule::Finalize(CompilerContext& context)
{
	std::cout << "Types Begin\n";
	auto it = context.symbols.vals.find("type");
	if (it == context.symbols.vals.end()) return;
	for (const auto& i : it->second) {
		std::cout << "\t" << i.first.val << " = " << printTypeDefinition((CoreModule::TypeDefinition*)i.second) << std::endl;
	}
	std::cout << "Types End\n";
}

CoreModule::~CoreModule()
{
	for (auto i : this->procedures) {
		delete i.second;
	}
	for (auto i : this->types) {
		delete i.second;
	}
}


CoreModule::BlockStatement CoreModule::parseBody(const String& body, CompilerContext& context) {
	auto tokens = _CoreModule::Lexer::lexer(body, context);
	for (auto& i : tokens) {
		std::cout << "Token : " << i.type << " -> " << i.val.val << std::endl;
	}
	return BlockStatement{};
}

std::optional<std::unique_ptr<CoreModule::ReturnStatement>> CoreModule::parseReturnStatement(const String& name, int& offset, CompilerContext& context)
{
	return std::optional<std::unique_ptr<CoreModule::ReturnStatement>>();
}

std::optional<std::unique_ptr<CoreModule::LiteralExpression>> CoreModule::parseLiteralExpression(const String& name, int& offset, CompilerContext& context)
{
	return std::optional<std::unique_ptr<CoreModule::LiteralExpression>>();
}

std::optional<std::unique_ptr<CoreModule::OperatinExpresion>> CoreModule::parseOperatinExpresion(const String& name, int& offset, CompilerContext& context)
{
	return std::optional<std::unique_ptr<CoreModule::OperatinExpresion>>();
}

std::optional<std::unique_ptr<CoreModule::ProcedureCallExpression>> CoreModule::parseProcedureCallExpression(const String& name, int& offset, CompilerContext& context)
{
	return std::optional<std::unique_ptr<CoreModule::ProcedureCallExpression>>();
}

std::optional<std::unique_ptr<CoreModule::ExpresionStatement>> CoreModule::parseExpresionStatement(const String& name, int& offset, CompilerContext& context)
{
	return std::optional<std::unique_ptr<CoreModule::ExpresionStatement>>();
}

std::optional<std::unique_ptr<CoreModule::VaribleDeclarationStatement>> CoreModule::parseVaribleDeclarationStatement(const String& name, int& offset, CompilerContext& context)
{
	return std::optional<std::unique_ptr<CoreModule::VaribleDeclarationStatement>>();
}

std::optional<std::unique_ptr<CoreModule::IfStatement>> CoreModule::parseIfStatement(const String& name, int& offset, CompilerContext& context)
{
	return std::optional<std::unique_ptr<CoreModule::IfStatement>>();
}

std::optional<std::unique_ptr<CoreModule::WhileStatement>> CoreModule::parseWhileStatement(const String& name, int& offset, CompilerContext& context)
{
	return std::optional<std::unique_ptr<CoreModule::WhileStatement>>();
}