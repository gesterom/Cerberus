#include "pch.h"
#include "Lexer.h"

#include <vector>
#include <string>
#include <iostream>

#include "../main_compiler/ModuleInterface.h"

#include "Consts.h"
#include <algorithm>

String Combine(const Preambule& code) {
	String res;
	for (const auto& i : code.body.lines) {
		res.val += i.val + '\n';
	}
	if (code.body.lines.size() == 0) return res;
	res.pos = code.body.lines[0].pos;
	return res;
}

bool operator!=(const String& a, const String& b) {
	return a.val != b.val;
}
bool operator==(const String& a, const String& b) {
	return a.val == b.val;
}

bool operator!=(const String& s, const std::string& cs) {
	return s.val != cs;
}
bool operator==(const String& s, const std::string& cs) {
	return s.val == cs;
}

Position moveCursor(const String& str, int n)
{
	Position res = str.pos;
	for (int i = 0; i < n; i++) {
		if (str.val[i] == '\n') {
			res.line++;
			res.character = 1;
		}
		else res.character++;
	}
	return res;
}

String operator+=(String& s, char c) {
	s.val += c;
	return s;
}

bool inString(char c, const std::string& str) {
	for (int i = 0; i < str.size(); i++) {
		if (c == str[i]) return true;
	}
	return false;
}

int isMultiSymbolOperator(const String& body, int offset) {

	for (const auto& str : operators) {
		if (body.val.substr(offset, str.representation.size()) == str.representation) {
			return (int)str.representation.size(); // not predicting any operator to be longer then 100 character so this shoude be ok;
		}
	}
	return 0;
}

bool isKeyword(const String& str) {
	//const std::vector<std::string> list = { "return","Let","copy","move","ref","if","else","while" };
	for (const auto& s : keywords) {
		if (str == s) return true;
	}
	return false;
}

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

bool isTypeName(const String& str) {
	if (str.val[0] >= 'A' and str.val[0] <= 'Z') return true;
	return false;
}

std::vector<Token> lex(String body, CompilerInterface* context) {
	enum class Mode
	{
		id,
		number_literal,
		string_literal,
		character_literal,
		op,
	};
	Token t;
	t.type = (uint16_t)Lexer::TokenType::id;
	std::vector<Token> res;

	Position pos = body.pos;

	auto addToken = [&t, &res, &pos]() {
		if (t.val != "") {
			res.push_back(t);
			t = Token{};
			t.type = (uint16_t)Lexer::TokenType::id;
		}
		t.val.pos = pos;
		};

	Mode mode = Mode::id;
	bool b_dot = false;
	for (int i = 0; i < body.val.size(); i++) {
		char c = body.val[i];

		if (c == '\n') {
			pos.line++;
			pos.character = 0;
		}

		pos.character++;

		int a = isMultiSymbolOperator(body, i);
		if (a > 0 and mode == Mode::id) {
			addToken();
			for (int j = i; j < a + i; j++) {
				t.val += body.val[j];
			}
			t.type = (uint16_t)Lexer::TokenType::op;
			t.val.pos = pos;
			addToken();
			i += a - 1;
		}
		else if (mode == Mode::id and inString(c, parentheses)) {
			addToken();
			t.val += c;
			t.type = (uint16_t)Lexer::TokenType::parentheses;
			t.val.pos = pos;
			addToken();
		}
		else if ((mode == Mode::id or mode == Mode::number_literal) and inString(c, seperators)) {
			addToken();
			t.val += c;
			t.type = (uint16_t)Lexer::TokenType::op;
			t.val.pos = pos;
			addToken();
		}
		else if (mode == Mode::id and c == '\'') {
			addToken();
			mode = Mode::character_literal;
			t.type = (uint16_t)Lexer::TokenType::character_literal;
		}
		else if (mode == Mode::character_literal and c != '\'') {
			t.val += c;
		}
		else if (mode == Mode::character_literal and c == '\'') {
			mode = Mode::id;
		}
		else if (mode == Mode::id and c == '"') {
			addToken();
			mode = Mode::string_literal;
			t.type = (uint16_t)Lexer::TokenType::string_literal;
		}
		else if (mode == Mode::string_literal and c != '"') {
			t.val += c;
		}
		else if (mode == Mode::string_literal and c == '"') {
			addToken();
			mode = Mode::id;
		}
		else if (mode == Mode::id and isdigit(c) and t.val.val == "") {
			mode = Mode::number_literal;
			t.type = (uint16_t)Lexer::TokenType::number_literal;
			t.val.pos = pos;
			t.val += c;
			b_dot = false;
		}
		else if (mode == Mode::number_literal and not (isdigit(c) or c == '_' or c == '.')) {
			mode = Mode::id;
			addToken();
			b_dot = false;
			if(c != '\n')
			{
				i--;
				pos.character--;
			}
		}
		else if (mode == Mode::number_literal and (isdigit(c) or c == '_' or c == '.')) {
			if (c == '.' and b_dot == true) {
				context->critical_error_msg(context->context, CriticalErrorType::SyntaxError, moveCursor(body, i), "double dot inside number");
			}
			if (c == '.') b_dot = true;
			t.val += c;
		}
		else if (mode == Mode::id and not isspace(c)) {
			if (t.val.val == "")
				t.val.pos = pos;
			t.val += c;
		}
		else if (mode == Mode::id and isspace(c)) {
			addToken();
		}

	}
	addToken();
	auto tokenKeywords = [](std::vector<Token>& tokens) {
		for (auto& i : tokens) {
			if (isKeyword(i.val) and i.val == "Let") {
				i.type = (uint16_t)Lexer::TokenType::TypeName;
			}
			else if (i.type == (uint16_t)Lexer::TokenType::op and i.val.val == ":") {
				i.type = (uint16_t)Lexer::TokenType::colon;
			}
			else if (isKeyword(i.val)) {
				i.type = (uint16_t)Lexer::TokenType::keyword;
			}
			else if (isTypeName(i.val) and i.type == (uint16_t)Lexer::TokenType::id) {
				i.type = (uint16_t)Lexer::TokenType::TypeName;
			}
		}
		};
	tokenKeywords(res);
	return res;
}

