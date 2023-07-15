// main_compiler.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
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
	s.val += c;
	return s;
}


struct CodeFragment {
	std::unordered_map<std::string, std::string> options;
	String preambule;
	String name;
	String body;
};

void print(const CodeFragment& code) {
	std::cout << "Position : " << code.preambule.pos << std::endl;
	for (const auto& op : code.options) {
		std::cout << op.first << "=" << op.second << " ";
	}
	std::cout << "\npreambule : " << code.preambule.val << std::endl;
	std::cout << "name : " << code.name.val << std::endl;
	std::cout << "body : " << code.body.val << std::endl;
}

bool isAllowedSpecialCaracter(char ch) {
	std::string a = "!@$%^&*_";
	for (auto i : a) {
		if (ch == i) return true;
	}return false;
}

void error_Unexpected_NotASCII_character(Position position) {
	std::cout << "ERROR : unexpected ascii character at \n" << position << std::endl;
	exit(-1);
}

void error_Unexpected_Character(Position position) {
	std::cout << "ERROR : unexpected character at \n" << position << std::endl;
	exit(-2);
}

void error_UnMatched_parentice(Position position) {
	std::cout << "ERROR : unmached parenthes at \n" << position << std::endl;
	exit(-3);
}

bool IsASCII(char c) {
	return c >= 0 and c <= 127;
}

