// dllmain.cpp : Definiuje punkt wejścia dla aplikacji DLL.
#include "pch.h"

#include "../main_compiler/ModuleInterface.h"

#include "Lexer.h"
#include "Parser.h"

#include "Consts.h"
#include "IREmiterVisitor.h"
#include "TypeCheckVisitor.h"
#include <algorithm>
#include <memory>
#include <iostream>
#include <sstream>
#include <iomanip>

IREmiterVisitor* emiter;
TypeCheckVisitor* typeChecker;
SymbolTypeId procedureId;
SymbolTypeId typeId;
SymbolTypeId llvmId;

int initLexer() {

	std::sort(operators.begin(), operators.end(), [](Operator_t a, Operator_t b) -> bool {
		return a.representation.size() > b.representation.size();
		});

	return 0;
}

int nopPhase() {
	return 0;
}

pointerToAST parse_code(const Preambule& code, CompilerInterface* context) {
	if (code.preambule_name.val == "procedure") {
		Procedure* res = new Procedure();
		*res = parseProcedure(code.tokenizedStream, context);
		if (code.options.find("ParserDebug") != code.options.end() and code.options.at("ParserDebug") == "True") {
			PrinterVisitor p;
			p.icompiler = context;
			for (const auto& i : res->code) {
				i->visit(&p);
			}
		}
		return new StoredASTSumType(res);
	}
	else if (code.preambule_name.val == "type") {
		StructType* st = parseStruct(code.tokenizedStream,context);
		return new StoredASTSumType(st);
	}
	else if (code.preambule_name.val == "import_c") {
		Procedure* res = new Procedure;
		parseHeaderOffFunction(*res, code.tokenizedStream, context);

		return new StoredASTSumType(res);
	}
	return nullptr;
}

std::string convertVoidPointerToString(const void* ptr) {
	std::stringstream ss;
	ss << "0x" << std::hex << std::setw(sizeof(uintptr_t) * 2) << std::setfill('0') << reinterpret_cast<uintptr_t>(ptr);
	return ss.str();
}

SymbolInfo defineType(CompilerInterface* context, std::string str) {
	auto at = context->registerSymbolByID(context->context, typeId, String(str));
	context->defineSymbolByID(context->context, typeId, at, new TypeNameExpression(str,at), sizeof(void*));
	return context->findSymbolById(context->context, typeId, at);
}

int initModule(CompilerInterface* context) {

	typeChecker = new TypeCheckVisitor();

	auto ti = context->getSymbolTypeInfo(context->context, "procedure");
	if (not ti.found) {
		procedureId = context->registerSymbolType(context->context, "procedure", SymbolSchema::Procedure, nullptr);
	}
	else {
		procedureId = ti.id;
	}
	ti = context->getSymbolTypeInfo(context->context, "type");
	if (not ti.found) {
		typeId = context->registerSymbolType(context->context, "type", SymbolSchema::Raw_pointer, nullptr);
	}
	else {
		typeId = ti.id;
	}

	//auto context->registerSymbolByID(context->context, typeId, String("Int"));
	auto i_t = defineType(context, "Int");
	auto charID = defineType(context, "Char");
	//defineType(context, "String");
	defineType(context, "Float");
	defineType(context, "Bool");
	auto ptr_t = defineType(context, "Ptr");
	auto vtt = new InternalType("Void");
	auto v_symbolId = context->registerSymbolByID(context->context, typeId, String(vtt->toString()));
	vtt->typeId = v_symbolId;
	auto v_t = context->defineSymbolByID(context->context, typeId, v_symbolId, vtt, sizeof(void*));

	auto llvmSymbols = context->getSymbolTypeInfo(context->context, "llvm");

	auto stringID = context->registerSymbolByID(context->context, typeId, String("String"));
	context->defineSymbolByID(context->context, typeId, stringID, new ArrayType( new TypeNameExpression("Char", charID.id) ), sizeof(void*));
	context->findSymbolById(context->context, typeId, stringID);

	emiter = nullptr;
	if (llvmSymbols.found == false) {
		llvmId = context->registerSymbolType(context->context, "llvm", SymbolSchema::Raw_pointer, convertVoidPointerToString);
		emiter = new IREmiterVisitor(nullptr, nullptr, nullptr);
		context->defineSymbol(context->context, llvmId, String("builder"), emiter->Builder, sizeof(void*));
		context->defineSymbol(context->context, llvmId, String("context"), emiter->TheContext, sizeof(void*));
		context->defineSymbol(context->context, llvmId, String("module"), emiter->TheModule, sizeof(void*));
	}
	else {
		emiter = new IREmiterVisitor(
			(llvm::LLVMContext*)context->findSymbol(context->context, llvmSymbols.id, "context").data,
			(llvm::Module*)context->findSymbol(context->context, llvmSymbols.id, "module").data,
			(llvm::IRBuilder<>*)context->findSymbol(context->context, llvmSymbols.id, "builder").data
		);
	}
	emiter->procedureSymbolTypeId = procedureId;
	emiter->typeSymbolTypeId = typeId;
	emiter->meta_context = context;

	typeChecker->procedureSymbolTypeId = procedureId;
	typeChecker->typeSymbolTypeId = typeId;

	SymbolInfo_Procedure* printf_t = new SymbolInfo_Procedure();
	printf_t->return_type = v_symbolId;
	//printf_t->args.emplace_back(std::make_pair(ptr_t, String("val")));
	printf_t->var_args = true;
	context->defineSymbol(context->context, procedureId, String("printf"), printf_t, sizeof(void*));
	
	SymbolInfo_Procedure* parseInt_t = new SymbolInfo_Procedure();
	parseInt_t->return_type = i_t.id;
	//parseInt_t->args.emplace_back(std::make_pair(ptr_t, String("val")));
	parseInt_t->var_args = false;
	context->defineSymbol(context->context, procedureId, String("parseInt"), parseInt_t, sizeof(void*));

	return (emiter == nullptr or typeChecker == nullptr) ? -1 : 0;
}

