#include "pch.h"
#include "Parser.h"
#include "Consts.h"
#include <cassert>

#ifndef nollvm
#include <omp_llvm.h>
#endif // !nollvm

const std::string OutOfBound = "End of procedure";

bool isKeyword(TokenizedStream* stream, std::string name) {
	return
		stream != nullptr and
		stream->body.size() > stream->body_offset and
		stream->body[stream->body_offset].type == (uint16_t)Lexer::TokenType::keyword and stream->body[stream->body_offset].val.val == name;
}

bool isType(TokenizedStream* stream, Lexer::TokenType type) {
	return
		stream != nullptr and
		stream->body.size() > stream->body_offset and
		stream->body[stream->body_offset].type == (uint16_t)type;
}

bool name_IsType(TokenizedStream* stream, Lexer::TokenType type) {
	return
		stream != nullptr and
		stream->name.size() > stream->name_offset and
		stream->name[stream->name_offset].type == (uint16_t)type;
}

bool isValue(TokenizedStream* stream, std::string str) {
	return
		stream != nullptr and
		stream->body.size() > stream->body_offset and
		stream->body[stream->body_offset].val.val == str;
}

bool name_IsValue(TokenizedStream* stream, std::string str) {
	return
		stream != nullptr and
		stream->name.size() > stream->name_offset and
		stream->name[stream->name_offset].val.val == str;
}

bool isExacly(TokenizedStream* stream, Lexer::TokenType type, std::string str) {
	return isType(stream, type) and isValue(stream, str);
}

bool name_isExacly(TokenizedStream* stream, Lexer::TokenType type, std::string str) {
	return name_IsType(stream, type) and name_IsValue(stream, str);
}

bool isPrefixOperator(TokenizedStream* stream, Operator_t* op = nullptr) {
	if (stream == nullptr) return false;
	if (stream->body.size() < stream->body_offset) return false;
	for (const auto& i : prefixOperators) {
		if (isExacly(stream, Lexer::TokenType::op, i)) {
			if (op != nullptr) {
				for (const auto& o : operators) {
					if (o.representation == i) {
						*op = o;
					}
				}
			}
			return true;
		}
	}
	return false;
}

bool isSufixOperator(TokenizedStream* stream, Operator_t* op = nullptr) {
	if (stream == nullptr) return false;
	if (stream->body.size() < stream->body_offset) return false;
	for (const auto& i : sufixOperators) {
		if (isExacly(stream, Lexer::TokenType::op, i)) {
			if (op != nullptr) {
				for (const auto& o : operators) {
					if (o.representation == i) {
						*op = o;
					}
				}
			}
			return true;
		}
	}
	return false;
}

bool isInfixOperator(TokenizedStream* stream, Operator_t* op = nullptr) {
	if (stream == nullptr) return false;
	if (stream->body.size() < stream->body_offset) return false;
	for (const auto& i : infixOperators) {
		if (isExacly(stream, Lexer::TokenType::op, i)) {
			if (op != nullptr) {
				for (const auto& o : operators) {
					if (o.representation == i) {
						*op = o;
					}
				}
			}
			return true;
		}
	}
	return false;
}

void expectKeyword(TokenizedStream* stream, CompilerInterface* context, std::string keyword) {
	if (not isKeyword(stream, keyword)) {
		std::string msg = std::string() + "Expected Keyword" + keyword + " \nGet : " + ((stream->body_offset >= stream->body.size()) ? (OutOfBound) : (stream->body[stream->body_offset].val.val));
		context->critical_error_msg(context->context,
			CriticalErrorType::SyntaxError,
			stream->body_offset >= stream->body.size() ? stream->body[stream->body.size() - 1].val.pos : stream->body[stream->body_offset].val.pos,
			msg.c_str()
		);
	}
	stream->body_offset++;
}

Token expectTypeName(TokenizedStream* stream, CompilerInterface* context) {
	if (not isType(stream, Lexer::TokenType::TypeName)) {
		std::string msg = std::string() + "Expected Type Name \nGet : " + ((stream->body_offset >= stream->body.size()) ? (OutOfBound) : (stream->body[stream->body_offset].val.val));
		context->critical_error_msg(context->context,
			CriticalErrorType::SyntaxError,
			stream->body_offset >= stream->body.size() ? stream->body[stream->body.size() - 1].val.pos : stream->body[stream->body_offset].val.pos,
			msg.c_str()
		);
	}
	return stream->body[stream->body_offset++];
}

Token name_expectTypeName(TokenizedStream* stream, CompilerInterface* context) {
	if (not name_IsType(stream, Lexer::TokenType::TypeName)) {
		std::string msg = std::string() + "Expected Type Name \nGet : " + ((stream->name_offset >= stream->name.size()) ? (OutOfBound) : (stream->name[stream->name_offset].val.val));
		context->critical_error_msg(context->context,
			CriticalErrorType::SyntaxError,
			stream->name_offset >= stream->name.size() ? stream->name[stream->name.size() - 1].val.pos : stream->name[stream->name_offset].val.pos,
			msg.c_str()
		);
	}
	return stream->name[stream->name_offset++];
}

Token expectId(TokenizedStream* stream, CompilerInterface* context) {
	if (not isType(stream, Lexer::TokenType::id)) {
		std::string msg = std::string() + "Expected id \nGet : " + ((stream->body_offset >= stream->body.size()) ? (OutOfBound) : (stream->body[stream->body_offset].val.val));
		context->critical_error_msg(context->context,
			CriticalErrorType::SyntaxError,
			stream->body_offset >= stream->body.size() ? stream->body[stream->body.size() - 1].val.pos : stream->body[stream->body_offset].val.pos,
			msg.c_str()
		);
	}
	return stream->body[stream->body_offset++];
}
Token expectNumberLiteral(TokenizedStream* stream, CompilerInterface* context) {
	if (not isType(stream, Lexer::TokenType::Integer_literal) and not isType(stream, Lexer::TokenType::Float_literal)) {
		std::string msg = std::string() + "Expected Number literal \nGet : " + ((stream->body_offset >= stream->body.size()) ? (OutOfBound) : (stream->body[stream->body_offset].val.val));
		context->critical_error_msg(context->context,
			CriticalErrorType::SyntaxError,
			stream->body_offset >= stream->body.size() ? stream->body[stream->body.size() - 1].val.pos : stream->body[stream->body_offset].val.pos,
			msg.c_str()
		);
	}
	return stream->body[stream->body_offset++];
}

