#pragma once
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <vector>

struct ModuleInterface;
struct LexerInterface;
struct ParserInterface;

typedef uint32_t SymbolTypeId ;
typedef uint32_t SymbolId;

struct Position {
	std::string filename;
	int line = 1;
	int character = 0;
};

struct String {
	std::string val;
	Position pos;
};

struct Token {
	uint16_t module_id = 0;
	uint16_t type = 0;
	String val;
};

struct TokenizedStream {
	std::vector<Token> name;
	uint32_t name_offset =0 ;
	std::vector<Token> body;
	uint32_t body_offset = 0;
};

struct Preambule {

	struct Body_t {
		std::vector<String> lines;
	};

	std::unordered_map<std::string, std::string> options;
	String preambule_name;
	String name;
	Body_t body;
	TokenizedStream* tokenizedStream = nullptr;
	void* ast = nullptr;
};

enum class CriticalErrorType {
	SyntaxError,
	UnMatchedParentice,
	UnexpectedCharacter,
	UnexpectedToken,
	ExpectToken
};

enum class ErrorType {
	SyntaxError,
	UnMatchedParentice,
	UnexpectedCharacter,
};

enum class WarningType {

};

enum class SymbolSchema {
	Integer64,
	key_value_pairs,
	Struct,
	procedure,
	raw_pointer
};

typedef std::string(*print_symbol_fun_t)(void*);

struct SymbolTypeInfo {
	SymbolTypeId id;
	SymbolSchema schema;
	const char* symbolTypeName;
	const char* originModule;
	print_symbol_fun_t print; //TODO change to const char* but memmory safe
};

struct SymbolInfo {
	SymbolId id;
	String symbolName;
	SymbolTypeId typeId;
	const char* originModule;
	size_t data_size;
	void* data;
};

typedef void CompilerContext;

struct CompilerInterface {
	CompilerContext* context = nullptr;

	//symbol table;
	SymbolTypeId (*registerSymbolType)(CompilerContext* context,const char* symbolTypeName,SymbolSchema schama, print_symbol_fun_t print) = nullptr;
	SymbolTypeInfo (*getSymbolTypeInfo)(CompilerContext* context,const char* symbolTypeName) = nullptr;

	SymbolId(*registerSymbol)(CompilerContext* context,SymbolTypeId, String name) = nullptr;
	bool (*defineSymbol)(CompilerContext* context,SymbolTypeId typeId, SymbolId id, void* definition) = nullptr;
	SymbolInfo (*findSymbol)(CompilerContext* context,SymbolTypeId typeId, const char* name) = nullptr;
	SymbolInfo (*findSymbolById)(CompilerContext* context, SymbolTypeId typeId, SymbolId id) = nullptr;

	//errorHandling and logging
	void (*critical_error_msg)(CompilerContext* context,CriticalErrorType type, Position pos,const char* msg) = nullptr;
	void (*error_msg)(CompilerContext* context,ErrorType type, Position pos, const char* msg) = nullptr;
	void (*warning_msg)(CompilerContext* context,WarningType type, Position pos, const char* msg) = nullptr;
	void (*log_msg)(CompilerContext* context,uint64_t levelMask, const char* msg) = nullptr;
	void (*log_msg_pos)(CompilerContext* context, uint64_t levelMask,Position pos, const char* msg) = nullptr;
};

typedef int (*event_t)();
typedef int (*moduleHandler_phase_t)(const Preambule& code, CompilerInterface* context);

struct ModuleInterface
{
	uint32_t Struct_Version = 0 ;
	uint8_t Module_Version[4] = {0};
	const char* ModuleName = nullptr;
	const char* ModuleLoadErrorMsg = nullptr;
	std::vector<std::string> supportedPreambules;
	event_t initModule = nullptr;
	moduleHandler_phase_t phase_registerSymbols = nullptr;
	moduleHandler_phase_t phase_defineSymbols = nullptr;
	moduleHandler_phase_t phase_generateCode = nullptr;
	event_t finalizeModule = nullptr;
	event_t destroy = nullptr;
};

typedef TokenizedStream* (*lexer_fun_t)(const Preambule& code, CompilerInterface* context);

struct LexerInterface {
	std::vector<std::string> supportedPreambules;
	lexer_fun_t lex = nullptr;
	event_t init = nullptr;
	event_t destroy = nullptr;
};

typedef void* pointerToAST;
typedef pointerToAST(*parse_fun_t)(const Preambule& code, CompilerInterface* context);
typedef void (*freeAST_fun_t)(pointerToAST);

//semanics analisis
struct ParserInterface {
	std::vector<std::string> supportedPreambules;
	parse_fun_t parse_fun = nullptr;
	freeAST_fun_t freeAST = nullptr;
	event_t init = nullptr;;
	event_t destroy = nullptr;
};

typedef int (*registerModule_fun_t)(ModuleInterface*, LexerInterface* lexer, ParserInterface* parser);


//int registerModule(ModuleInterface* module, LexerInterface* lexer, ParserInterface* parser);



//change CompilerContext to struct with function fields as interface;