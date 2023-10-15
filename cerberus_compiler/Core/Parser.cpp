#include "pch.h"
#include "Parser.h"
#include "Consts.h"
#include <cassert>

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

bool isValue(TokenizedStream* stream, std::string str) {
	return
		stream != nullptr and
		stream->body.size() > stream->body_offset and
		stream->body[stream->body_offset].val.val == str;
}

bool isExacly(TokenizedStream* stream, Lexer::TokenType type, std::string str) {
	return isType(stream, type) and isValue(stream, str);
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
	if (not isType(stream, Lexer::TokenType::number_literal)) {
		std::string msg = std::string() + "Expected number literal \nGet : " + ((stream->body_offset >= stream->body.size()) ? (OutOfBound) : (stream->body[stream->body_offset].val.val));
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

std::unique_ptr<VaribleDeclaration> parseVaribleDeclaration(TokenizedStream* stream, CompilerInterface* context) {
	auto res = std::make_unique<VaribleDeclaration>();
	res->type = expectTypeName(stream, context).val;
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
		res->init = {};
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

std::unique_ptr<IExpression> parseExpression(TokenizedStream* stream, CompilerInterface* context) {

	std::vector<std::pair<Operator_t, Position>> stack;
	std::vector<std::unique_ptr<IExpression>> output;

	auto combaineExpression = [&stack, &output]() {
		if (contain(prefixOperators, stack[stack.size() - 1].first.representation)) {
			std::unique_ptr<PrefixOperatorExpression> res = std::make_unique<PrefixOperatorExpression>();
			res->op = String(stack[stack.size() - 1].first.representation, stack[stack.size() - 1].second);
			res->right = std::move(output[output.size() - 1]);
			stack.pop_back();
			output.pop_back();
			output.emplace_back(std::move(res));
		}
		else {
			std::unique_ptr<BinaryOperatorExpression> res = std::make_unique<BinaryOperatorExpression>();
			res->op = String(stack[stack.size() - 1].first.representation, stack[stack.size() - 1].second);
			res->op_type = stack[stack.size() - 1].first;
			res->pos = stack[stack.size() - 1].second;
			res->right = std::move(output[output.size() - 1]);
			res->left = std::move(output[output.size() - 2]);
			stack.pop_back();
			output.pop_back();
			output.pop_back();


			output.emplace_back(std::move(res));
		}
		};

	while (not (
		isExacly(stream, Lexer::TokenType::op, ";") or
		isExacly(stream, Lexer::TokenType::op, ",") or
		isExacly(stream, Lexer::TokenType::parentheses, "{") or
		isExacly(stream, Lexer::TokenType::parentheses, ")")
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
			std::unique_ptr<ConstructorExpression> a = std::make_unique<ConstructorExpression>();
			auto t = expectTypeName(stream, context);
			a->pos = t.val.pos;
			a->typeName = t.val;
			//a->varibleName = expectId(stream, context).val;
			output.emplace_back(std::move(a));
		}
		else if (isType(stream, Lexer::TokenType::character_literal)) {
			std::unique_ptr<LiteralExpression> a = std::make_unique<LiteralExpression>();
			auto t = expectCharacterLiteral(stream, context);
			a->value = t.val;
			a->type = "Char";
			a->pos = t.val.pos;
			output.emplace_back(std::move(a));
		}
		else if (isType(stream, Lexer::TokenType::string_literal)) {
			std::unique_ptr<LiteralExpression> a = std::make_unique<LiteralExpression>();
			auto t = expectStringLiteral(stream, context);
			a->value = t.val;
			a->type = "String";
			a->pos = t.val.pos;
			output.emplace_back(std::move(a));
		}
		else if (isType(stream, Lexer::TokenType::number_literal)) {
			std::unique_ptr<LiteralExpression> a = std::make_unique<LiteralExpression>();
			auto t = expectNumberLiteral(stream, context);
			a->value = t.val;
			a->type = "Number";
			a->pos = t.val.pos;
			output.emplace_back(std::move(a));
		}
		//function call
		//parenthisis
		//array access
		//constructors
		else if (isExacly(stream, Lexer::TokenType::parentheses, "(")) {
			//expectParanthis(stream, context, "(");
			if (stream->body[stream->body_offset - 1].type == (uint16_t)Lexer::TokenType::op) {
				//force order of expression
				expectParanthis(stream,context,"(");
				auto a = expectExpression(stream,context);
				output.emplace_back(std::move(a));
				expectParanthis(stream, context, ")");
			}
			else {
				//function call
			}
		}
		//else if (isExacly(stream, Lexer::TokenType::parentheses, ")")) {
		//	//expectParanthis(stream, context, "(");
		//	stream->body_offset++;
		//}
		else if (isExacly(stream, Lexer::TokenType::parentheses, "[")) {
			stream->body_offset++;
		}
		else if (isExacly(stream, Lexer::TokenType::parentheses, "]")) {
			stream->body_offset++;
		}
		else if (isType(stream, Lexer::TokenType::op)) {
			Operator_t op;
			if (isPrefixOperator(stream, &op)) {
				stack.emplace_back(std::make_pair(op, stream->body[stream->body_offset].val.pos));
			}
			else if (isInfixOperator(stream, &op)) {
				while (stack.size() > 0 and
					(
						(op.precedence < stack[stack.size() - 1].first.precedence) or
						(op.precedence <= stack[stack.size() - 1].first.precedence and stack[stack.size() - 1].first.leftAssociativity == false)
						)
					) {
					combaineExpression();
				}
				stack.emplace_back(std::make_pair(op, stream->body[stream->body_offset].val.pos));
			}
			else if (isSufixOperator(stream, &op)) {

			}
			stream->body_offset++;
		}
		else
			if (output.size() == 0)
				return nullptr;
			else
				expectExpressionErrorMSG(stream, context);
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
				context->log_msg(context->context, 1, item->pos, item->toString().c_str());
			}
			expectExpressionErrorMSG(stream, context);
			return nullptr;
		}
}

std::unique_ptr<IStatement> parseStatement(TokenizedStream* stream, CompilerInterface* context);

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

Procedure parseProcedure(TokenizedStream* stream, CompilerInterface* context) {
	Procedure res;
	std::unique_ptr<IStatement> t = nullptr;
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
	res.returnType = String("VOID");
	return res;
}

std::string VaribleExpression::toString()
{
	return std::string("Varible[") + this->varibleName.val + "]";
}

std::string ConstructorExpression::toString()
{
	return std::string("Constructor[") + this->typeName.val + "]";
}

std::string LiteralExpression::toString()
{
	return std::string("Literal_") + this->type + "[" + this->value.val + "]";
}

std::string PrefixOperatorExpression::toString()
{
	return std::string("Prefix[") + this->op.val + "]" + "\n" + this->right->toString();
}

std::string BinaryOperatorExpression::toString()
{
	return std::string("Infix[") + this->op.val + "]" + "\n" + this->left->toString() + "\n" + this->right->toString();
}

std::string PostfixOperatorExpression::toString()
{
	return std::string("Postfix[") + this->op.val + "]" + "\n" + this->left->toString();
}

std::string ProcedureCallExpression::toString()
{
	return std::string("Procedure[]");
}
