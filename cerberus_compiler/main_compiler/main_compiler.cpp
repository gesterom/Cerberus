// main_compiler.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

struct Position {
	std::string filename;
	int line = 1;
	int character = 1;
	void newLine() {
		line++;
		character = 1;
	}
};

std::ostream& operator<<(std::ostream& out, const Position& pos) {
	out << pos.filename << " : " << pos.line << " _ " << pos.character;
	return out;
}

struct String {
	std::string val;
	Position pos;
};

bool operator!=(const String& s, const std::string& cs) {
	return s.val != cs;
}
bool operator==(const String& s, const std::string& cs) {
	return s.val == cs;
}
String operator+=(String& s, char c) {
	s.val+=c;
	return s;
}


struct CodeFragment {
	String preambule;
	String name;
	String body;
};

void print(const CodeFragment& code) {
	std::cout<<"Position : "<<code.preambule.pos<<std::endl;
	std::cout<<"preambule : " << code.preambule.val<<std::endl;
	std::cout << "name : " << code.name.val << std::endl;
	std::cout << "body : " << code.body.val << std::endl;
}

bool isAllowedSpecialCaracter(char ch) {
	std::string a = "!@#$%^&*_";
	for (auto i : a) {
		if (ch == i) return true;
	}return false;
}

void error_Unexpected_NotASCII_character(Position position) {
	std::cout << "ERROR : unexpected ascii character at \n"<<position << std::endl;
	exit(-1);
}

void error_Unexpected_Character(Position position){
	std::cout << "ERROR : unexpected character at \n" << position << std::endl;
	exit(-2);
}

void error_UnMatched_parentice(Position position) {
	std::cout << "ERROR : unmached parenthes at \n" << position << std::endl;
	exit(-3);
}

bool IsASCII(char c) {
	return c>=0 and c <=127;
}

std::vector<CodeFragment> parse(std::istream& in,std::string filename="main.dog") {
	std::vector<CodeFragment> res;
	CodeFragment partial;

	enum class Mode
	{
		preambule,
		name,
		body
	};
	Mode mode = Mode::preambule;
	Position pos;
	pos.filename = filename;
	int parantesCounter = 0;
	while (in.good()) {
		char c = in.get();
		switch (mode)
		{
		case Mode::preambule:
			if (std::isspace(c) and partial.preambule!="")
			{
				mode = Mode::name;
			}
			else if (not std::isspace(c) and partial.preambule == "") {
				partial.preambule.pos = pos;
				partial.preambule += c;
			}
			else if (std::isalpha(c) or std::isdigit(c) or isAllowedSpecialCaracter(c))
			{
				partial.preambule += c;
			}
			else if (std::isspace(c)) {
				;
			}
			else if (c == '{' and partial.preambule!="") {
				mode = Mode::body;
			}
			else
			{
				error_Unexpected_Character(pos);
			}
			break;
		case Mode::name:
			if(partial.name.val=="")
				partial.name.pos = pos;
			if(c == '{') 
			{
				mode = Mode::body;
				parantesCounter=1;
			}else partial.name+=c;
			break;
		case Mode::body:
			if (partial.body.val == "")
				partial.body.pos = pos;
			if(c == '{') parantesCounter++;
			if(c=='}') parantesCounter--;
			if(parantesCounter == 0) 
			{
				res.push_back(partial);
				partial = CodeFragment();
				mode = Mode::preambule;
			}
			else partial.body+=c;
			break;
		default:
			break;
		}
		pos.character++;
		if (c == '\n')
			pos.newLine();
	}
	if (mode != Mode::preambule) {
		error_UnMatched_parentice(pos);
	}
	return res; 
}

int main(int arg, char** args)
{
	std::ifstream file(args[1]);
	for (const auto& i : parse(file,args[1])) {
		print(i);
	}
}