#include "CoreModule.h"

#include "IModuleRepository.h"
#include "CodeFragment.h"
#include "CompilerContext.h"
#include "String.h"
#include <iostream>

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

void CoreModule::RegisterSymbols(const CodeFragment& code, CompilerContext& context)
{
	if (code.preambule == "type") {
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
				context.critical_Unexpected_Character(moveCursor(code.name, i), code.name.val[i]);
			}
			else if (not NameAlowedCharacters(code.name.val[i])) {
				context.critical_Unexpected_Character(moveCursor(code.name, i), code.name.val[i]);
			}
			type += code.name.val[i];
		}
		context.symbols.add("type", type);
	}
	else if (code.preambule == "procedure") {

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
	int i = 0;
	while (isspace(str[i]))i++;
	for (; i < str.size(); i++) {
		res += str[i];
	}
	return res;
}

std::string righttrim(std::string str) {
	std::string res;
	int i = str.size() - 1;
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
				if (it == context.symbols.vals.end()) context.critical_InternalError(moveCursor(code.body, i), "CoreModule", " Type Symbole Table was uninizilied");
				auto itt = it->second.find(typeName);
				if (itt == it->second.end()) { context.error_UnknownType(typeName.pos, typeName); }

				def->types.emplace_back(typeName, fieldName);

				mode = TypeParserMode::TypeName;
				typeName = CompilerContext::SymbolName();
				fieldName = CompilerContext::SymbolName();
			}
			else if (mode == TypeParserMode::Semicolon and c!= ';') {
				context.critical_Unexpected_Character(moveCursor(code.body,i),c);
			}
			context.symbols.define("type", trim(code.name), def);
		}
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