Token expectBoolLiteral(TokenizedStream* stream, CompilerInterface* context) {
	if (not isType(stream, Lexer::TokenType::Bool_literal)) {
		std::string msg = std::string() + "Expected Bool literal \nGet : " + ((stream->body_offset >= stream->body.size()) ? (OutOfBound) : (stream->body[stream->body_offset].val.val));
		context->critical_error_msg(context->context,
			CriticalErrorType::SyntaxError,
			stream->body_offset >= stream->body.size() ? stream->body[stream->body.size() - 1].val.pos : stream->body[stream->body_offset].val.pos,
			msg.c_str()
		);
	}
	return stream->body[stream->body_offset++];
}

Token name_expectIntegerLiteral(TokenizedStream* stream, CompilerInterface* context) {
	if (not name_IsType(stream, Lexer::TokenType::Integer_literal)) {
		std::string msg = std::string() + "Expected Integer literal \nGet : " + ((stream->name_offset >= stream->name.size()) ? (OutOfBound) : (stream->name[stream->name_offset].val.val));
		context->critical_error_msg(context->context,
			CriticalErrorType::SyntaxError,
			stream->name_offset >= stream->name.size() ? stream->name[stream->name.size() - 1].val.pos : stream->name[stream->name_offset].val.pos,
			msg.c_str()
		);
	}
	return stream->name[stream->name_offset++];
}

Token expectIntegerLiteral(TokenizedStream* stream, CompilerInterface* context) {
	if (not isType(stream, Lexer::TokenType::Integer_literal)) {
		std::string msg = std::string() + "Expected Integer literal \nGet : " + ((stream->body_offset >= stream->body.size()) ? (OutOfBound) : (stream->body[stream->body_offset].val.val));
		context->critical_error_msg(context->context,
			CriticalErrorType::SyntaxError,
			stream->body_offset >= stream->body.size() ? stream->body[stream->body.size() - 1].val.pos : stream->body[stream->body_offset].val.pos,
			msg.c_str()
		);
	}
	return stream->body[stream->body_offset++];
}

Token expectFloatLiteral(TokenizedStream* stream, CompilerInterface* context) {
	if (not isType(stream, Lexer::TokenType::Float_literal)) {
		std::string msg = std::string() + "Expected Float literal \nGet : " + ((stream->body_offset >= stream->body.size()) ? (OutOfBound) : (stream->body[stream->body_offset].val.val));
		context->critical_error_msg(context->context,
			CriticalErrorType::SyntaxError,
			stream->body_offset >= stream->body.size() ? stream->body[stream->body.size() - 1].val.pos : stream->body[stream->body_offset].val.pos,
			msg.c_str()
		);
	}
	return stream->body[stream->body_offset++];
}

Token expectStringLiteral(TokenizedStream* stream, CompilerInterface* context) {
	if (not isType(stream, Lexer::TokenType::string_literal)) {
		std::string msg = std::string() + "Expected string literal \nGet : " + ((stream->body_offset >= stream->body.size()) ? (OutOfBound) : (stream->body[stream->body_offset].val.val));
		context->critical_error_msg(context->context,
			CriticalErrorType::SyntaxError,
			stream->body_offset >= stream->body.size() ? stream->body[stream->body.size() - 1].val.pos : stream->body[stream->body_offset].val.pos,
			msg.c_str()
		);
	}
	return stream->body[stream->body_offset++];
}

Token expectCharacterLiteral(TokenizedStream* stream, CompilerInterface* context) {
	if (not isType(stream, Lexer::TokenType::character_literal)) {
		std::string msg = std::string() + "Expected character literal \nGet : " + ((stream->body_offset >= stream->body.size()) ? (OutOfBound) : (stream->body[stream->body_offset].val.val));
		context->critical_error_msg(context->context,
			CriticalErrorType::SyntaxError,
			stream->body_offset >= stream->body.size() ? stream->body[stream->body.size() - 1].val.pos : stream->body[stream->body_offset].val.pos,
			msg.c_str()
		);
	}
	return stream->body[stream->body_offset++];
}

void expectParanthis(TokenizedStream* stream, CompilerInterface* context, std::string param) {
	if (not isExacly(stream, Lexer::TokenType::parentheses, param)) {
		std::string msg = std::string() + "Expected Paranthis : " + param + "\nGet : " + ((stream->body_offset >= stream->body.size()) ? (OutOfBound) : (stream->body[stream->body_offset].val.val));
		context->critical_error_msg(context->context,
			CriticalErrorType::SyntaxError,
			stream->body_offset >= stream->body.size() ? stream->body[stream->body.size() - 1].val.pos : stream->body[stream->body_offset].val.pos,
			msg.c_str()
		);
	}
	stream->body_offset++;
}
void name_expectParanthis(TokenizedStream* stream, CompilerInterface* context, std::string param) {
	if (not name_isExacly(stream, Lexer::TokenType::parentheses, param)) {
		std::string msg = std::string() + "Expected Paranthis : " + param + "\nGet : " + ((stream->name_offset >= stream->name.size()) ? (OutOfBound) : (stream->name[stream->name_offset].val.val));
		context->critical_error_msg(context->context,
			CriticalErrorType::SyntaxError,
			stream->name_offset >= stream->name.size() ? stream->name[stream->name.size() - 1].val.pos : stream->name[stream->name_offset].val.pos,
			msg.c_str()
		);
	}
	stream->name_offset++;
}

void expectSemicolon(TokenizedStream* stream, CompilerInterface* context) {
	if (not isExacly(stream, Lexer::TokenType::op, ";")) {
		std::string msg = std::string() + "Expected Semicolon\nGet : " + ((stream->body_offset >= stream->body.size()) ? (OutOfBound) : (stream->body[stream->body_offset].val.val));
		context->critical_error_msg(context->context,
			CriticalErrorType::SyntaxError,
			stream->body_offset >= stream->body.size() ? stream->body[stream->body.size() - 1].val.pos : stream->body[stream->body_offset].val.pos,
			msg.c_str()
		);
	}
	stream->body_offset++;
}

