#pragma once

#include <string>
#include <unordered_map>

struct Position {
	std::string filename;
	int line = 1;
	int character = 0;
	void newLine() {
		line++;
		character = 0;
	}
};

struct String {
	std::string val;
	Position pos;
	size_t size() const {
		return val.size();
	}
	bool empty() const {
		return val.empty();
	}
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
Position moveCursor(const String&, int);
String operator+=(String& s, char c);

struct Token {
	uint32_t type;
	String val;
};

struct TokenizedStream {
	std::vector<Token> name;
	std::vector<Token> body;
};

struct Preambule {
	
	struct Body_t {
		std::vector<String> lines;
		String val() const ;
	};

	std::unordered_map<std::string, std::string> options;
	String preambule_name;
	String name;
	Body_t body;
	TokenizedStream* tokenizedStream = nullptr;
	void* ast = nullptr;
};

