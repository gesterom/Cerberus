#include "CodeFragment.h"
#include <iostream>

std::ostream& operator<<(std::ostream& out, const Position& pos) {
	out << pos.filename << " : " << pos.line << " _ " << pos.character;
	return out;
}

bool operator!=(const String& s, const std::string& cs) {
	return s.val != cs;
}
bool operator==(const String& s, const std::string& cs) {
	return s.val == cs;
}
String operator+=(String& s, char c) {
	s.val += c;
	return s;
}