void expectExacly(TokenizedStream* stream, CompilerInterface* context, Lexer::TokenType type, std::string val) {
	if (not isExacly(stream, type, val)) {
		std::string msg = std::string() + "Expected '" + val + "'\nGet : " + ((stream->body_offset >= stream->body.size()) ? (OutOfBound) : (stream->body[stream->body_offset].val.val));
		context->critical_error_msg(context->context,
			CriticalErrorType::SyntaxError,
			stream->body_offset >= stream->body.size() ? stream->body[stream->body.size() - 1].val.pos : stream->body[stream->body_offset].val.pos,
			msg.c_str()
		);
	}
	stream->body_offset++;
}

void expectExpressionErrorMSG(TokenizedStream* stream, CompilerInterface* context) {
	std::string msg = std::string() + "Expected Expression\nGet : " + ((stream->body_offset >= stream->body.size()) ? (OutOfBound) : (stream->body[stream->body_offset].val.val));
	context->critical_error_msg(context->context,
		CriticalErrorType::SyntaxError,
		stream->body_offset >= stream->body.size() ? stream->body[stream->body.size() - 1].val.pos : stream->body[stream->body_offset].val.pos,
		msg.c_str()
	);
}

std::unique_ptr<IExpression> parseExpression(TokenizedStream* stream, CompilerInterface* context);
std::unique_ptr<IStatement> expectStatement(TokenizedStream* stream, CompilerInterface* context);

std::unique_ptr<IExpression> expectExpression(TokenizedStream* stream, CompilerInterface* context) {
	auto exp = parseExpression(stream, context);
	if (not exp) {
		expectExpressionErrorMSG(stream, context);
	}
	else {
		return std::move(exp);
	}
	return {};
}

std::unique_ptr<IfStatement> parseIfStatement(TokenizedStream* stream, CompilerInterface* context) {
	expectKeyword(stream, context, "if");
	std::unique_ptr<IfStatement> res = std::make_unique<IfStatement>();
	res->condition = expectExpression(stream, context);
	res->ifTrue = expectStatement(stream, context);
	res->ifFalse = nullptr;
	if (isKeyword(stream, "else")) {
		expectKeyword(stream, context, "else");
		res->ifFalse = expectStatement(stream, context);
	}
	return res;
}

std::unique_ptr<WhileStatement> parseWhileStatement(TokenizedStream* stream, CompilerInterface* context) {
	std::unique_ptr<WhileStatement> res = std::make_unique<WhileStatement>();
	expectKeyword(stream, context, "while");
	res->condition = expectExpression(stream, context);
	res->body = expectStatement(stream, context);
	return res;
}

std::unique_ptr<ReturnStatement> parseReturnStatement(TokenizedStream* stream, CompilerInterface* context) {
	expectKeyword(stream, context, "return");
	std::unique_ptr<ReturnStatement> res = std::make_unique<ReturnStatement>();
	res->exp = expectExpression(stream, context);
	expectSemicolon(stream, context);
	return res;
}

std::unique_ptr<ExpressionStatement> parseExpresionStatement(TokenizedStream* stream, CompilerInterface* context) {
	uint32_t save = stream->body_offset;
	auto a = parseExpression(stream, context);
	if (a) {
		expectSemicolon(stream, context);
		auto res = std::make_unique<ExpressionStatement>();
		res->exp = std::move(a);
		return res;
	}
	else {
		stream->body_offset = save;
		if (isExacly(stream, Lexer::TokenType::op, ";")) {
			expectSemicolon(stream, context);
		}
	}
	return {};
}

IType* parseType(TokenizedStream* stream, CompilerInterface* context) {
	//()
	//Type
	//Type[]
	//(Int,Ptr)->Int
	//(ref Int)->Int
	IType* res = nullptr;
	if (isType(stream, Lexer::TokenType::TypeName)) {
		auto typeName = expectTypeName(stream, context).val;
		IType* t= nullptr;
		if (typeName.val == "String") {
			t=new ArrayType(new TypeNameExpression("Char"));
		}
		else {
			t = new TypeNameExpression(typeName, -1);
		}
		if (isExacly(stream, Lexer::TokenType::parentheses, "[")) {
			stream->body_offset++;

			if (isType(stream, Lexer::TokenType::Integer_literal)) {
				auto size = expectIntegerLiteral(stream, context);
				res = new ArrayType(t, std::atoi(size.val.val.c_str()));
			}
			else {
				res = new ArrayType(t);
			}
			expectExacly(stream, context, Lexer::TokenType::parentheses, "]");
		}
		else {
			return t;
		}
	}
	else if (isExacly(stream, Lexer::TokenType::op, "ref")) {
		stream->body_offset++;
		res = new PointerType(parseType(stream, context));
	}
	else if (isExacly(stream, Lexer::TokenType::parentheses, "(")) {
		while (not isExacly(stream, Lexer::TokenType::parentheses, ")")) {
			assert(false);
		}
		if (isExacly(stream, Lexer::TokenType::op, "->")) {
			assert(false);
		}
	}
	return res;
}

std::unique_ptr<VaribleDeclaration> parseVaribleDeclaration(TokenizedStream* stream, CompilerInterface* context) {
	auto res = std::make_unique<VaribleDeclaration>();
	res->type = parseType(stream,context);
	res->varibleName = expectId(stream, context).val;

	if (isType(stream, Lexer::TokenType::colon)) {
		stream->body_offset++;
		//TODO
		while (not isExacly(stream, Lexer::TokenType::op, "=")) {
			stream->body_offset++;
		}
	}
	if (isExacly(stream, Lexer::TokenType::op, "=")) {
		stream->body_offset++;
		res->init = expectExpression(stream, context);
	}
	else {
		res->init = nullptr;
	}
	expectSemicolon(stream, context);
	return res;
}

bool contain(std::vector<std::string> vec, std::string val) {
	for (const auto& i : vec) {
		if (i == val) return true;
	}
	return false;
}

