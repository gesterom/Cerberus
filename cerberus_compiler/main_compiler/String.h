#pragma once

#include <string>
#include <unordered_map>

#include "ModuleInterface.h"

struct StringHash
{
	std::size_t operator()(String const& s) const noexcept;
};

std::ostream& operator<<(std::ostream& out, const Position& pos);
std::string toString(const Position& pos);
bool operator!=(const String& s, const std::string& cs);
bool operator!=(const String& a, const String& b);
bool operator==(const String& s, const std::string& cs);
bool operator==(const String& a, const String& b);
Position moveCursor(const String&, int);
String operator+=(String& s, char c);