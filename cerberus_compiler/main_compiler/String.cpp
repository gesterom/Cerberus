#include "String.h"

#include <iostream>

std::ostream& operator<<(std::ostream& out, const Position& pos) {
	out << pos.filename << " : " << pos.line << "_" << pos.character;
	return out;
}

bool operator!=(const String& s, const std::string& cs) {
	return s.val != cs;
}
bool operator==(const String& s, const std::string& cs) {
	return s.val == cs;
}

bool operator!=(const String& a, const String& b) {
	return a.val != b.val;
}
bool operator==(const String& a, const String& b) {
	return a.val == b.val;
}
Position moveCursor(const String& str, int n)
{
	Position res = str.pos;
	for (int i = 0; i < n; i++) {
		if(str.val[i] == '\n') res.newLine();
		else res.character++;
	}
	return res;
}
String operator+=(String& s, char c) {
	s.val += c;
	return s;
}

std::size_t StringHash::operator()(String const& s) const noexcept
{
	return std::hash<std::string>{}(s.val);
}