std::vector<std::unique_ptr<IExpression>> parseProcedureArguments(TokenizedStream* stream, CompilerInterface* context) {
	expectExacly(stream, context, Lexer::TokenType::parentheses, "(");
	std::vector<std::unique_ptr<IExpression>> res;
	while (true) {
		uint32_t save = stream->body_offset;
		auto t = parseExpression(stream, context);
		if (not t)
		{
			stream->body_offset = save;
			break;
		}
		res.emplace_back(std::move(t));
		if (isExacly(stream, Lexer::TokenType::parentheses, ")")) {
			break;
		}
		else if (isExacly(stream, Lexer::TokenType::op, ",")) {
			stream->body_offset++;
		}
		else
			assert(false);
	};
	expectExacly(stream, context, Lexer::TokenType::parentheses, ")");
	return res;
}

IType* name_parseType(TokenizedStream* stream, CompilerInterface* context) {
	//()
	//Type
	//Type[]
	//(Int,Ptr)->Int
	//(ref Int)->Int
	if (name_IsType(stream, Lexer::TokenType::TypeName)) {
		auto typeName = name_expectTypeName(stream, context).val;
		IType* t = nullptr;
		if (typeName.val == "String") {
			t = new ArrayType(new TypeNameExpression("Char"));
		}
		else {
			t = new TypeNameExpression(typeName, -1);
		}
		if (name_isExacly(stream, Lexer::TokenType::parentheses, "[")) {
			stream->name_offset++;
			if (name_IsType(stream, Lexer::TokenType::Integer_literal)) {
				auto size = name_expectIntegerLiteral(stream, context);
				name_expectParanthis(stream, context, "]");
				return new ArrayType(t, std::atoi(size.val.val.c_str()));
			}
			else {
				name_expectParanthis(stream, context, "]");
				return new ArrayType(t);
			}

		}
		else {
			return t;
		}
	}
	else if (name_isExacly(stream, Lexer::TokenType::op, "ref")) {
		return new PointerType(parseType(stream, context));
	}
	else if (name_isExacly(stream, Lexer::TokenType::parentheses, "(")) {
		while (not name_isExacly(stream, Lexer::TokenType::parentheses, ")")) {

		}
		if (name_isExacly(stream, Lexer::TokenType::op, "->")) {

		}
	}
	return nullptr;
}