std::vector<CodeFragment> parse(std::istream& in, std::string filename) {
	std::vector<CodeFragment> res;
	CodeFragment partial;
	std::string optionName;
	std::string optionValue;
	std::unordered_map<std::string, std::string> fileOptions;
	bool fileOption = false;

	enum class Mode
	{
		preambule,
		name,
		body,
		OptionName,
		OptionValue,
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
			if (std::isspace(c) and partial.preambule != "")
			{
				mode = Mode::name;
			}
			else if (c == '{') {
				parantesCounter++;
				mode = Mode::body;
			}
			else if (c == '#' and partial.preambule == "") {
				optionName = "";
				optionValue = "";
				fileOption = false;
				mode = Mode::OptionName;
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
			else
			{
				error_Unexpected_Character(pos);
			}
			break;
		case Mode::name:
			if (partial.name.val == "")
				partial.name.pos = pos;
			if (c == '{')
			{
				mode = Mode::body;
				parantesCounter = 1;
			}
			else partial.name += c;
			break;
		case Mode::body:
			if (partial.body.val == "")
				partial.body.pos = pos;
			if (c == '{') parantesCounter++;
			if (c == '}') parantesCounter--;
			if (parantesCounter == 0)
			{
				for (const auto& item : fileOptions) {
					partial.options.emplace(item);
				}
				res.push_back(partial);
				partial = CodeFragment();
				mode = Mode::preambule;
			}
			else partial.body += c;
			break;
		case Mode::OptionName:
			if (c == '\n') {
				if (fileOption) {
					fileOptions.emplace(std::make_pair(optionName, optionValue));
				}
				else {
					partial.options.emplace(std::make_pair(optionName, optionValue));
				}
				mode = Mode::preambule;
			}
			else if (c == '#' and optionName == "") fileOption = true;
			else if (isspace(c) and optionName == "");
			else if (isspace(c) and optionName != "") {
				mode = Mode::OptionValue;
			}
			else {
				optionName += c;
			}
			break;
		case Mode::OptionValue:
			if (c == '\n') {
				if (fileOption) {
					fileOptions.emplace(std::make_pair(optionName, optionValue));
				}
				else {
					partial.options.emplace(std::make_pair(optionName, optionValue));
				}
				mode = Mode::preambule;
			}
			else if (not isspace(c) or optionValue != "") {
				optionValue += c;
			}
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
class ICompilerModule;
class IModuleRepository;


class ICompilerModule {
public:
	virtual void RegisterPreambule(IModuleRepository&) = 0;
	virtual void Init() = 0;
	virtual void RegisterSymbols(const CodeFragment&) = 0;
	virtual void GenerateCode(const CodeFragment&) = 0;
	virtual void Finalize() = 0;
};

class IModuleRepository {
public:
	virtual void registerToPreambule(std::string preambule, ICompilerModule*) = 0;
	virtual void RegisterSymbols(const CodeFragment& code) = 0;
	virtual void GenerateCode(const CodeFragment& code) = 0;
	virtual void runFinalizeOnModules() = 0;
};

class ModuleRepo :public IModuleRepository {
	std::unordered_map<std::string, std::vector<ICompilerModule*>> vec;
	std::unordered_map<ICompilerModule*, bool> map;
public:
	virtual void registerToPreambule(std::string preambule, ICompilerModule* _module) {
		vec[preambule].push_back(_module);
		map[_module] = false;
	}
	virtual void RegisterSymbols(const CodeFragment& code) {
		for (auto& m : vec[code.preambule.val]) {
			if (map[m] == false)
			{
				m->Init();
				map[m] = true;
			}
			m->RegisterSymbols(code);
		}
	}
	virtual void GenerateCode(const CodeFragment& code) {
		for (auto& m : vec[code.preambule.val]) {
			if (map[m] == true)
			{
				m->GenerateCode(code);
			}
		}
	}
	virtual void runFinalizeOnModules() {
		for (auto m : map) {
			if (m.second) m.first->Finalize();
		}
	}
};

class Printer : public ICompilerModule {
	virtual void RegisterPreambule(IModuleRepository& repo) {
		repo.registerToPreambule("__c_procedure_", this);
		repo.registerToPreambule("__c_function_", this);
		repo.registerToPreambule("__c_class_", this);
		repo.registerToPreambule("__c_interface_", this);
		repo.registerToPreambule("__c_type_", this);
	}
	virtual void Init() { std::cout << "Init\n"; }
	virtual void RegisterSymbols(const CodeFragment& code) {

	}
	virtual void GenerateCode(const CodeFragment& code) {
		std::cout << "=============================\n";
		print(code);
	}
	virtual void Finalize() { std::cout << "Finish\n"; }
};

class BrainFuck : public ICompilerModule {
	char* mem = nullptr;
public:
	virtual void RegisterPreambule(IModuleRepository& repo) {
		repo.registerToPreambule("BrainFuck", this);
	}
	virtual void Init() {
		mem = new char[1024];
		memset(mem, 0, 1024);
	}
	virtual void RegisterSymbols(const CodeFragment& code) {

	}
	virtual void GenerateCode(const CodeFragment& code) {
		//zliczaj nawiasy
		auto it = code.options.find("exec");
		if (it == code.options.end() or it->second != "True") return;
		int memIndex = 0;
		int pc = 0;
		int paramCount = 0;
		char c;
		while (pc >= 0 and pc < code.body.val.size()) {
			c = code.body.val[pc];
			switch (c)
			{
			case '>': ++memIndex; break;
			case '<': --memIndex; break;
			case '+': ++mem[memIndex]; break;
			case '-': --mem[memIndex]; break;
			case '.': putchar(mem[memIndex]); break;
			case ',': std::cin >> mem[memIndex]; break;
			case '[':
				paramCount = 1;
				pc++;
				while (pc < code.body.val.size() and mem[memIndex] == 0) {
					if (code.body.val[pc] == ']') {
						paramCount--;
						if (paramCount == 0) break;
					}
					else if (code.body.val[pc] == '[') paramCount++;
					pc++;
				}
				break;
			case ']':
				paramCount = 1;
				pc--;
				while (pc > 0 and paramCount > 0 ) {
					if (code.body.val[pc] == ']') paramCount++;
					else if (code.body.val[pc] == '[') paramCount--;
					pc--;
				}
				break;
			default:
				break;
			}
			pc++;
		}
	}
	virtual void Finalize() {
		delete[] mem;
	}
};

std::vector<ICompilerModule*> modules = {
	new Printer(),
	new BrainFuck()
};

int main(int arg, char** args)
{
	std::ifstream file(args[1]);
	std::cout << args[1] << std::endl;
	ModuleRepo repo;
	for (const auto& _module : modules) {
		_module->RegisterPreambule(repo);
	}
	auto codes = parse(file, args[1]);
	for (const auto& i : codes) {
		repo.RegisterSymbols(i);
	}
	for (const auto& i : codes) {
		repo.GenerateCode(i);
	}
	repo.runFinalizeOnModules();
}