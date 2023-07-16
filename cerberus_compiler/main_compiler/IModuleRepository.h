#pragma once

#include <string>

class ICompilerModule;
struct CodeFragment;

class IModuleRepository {
public:
	virtual void registerToPreambule(std::string preambule, ICompilerModule*) = 0;
	virtual void RegisterSymbols(const CodeFragment& code) = 0;
	virtual void GenerateCode(const CodeFragment& code) = 0;
	virtual void runFinalizeOnModules() = 0;
};