int finileStep(CompilerInterface* context) {

	emiter->finish();
	return 0;
}


void freeAST(pointerToAST node) {
	StoredASTSumType* t = (StoredASTSumType*)node;
	if (t->type == StoredASTSumType::Type::Procedure) {
		delete (Procedure*)t->ptr;
	}
	else if(t->type == StoredASTSumType::Type::Struct) {
		delete (IType*)t->ptr;
	}
	else {
		assert(false);
	}
	delete (StoredASTSumType*)node;
}

int generateCodePhase(const Preambule& code, CompilerInterface* context) {
	if (code.preambule_name.val == "procedure") {
		//IREmiterVisitor* emiter = new IREmiterVisitor();
		emiter->compile(code, context);
	}
	else if (code.preambule_name.val == "type") {
		//IREmiterVisitor* emiter = new IREmiterVisitor();
		//
	}
	else if (code.preambule_name.val == "import_c") {
		//IREmiterVisitor* emiter = new IREmiterVisitor();
		//Procedure* p = (Procedure*)((StoredASTSumType*)code.ast)->ptr;
		//emiter->addExternalProcedure(p->name.val, p->args, p->returnType);
	}
	return 0;
}

int checkTypePhase(const Preambule& code, CompilerInterface* context) {

	if (code.preambule_name.val == "procedure") {
		//IREmiterVisitor* emiter = new IREmiterVisitor();
		typeChecker->check(code, context);
	}
	return 0;
}