std::unique_ptr<IExpression> parseExpression(TokenizedStream* stream, CompilerInterface* context) {

	std::vector<std::pair<Operator_t, Position>> stack;
	std::vector<std::unique_ptr<IExpression>> output;

	auto combaineExpression = [&stack, &output, &stream]() {
		if (stack[stack.size() - 1].first.Infix and output.size() > 1) {
			std::unique_ptr<BinaryOperatorExpression> res = std::make_unique<BinaryOperatorExpression>();
			res->op = String{ stack[stack.size() - 1].first.representation, stack[stack.size() - 1].second };
			res->op_type = stack[stack.size() - 1].first;
			res->pos = stack[stack.size() - 1].second;
			res->right = std::move(output[output.size() - 1]);
			res->left = std::move(output[output.size() - 2]);

			if (res->left->type == nullptr) {
				res->type = nullptr;
			}
			else {
				res->type = res->left->type->copy();
			}

			stack.pop_back();
			output.pop_back();
			output.pop_back();

			output.emplace_back(std::move(res));
		}
		else if (stack[stack.size() - 1].first.Prefix and stack.size() > 0) {
			std::unique_ptr<PrefixOperatorExpression> res = std::make_unique<PrefixOperatorExpression>();
			res->op = String{ stack[stack.size() - 1].first.representation, stack[stack.size() - 1].second };
			res->right = std::move(output[output.size() - 1]);

			if (res->right->type == nullptr) {
				res->type = nullptr;
			}
			else {
				res->type = res->right->type->copy();
			}
			stack.pop_back();
			output.pop_back();
			output.emplace_back(std::move(res));
		}
		else if (stack[stack.size() - 1].first.Sufix and stack.size() > 0) {
			std::unique_ptr<PostfixOperatorExpression> res = std::make_unique<PostfixOperatorExpression>();
			res->op = String{ stack[stack.size() - 1].first.representation, stack[stack.size() - 1].second };
			res->left = std::move(output[output.size() - 1]);

			if (res->left->type == nullptr) {
				res->type = nullptr;
			}
			else {
				res->type = res->left->type->copy();
			}
			stack.pop_back();
			output.pop_back();
			output.emplace_back(std::move(res));
		}
		else {
			assert(false);
		}
	};

	while (not (
		isExacly(stream, Lexer::TokenType::op, ";") or
		isExacly(stream, Lexer::TokenType::op, ",") or
		isExacly(stream, Lexer::TokenType::parentheses, "{") or
		isExacly(stream, Lexer::TokenType::parentheses, ")") or
		isExacly(stream, Lexer::TokenType::parentheses, "]")
		)
		)
	{
		if (isType(stream, Lexer::TokenType::id)) {
			std::unique_ptr<VaribleExpression> a = std::make_unique<VaribleExpression>();
			auto t = expectId(stream, context);
			a->varibleName = t.val;
			a->pos = t.val.pos;
			output.emplace_back(std::move(a));
		}
		else if (isType(stream, Lexer::TokenType::TypeName)) {
			auto t = expectTypeName(stream, context);
			std::unique_ptr<TypeNameExpression> a = std::make_unique<TypeNameExpression>(t.val);
			a->pos = t.val.pos;
			a->typeName = t.val;
			a->type = new InternalType("Type");
			//a->varibleName = expectId(stream, context).val;
			output.emplace_back(std::move(a));
		}
		else if (isType(stream, Lexer::TokenType::character_literal)) {
			std::unique_ptr<LiteralExpression> a = std::make_unique<LiteralExpression>();
			auto t = expectCharacterLiteral(stream, context);
			a->value = t.val;
			a->type = new TypeNameExpression("Char");
			a->pos = t.val.pos;
			output.emplace_back(std::move(a));
		}
		else if (isType(stream, Lexer::TokenType::string_literal)) {
			std::unique_ptr<LiteralExpression> a = std::make_unique<LiteralExpression>();
			auto t = expectStringLiteral(stream, context);
			a->value = t.val;
			a->type = new ArrayType(new TypeNameExpression("Char"));;
			a->pos = t.val.pos;
			output.emplace_back(std::move(a));
		}
		else if (isType(stream, Lexer::TokenType::Integer_literal)) {
			std::unique_ptr<LiteralExpression> a = std::make_unique<LiteralExpression>();
			auto t = expectNumberLiteral(stream, context);
			a->value = t.val;
			a->type = new TypeNameExpression("Int");
			a->pos = t.val.pos;
			output.emplace_back(std::move(a));
		}
		else if (isType(stream, Lexer::TokenType::Float_literal)) {
			std::unique_ptr<LiteralExpression> a = std::make_unique<LiteralExpression>();
			auto t = expectNumberLiteral(stream, context);
			a->value = t.val;
			a->type = new TypeNameExpression("Float");
			a->pos = t.val.pos;
			output.emplace_back(std::move(a));
		}
		else if (isType(stream, Lexer::TokenType::Bool_literal)) {
			std::unique_ptr<LiteralExpression> a = std::make_unique<LiteralExpression>();
			auto t = expectBoolLiteral(stream, context);
			a->value = t.val;
			a->type = new TypeNameExpression("Bool");
			a->pos = t.val.pos;
			output.emplace_back(std::move(a));
		}
		//constructors
		else if (isExacly(stream, Lexer::TokenType::parentheses, "(")) {
			//expectParanthis(stream, context, "(");
			if (stream->body[(size_t)stream->body_offset - 1].type == (uint16_t)Lexer::TokenType::op or output.size() == 0) {
				//force order of expression
				expectParanthis(stream, context, "(");
				auto a = expectExpression(stream, context);
				output.emplace_back(std::move(a));
				expectParanthis(stream, context, ")");
			}
			else {
				//function call
				while (stack.size() > 0 and
					(
						(2 > stack[stack.size() - 1].first.precedence) or
						(2 >= stack[stack.size() - 1].first.precedence and stack[stack.size() - 1].first.leftAssociativity == false)
						)
					)
				{
					combaineExpression();
				}
				std::unique_ptr<ProcedureCallExpression> t = std::make_unique<ProcedureCallExpression>();

				t->function = std::move(output[output.size() - 1]);
				output.pop_back();
				t->args = parseProcedureArguments(stream, context);
				output.emplace_back(std::move(t));
			}
		}
		//else if (isExacly(stream, Lexer::TokenType::parentheses, ")")) {
		//	//expectParanthis(stream, context, "(");
		//	stream->body_offset++;
		//}
		else if (isExacly(stream, Lexer::TokenType::parentheses, "[")) {

			// array literal
			if (stream->body[(size_t)stream->body_offset - 1].type == (uint16_t)Lexer::TokenType::op or output.size() == 0) {
				//force order of expression
				expectParanthis(stream, context, "[");
				auto t = new ArrayLiteralExpression();
				while(true){
					auto a = expectExpression(stream, context);
					if (not a) {
						expectExpressionErrorMSG(stream, context);
					}
					t->values.emplace_back(std::move(a));
					if(isExacly(stream, Lexer::TokenType::parentheses, "]")){
						output.emplace_back(std::move(t));
						break;
					}
					else {
						expectExacly(stream,context,Lexer::TokenType::colon,",");
					}
				}
				expectParanthis(stream, context, "]");
			}
			else if (output.size() > 0 and output[output.size() - 1]->type != nullptr and output[output.size() - 1]->type->toString() == "@Type") {
				//Int[a];

				expectParanthis(stream, context, "[");
				auto t = new ArrayAlloc();
				t->innerArrayType = ((IType*)(output[output.size()-1].get()))->copy();
				output.pop_back();
				auto a = expectExpression(stream, context);
				if (not a) {
					expectExpressionErrorMSG(stream, context);
				}
				t->size = std::move(a);
				expectParanthis(stream, context, "]");			
				output.emplace_back(std::move(t));
			}
			else {
				//array acess
				stream->body_offset++;
				auto e = expectExpression(stream, context);
				auto t = new ArrayAcceseExpression();
				t->array = std::move(output[output.size() - 1]);
				t->args = std::move(e);
				output.pop_back();
				output.emplace_back(std::move(t));
				expectParanthis(stream, context, "]");
			}
		}
		else if (isType(stream, Lexer::TokenType::op)) {
			Operator_t op;
			if (isPrefixOperator(stream, &op) and (stream->body[(size_t)stream->body_offset - 1].type == (uint16_t)Lexer::TokenType::op or output.size() == 0)) {
				op.Infix = false;
				op.Sufix = false;
				stack.emplace_back(std::make_pair(op, stream->body[stream->body_offset].val.pos));
			}
			else if (isInfixOperator(stream, &op)) {
				op.Prefix = false;
				op.Sufix = false;
				while (stack.size() > 0 and
					(
						(op.precedence > stack[stack.size() - 1].first.precedence) or
						(op.precedence >= stack[stack.size() - 1].first.precedence and stack[stack.size() - 1].first.leftAssociativity == false)
						)
					) {
					combaineExpression();
				}
				stack.emplace_back(std::make_pair(op, stream->body[stream->body_offset].val.pos));
			}
			else if (isSufixOperator(stream, &op)) {
				op.Infix = false;
				op.Prefix = false;
				stack.emplace_back(std::make_pair(op, stream->body[stream->body_offset].val.pos));
			}
			stream->body_offset++;
		}
		else
			if (output.size() == 0)
				return nullptr;
			else {
				for (auto& item : output) {
					context->log_msg_pos(context->context, 1, item->pos, item->toString().c_str());
				}
				expectExpressionErrorMSG(stream, context);
			}
	}
	while (stack.size() > 0) {
		combaineExpression();
	}

	if (output.size() == 0)
		return nullptr;
	else
		if (output.size() == 1) {
			return std::move(output[0]);
		}
		else {
			for (auto& item : output) {
				context->log_msg_pos(context->context, 1, item->pos, item->toString().c_str());
			}
			expectExpressionErrorMSG(stream, context);
			return nullptr;
		}
}

std::unique_ptr<IStatement> parseStatement(TokenizedStream* stream, CompilerInterface* context);

void ArrayAlloc::visit(IVisitor* visitor) {
	visitor->visit(this);
}
std::string ArrayAlloc::toString() {
	return "ArrayAlloc[" + this->type->toString() + "("+this->size->toString() + ")]";
}