TokenizedStream* lexer_fun(const Preambule& code, CompilerInterface* context) {

	if (code.preambule_name.val != "procedure") return nullptr;

	auto body = Combine(code);


	TokenizedStream* token_stream = new TokenizedStream();
	token_stream->body = lex(body, context);
	token_stream->name = lex(code.name, context);

	return token_stream;
}

//ProcedureDefinition* parseProcedureName(const String& procedureHead, CompilerContext& context) {
//	ParseProcedureName mode = ParseProcedureName::procedureName;
//	ProcedureDefinition* def = new ProcedureDefinition();
//	Varible t;
//	def->name.pos = procedureHead.pos;
//	for (int i = 0; i < procedureHead.val.size(); i++) {
//		char c = procedureHead.val[i];
//		if (mode == ParseProcedureName::procedureName and c != '(' and not isspace(c)) {
//			def->name.val += c;
//		}
//		else if (mode == ParseProcedureName::procedureName and c == '(') {
//			mode = ParseProcedureName::ArgumentType;
//		}
//		else if (mode == ParseProcedureName::procedureName and isspace(c) and def->name.val.empty()) {
//			mode = ParseProcedureName::parentethisLeft;
//		}
//		else if (mode == ParseProcedureName::parentethisLeft and c == '(') {
//			mode = ParseProcedureName::ArgumentType;
//		}
//		else if (mode == ParseProcedureName::parentethisLeft and not isspace(c))
//		{
//			context.critical_Unexpected_Character(moveCursor(procedureHead, i), c);
//		}
//		else if (mode == ParseProcedureName::ArgumentType and not isspace(c) and NameAlowedCharacters(c)) {
//			if (t.type.val.empty()) {
//				t.type.pos = moveCursor(procedureHead, i);
//			}
//			t.type += c;
//		}
//		else if (mode == ParseProcedureName::ArgumentType and not isspace(c) and not NameAlowedCharacters(c)) {
//			context.critical_syntaxError(moveCursor(procedureHead, i), ErrorMsgInvalidTypeNameChar(c));
//		}
//		else if (mode == ParseProcedureName::ArgumentType and isspace(c)) {
//			mode = ParseProcedureName::ArgumentName;
//		}
//		else if (mode == ParseProcedureName::ArgumentName and not isspace(c) and NameAlowedCharacters(c)) {
//			if (t.name.val.empty()) {
//				t.name.pos = moveCursor(procedureHead, i);
//			}
//			t.name += c;
//		}
//		else if (mode == ParseProcedureName::ArgumentName and not isspace(c) and c == ',') {
//			mode = ParseProcedureName::ArgumentType;
//			def->arguments.push_back(t);
//			t = Varible();
//		}
//		else if (mode == ParseProcedureName::ArgumentName and not isspace(c) and c == ')') {
//			def->arguments.push_back(t);
//			t = Varible();
//			mode = ParseProcedureName::returnType;
//		}
//		else if (mode == ParseProcedureName::ArgumentType and not isspace(c) and not NameAlowedCharacters(c)) {
//			context.critical_syntaxError(moveCursor(procedureHead, i), ErrorMsgInvalidTypeNameChar(c));
//		}
//		else if (mode == ParseProcedureName::returnType and not isspace(c)) {
//			if (def->returnType.val.empty()) {
//				def->returnType.pos = moveCursor(procedureHead, i);
//			}
//			def->returnType += c;
//		}
//		else if (mode == ParseProcedureName::returnType and isspace(c) and not def->returnType.val.empty()) {
//			mode = ParseProcedureName::end;
//		}
//		else if (mode == ParseProcedureName::end and not isspace(c)) {
//			context.critical_syntaxError(moveCursor(procedureHead, i), ErrorMsgInvalidTypeNameChar(c));
//		}
//	}
//	return def;
//}

//CompilerContext::SymbolName parseTypeName(const CodeFragment& code, CompilerContext& context) {
//	CompilerContext::SymbolName type;
//	type.pos = code.name.pos;
//	for (int i = 0; i < code.name.val.size(); i++) {
//		if (isspace(code.name.val[i])) {
//			for (int j = i; j < code.name.val.size(); j++) {
//				if (not isspace(code.name.val[j])) context.critical_Unexpected_Character(moveCursor(code.name, i), code.name.val[i]);
//			}
//			break;
//		}
//		if (i == 0 and (lowercase(code.name.val[i]) or digit(code.name.val[i]))) {
//			context.critical_syntaxError(moveCursor(code.name, i), ErrorMsgInvalidTypeNameChar(code.name.val[i]));
//		}
//		else if (not NameAlowedCharacters(code.name.val[i])) {
//			context.critical_Unexpected_Character(moveCursor(code.name, i), code.name.val[i]);
//		}
//		type += code.name.val[i];
//	}
//	return type;
//}