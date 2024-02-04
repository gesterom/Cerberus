// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include "../main_compiler/ModuleInterface.h"
#include "Emitter.h"
#include <sstream>
#include <iomanip>

SymbolTypeId procedureId;
SymbolTypeId typeId;
SymbolTypeId llvmId;

Emitter* emit;

uint16_t tokenType(String str) {
	for (const auto& i : control) {
		if (str.val == i) {
			return (uint16_t)StackScript::TokenType::control;
		}
	}
	for (const auto& i : ops) {
		if (str.val == i) {
			return (uint16_t)StackScript::TokenType::op;
		}
	}
	if (str.val == "{" or str.val == "}") return (uint16_t)StackScript::TokenType::parentheses;
	bool flag = true;
	for (const auto& i : str.val) {
		if (not isdigit(i))
		{
			flag = false;
			break;
		}
	}
	if (flag) {
		return (uint16_t)StackScript::TokenType::literal;
	}
	return  (uint16_t)StackScript::TokenType::uknown;
}

TokenizedStream* lexer_fun(const Preambule& code, CompilerInterface* context) {

	if (code.preambule_name.val == "stackscript") {
		TokenizedStream* res = new TokenizedStream();

		std::string func_name;
		std::string argsNumber;

		bool name_done = false;
		for (const auto& i : code.name.val) {
			if (not std::isspace(i) and name_done == false) {
				func_name += i;
			}
			else if (std::isspace(i) and name_done == false) {
				name_done = true;
			}
			else if(std::isspace(i) and argsNumber=="" and name_done) {
				//skip
			}
			else if (name_done and std::isdigit(i)) {
				argsNumber+=i;
			}
			else {
				break;
			}
			//add arguments
		}
		Token t;
		t.type = (uint16_t)StackScript::TokenType::name;
		t.val.val = func_name;
		t.val.pos = code.name.pos;

		res->name.push_back(t);

		t.type = (uint16_t)StackScript::TokenType::literal;
		t.val.pos = code.name.pos;
		t.val.val = argsNumber!="" ? argsNumber : "0";
		res->name.push_back(t);

		for (const auto& i : code.body.lines) {
			std::string temp;
			Position p;
			p.line = i.pos.line;
			for (int j = 0; j < i.val.size(); j++) {
				if (temp == "" and not isspace(i.val[j])) {
					p.character = j;
					temp += i.val[j];
				}
				else if (temp != "" and not isspace(i.val[j])) {
					temp += i.val[j];
				}
				else if (temp != "" and isspace(i.val[j])) {
					Token t;
					String str;
					str.pos = p;
					str.val = temp;
					t.val = str;
					t.type = tokenType(str);
					res->body.push_back(t);
					temp = "";
				}
			}
			if (temp != "") {
				Token t;
				String str;
				str.pos = p;
				str.val = temp;
				t.val = str;
				res->body.push_back(t);
				temp = "";
			}
		}
		return res;
	}
	else {
		return nullptr;
	}
}

bool canBeFunnction(String str) {
	for (int i = 0; i < str.val.size(); i++) {
		if (not isalpha(str.val[i])) {
			return false;
		}
	}
	return true;
}

std::string convertVoidPointerToString(const void* ptr) {
	std::stringstream ss;
	ss << "0x" << std::hex << std::setw(sizeof(uintptr_t) * 2) << std::setfill('0') << reinterpret_cast<uintptr_t>(ptr);
	return ss.str();
}

llvm::Function* createPrintfDeclaration(llvm::Module* TheModule, llvm::LLVMContext& context) {
	auto f = TheModule->getFunction("printf");
	if (not f) {
		llvm::FunctionType* printfFuncType = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), { llvm::Type::getInt8PtrTy(context) }, true);
		return llvm::Function::Create(printfFuncType, llvm::Function::ExternalLinkage, "printf", TheModule);
	}
	return f;
}

