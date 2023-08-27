//#include "Lexer.h"
//
//#include <string>
//
//inline const std::string moduleName = "Core";
//
//std::string to_string(_CoreModule::Lexer::Token::TokenType t) {
//	switch (t)
//	{
//	case _CoreModule::Lexer::Token::TokenType::unkowwn:
//		return "unknown";
//		break;
//	case _CoreModule::Lexer::Token::TokenType::op:
//		return "operator";
//		break;
//	case _CoreModule::Lexer::Token::TokenType::id:
//		return "id";
//		break;
//	case _CoreModule::Lexer::Token::TokenType::TypeName:
//		return "TypeName";
//		break;
//	case _CoreModule::Lexer::Token::TokenType::parentheses:
//		return  "parentheses";
//		break;
//	case _CoreModule::Lexer::Token::TokenType::keyword:
//		return "keyword";
//		break;
//	case _CoreModule::Lexer::Token::TokenType::string_literal:
//		return "string_literal";
//		break;
//	case _CoreModule::Lexer::Token::TokenType::character_literal:
//		return  "character_literal";
//		break;
//	case _CoreModule::Lexer::Token::TokenType::number_literal:
//		return "number_literal";
//		break;
//
//	default:
//		return "unseted";
//		break;
//	}
//	return "__unseted__";
//}
//
//std::ostream& operator<<(std::ostream& out, _CoreModule::Lexer::Token::TokenType t) {
//	out << to_string(t);
//	return out;
//}
//
//bool inString(char c, const std::string& str) {
//	for (int i = 0; i < str.size(); i++) {
//		if (c == str[i]) return true;
//	}
//	return false;
//}
//
//int isMultiSymbolOperator(const String& body, int offset) {
//	if (body.val.substr(offset, 2) == "==") return 2;
//	if (body.val.substr(offset, 2) == "!=") return 2;
//	if (body.val.substr(offset, 2) == ">=") return 2;
//	if (body.val.substr(offset, 2) == "<=") return 2;
//	if (body.val.substr(offset, 2) == "->") return 2;
//	if (body.val.substr(offset, 2) == "++") return 2;
//	if (body.val.substr(offset, 2) == "--") return 2;
//	if (body.val.substr(offset, 2) == "+=") return 2;
//	if (body.val.substr(offset, 2) == "-=") return 2;
//	if (body.val.substr(offset, 2) == "*=") return 2;
//	if (body.val.substr(offset, 2) == "/=") return 2;
//	if (body.val.substr(offset, 2) == "%=") return 2;
//	if (body.val.substr(offset, 2) == "|=") return 2;
//	if (body.val.substr(offset, 2) == "<=>") return 3;
//	return 0;
//}
//
//bool isKeyword(const String& str) {
//	const std::vector<std::string> list = { "return","Let","copy","move","ref","if","else","while","xor","and","or"};
//	for (const auto& s : list) {
//		if (str == s) return true;
//	}
//	return false;
//}
//
//bool isTypeName(const String& str) {
//	if (str.val[0] >= 'A' and str.val[0] <= 'Z') return true;
//	return false;
//}
//
//std::vector<_CoreModule::Lexer::Token> _CoreModule::Lexer::lexer(const String& body, CompilerInterface* context) {
//	enum class Mode
//	{
//		id,
//		number_literal,
//		string_literal,
//		character_literal,
//		op,
//	};
//	Token t;
//	std::vector<Token> res;
//	auto addToken = [&t, &res]() {
//		if (t.val != "") {
//			res.push_back(t);
//			t = Token{};
//		}
//	};
//	std::string seperators = "+-*/%|!=<>.,;";
//	std::string parentheses = "(){}[]";
//	Mode mode = Mode::id;
//	bool b_dot = false;
//	for (int i = 0; i < body.val.size(); i++) {
//		char c = body.val[i];
//		int a = isMultiSymbolOperator(body, i);
//		if (a > 0) {
//			addToken();
//			for (int j = i; j < a + i; j++) {
//				t.val += body.val[j];
//			}
//			t.type = Token::TokenType::op;
//			addToken();
//			i += a;
//		}
//		else if (mode == Mode::id and inString(c, parentheses)) {
//			addToken();
//			t.val += c;
//			t.type = Token::TokenType::parentheses;
//			addToken();
//		}
//		else if (mode == Mode::id and inString(c, seperators)) {
//			addToken();
//			t.val += c;
//			t.type = Token::TokenType::op;
//			addToken();
//		}
//		else if (mode == Mode::id and c == '\'') {
//			addToken();
//			mode = Mode::character_literal;
//			t.type = Token::TokenType::character_literal;
//		}
//		else if (mode == Mode::character_literal and c != '\'') {
//			t.val += c;
//		}
//		else if (mode == Mode::character_literal and c == '\'') {
//			mode = Mode::id;
//		}
//		else if (mode == Mode::id and c == '"') {
//			addToken();
//			mode = Mode::string_literal;
//			t.type = Token::TokenType::string_literal;
//		}
//		else if (mode == Mode::string_literal and c != '"') {
//			t.val += c;
//		}
//		else if (mode == Mode::string_literal and c == '"') {
//			mode = Mode::id;
//		}
//		else if (mode == Mode::id and isdigit(c) and t.val.val == "") {
//			mode = Mode::number_literal;
//			t.type = Token::TokenType::number_literal;
//			t.val += c;
//		}
//		else if (mode == Mode::number_literal and not (isdigit(c) or c == '_' or c == '.')) {
//			mode = Mode::id;
//			addToken();
//			b_dot = false;
//		}
//		else if (mode == Mode::number_literal and (isdigit(c) or c == '_' or c == '.')) {
//			if (c == '.' and b_dot == true) {
//				context->critical_error_msg(context->context,CriticalErrorType::SyntaxError,moveCursor(body, i), "double dot inside number");
//			}
//			if (c == '.') b_dot = true;
//			t.val += c;
//		}
//		else if (mode == Mode::id and not isspace(c)) {
//			t.type = Token::TokenType::id;
//			t.val += c;
//		}
//		else if (mode == Mode::id and isspace(c)) {
//			addToken();
//		}
//	}
//	auto tokenKeywords = [](std::vector<Token>& tokens) {
//		for (auto& i : tokens) {
//			if (isKeyword(i.val)) {
//				i.type = Token::TokenType::keyword;
//			}
//			else if (isTypeName(i.val)) {
//				i.type = Token::TokenType::TypeName;
//			}
//		}
//	};
//	tokenKeywords(res);
//	return res;
//}