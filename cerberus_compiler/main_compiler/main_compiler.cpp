// main_compiler.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <Windows.h>


#include "Preambule.h"
#include "ModuleInterface.h"
#include "CompilerContext.h"

//#include "BrainFuck.h"
//#include "CoreModule.h"

//bool isAllowedSpecialCaracter(char ch) {
//	std::string a = "!@$%^&*_";
//	for (auto i : a) {
//		if (ch == i) return true;
//	}return false;
//}
//
//bool IsASCII(char c) {
//	return c >= 0 and c <= 127;
//}

//std::vector<CodeFragment> parse(std::istream& in, std::string filename, CompilerContext& context) {
//	std::vector<CodeFragment> res;
//	CodeFragment partial;
//	std::string optionName;
//	std::string optionValue;
//	std::unordered_map<std::string, std::string> fileOptions;
//	bool fileOption = false;
//
//	enum class Mode
//	{
//		preambule,
//		name,
//		body,
//		OptionName,
//		OptionValue,
//	};
//	Mode mode = Mode::preambule;
//	Position pos;
//	pos.filename = filename;
//	int parantesCounter = 0;
//	while (in.good()) {
//		char c = in.get();
//		switch (mode)
//		{
//		case Mode::preambule:
//			if (std::isspace(c) and partial.preambule != "")
//			{
//				mode = Mode::name;
//			}
//			else if (c == '{') {
//				parantesCounter++;
//				mode = Mode::body;
//			}
//			else if (c == '#' and partial.preambule == "") {
//				optionName = "";
//				optionValue = "";
//				fileOption = false;
//				mode = Mode::OptionName;
//			}
//			else if (not std::isspace(c) and partial.preambule == "") {
//				partial.preambule.pos = pos;
//				partial.preambule += c;
//			}
//			else if (std::isalpha(c) or std::isdigit(c) or isAllowedSpecialCaracter(c))
//			{
//				partial.preambule += c;
//			}
//			else if (std::isspace(c)) {
//				;
//			}
//			else
//			{
//				context.critical_Unexpected_Character(pos, c);
//			}
//			break;
//		case Mode::name:
//			if (partial.name.val == "")
//				partial.name.pos = pos;
//			if (c == '{')
//			{
//				mode = Mode::body;
//				parantesCounter = 1;
//			}
//			else if (not isspace(c) or partial.name != "") partial.name += c;
//			break;
//		case Mode::body:
//			if (partial.body.val == "")
//				partial.body.pos = pos;
//			if (c == '{') parantesCounter++;
//			if (c == '}') parantesCounter--;
//			if (parantesCounter == 0)
//			{
//				for (const auto& item : fileOptions) {
//					partial.options.emplace(item);
//				}
//				res.push_back(partial);
//				partial = CodeFragment{};
//				mode = Mode::preambule;
//			}
//			else partial.body += c;
//			break;
//		case Mode::OptionName:
//			if (c == '\n') {
//				if (fileOption) {
//					fileOptions.emplace(std::make_pair(optionName, optionValue));
//				}
//				else {
//					partial.options.emplace(std::make_pair(optionName, optionValue));
//				}
//				mode = Mode::preambule;
//			}
//			else if (c == '#' and optionName == "") fileOption = true;
//			else if (isspace(c) and optionName == "");
//			else if (isspace(c) and optionName != "") {
//				mode = Mode::OptionValue;
//			}
//			else {
//				optionName += c;
//			}
//			break;
//		case Mode::OptionValue:
//			if (c == '\n') {
//				if (fileOption) {
//					fileOptions.emplace(std::make_pair(optionName, optionValue));
//				}
//				else {
//					partial.options.emplace(std::make_pair(optionName, optionValue));
//				}
//				mode = Mode::preambule;
//			}
//			else if (not isspace(c) or optionValue != "") {
//				optionValue += c;
//			}
//			break;
//		default:
//			break;
//		}
//		pos.character++;
//		if (c == '\n')
//			pos.newLine();
//	}
//	if (mode != Mode::preambule) {
//		context.critical_UnMatched_parentice(pos);
//	}
//	return res;
//}

