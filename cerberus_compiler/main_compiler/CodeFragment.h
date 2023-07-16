#pragma once

#include <string>
#include <unordered_map>

#include "String.h"

struct CodeFragment {
	std::unordered_map<std::string, std::string> options;
	String preambule;
	String name;
	String body;
	CodeFragment() = default;
	CodeFragment(const CodeFragment& other) = default;
	CodeFragment& operator=(const CodeFragment& other) = default;
	CodeFragment(CodeFragment&& other) noexcept;
};