std::unique_ptr<BlockStatement> parseBlockStatement(TokenizedStream* stream, CompilerInterface* context) {
	expectParanthis(stream, context, "{");
	auto res = std::make_unique<BlockStatement>();
	std::unique_ptr<IStatement> t = nullptr;
	while (true) {
		uint32_t save = stream->body_offset;
		t = parseStatement(stream, context);
		if (not t)
		{
			stream->body_offset = save;
			break;
		}
		res->body.emplace_back(std::move(t));
	};
	expectParanthis(stream, context, "}");
	return res;
}

std::unique_ptr<IStatement> parseStatement(TokenizedStream* stream, CompilerInterface* context)
{
	if (isKeyword(stream, "if")) {
		return parseIfStatement(stream, context);
	}
	else if (isKeyword(stream, "while")) {
		return parseWhileStatement(stream, context);
	}
	else if (isKeyword(stream, "return")) {
		return parseReturnStatement(stream, context);
	}
	else if (isExacly(stream, Lexer::TokenType::parentheses, "{")) {
		return parseBlockStatement(stream, context);
	}
	else if (isKeyword(stream, "Let") or isType(stream, Lexer::TokenType::TypeName)) {
		return parseVaribleDeclaration(stream, context);
	}
	else {
		return parseExpresionStatement(stream, context);
	}
}

std::unique_ptr<IStatement> expectStatement(TokenizedStream* stream, CompilerInterface* context) {
	auto exp = parseStatement(stream, context);
	if (not exp) {
		std::string msg = std::string() + "Expected Statement\nGet : " + stream->body[stream->body_offset].val.val;
		context->critical_error_msg(context->context,
			CriticalErrorType::SyntaxError,
			stream->body[stream->body_offset].val.pos,
			msg.c_str()
		);
	}
	else {
		return std::move(exp);
	}
	return {};
}

Token name_ExpectId(TokenizedStream* stream, CompilerInterface* context) {
	if (not name_IsType(stream, Lexer::TokenType::id)) {
		std::string msg = std::string() + "Expected id \nGet : " + ((stream->name_offset >= stream->name.size()) ? (OutOfBound) : (stream->name[stream->name_offset].val.val));
		context->critical_error_msg(context->context,
			CriticalErrorType::SyntaxError,
			stream->name_offset >= stream->name.size() ? stream->name[stream->name.size() - 1].val.pos : stream->name[stream->name_offset].val.pos,
			msg.c_str()
		);
	}
	return stream->name[stream->name_offset++];
}

void parseHeaderOffFunction(Procedure& proc, TokenizedStream* stream, CompilerInterface* context) {
	stream->name;
	proc.name = name_ExpectId(stream, context).val;
	name_expectParanthis(stream, context, "(");
	while (stream->name_offset < stream->name.size()) {
		if (name_IsType(stream, Lexer::TokenType::TypeName)) {
			auto ty = name_parseType(stream, context);
			auto na = name_ExpectId(stream, context).val;
			proc.args.push_back(std::make_pair(ty, na));
			if (name_isExacly(stream, Lexer::TokenType::op, ",")) {
				stream->name_offset++;
			}
			else {
				break;
			}
		}
		else if (name_isExacly(stream, Lexer::TokenType::parentheses, ")")) {
			break;
		}
		else if (name_isExacly(stream, Lexer::TokenType::op, "ref")) {
			stream -> name_offset++;
			auto ty = name_parseType(stream, context);
			auto na = name_ExpectId(stream, context).val;
			proc.args.push_back(std::make_pair(new PointerType( ty ), na));
			if (name_isExacly(stream, Lexer::TokenType::op, ",")) {
				stream->name_offset++;
			}
			else {
				break;
			}
		}
		else {
			assert(false);
			break;
		}
	}
	name_expectParanthis(stream, context, ")");
	if (name_isExacly(stream, Lexer::TokenType::op, "->")) {
		stream->name_offset++;
		proc.returnType = name_parseType(stream,context);
	}
	else {
		proc.returnType = nullptr;
	}
}

Procedure parseProcedure(TokenizedStream* stream, CompilerInterface* context) {
	Procedure res;
	std::unique_ptr<IStatement> t = nullptr;

	parseHeaderOffFunction(res, stream, context);

	while (true and stream->body.size() > stream->body_offset) {
		t = parseStatement(stream, context);
		if (not t) break;
		res.code.emplace_back(std::move(t));
	};

	if (stream->body_offset < stream->body.size()) {
		std::string msg = "Unexpected end of file \nGet : " + stream->body[stream->body_offset].val.val;
		context->error_msg(context->context,
			ErrorType::SyntaxError,
			stream->body[stream->body_offset].val.pos,
			msg.c_str()
		);
	}

	//PrinterVisitor* printer = new PrinterVisitor();
	//printer->icompiler = context;
	//for (const auto& i : res.code) {
	//	i->visit(printer);
	//}
	//delete printer;

	return res;
}

bool operator!=(std::unique_ptr<IType>& left, std::unique_ptr<IType>& right)
{
	return false;
}

bool operator==(std::unique_ptr<IType>& left, std::unique_ptr<IType>& right)
{
	return false;
}

StructType* parseStruct(TokenizedStream* stream, CompilerInterface* context)
{
	StructType* res = new StructType();
	res->name = stream->name[0].val;
	res->pos = stream->name[0].val.pos;
	while (true and stream->body.size() > stream->body_offset) {
		IType* ty = parseType(stream, context);
		String id = expectId(stream, context).val;
		expectSemicolon(stream,context);
		res->inner_type.push_back(std::make_pair(ty,id));
	}
	return res;
}

std::string VaribleExpression::toString()
{
	return std::string("Varible[") + this->varibleName.val + "]";
}

void VaribleExpression::visit(IVisitor* visitor)
{
	visitor->visit(this);
}

TypeNameExpression::TypeNameExpression(std::string typeName,SymbolId id)
{
	this->type = new InternalType("Type");
	this->typeName = String(typeName);
	this->typeId = id;
}

TypeNameExpression::TypeNameExpression(String typeName,SymbolId id)
{
	this->type = new InternalType("Type");
	this->typeName = typeName;
	this->typeId = id;
}