//
//class ModuleRepository : public IModuleRepository {
//	std::unordered_map<std::string, std::vector<ICompilerModule*>> vec;
//	std::unordered_map<ICompilerModule*, bool> map;
//	CompilerContext symbols;
//public:
//	virtual void registerToPreambule(std::string preambule, ICompilerModule* _module) {
//		vec[preambule].push_back(_module);
//		map[_module] = false;
//	}
//	virtual void registerLexer(std::string preambule, ILexer* lexer) {
//
//	}
//	virtual void RegisterSymbols(const CodeFragment& code) {
//		for (auto& m : vec[code.preambule.val]) {
//			if (map[m] == false)
//			{
//				m->Init(symbols);
//				map[m] = true;
//			}
//			m->RegisterSymbols(code, symbols);
//		}
//		for (auto& m : vec[code.preambule.val]) {
//			if (map[m] == true)
//			{
//				m->DefinitionOfSymbols(code, symbols);
//			}
//		}
//	}
//	virtual void GenerateCode(const CodeFragment& code) {
//		for (auto& m : vec[code.preambule.val]) {
//			if (map[m] == true)
//			{
//				m->GenerateCode(code, symbols);
//			}
//		}
//	}
//	virtual void runFinalizeOnModules() {
//		for (auto m : map) {
//			if (m.second) m.first->Finalize(symbols);
//		}
//	}
//};
//
//void print(const CodeFragment& code) {
//	std::cout << "Position : " << code.preambule.pos << std::endl;
//	for (const auto& op : code.options) {
//		std::cout << op.first << "=" << op.second << " ";
//	}
//	std::cout << "\npreambule : " << code.preambule.val << std::endl;
//	std::cout << "name : " << code.name.val << std::endl;
//	std::cout << "body : " << code.body.val().val << std::endl;
//}
//
//class Printer : public ICompilerModule {
//	virtual void RegisterPreambule(IModuleRepository& repo) {
//		//repo.registerToPreambule("__c_procedure_", this);
//		//repo.registerToPreambule("__c_function_", this);
//		//repo.registerToPreambule("__c_class_", this);
//		//repo.registerToPreambule("__c_interface_", this);
//		//repo.registerToPreambule("__c_type_", this);
//		repo.registerToPreambule("comment", this);
//	}
//	virtual const char* const ModuleName() {
//		return "Printer";
//	}
//	virtual const char* const Version() {
//		return "1.0.0";
//	}
//	virtual void Init(CompilerContext& symbols) { std::cout << "Init\n"; }
//	virtual void RegisterSymbols(const CodeFragment& code, CompilerContext& symbols) {
//
//	}
//	virtual void DefinitionOfSymbols(const CodeFragment&, CompilerContext& symbols) {}
//	virtual void GenerateCode(const CodeFragment& code, CompilerContext& symbols) {
//		std::cout << "=============================\n";
//		print(code);
//	}
//	virtual void Finalize(CompilerContext& symbols) { std::cout << "Finish\n"; }
//};

bool allowedInPreambule(std::string name) {
	auto allowAsFirstCharacter = [](char c) {
		return c >= 'a' and c <= 'z';
		};
	if (name.size() == 0) {
		return false;
	}
	else {
		if (allowAsFirstCharacter(name[0]) == false) return false;
		for (const auto& i : name) {
			if (not (
				(i >= 'a' and i <= 'z')
				or (i >= 'A' and i <= 'Z')
				or (i >= '0' and i <= '9')
				or i == '_'
				))
			{
				return false;
			}
		}
		return true;
	}
}

