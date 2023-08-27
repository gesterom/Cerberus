#pragma once

#include <vector>

#include "String.h"
#include "CompilerContext.h"

#include <iostream>
/*
namespace _CoreModule {

	class Lexer 
	{
	public:
		struct Token {
			enum class TokenType
			{
				unkowwn,
				op,
				id,
				TypeName,
				parentheses,
				keyword,
				string_literal,
				character_literal,
				number_literal
			};
			TokenType type = TokenType::unkowwn;
			String val;
		};
		static std::vector<Token> lexer(const String& body, CompilerContext& context);
	};

}
std::ostream& operator<<(std::ostream& o*/ut, _CoreModule::Lexer::Token::TokenType t);