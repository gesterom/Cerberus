#include "pch.h"
#include "Parser.h"

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

void expectKeyword(TokenizedStream* stream, CompilerInterface* context, std::string keyword) {
	if (not isKeyword(stream, keyword)) {
		std::string msg = std::string() + "Expected Keyword" + keyword + " \nGet : " + stream->body[stream->body_offset].val.val;
		context->critical_error_msg(context->context,
			CriticalErrorType::SyntaxError,
			stream->body[stream->body_offset].val.pos,
			msg.c_str()
		);
	}
	stream->body_offset++;
}

Token expectTypeName(TokenizedStream* stream, CompilerInterface* context) {
	if (not isType(stream, Lexer::TokenType::TypeName)) {
		std::string msg = std::string() + "Expected Type Name \nGet : " + stream->body[stream->body_offset].val.val;
		context->critical_error_msg(context->context,
			CriticalErrorType::SyntaxError,
			stream->body[stream->body_offset].val.pos,
			msg.c_str()
		);
	}
	return stream->body[stream->body_offset++];
}

Token expectId(TokenizedStream* stream, CompilerInterface* context) {
	if (not isType(stream, Lexer::TokenType::id)) {
		std::string msg = std::string() + "Expected id \nGet : " + stream->body[stream->body_offset].val.val;
		context->critical_error_msg(context->context,
			CriticalErrorType::SyntaxError,
			stream->body[stream->body_offset].val.pos,
			msg.c_str()
		);
	}
	return stream->body[stream->body_offset++];
}

void expectParanthis(TokenizedStream* stream, CompilerInterface* context, std::string param) {
	if (not isExacly(stream, Lexer::TokenType::parentheses, param)) {
		std::string msg = std::string() + "Expected Paranthis : " + param + "\nGet : " + stream->body[stream->body_offset].val.val;
		context->critical_error_msg(context->context,
			CriticalErrorType::SyntaxError,
			stream->body[stream->body_offset].val.pos,
			msg.c_str()
		);
	}
	stream->body_offset++;
}

void expectSemicolon(TokenizedStream* stream, CompilerInterface* context) {
	if (not isExacly(stream, Lexer::TokenType::op, ";")) {
		std::string msg = std::string() + "Expected Semicolon\nGet : " + stream->body[stream->body_offset].val.val;
		context->critical_error_msg(context->context,
			CriticalErrorType::SyntaxError,
			stream->body[stream->body_offset].val.pos,
			msg.c_str()
		);
	}
	stream->body_offset++;
}


std::optional<std::unique_ptr<IExpression>> parseExpression(TokenizedStream* stream, CompilerInterface* context);
std::unique_ptr<IStatement> expectStatement(TokenizedStream* stream, CompilerInterface* context);

std::unique_ptr<IExpression> expectExpression(TokenizedStream* stream, CompilerInterface* context) {
	auto exp = parseExpression(stream, context);
	if (not exp.has_value()) {
		std::string msg = std::string() + "Expected Expression\nGet : " + stream->body[stream->body_offset].val.val;
		context->critical_error_msg(context->context,
			CriticalErrorType::SyntaxError,
			stream->body[stream->body_offset].val.pos,
			msg.c_str()
		);
	}
	else {
		return std::move(exp.value());
	}
	return {};
}

std::optional<std::unique_ptr<IfStatement>> parseIfStatement(TokenizedStream* stream, CompilerInterface* context) {
	return {};
}

std::optional<std::unique_ptr<WhileStatement>> parseWhileStatement(TokenizedStream* stream, CompilerInterface* context) {
	expectKeyword(stream, context, "while");
	WhileStatement res;
	res.condition = expectExpression(stream, context);
	res.body = expectStatement(stream, context);
	return {};
}

std::optional<std::unique_ptr<ReturnStatement>> parseReturnStatement(TokenizedStream* stream, CompilerInterface* context) {
	return {};
}

std::optional<std::unique_ptr<ExpressionStatement>> parseExpresionStatement(TokenizedStream* stream, CompilerInterface* context) {
	uint32_t save = stream->body_offset;
	auto a = parseExpression(stream, context);
	if (a.has_value()) {
		expectSemicolon(stream, context);
		auto res = std::make_unique<ExpressionStatement>();
		res->exp = std::move(a.value());
		return res;
	}
	else {
		stream->body_offset = save;
	}
	return {};
}

std::optional<std::unique_ptr<VaribleDeclaration>> parseVaribleDeclaration(TokenizedStream* stream, CompilerInterface* context) {
	auto res = std::make_unique<VaribleDeclaration>();
	res->type = expectTypeName(stream, context).val;
	res->varibleName = expectId(stream, context).val;
	auto exp = parseExpression(stream, context);
	if (exp.has_value()) {
		res->init = std::move(exp.value());
	}
	else {
		res->init = {};
	}
	expectSemicolon(stream, context);
	return res;
}

std::optional<std::unique_ptr<IExpression>> parseExpression(TokenizedStream* stream, CompilerInterface* context) {

	

	return {};
}

std::optional<std::unique_ptr<IStatement>> parseStatement(TokenizedStream* stream, CompilerInterface* context);

std::optional<std::unique_ptr<BlockStatement>> parseBlockStatement(TokenizedStream* stream, CompilerInterface* context) {
	expectParanthis(stream, context, "{");
	auto res =std::make_unique<BlockStatement>();
	std::optional<std::unique_ptr<IStatement>> t = parseStatement(stream, context);
	while (t.has_value()) {
		res->body.emplace_back(std::move(t.value()));
		t = parseStatement(stream, context);
	}
	expectParanthis(stream, context, "}");
	return res;
}

std::optional<std::unique_ptr<IStatement>> parseStatement(TokenizedStream* stream, CompilerInterface* context)
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
	else if (isKeyword(stream, "Let") or isType(stream, Lexer::TokenType::TypeName)) {
		return parseVaribleDeclaration(stream, context);
	}
	else if (isExacly(stream, Lexer::TokenType::parentheses, "{")) {
		return parseBlockStatement(stream, context);
	}
	else {
		return parseExpresionStatement(stream, context);
	}

	return {};
}

std::unique_ptr<IStatement> expectStatement(TokenizedStream* stream, CompilerInterface* context) {
	auto exp = parseStatement(stream, context);
	if (not exp.has_value()) {
		std::string msg = std::string() + "Expected Statement\nGet : " + stream->body[stream->body_offset].val.val;
		context->critical_error_msg(context->context,
			CriticalErrorType::SyntaxError,
			stream->body[stream->body_offset].val.pos,
			msg.c_str()
		);
	}
	else {
		return std::move(exp.value());
	}
	return {};
}

Procedure parseProcedure(TokenizedStream* stream, CompilerInterface* context) {
	Procedure res;
	std::optional<std::unique_ptr<IStatement>> t = parseStatement(stream, context);
	while (t.has_value()) {
		res.code.emplace_back(std::move(t.value()));
		t = parseStatement(stream, context);
	}
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
