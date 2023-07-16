#pragma once

#include <vector> 
#include "ICompilerModule.h"

struct String;


class CoreModule : public ICompilerModule
{
public:
	struct TypeDefinition {
		std::vector<std::pair<String,String>> types;
	};
	virtual void RegisterPreambule(IModuleRepository&);
	virtual void Init(CompilerContext& symbols);
	virtual void RegisterSymbols(const CodeFragment&, CompilerContext& symbols);
	virtual void DefinitionOfSymbols(const CodeFragment&, CompilerContext& symbols);
	virtual void GenerateCode(const CodeFragment&, CompilerContext& symbols);
	virtual void Finalize(CompilerContext& symbols);
};