IType* TypeNameExpression::copy()
{
	TypeNameExpression* res = new TypeNameExpression(this->typeName.val);
	return res;
}

std::string TypeNameExpression::toString()
{
	return this->typeName.val;
}

void TypeNameExpression::visit(IVisitor* visitor)
{
	visitor->visit(this);
}

std::string LiteralExpression::toString()
{
	return std::string("Literal_") + this->type->toString() + "[" + this->value.val + "]";
}

std::string ArrayLiteralExpression::toString()
{
	std::string res;
	for (const auto& i : this->values) {
		if (res != "") {
			res+=",";
		}
		res+=i->toString();
	}
	return std::string("Literal_Array") + this->type->toString() + "[" + res + "]";
}

void LiteralExpression::visit(IVisitor* visitor)
{
	visitor->visit(this);
}
	
std::string PrefixOperatorExpression::toString()
{
	return std::string("Prefix[") + this->op.val + "]" + "\n" + this->right->toString();
}

void PrefixOperatorExpression::visit(IVisitor* visitor)
{
	visitor->visit(this);
}

std::string BinaryOperatorExpression::toString()
{
	return std::string("Infix[") + this->op.val + "]" + "\n" + this->left->toString() + "\n" + this->right->toString();
}

void BinaryOperatorExpression::visit(IVisitor* visitor)
{
	visitor->visit(this);
}

std::string PostfixOperatorExpression::toString()
{
	return std::string("Postfix[") + this->op.val + "]" + "\n" + this->left->toString();
}

void PostfixOperatorExpression::visit(IVisitor* visitor)
{
	visitor->visit(this);
}

std::string ProcedureCallExpression::toString()
{
	return std::string("Procedure()");
}

void ProcedureCallExpression::visit(IVisitor* visitor)
{
	visitor->visit(this);
}

std::string ArrayAcceseExpression::toString()
{
	return std::string("Array[]");
}

void ArrayAcceseExpression::visit(IVisitor* visitor)
{
	visitor->visit(this);
}

std::string PrinterVisitor::indent()
{
	std::string res;
	for (int i = 0; i < this->indent_no; i++) {
		res += " ";
	}
	return res;
}

void PrinterVisitor::visit(VaribleExpression* a) {
	this->icompiler->log_msg(this->icompiler->context, 256, std::string(indent() + a->varibleName.val).c_str());
}
void PrinterVisitor::visit(LiteralExpression* a) {
	this->icompiler->log_msg(this->icompiler->context, 256, std::string(indent() + "[" + a->type->toString() + "] " + a->value.val).c_str());
}
void PrinterVisitor::visit(PrefixOperatorExpression* a) {
	this->icompiler->log_msg(this->icompiler->context, 256, std::string(indent() + a->op.val).c_str());
	this->indent_no++;
	a->right->visit(this);
	this->indent_no--;
}
void PrinterVisitor::visit(BinaryOperatorExpression* a) {
	this->icompiler->log_msg(this->icompiler->context, 256, std::string(indent() + a->op.val).c_str());
	this->indent_no++;
	a->left->visit(this);
	a->right->visit(this);
	this->indent_no--;
}
void PrinterVisitor::visit(PostfixOperatorExpression* a) {
	this->icompiler->log_msg(this->icompiler->context, 256, std::string(indent() + a->op.val).c_str());
	this->indent_no++;
	a->left->visit(this);
	this->indent_no--;
}
void PrinterVisitor::visit(ProcedureCallExpression* a) {
	this->icompiler->log_msg(this->icompiler->context, 256, std::string(indent() + "ProcedureCall").c_str());
	this->indent_no++;
	a->function->visit(this);
	for (const auto& i : a->args) {
		i->visit(this);
	}
	this->indent_no--;
}
void PrinterVisitor::visit(ArrayAcceseExpression* a) {
	this->icompiler->log_msg(this->icompiler->context, 256, std::string(indent() + "Args").c_str());
	this->indent_no++;
	a->array->visit(this);
	a->args->visit(this);
	//for (const auto& i : a->args) {
	//	i->visit(this);
	//}
	this->indent_no--;
}

void PrinterVisitor::visit(IfStatement* a)
{
	this->icompiler->log_msg(this->icompiler->context, 256, std::string(indent() + "If").c_str());
	this->indent_no++;
	a->condition->visit(this);
	a->ifTrue->visit(this);
	if (a->ifFalse)
		a->ifFalse->visit(this);
	this->indent_no--;
}

void PrinterVisitor::visit(WhileStatement* a)
{
	this->icompiler->log_msg(this->icompiler->context, 256, std::string(indent() + "While").c_str());
	this->indent_no++;
	a->condition->visit(this);
	a->body->visit(this);
	this->indent_no--;
}

void PrinterVisitor::visit(ReturnStatement* a)
{
	this->icompiler->log_msg(this->icompiler->context, 256, std::string(indent() + "Return").c_str());
	this->indent_no++;
	a->exp->visit(this);
	this->indent_no--;
}

void PrinterVisitor::visit(ExpressionStatement* a)
{
	this->icompiler->log_msg(this->icompiler->context, 256, std::string(indent() + "Expression").c_str());
	this->indent_no++;
	a->exp->visit(this);
	this->indent_no--;
}

void PrinterVisitor::visit(BlockStatement* a)
{
	this->icompiler->log_msg(this->icompiler->context, 256, std::string(indent() + "Block").c_str());
	this->indent_no++;
	for (const auto& i : a->body) {
		i->visit(this);
	}
	this->indent_no--;
}

void PrinterVisitor::visit(ArrayLiteralExpression* a)
{
	this->icompiler->log_msg(this->icompiler->context, 256, std::string(indent() + "Array Literal[").c_str());
	this->indent_no++;
	for (const auto& i : a->values) {
		i->visit(this);
	}
	this->indent_no--;
	this->icompiler->log_msg(this->icompiler->context, 256, std::string(indent() + "]" ).c_str());
}