class Compiler {
public:
	struct Config {
		std::string projectName;
		std::vector<std::string> requiredModules;
		std::vector<std::string> requiredPackages;
		std::vector<std::string> files;
		std::unordered_map<std::string, std::unordered_map<std::string, std::string>> options;
	};
private:
	struct Module_impl_t {
		Module_t* _module = nullptr;
		HINSTANCE dll_handler = NULL;
		bool init = false;
	};
	struct Parser_impl_t {
		Parser_t* parser = nullptr;
		bool init = false;
		std::string parentModule;
	};
	struct Lexer_impl_t {
		Lexer_t* lexer = nullptr;
		bool init = false;
		std::string parentModule;
	};

	template<typename T>
	class Repository {
		std::vector<T*> owner;
		std::unordered_map<std::string, T*> refs;
		std::function<void(T*)> destroyer;
	public:
		Repository(std::function<void(T*)> _destroyer) {
			this->destroyer = _destroyer;
		}
		void add(std::vector<std::string> keys, T* new_obj, std::function<void(T* first, T* second, std::string key)> duplicatNotice) {
			for (auto& i : keys) {
				auto it = refs.find(i);
				if (it != refs.end()) {
					duplicatNotice(it->second, new_obj, i);
				}
				else {
					refs.emplace(i, new_obj);
				}
			}
			owner.push_back(new_obj);
		}
		T* find(std::string key) {
			auto it = refs.find(key);
			if (it != refs.end()) {
				return it->second;
			}
			return nullptr;
		}
		~Repository() {
			for (auto i : owner) {
				destroyer(i);
				delete i;
			}
		}
	};

	//TODO change so can handle more modules per preambule
	Repository<Module_impl_t> modules{ [](Module_impl_t* _t) {
		if (_t->init) {
			_t->_module->destroy();
		}
		if (_t->dll_handler != nullptr)
			FreeLibrary(_t->dll_handler);
		delete _t->_module;
	} };
	Repository<Parser_impl_t> parsers{ [](Parser_impl_t* _t) {
		if (_t->init) {
			_t->parser->destroy();
		}
		delete _t->parser;
	} };
	Repository<Lexer_impl_t> lexers{ [](Lexer_impl_t* _t) {
		if (_t->init) {
			_t->lexer->destroy();
		}
		delete _t->lexer;
	} };

	CompilerContext context;
	Config config;
	void loadModules() {
		for (auto& m : this->config.requiredModules) {
			context.LogInfo(0, "Loading Module : " + m + "\n");
			std::wstring dllFileNameW = std::wstring(m.begin(), m.end()) + L".dll";
			HINSTANCE hDLL = LoadLibrary(dllFileNameW.c_str());
			if (hDLL != NULL) {
				registerModule_fun_t dllFunction = (registerModule_fun_t)GetProcAddress(hDLL, "registerModule");
				if (dllFunction != NULL) {
					Module_t* module_temp = new Module_t();
					module_temp->Struct_Version = 0;
					Lexer_t* lex_temp = new Lexer_t();
					Parser_t* pars_temp = new Parser_t();

					int result = dllFunction(module_temp, lex_temp, pars_temp);
					if (result != 0) {
						context.critical_ModuleLoadError(m, module_temp->ModuleLoadErrorMsg, result);
					}

					if (lex_temp->headnledPreambules.size() != 0) {
						for (auto i : lex_temp->headnledPreambules) {
							if (not allowedInPreambule(i)) context.critical_Module_notAllowedPreambuleName(m, i);
						}
						lexers.add(
							lex_temp->headnledPreambules,
							new Lexer_impl_t{ lex_temp,false,m },
							[this](Lexer_impl_t* first, Lexer_impl_t* second, std::string pp) {
								this->context.critical_conflict_Lexer(first->parentModule, second->parentModule, pp);
							});
					}
					else {
						delete lex_temp;
						lex_temp = nullptr;
					}

					if (pars_temp->headnledPreambules.size() != 0) {
						for (auto i : pars_temp->headnledPreambules) {
							if (not allowedInPreambule(i)) context.critical_Module_notAllowedPreambuleName(m, i);
						}
						parsers.add(
							pars_temp->headnledPreambules,
							new Parser_impl_t{ pars_temp,false,m },
							[this](Parser_impl_t* first, Parser_impl_t* second, std::string pp) {
								this->context.critical_conflict_Parser(first->parentModule, second->parentModule, pp);
							});
					}
					else {
						delete pars_temp;
						pars_temp = nullptr;
					}

					if (module_temp->headnledPreambules.size() != 0) {
						for (auto i : module_temp->headnledPreambules) {
							if (not allowedInPreambule(i)) context.critical_Module_notAllowedPreambuleName(m, i);
						}
						modules.add(
							module_temp->headnledPreambules,
							new Module_impl_t{ module_temp,hDLL,false },
							[this](Module_impl_t* first, Module_impl_t* second, std::string pp) {
								//More then one module can handle that particular preambule;
							});
					}
					else {
						delete module_temp;
						module_temp = nullptr;
					}
				}
				else {
					context.critical_CoruptedModule(m, "registerModule");
				}
			}
			else {
				context.critical_UnableToFindModule(m);
			}
		}
	}