llvm::Function* createScanfDeclaration(llvm::Module* TheModule, llvm::LLVMContext& context) {
	auto f = TheModule->getFunction("scanf");
	if (not f) {
		llvm::FunctionType* scanfFuncType = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), { llvm::Type::getInt8PtrTy(context) }, true);
		return llvm::Function::Create(scanfFuncType, llvm::Function::ExternalLinkage, "scanf", TheModule);
	}
	return f;
}

int initModule(CompilerInterface* context) {

	auto ti = context->getSymbolTypeInfo(context->context, "procedure");
	if (not ti.found) {
		procedureId = context->registerSymbolType(context->context, "procedure", SymbolSchema::Procedure, nullptr);
	}
	else {
		procedureId = ti.id;
	}
	ti = context->getSymbolTypeInfo(context->context, "type");
	if (not ti.found) {
		typeId = context->registerSymbolType(context->context, "type", SymbolSchema::Struct, nullptr);
	}
	else {
		typeId = ti.id;
	}

	auto llvmSymbols = context->getSymbolTypeInfo(context->context, "llvm");

	if (llvmSymbols.found == false) {
		llvmId = context->registerSymbolType(context->context, "llvm", SymbolSchema::Raw_pointer, convertVoidPointerToString);
		emit = new Emitter(nullptr, nullptr, nullptr);
		context->defineSymbol(context->context, llvmId, String("builder"), emit->Builder, sizeof(void*));
		context->defineSymbol(context->context, llvmId, String("context"), emit->TheContext, sizeof(void*));
		context->defineSymbol(context->context, llvmId, String("module"), emit->TheModule, sizeof(void*));
	}
	else {
		emit = new Emitter(
			(llvm::LLVMContext*)context->findSymbol(context->context, llvmSymbols.id, "context").data,
			(llvm::Module*)context->findSymbol(context->context, llvmSymbols.id, "module").data,
			(llvm::IRBuilder<>*)context->findSymbol(context->context, llvmSymbols.id, "builder").data
		);
	}
	llvm::ArrayType* arrayType = llvm::ArrayType::get(llvm::Type::getInt32Ty(*emit->TheContext), 1024);

	emit->stack = new llvm::GlobalVariable(
		*emit->TheModule,                        // Module to add the global variable to
		arrayType,                      // Type of the global variable
		false,                          // Is the variable constant? (false for non-constant)
		llvm::GlobalValue::ExternalLinkage, // Linkage type (you can adjust this based on your needs)
		llvm::ConstantAggregateZero::get(arrayType), // Initializer (zero-initializer in this case)
		"stackscript_stack"                   // Variable name
	);

	llvm::Type* intType = llvm::Type::getInt32Ty(*emit->TheContext);
	emit->stackPointer = new llvm::GlobalVariable(
		*emit->TheModule,
		intType,
		false,
		llvm::GlobalValue::ExternalLinkage,
		llvm::ConstantInt::get(intType, 0),  // Initial value (you can set it as needed)
		"stack_pointer"
	);

	emit->print = createPrintfDeclaration(emit->TheModule, *emit->TheContext);
	std::vector<llvm::Type*> printfArgs(1, llvm::Type::getInt8PtrTy(*emit->TheContext));
	llvm::FunctionType* printfType = llvm::FunctionType::get(llvm::Type::getInt32Ty(*emit->TheContext), printfArgs, true);
	emit->TheModule->getOrInsertFunction("printf", printfType);

	emit->scan = createScanfDeclaration(emit->TheModule, *emit->TheContext);
	std::vector<llvm::Type*> scanfArgs(1, llvm::Type::getInt8PtrTy(*emit->TheContext));
	llvm::FunctionType* scanfType = llvm::FunctionType::get(llvm::Type::getInt32Ty(*emit->TheContext), scanfArgs, true);
	auto ttt = emit->TheModule->getOrInsertFunction("scanf", scanfType);

	return 0;
}

