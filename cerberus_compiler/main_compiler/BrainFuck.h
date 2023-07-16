#pragma once

#include "ICompilerModule.h"

class IModuleRepository;
struct SymbolsRepository;
struct CodeFragment;

class BrainFuck : public ICompilerModule {
	char* mem = nullptr;
public:
	virtual void RegisterPreambule(IModuleRepository& repo) ;
	virtual void Init(SymbolsRepository& symbols) ;
	virtual void RegisterSymbols(const CodeFragment&, SymbolsRepository& symbols);
	virtual void GenerateCode(const CodeFragment&, SymbolsRepository& symbols);
	virtual void Finalize(SymbolsRepository& symbols) ;
};