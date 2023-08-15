#pragma once

//#include "ICompilerModule.h"
//
//class IModuleRepository;
//struct CompilerContext;
//struct CodeFragment;
//
//class BrainFuck : public ICompilerModule {
//	char* mem = nullptr;
//public:
//	virtual void RegisterPreambule(IModuleRepository& repo) ;
//	virtual void Init(CompilerContext& symbols) ;
//	virtual void RegisterSymbols(const CodeFragment&, CompilerContext& symbols);
//	virtual void DefinitionOfSymbols(const CodeFragment&, CompilerContext& symbols);
//	virtual void GenerateCode(const CodeFragment&, CompilerContext& symbols);
//	virtual void Finalize(CompilerContext& symbols) ;
//	virtual const char* const ModuleName() ;
//	virtual const char* const Version() ;
//};