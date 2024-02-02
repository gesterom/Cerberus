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
		Integer_literal,
		Float_literal,
		Bool_literal,
		colon,
	};
}
TokenizedStream* lexer_fun(const Preambule& body, CompilerInterface* context);