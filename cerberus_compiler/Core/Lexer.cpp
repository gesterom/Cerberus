#include "pch.h"
#include "Lexer.h"

#include <vector>
#include <string>
#include <iostream>

#include "../main_compiler/ModuleInterface.h"

String Preambule::Body_t::val() const
{
	String res;
	for (const auto& i : lines) {
		res.val += i.val;
	}
	if (lines.size() != 0) {
		res.pos = lines[0].pos;
	}
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
		if (str.val[i] == '\n') res.newLine();
		else res.character++;
	}
	return res;
}

String operator+=(String& s, char c) {
	s.val += c;
	return s;
}

const std::string criticalErrorMSG =
"\n"
"/-------------------------\\\n"
"[Encoutered Critical Error]\n"
"\\-------------------------/\n";

std::ostream& operator<<(std::ostream& out, const Position& pos) {
	out << pos.filename << " : " << pos.line << "_" << pos.character;
	return out;
}

void CompilerContext::critical_syntaxError(Position position, std::string errorMsg)
{
	std::cout << criticalErrorMSG << std::endl;
	std::cout << "Module : " << this->moduleName << " Position : " << position << std::endl;
	std::cout << "Error: " << errorMsg << std::endl;
	exit(-5);
}

bool inString(char c, const std::string& str) {
	for (int i = 0; i < str.size(); i++) {
		if (c == str[i]) return true;
	}
	return false;
}

int isMultiSymbolOperator(const String& body, int offset) {
	if (body.val.substr(offset, 2) == "==") return 2;
	if (body.val.substr(offset, 2) == "!=") return 2;
	if (body.val.substr(offset, 2) == ">=") return 2;
	if (body.val.substr(offset, 2) == "<=") return 2;
	if (body.val.substr(offset, 2) == "->") return 2;
	if (body.val.substr(offset, 2) == "++") return 2;
	if (body.val.substr(offset, 2) == "--") return 2;
	if (body.val.substr(offset, 2) == "+=") return 2;
	if (body.val.substr(offset, 2) == "-=") return 2;
	if (body.val.substr(offset, 2) == "*=") return 2;
	if (body.val.substr(offset, 2) == "/=") return 2;
	if (body.val.substr(offset, 2) == "%=") return 2;
	if (body.val.substr(offset, 2) == "|=") return 2;
	if (body.val.substr(offset, 2) == "<=>") return 3;
	return 0;
}

bool isKeyword(const String& str) {
	const std::vector<std::string> list = { "return","Let","copy","move","ref","if","else","while","xor","and","or" };
	for (const auto& s : list) {
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

TokenizedStream* lexer_fun(const Preambule& code, CompilerContext& context) {
	enum class Mode
	{
		id,
		number_literal,
		string_literal,
		character_literal,
		op,
	};
	Token t;
	std::vector<Token> res;
	auto addToken = [&t, &res]() {
		if (t.val != "") {
			res.push_back(t);
			t = Token{};
		}
		};
	std::string seperators = "+-*/%|!=<>.,;";
	std::string parentheses = "(){}[]";
	Mode mode = Mode::id;
	bool b_dot = false;
	for (int i = 0; i < code.body.val().size(); i++) {
		char c = code.body.val().val[i];
		int a = isMultiSymbolOperator(code.body.val(), i);
		if (a > 0) {
			addToken();
			for (int j = i; j < a + i; j++) {
				t.val += code.body.val().val[j];
			}
			t.type = (int32_t)TokenType::op;
			addToken();
			i += a;
		}
		else if (mode == Mode::id and inString(c, parentheses)) {
			addToken();
			t.val += c;
			t.type = (int32_t)TokenType::parentheses;
			addToken();
		}
		else if (mode == Mode::id and inString(c, seperators)) {
			addToken();
			t.val += c;
			t.type = (int32_t)TokenType::op;
			addToken();
		}
		else if (mode == Mode::id and c == '\'') {
			addToken();
			mode = Mode::character_literal;
			t.type = (int32_t)TokenType::character_literal;
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
			t.type = (int32_t)TokenType::string_literal;
		}
		else if (mode == Mode::string_literal and c != '"') {
			t.val += c;
		}
		else if (mode == Mode::string_literal and c == '"') {
			mode = Mode::id;
		}
		else if (mode == Mode::id and isdigit(c) and t.val.val == "") {
			mode = Mode::number_literal;
			t.type = (int32_t)TokenType::number_literal;
			t.val += c;
		}
		else if (mode == Mode::number_literal and not (isdigit(c) or c == '_' or c == '.')) {
			mode = Mode::id;
			addToken();
			b_dot = false;
		}
		else if (mode == Mode::number_literal and (isdigit(c) or c == '_' or c == '.')) {
			if (c == '.' and b_dot == true) {
				context.critical_syntaxError(moveCursor(code.body.val(), i), "double dot inside number");
			}
			if (c == '.') b_dot = true;
			t.val += c;
		}
		else if (mode == Mode::id and not isspace(c)) {
			t.type = (int32_t)TokenType::id;
			t.val += c;
		}
		else if (mode == Mode::id and isspace(c)) {
			addToken();
		}
	}
	auto tokenKeywords = [](std::vector<Token>& tokens) {
		for (auto& i : tokens) {
			if (isKeyword(i.val)) {
				i.type = (int32_t)TokenType::keyword;
			}
			else if (isTypeName(i.val)) {
				i.type = (int32_t)TokenType::TypeName;
			}
		}
		};
	tokenKeywords(res);

	TokenizedStream* token_stream = new TokenizedStream();
	token_stream->body = res;

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