int phaseRegisterSymbols(const Preambule& code, CompilerInterface* context) {
	if (code.preambule_name.val == "stackscript") {
		if (not canBeFunnction(code.tokenizedStream->name[0].val)) {
			context->error_msg(context->context, ErrorType::Other, code.preambule_name.pos, "stacksript can have this name");
		}
		context->registerSymbolByID(context->context, procedureId, code.tokenizedStream->name[0].val);
	}
	return 0;
}

int phaseDefineSymbols(const Preambule& code, CompilerInterface* context) {
	if (code.preambule_name.val == "stackscript") {
		auto id = context->findSymbol(context->context, procedureId, code.tokenizedStream->name[0].val.val.c_str()).id;
		SymbolInfo_Procedure * info = new SymbolInfo_Procedure();
		auto intId = context->findSymbol(context->context, typeId, "Int").id;
		int numofArgs = 0;
		if (code.tokenizedStream->name.size() > 1) {
			numofArgs = stoi(code.tokenizedStream->name[1].val.val);
		}

		for (int i = 0; i < numofArgs; i++) {
			info->args.push_back(std::make_pair( intId, String("a") ) );
		}
		std::vector<llvm::Type*> args_ty;
		for (int i = 0; i < numofArgs; i++) {
			args_ty.push_back( emit->Builder->getInt32Ty() );
		}
		info->return_type = intId;
		info->var_args = false;

		llvm::FunctionType* functionType = llvm::FunctionType::get(
			emit->Builder->getInt32Ty(), // Replace with the actual return type.
			llvm::ArrayRef<llvm::Type*>(args_ty.data(), args_ty.size()),
			/*isVarArg=*/false
		);

		info->llvm_func = llvm::Function::Create(
			functionType,
			llvm::Function::ExternalLinkage,
			llvm::Twine(code.tokenizedStream->name[0].val.val),
			emit->TheModule
		);
		context->defineSymbolByID(context->context,procedureId,id,info,sizeof(info));
	}
	return 0;
}

int generateCodePhase(const Preambule& code, CompilerInterface* context) {
	if (code.preambule_name.val == "stackscript") {

		auto proc = context->findSymbol(context->context, procedureId, code.tokenizedStream->name[0].val.val.c_str());
		emit->func = (llvm::Function*)((SymbolInfo_Procedure*)proc.data)->llvm_func;


		llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(emit->TheModule->getContext(), "entry", emit->func);
		emit->Builder->SetInsertPoint(entryBlock);

		emit->clearStack();

		auto it = emit->func->arg_begin();
		for (int i = 0; i < emit->func->arg_size(); i++) {
			auto arg = &*(it);
			emit->PushValueonStack(arg);
			it++;
		}
		emit->compile(code.tokenizedStream,context);
		emit->returnStatement();
		llvm::verifyFunction(*emit->func);
		//llvm::raw_fd_ostream file("output.ll", EC, llvm::sys::fs::OF_Text);
		//TheModule->print(file, nullptr);
	}
	return 0;
}

extern "C" {
	__declspec(dllexport) int registerModule(ModuleInterface* module, LexerInterface* lexer, ParserInterface* parser) {

		module->Module_Version[0] = 0;
		module->Module_Version[1] = 0;
		module->Module_Version[2] = 1;
		module->Module_Version[3] = 0;
		module->ModuleName = "StackCraft";
		module->ModuleLoadErrorMsg = "";
		module->supportedPreambules = { "stackscript" };
		module->initModule = initModule;
		module->phase_registerSymbols = phaseRegisterSymbols;
		module->phase_defineSymbols = phaseDefineSymbols;
		module->phase_staticAnalysis = nullptr;
		module->phase_generateCode = generateCodePhase;
		module->finalizeModule = nullptr;
		module->destroy = nullptr;

		lexer->supportedPreambules = { "stackscript" };
		lexer->lex = lexer_fun;
		lexer->init = nullptr;
		lexer->destroy = nullptr;

		parser->supportedPreambules = { "stackscript" };
		parser->init = nullptr;
		parser->parse_fun = nullptr;
		parser->freeAST = nullptr;
		parser->destroy = nullptr;
		return 0;
	}
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

