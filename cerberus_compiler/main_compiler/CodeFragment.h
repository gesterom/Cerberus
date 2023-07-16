#pragma once

#include <string>
#include <unordered_map>

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
};

struct CodeFragment {
	std::unordered_map<std::string, std::string> options;
	String preambule;
	String name;
	String body;
};

std::ostream& operator<<(std::ostream& out, const Position& pos);
bool operator!=(const String& s, const std::string& cs);
bool operator==(const String& s, const std::string& cs);
String operator+=(String& s, char c);