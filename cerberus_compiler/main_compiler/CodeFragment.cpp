#include "CodeFragment.h"


CodeFragment::CodeFragment(CodeFragment&& other) noexcept
{
	this->options = other.options;
	this->preambule = other.preambule;
	this->name = other.name;
	this->body = other.body;
}