	std::vector<Preambule>& parse(std::vector<Preambule>& res, std::ifstream& file, std::string filename) {
		Preambule partial;

		enum class Mode
		{
			preambule,
			name,
			body,
			OptionName,
			OptionValue,
		};
		Mode mode = Mode::preambule;

		std::unordered_map<std::string, std::string> fileOptions;
		std::string option_name;
		std::string option_value;
		bool fileOption = false;

		String lineInBody;
		bool insideBody = false;

		Position pos;
		pos.filename = filename;

		//TODO make abstraction for file and file positions
		auto getChar = [&pos, &file]() -> char {
			int c = file.get();
			if (c == '\r') {
				int h = file.get();
				if (h == '\n')
				{
					c = h;
				}
				else {
					file.unget(); // h
				}
			}
			pos.character++;
			if (c == '\n')
				pos.newLine();
			return c;
			};
		auto ungetChar = [&pos, &file]() {

			file.unget();
			char c = file.get();
			if (c == '\n') {
				//FIXME
				pos.line--;
			}
			file.unget();
			};

		while (file.good()) {
			int c = getChar();

			switch (mode)
			{
			case Mode::preambule:

				if (c == '#' and partial.preambule_name.empty()) { mode = Mode::OptionName; }
				else if (c == '\n' and not partial.preambule_name.empty()) {
					mode = Mode::body;
				}
				else if (isspace(c) and partial.preambule_name.empty());
				else if (isspace(c) and not partial.preambule_name.empty()) {
					mode = Mode::name;
				}
				else if (not isspace(c) or not partial.preambule_name.empty()) {
					if (partial.preambule_name.empty()) {
						partial.preambule_name.pos = pos;
					}
					partial.preambule_name += c;
				}
				else if (not allowedInPreambule(partial.preambule_name.val + (char)c)) {
					context.critical_notallowedPreambuleName(partial.preambule_name, c);
				}
				break;
			case Mode::name:
				if (c == '\n') {
					mode = Mode::body;
				}
				else if (not isspace(c) or not partial.name.empty()) {
					partial.name += c;
				}
				break;
			case Mode::body:
				if (insideBody) {
					if (c == '\n') {
						if (not lineInBody.empty())
							partial.body.lines.push_back(lineInBody);
						lineInBody.val = "";
						insideBody = false;
					}
					else {
						if (lineInBody.empty()) {
							lineInBody.pos = pos;
						}
						lineInBody += c;
					}
				}
				else {
					if (c == '\t') insideBody = true;
					else {
						ungetChar();
						mode = Mode::preambule;
						res.push_back(partial);
						partial = Preambule{};
					}
				}
				break;
			case Mode::OptionName:
				if (c == '\n') {
					if (fileOption) {
						fileOptions.emplace(std::make_pair(option_name, option_value));
					}
					else {
						partial.options.emplace(std::make_pair(option_name, option_value));
					}
					mode = Mode::preambule;
					option_name = "";
					option_value = "";
				}
				else if (c == '#' and option_name == "") fileOption = true;
				else if (isspace(c) and option_name == "");
				else if (isspace(c) and option_name != "") {
					mode = Mode::OptionValue;
				}
				else {
					option_value += c;
				}
				break;
			case Mode::OptionValue:
				if (c == '\n') {
					if (fileOption) {
						fileOptions.emplace(std::make_pair(option_name, option_value));
					}
					else {
						partial.options.emplace(std::make_pair(option_name, option_value));
					}
					mode = Mode::preambule;
					option_name = "";
					option_value = "";
				}
				else if (not isspace(c) or option_value != "") {
					option_value += c;
				}
				break;
			default:
				break;
			}
		}
		return res;
	}
public:
	Compiler(Config config) {
		this->config = config;
		this->context.projectName = config.projectName;
		context.LogInfo(1, "Loading Modules\n");
		loadModules();
	}
	void compile() {
		std::vector<Preambule> code;
		context.LogInfo(1, "Compiling Project " + config.projectName + "\n");
		for (auto& file_name : config.files) {
			context.LogInfo(0, "Compile File : " + file_name + "\n");
			std::ifstream file(file_name, std::ios::binary);
			parse(code, file, file_name);
		}
		for (auto& i : code) {
			auto p_m = modules.find(i.preambule_name.val);
			if (p_m == nullptr) context.critical_unrecognized_preambule(i.preambule_name);
		}
		for (auto& i : code) {
			auto it_lex = lexers.find(i.preambule_name.val);
			if (it_lex == nullptr) continue;

			if (it_lex->init == false) {
				it_lex->lexer->init();
				it_lex->init = true;
			}
			i.tokenizedStream = it_lex->lexer->lex(i);
			if (i.tokenizedStream == nullptr) continue;
			auto it_pars = parsers.find(i.preambule_name.val);
			if (it_pars == nullptr) continue;
			if (it_pars->init == false) {
				it_pars->parser->init();
				it_pars->init = true;
			}
			i.ast = it_pars->parser->parse_fun(i.tokenizedStream);
		}

		for (auto& i : code) {

		}

		//delete tokens and ast
		for (auto& i : code) {
			if (i.tokenizedStream != nullptr) {
				delete i.tokenizedStream;
			}
			if (i.ast != nullptr) {
				auto it = parsers.find(i.preambule_name.val);
				if (it != nullptr) {
					it->parser->freeAST(i.ast);
					i.ast = nullptr;
				}
				else {
					context.critical_InternalError("cant dealocate AST\n parser not found (was null)");
				}
			}
		}
	}
	~Compiler() {
	}
};

