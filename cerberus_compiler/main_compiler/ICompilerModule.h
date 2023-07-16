#pragma once

class IModuleRepository;
struct SymbolsRepository;
struct CodeFragment;

class ICompilerModule {
public:
	virtual void RegisterPreambule(IModuleRepository&) = 0;
	virtual void Init(SymbolsRepository& symbols) = 0;
	virtual void RegisterSymbols(const CodeFragment&, SymbolsRepository& symbols) = 0;
	virtual void GenerateCode(const CodeFragment&, SymbolsRepository& symbols) = 0;
	virtual void Finalize(SymbolsRepository& symbols) = 0;
};