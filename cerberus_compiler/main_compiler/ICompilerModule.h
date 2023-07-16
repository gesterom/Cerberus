#pragma once

class IModuleRepository;
struct CompilerContext;
struct CodeFragment;

class ICompilerModule {
public:
	virtual void RegisterPreambule(IModuleRepository&) = 0;
	virtual void Init(CompilerContext& symbols) = 0;
	virtual void RegisterSymbols(const CodeFragment&, CompilerContext& symbols) = 0;
	virtual void DefinitionOfSymbols(const CodeFragment&, CompilerContext& symbols) = 0;
	virtual void GenerateCode(const CodeFragment&, CompilerContext& symbols) = 0;
	virtual void Finalize(CompilerContext& symbols) = 0;
};