struct loadConfigResult {

	enum class Error_t
	{
		Ok,
		incorectConfig
	};
	Error_t error;
	Compiler::Config res;

};

loadConfigResult loadConfig(int arg, char** args) {
	if (arg < 2) return loadConfigResult{ loadConfigResult::Error_t::incorectConfig,Compiler::Config{} };
	Compiler::Config config;
	config.files = { args[1] };
	config.projectName = args[1];
	config.requiredModules = { "BrainfuckModule","PrinterModule" };
	return loadConfigResult{
		loadConfigResult::Error_t::Ok,
		config
	};
}


int main(int arg, char** args)
{
	auto config = loadConfig(arg, args);
	if (config.error != loadConfigResult::Error_t::Ok) return -1;
	Compiler compiler(config.res);
	compiler.compile();
	//std::ifstream file(args[1]);
	//std::cout << args[1] << std::endl;
	//ModuleRepository repo;
	//for (const auto& _module : modules) {
	//	_module->RegisterPreambule(repo);
	//}
	//CompilerContext context;
	//auto codes = parse(file, args[1], context);
	//for (const auto& i : codes) {
	//	repo.RegisterSymbols(i);
	//}
	//for (const auto& i : codes) {
	//	repo.GenerateCode(i);
	//}
	//repo.runFinalizeOnModules();
}