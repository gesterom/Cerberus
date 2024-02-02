#include "pch.h"
#include "Lexer.h"

#include <vector>
#include <string>
#include <iostream>

#include "../main_compiler/ModuleInterface.h"

#include "Consts.h"
#include <algorithm>
#include <cassert>

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
			//for (const auto& i : hidenOperators) {
			//	if(str.representation == i) return 0;
			//}
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
		comment,
	};
	bool escapeCharacter = false;
	Token t;
	t.type = (uint16_t)Lexer::TokenType::id;
	std::vector<Token> res;

	Position pos = body.pos;
	Mode mode = Mode::id;
	bool b_dot = false;

	auto addToken = [&t, &res, &pos,&mode,&b_dot]() {
		if (t.val != "") {
			res.push_back(t);
			t = Token{};
			t.type = (uint16_t)Lexer::TokenType::id;
			mode = Mode::id;
			b_dot = false;
		}
		t.val.pos = pos;
		};

	
	
	for (int i = 0; i < body.val.size(); i++) {
		char c = body.val[i];

		if (c == '\n') {
			pos.line++;
			pos.character = 0;
		}

		pos.character++;

		int a = isMultiSymbolOperator(body, i);
		if (body.val.substr(i, 2) == "//" and mode != Mode::character_literal and mode != Mode::string_literal) {
			mode = Mode::comment;
		}
		else if (mode == Mode::comment) {
			if (c == '\n') mode = Mode::id;
		}
		else if (a > 0 and ((mode == Mode::id and t.val == "") or inString(c, seperators)) and mode != Mode::string_literal ) {
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
		else if (mode == Mode::character_literal and c != '\'' and c != '\\' and escapeCharacter == false) {
			t.val += c;
		}
		else if (mode == Mode::character_literal and c == '\\' and escapeCharacter == false) {
			escapeCharacter = true;
		}
		else if (mode == Mode::character_literal and escapeCharacter == true) {
			if (c == 'n') {
				t.val += '\n';
			}
			else if (c == 'r') {
				t.val += '\r';
			}
			else if (c == '\\') {
				t.val += '\\';
			}
			else if (c == 't') {
				t.val += '\t';
			}
			else if (c == '"') {
				t.val += '"';
			}
			else if (c == '0') {
				t.val += '\0';
			}
			escapeCharacter = false;
		}
		else if (mode == Mode::character_literal and c == '\'') {
			mode = Mode::id;
			addToken(); // can cuse problems 
		}
		else if (mode == Mode::id and c == '"') {
			addToken();
			mode = Mode::string_literal;
			t.type = (uint16_t)Lexer::TokenType::string_literal;
		}
		else if (mode == Mode::string_literal and c != '"' and c != '\\' and escapeCharacter == false) {
			t.val += c;
		}
		else if (mode == Mode::string_literal and c == '\\') {
			escapeCharacter = true;
		}
		else if (mode == Mode::string_literal and escapeCharacter == true) {
			if (c == 'n') {
				t.val += '\n';
			}
			else if (c == 'r') {
				t.val += '\r';
			}
			else if (c == '\\') {
				t.val += '\\';
			}
			else if (c == 't') {
				t.val += '\t';
			}
			else if (c == '"') {
				t.val += '"';
			}
			escapeCharacter = false;
		}
		else if (mode == Mode::string_literal and c == '"') {
			{//without if ( for empty string as tokens )
				res.push_back(t);
				t = Token{};
				t.type = (uint16_t)Lexer::TokenType::id;
				t.val.pos = pos;
			}
			mode = Mode::id;
		}
		else if (mode == Mode::id and isdigit(c) and t.val.val == "") {
			mode = Mode::number_literal;
			if (b_dot) {
				t.type = (uint16_t)Lexer::TokenType::Float_literal;
			}
			else {
				t.type = (uint16_t)Lexer::TokenType::Integer_literal;
			}
			t.val.pos = pos;
			t.val += c;
			b_dot = false;
		}
		else if (mode == Mode::number_literal and not (isdigit(c) or c == '_' or c == '.')) {
			mode = Mode::id;
			addToken();
			b_dot = false;
			if (c != '\n')
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
			else if (isKeyword(i.val) and (i.val == "true" or i.val == "false")) {
				i.type = (uint16_t)Lexer::TokenType::Bool_literal;
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

	if (code.preambule_name.val == "procedure") {
		auto body = Combine(code);

		TokenizedStream* token_stream = new TokenizedStream();
		token_stream->body = lex(body, context);
		token_stream->name = lex(code.name, context);

		return token_stream;
	}
	else if (code.preambule_name.val == "import_c") {
		TokenizedStream* token_stream = new TokenizedStream();
		token_stream->name = lex(code.name, context);
		return token_stream;
	}
	else if (code.preambule_name.val == "type") {

		auto body = Combine(code);

		TokenizedStream* token_stream = new TokenizedStream();
		token_stream->body = lex(body, context);
		token_stream->name = lex(code.name, context);

		return token_stream;
	}
	return nullptr;
}