int phaseDefineSymbols(const Preambule& code, CompilerInterface* context) {
	
	if (code.preambule_name.val == "type") {
		StructType* st = ((StructType*)((StoredASTSumType*)code.ast)->ptr);
		auto symbol = context->findSymbol(context->context, typeId, st->name.val.c_str());

		assert(symbol.found);
		
		StructType* type = new StructType();

		for (const auto& i : st->inner_type) {
			//auto symbolid = context->findSymbol(context->context,typeId,i.first->toString().c_str()).id;
			type->inner_type.emplace_back(std::make_pair(i.first,i.second));
		}
		context->defineSymbolByID(context->context, typeId, symbol.id, type,sizeof(type));
	}
	else if (code.preambule_name.val == "procedure") {
		Procedure* p = ((Procedure*)((StoredASTSumType*)code.ast)->ptr);
		auto symbol = context->findSymbol(context->context, procedureId, p->name.val.c_str()).id;

		SymbolInfo_Procedure* type = new SymbolInfo_Procedure();
		if (p->returnType == nullptr) {
			type->return_type = context->findSymbol(context->context, typeId, "@Void").id;
		}
		else {
			auto retTypeSymbol = context->findSymbol(context->context, typeId, p->returnType->toString().c_str());
			auto arrayType = dynamic_cast<ArrayType*>(p->returnType);
			if (not retTypeSymbol.found and arrayType) {
				auto arrTypeSymbolID = context->registerSymbolByID(context->context,typeId,String(p->returnType->toString()));
				context->defineSymbolByID(context->context,typeId,arrTypeSymbolID, p->returnType ,sizeof(p->returnType));
				type->return_type = arrTypeSymbolID;
			}
			else {
				type->return_type = retTypeSymbol.id;
			}
		}

		for (const auto& i : p->args) {
			auto retTypeSymbol = context->findSymbol(context->context, typeId, i.first->toString().c_str());
			auto arrayType = dynamic_cast<ArrayType*>(i.first);
			if (not retTypeSymbol.found and arrayType) {
				auto arrTypeSymbolID = context->registerSymbolByID(context->context, typeId, String(i.first->toString()));
				context->defineSymbolByID(context->context, typeId, arrTypeSymbolID, i.first, sizeof(i.first));
			}
		}

		for (const auto& i : p->args) {
			type->args.push_back(std::make_pair(context->findSymbol(context->context, typeId, i.first->toString().c_str()).id, i.second));
		}
		type->llvm_func = emiter->addExternalProcedure(p->name.val, p->args, p->returnType);

		context->defineSymbolByID(context->context, procedureId, symbol, type, sizeof(void*));

	}
	else if (code.preambule_name.val == "import_c") {
		Procedure* p = ((Procedure*)((StoredASTSumType*)code.ast)->ptr);;
		auto symbol = context->findSymbol(context->context,procedureId,p->name.val.c_str()).id;

		SymbolInfo_Procedure* type = new SymbolInfo_Procedure();
		if (p->returnType == nullptr) {
			type->return_type = context->findSymbol(context->context, typeId, "@Void").id;
		}
		else {
			type->return_type = context->findSymbol(context->context, typeId, p->returnType->toString().c_str()).id;
		}
		for (const auto& i : p->args) {
			type->args.push_back(std::make_pair(context->findSymbol(context->context, typeId, i.first->toString().c_str()).id, i.second));
		}
		type->llvm_func = emiter->addExternalProcedure(p->name.val, p->args, p->returnType);

		context->defineSymbolByID(context->context, procedureId, symbol, type, sizeof(void*));
	}
	return 0;

}

int phaseRegisterSymbols(const Preambule& code, CompilerInterface* context)
{
	if (code.preambule_name.val == "type") {
		StructType* st = ((StructType*)((StoredASTSumType*)code.ast)->ptr);
		auto symbol = context->registerSymbolByID(context->context, typeId, st->name);
	}
	else if (code.preambule_name.val == "procedure") {
		//std::unique_ptr<IREmiterVisitor> v = std::make_unique<IREmiterVisitor>();
		//v->compile(code,context);
		Procedure* p = ((Procedure*)((StoredASTSumType*)code.ast)->ptr);
		//parseHeaderOffFunction(*p,code.tokenizedStream,context);
		auto symbol = context->registerSymbolByID(context->context, procedureId, p->name);
	}
	else if (code.preambule_name.val == "import_c") {
		Procedure* p = ((Procedure*)((StoredASTSumType*)code.ast)->ptr);
		//parseHeaderOffFunction(*p, code.tokenizedStream, context);

		auto symbol = context->registerSymbolByID(context->context, procedureId, p->name);
	}
	return 0;
}

extern "C" {
	__declspec(dllexport) int registerModule(ModuleInterface* module, LexerInterface* lexer, ParserInterface* parser) {
		//int a = 0;
		//std::cout << a << std::endl;
		module->Module_Version[0] = 0;
		module->Module_Version[1] = 0;
		module->Module_Version[2] = 1;
		module->Module_Version[3] = 0;
		module->ModuleName = "Core";
		module->ModuleLoadErrorMsg = "";
		module->supportedPreambules = { "procedure","type","import_c" };
		module->initModule = initModule;
		module->phase_registerSymbols = phaseRegisterSymbols;
		module->phase_defineSymbols = phaseDefineSymbols;
		module->phase_staticAnalysis = checkTypePhase;
		module->phase_generateCode = generateCodePhase;
		module->finalizeModule = finileStep;
		module->destroy = nopPhase;

		lexer->supportedPreambules = { "procedure","type","import_c" };
		lexer->lex = lexer_fun;
		lexer->init = initLexer;
		lexer->destroy = nopPhase;

		parser->supportedPreambules = { "procedure","type","import_c" };
		parser->init = nopPhase;
		parser->parse_fun = parse_code;
		parser->freeAST = freeAST;
		parser->destroy = nopPhase;
		return 0;
	}
}



BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	int a = 0;
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