void PrinterVisitor::visit(VaribleDeclaration* a)
{
	this->icompiler->log_msg(this->icompiler->context, 256, std::string(indent() + "Varible Declaration[" + a->varibleName.val + "]{" + a->type->toString() + "}").c_str());
	this->indent_no++;
	if (a->init)
		a->init->visit(this);
	this->indent_no--;
}
void PrinterVisitor::visit(TypeNameExpression* a)
{
	this->icompiler->log_msg(this->icompiler->context, 256, std::string(indent() + "TypeNameExpression[" + a->typeName.val + "]{" + a->type->toString() + "}").c_str());
}

llvm::Type* PrinterVisitor::getLLVMType(IType* visitor) {
	return nullptr;
}

llvm::Type* PrinterVisitor::getLLVMType(TypeNameExpression* a)
{
	return nullptr;
}

llvm::Type* PrinterVisitor::getLLVMType(ArrayType* visitor)
{
	return nullptr;
}

llvm::Type* PrinterVisitor::getLLVMType(FunctionType* visitor)
{
	return nullptr;
}

llvm::Type* PrinterVisitor::getLLVMType(PointerType* visitor)
{
	return nullptr;
}

llvm::Type* PrinterVisitor::getLLVMType(StructType* visitor)
{
	return nullptr;
}

void VaribleDeclaration::visit(IVisitor* visitor)
{
	visitor->visit(this);
}

void BlockStatement::visit(IVisitor* visitor)
{
	visitor->visit(this);
}

void ExpressionStatement::visit(IVisitor* visitor)
{
	visitor->visit(this);
}

void ReturnStatement::visit(IVisitor* visitor)
{
	visitor->visit(this);
}

void WhileStatement::visit(IVisitor* visitor)
{
	visitor->visit(this);
}

void IfStatement::visit(IVisitor* visitor)
{
	visitor->visit(this);
}

void ArrayLiteralExpression::visit(IVisitor* visitor)
{
	visitor->visit(this);
}

#ifndef nollvm

llvm::Type* TypeNameExpression::getLLVMType(IVisitor* visitor)
{
	return visitor->getLLVMType(this);
}
llvm::Type* ArrayType::getLLVMType(IVisitor* visitor)
{
	return visitor->getLLVMType(this);
}
llvm::Type* PointerType::getLLVMType(IVisitor* visitor)
{
	return visitor->getLLVMType(this);
}
llvm::Type* StructType::getLLVMType(IVisitor* visitor)
{
	return visitor->getLLVMType(this);
}
llvm::Type* InternalType::getLLVMType(IVisitor* visitor)
{
	assert(false && "internal type");
	return nullptr;
}
llvm::Type* FunctionType::getLLVMType(IVisitor* visitor)
{
	assert(false && "function type not implemented");
	return nullptr;
}
#else

llvm::Type* TypeNameExpression::getLLVMType(IVisitor* visitor)
{
	assert(false);
	return nullptr;
}
llvm::Type* ArrayType::getLLVMType(IVisitor* visitor)
{
	assert(false);
	return nullptr;
}
llvm::Type* PointerType::getLLVMType(IVisitor* visitor)
{
	assert(fals);
	return nullptr;
}
llvm::Type* StructType::getLLVMType(IVisitor* visitor)
{
	assert(false);
	return nullptr;
}
llvm::Type* InternalType::getLLVMType(IVisitor* visitor)
{
	assert(false);
	return nullptr;
}
llvm::Type* FunctionType::getLLVMType(IVisitor* visitor)
{
	assert(false);
	return nullptr;
}
#endif

ArrayType::ArrayType(IType* ini)
{
	this->inner_type = ini;
	this->size = 0;
	this->dynamic = true;
}

ArrayType::ArrayType(IType* ini, uint32_t size)
{
	this->inner_type = ini;
	this->size = size;
	this->dynamic = false;
}

IType* ArrayType::copy()
{
	ArrayType* res = new ArrayType(this->inner_type->copy());
	res->dynamic = this->dynamic;
	if (this->dynamic) {
		res->size = this->size;
	}
	else {
		res->size = 0;
	}
	return res;
}

std::string ArrayType::toString()
{
	if (this->size != 0) {
		return this->inner_type->toString() + "[" + std::to_string(this->size) + "]";
	}
	return this->inner_type->toString() + "[]";
}

void ArrayType::visit(IVisitor* visitor)
{
	assert(false && "array visit");
}

PointerType::PointerType(IType* type)
{
	this->inner_type = type;
	this->size = -1;
}



IType* PointerType::copy()
{
	PointerType* res = new PointerType(this->inner_type->copy());
	return res;
}

std::string PointerType::toString()
{
	return "ref " + this->inner_type->toString();
}

void PointerType::visit(IVisitor* visitor)
{
	assert(false && " pointer not implemented visitor");
}



IType* StructType::copy()
{
	assert(false && "struct type copy");
	return nullptr;
}

std::string StructType::toString()
{
	std::string res = "Struct{ ";
	for (const auto& i : inner_type) {
		if (res != "")
			res += ", ";
		res += i.first->toString();
	}
	return "Struct{ " + res + "}";
}

void StructType::visit(IVisitor* visitor)
{
	assert(false);
}

InternalType::InternalType(std::string name)
{
	this->typeName = name;
	this->type = nullptr;
}

IType* InternalType::copy()
{
	return new InternalType(this->typeName);
}

std::string InternalType::toString()
{
	return std::string("@") + this->typeName;
}

void InternalType::visit(IVisitor* visitor)
{
	assert(false && "internal visit -> probably compilation error ");
}

FunctionType::FunctionType(String name)
{
	this->funcName = name;
}

IType* FunctionType::copy()
{
	return nullptr;
}

std::string FunctionType::toString()
{
	return std::string("Func(") + this->funcName.val + ")";
}

void FunctionType::visit(IVisitor* visitor)
{
	assert(false && "function pointer visit");
}

StoredASTSumType::StoredASTSumType(Procedure* p)
{
	this->type = Type::Procedure;
	this->ptr = p;
}

StoredASTSumType::StoredASTSumType(StructType* t)
{
	this->type = Type::Struct;
	this->ptr = t;
}

StoredASTSumType::StoredASTSumType(ArrayType* t)
{
	this->type = Type::Array;
	this->ptr = t;
}

void PrinterVisitor::visit(ArrayAlloc* tt)
{
	assert(false);
}