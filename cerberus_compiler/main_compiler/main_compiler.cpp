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
#include "CompilerContextImpl.h"

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
		ModuleInterface* _module = nullptr;
		HINSTANCE dll_handler = NULL;
		bool init = false;
	};
	struct Parser_impl_t {
		ParserInterface* parser = nullptr;
		bool init = false;
		std::string parentModule;
	};
	struct Lexer_impl_t {
		LexerInterface* lexer = nullptr;
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

	class ModulesRepository {
		std::vector<Module_impl_t* > owner;
		std::unordered_map<std::string,std::vector<Module_impl_t*> > refs;
		public:
		void add(const std::vector<std::string>& preambules, Module_impl_t* m) {
			owner.push_back(m);
			for (auto& i : preambules) {
				refs[i].push_back(m);
			}
		}
		std::vector<Module_impl_t*> find(std::string key) {
			auto it = refs.find(key);
			if (it != refs.end()) {
				return it->second;
			}
			return std::vector<Module_impl_t*>{};
		}
		~ModulesRepository() {
			for (auto _t : owner) {
				if (_t->init) {
					_t->_module->destroy();
				}
				if (_t->dll_handler != nullptr)
					FreeLibrary(_t->dll_handler);
				delete _t->_module;
			}
		}
	};
	ModulesRepository modules;

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

	CompilerContextImpl context;
	Config config;
	void loadModules() {
		for (auto& m : this->config.requiredModules) {
			context.LogInfo(0, "Loading Module : " + m + "\n");
			std::wstring dllFileNameW = std::wstring(m.begin(), m.end()) + L".dll";
			HINSTANCE hDLL = LoadLibrary(dllFileNameW.c_str());
			if (hDLL != NULL) {
				registerModule_fun_t dllFunction = (registerModule_fun_t)GetProcAddress(hDLL, "registerModule");
				if (dllFunction != NULL) {
					ModuleInterface* module_temp = new ModuleInterface();
					module_temp->Struct_Version = 0;
					LexerInterface* lex_temp = new LexerInterface();
					ParserInterface* pars_temp = new ParserInterface();

					int result = dllFunction(module_temp, lex_temp, pars_temp);
					if (result != 0) {
						context.critical_ModuleLoadError(m, module_temp->ModuleLoadErrorMsg, result);
					}

					//check if all fields are not null;
					if (lex_temp->supportedPreambules.size() != 0) {
						for (auto i : lex_temp->supportedPreambules) {
							if (not allowedInPreambule(i)) context.critical_Module_notAllowedPreambuleName(m, i);
						}
						lexers.add(
							lex_temp->supportedPreambules,
							new Lexer_impl_t{ lex_temp,false,m },
							[this](Lexer_impl_t* first, Lexer_impl_t* second, std::string pp) {
								this->context.critical_conflict_Lexer(first->parentModule, second->parentModule, pp);
							});
					}
					else {
						delete lex_temp;
						lex_temp = nullptr;
					}

					if (pars_temp->supportedPreambules.size() != 0) {
						for (auto i : pars_temp->supportedPreambules) {
							if (not allowedInPreambule(i)) context.critical_Module_notAllowedPreambuleName(m, i);
						}
						parsers.add(
							pars_temp->supportedPreambules,
							new Parser_impl_t{ pars_temp,false,m },
							[this](Parser_impl_t* first, Parser_impl_t* second, std::string pp) {
								this->context.critical_conflict_Parser(first->parentModule, second->parentModule, pp);
							});
					}
					else {
						delete pars_temp;
						pars_temp = nullptr;
					}

					if (module_temp->supportedPreambules.size() != 0) {
						for (auto i : module_temp->supportedPreambules) {
							if (not allowedInPreambule(i)) context.critical_Module_notAllowedPreambuleName(m, i);
						}
						modules.add(
							module_temp->supportedPreambules,
							new Module_impl_t{ module_temp,hDLL,false });
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
			if (c == '\n') {
				pos.line++;
				pos.character = 1;
			}
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

				if (c == '#' and partial.preambule_name.val.empty()) { mode = Mode::OptionName; }
				else if (c == '\n' and not partial.preambule_name.val.empty()) {
					mode = Mode::body;
				}
				else if (isspace(c) and partial.preambule_name.val.empty());
				else if (isspace(c) and not partial.preambule_name.val.empty()) {
					mode = Mode::name;
				}
				else if (not isspace(c) or not partial.preambule_name.val.empty()) {
					if (partial.preambule_name.val.empty()) {
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
				else if (not isspace(c) or not partial.name.val.empty()) {
					partial.name += c;
				}
				break;
			case Mode::body:
				if (insideBody) {
					if (c == '\n') {
						if (not lineInBody.val.empty())
							partial.body.lines.push_back(lineInBody);
						lineInBody.val = "";
						insideBody = false;
					}
					else {
						if (lineInBody.val.empty()) {
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

						for (auto& i : fileOptions) {
							//auto itt = partial.options.find(i.first);
							//if (itt == partial.options.end()) {
								partial.options.emplace(i);
							//}							
						}

						res.push_back(partial);
						fileOption = false;
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
					fileOption = false;
				}
				else if (c == '#' and option_name == "") fileOption = true;
				else if (isspace(c) and option_name == "");
				else if (isspace(c) and option_name != "") {
					mode = Mode::OptionValue;
				}
				else {
					option_name += c;
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
					fileOption = false;
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

	void lexer_and_parse(std::vector<Preambule>& code) {
		for (auto& i : code) {
			auto it_lex = lexers.find(i.preambule_name.val);
			if (it_lex == nullptr) continue;

			if (it_lex->init == false) {
				it_lex->lexer->init();
				it_lex->init = true;
			}
			context.contextData.moduleName = it_lex->parentModule;
			auto temp_compilerInterface = context.getInterface();
			i.tokenizedStream = it_lex->lexer->lex(i,&temp_compilerInterface);
			if (i.tokenizedStream == nullptr) continue;
			auto it_pars = parsers.find(i.preambule_name.val);
			if (it_pars == nullptr) continue;
			if (it_pars->init == false) {
				it_pars->parser->init();
				it_pars->init = true;
			}
			i.ast = it_pars->parser->parse_fun(i, &temp_compilerInterface);
		}
	}

public:
	Compiler(Config config) {
		this->config = config;
		this->context.contextData.projectName = config.projectName;
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
			if (p_m.size() == 0) context.critical_unrecognized_preambule(i.preambule_name);
		}
		lexer_and_parse(code);

		for (auto& i : code) {
			for (auto m : modules.find(i.preambule_name.val)) {
				if (m->init == false) {
					int rrrr = m->_module->initModule();
					m->init = (rrrr == 0);
				}
				//TODO change context to struct with function pointes
				context.contextData.moduleName = m->_module->ModuleName;
				auto temp_interface = context.getInterface();
				m->_module->phase_generateCode(i,&temp_interface);
			}
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
	config.requiredModules = { "Core","BrainfuckModule","PrinterModule"};
	return loadConfigResult{
		loadConfigResult::Error_t::Ok,
		config
	};
}


int main(int arg, char** args)
{
	//TODO add error msg for handling missing file 
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