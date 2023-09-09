#pragma once

#include "../main_compiler/ModuleInterface.h"

namespace Lexer {

	enum class TokenType
	{
		unkowwn = 0,
		op,
		id,
		TypeName,
		parentheses,
		keyword,
		string_literal,
		character_literal,
		number_literal
	};
}
TokenizedStream* lexer_fun(const Preambule& body, CompilerInterface* context);