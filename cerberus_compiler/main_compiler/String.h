#pragma once

#include <string>

struct Position {
	std::string filename;
	int line = 1;
	int character = 1;
	void newLine() {
		line++;
		character = 1;
	}
};

struct String {
	std::string val;
	Position pos;
	String() noexcept = default;
	String(const String& other) noexcept = default;
};

struct StringHash
{
	std::size_t operator()(String const& s) const noexcept;
};

std::ostream& operator<<(std::ostream& out, const Position& pos);
bool operator!=(const String& s, const std::string& cs);
bool operator!=(const String& a, const String& b);
bool operator==(const String& s, const std::string& cs);
bool operator==(const String& a, const String& b);
Position moveCursor(const String&,int);
String operator+=(String& s, char c);


