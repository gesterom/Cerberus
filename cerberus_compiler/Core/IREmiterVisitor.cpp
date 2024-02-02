#include "pch.h"
#include "IREmiterVisitor.h"
#pragma warning(push, 0)
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"
#pragma warning(pop)

#include <iostream>

// Declare malloc function
llvm::Function* createMallocDeclaration(llvm::Module* TheModule, llvm::LLVMContext& context) {
	llvm::FunctionType* mallocFuncType = llvm::FunctionType::get(llvm::Type::getInt8PtrTy(context), { llvm::Type::getInt64Ty(context) }, false);
	return llvm::Function::Create(mallocFuncType, llvm::Function::ExternalLinkage, "malloc", TheModule);
}

// Declare strcpy function
llvm::Function* createStrcpyDeclaration(llvm::Module* TheModule, llvm::LLVMContext& context) {
	llvm::Type* charPtrType = llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0);
	llvm::FunctionType* strcpyFuncType = llvm::FunctionType::get(charPtrType, { charPtrType, charPtrType }, false);
	return llvm::Function::Create(strcpyFuncType, llvm::Function::ExternalLinkage, "strcpy", TheModule);
}

llvm::Function* createPrintfDeclaration(llvm::Module* TheModule, llvm::LLVMContext& context) {
	llvm::FunctionType* printfFuncType = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), { llvm::Type::getInt8PtrTy(context) }, true);
	return llvm::Function::Create(printfFuncType, llvm::Function::ExternalLinkage, "printf", TheModule);
}

// Declare scanf function
llvm::Function* createScanfDeclaration(llvm::Module* TheModule, llvm::LLVMContext& context) {
	llvm::FunctionType* scanfFuncType = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), { llvm::Type::getInt8PtrTy(context) }, true);
	return llvm::Function::Create(scanfFuncType, llvm::Function::ExternalLinkage, "scanf", TheModule);
}

IREmiterVisitor::IREmiterVisitor(
	llvm::LLVMContext* ctx,
	llvm::Module* tm,
	llvm::IRBuilder<>* b
)
{
	if (ctx == nullptr) {
		TheContext = new llvm::LLVMContext();
	}
	else {
		TheContext = ctx;
	}
	if (tm == nullptr) {
		TheModule = new llvm::Module("CoreModule", *TheContext);
	}
	else {
		TheModule = tm;
	}
	if (b == nullptr) {
		Builder = new llvm::IRBuilder<>(*TheContext);
	}
	else {
		Builder = b;
	}

	//std::cout<<"LLVM Version : " << LLVM_VERSION_STRING << std::endl;

	this->strlenFunc = nullptr;
	this->mallocFunc = nullptr;
	this->strcpyFunc = nullptr;
	this->concatFunc = nullptr;
	this->printfFunc = nullptr;
	this->scanfFunc = nullptr;

	printfFunc = createPrintfDeclaration(TheModule, *TheContext);
	std::vector<llvm::Type*> printfArgs(1, llvm::Type::getInt8PtrTy(*TheContext));
	llvm::FunctionType* printfType = llvm::FunctionType::get(llvm::Type::getInt32Ty(*TheContext), printfArgs, true);
	TheModule->getOrInsertFunction("printf", printfType);

	scanfFunc = createScanfDeclaration(TheModule, *TheContext);
	std::vector<llvm::Type*> scanfArgs(1, llvm::Type::getInt8PtrTy(*TheContext));
	llvm::FunctionType* scanfType = llvm::FunctionType::get(llvm::Type::getInt32Ty(*TheContext), scanfArgs, true);
	TheModule->getOrInsertFunction("scanf", scanfType);

	this->last = nullptr;
	this->meta_context = nullptr;
	this->procedureSymbolTypeId = -1;
	this->typeSymbolTypeId = -1;
	this->varible_address = nullptr;
}

llvm::Function* IREmiterVisitor::generateStringConcatenation()
{
	// Define the function type
	llvm::FunctionType* concatFuncType = llvm::FunctionType::get(
		Builder->getInt8PtrTy(),  // Return type: Pointer to string
		{ Builder->getInt8PtrTy(), Builder->getInt8PtrTy() }, // Arguments: Two pointers to strings
		false // Not a varargs function
	);

	// Create the function
	llvm::Function* concatFunction = llvm::Function::Create(
		concatFuncType, llvm::Function::ExternalLinkage, "concatStrings", *TheModule
	);

	// Define the function body
	llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(*TheContext, "entry", concatFunction);
	Builder->SetInsertPoint(entryBlock);

	// Load the two string pointers from arguments
	llvm::Value* str1 = concatFunction->arg_begin();
	llvm::Value* str2 = (concatFunction->arg_begin()) + 1;

	// Call the strlen function to get the lengths of the strings
	llvm::Value* len1 = Builder->CreateCall(strlenFunc, { str1 });
	llvm::Value* len2 = Builder->CreateCall(strlenFunc, { str2 });

	// Allocate memory for the concatenated string
	llvm::Value* totalLen = Builder->CreateAdd(len1, len2);
	llvm::Value* concatenated = Builder->CreateCall(mallocFunc, { totalLen });

	// Copy the first string  
	Builder->CreateCall(strcpyFunc, { concatenated, str1 });

	// Calculate the pointer to the next position in the concatenated string
	llvm::Value* ptrToNext = Builder->CreateGEP(Builder->getPtrTy(), concatenated, len1);

	// Copy the second string
	Builder->CreateCall(strcpyFunc, { ptrToNext, str2 });

	// Return the concatenated string
	Builder->CreateRet(concatenated);

	return concatFunction;
}

llvm::Function* IREmiterVisitor::addExternalProcedure(std::string name, const std::vector<std::pair<IType*, String>>& args, IType* ret) {
	std::vector<llvm::Type*> args_ty;
	for (const auto& i : args) {
		args_ty.push_back( this->getLLVMType(i.first));
	}

	llvm::FunctionType* functionType = llvm::FunctionType::get(
		ret ? this->getLLVMType(ret) : Builder->getVoidTy(), // Replace with the actual return type.
		llvm::ArrayRef<llvm::Type*>(args_ty.data(), args_ty.size()),
		/*isVarArg=*/false
	);

	llvm::Function* function = llvm::Function::Create(
		functionType,
		llvm::Function::ExternalLinkage,
		llvm::Twine(name),
		TheModule
	);
	return function;
}

void IREmiterVisitor::compile(const Preambule& code, CompilerInterface* context)
{
	this->meta_context = context;
	if (code.ast == nullptr) return;
	Procedure* p = (Procedure*)((StoredASTSumType*)code.ast)->ptr;
	varibles.clear();
	varibles.push_back({});

	auto symbol = context->findSymbol(context->context, procedureSymbolTypeId, p->name.val.c_str());
	llvm::Function* function = TheModule->getFunction(p->name.val.c_str());
	llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(TheModule->getContext(), "entry", function);
	Builder->SetInsertPoint(entryBlock);
	//auto function = ((llvm::Function*)((SymbolInfo_Procedure*)symbol.data)->bonusData);

	auto it = function->arg_begin();
	for (int i = 0; i < function->arg_size(); i++) {
		auto arg = &*(it);
		llvm::AllocaInst* argAlloca = this->Builder->CreateAlloca(arg->getType(), nullptr, arg->getName());

		// Store the argument value in the allocated space
		this->Builder->CreateStore(arg, argAlloca);
		//FIXME me complex types ?? 

		varibles[0].emplace(p->args[i].second.val, VariblesEntry{ p->args[i].second , p->args[i].first , argAlloca });
		// std::make_pair(p->args[i].second.val, std::make_pair(argAlloca, p->args[i].first)));
		it++;
	}

	/*
	  unsigned Idx = 0;
	  for (auto &Arg : F->args())
		Arg.setName(Args[Idx++]);
	*/
	// Emit code for the procedure.
	for (const auto& stmt : p->code) {

		stmt->visit(this);
	}

	if (lastReturn == false) {
		Builder->CreateRetVoid();
	}

	function->setDoesNotThrow();


	// Return a value based on the procedure's return type.
	//auto returnValue = nullptr; // Replace with the actual return value.
	//Builder->CreateRet(returnValue);
	llvm::verifyFunction(*function);

	auto p_data = (SymbolInfo_Procedure*)symbol.data;
	p_data->llvm_func = function;
	context->defineSymbolByID(context->context, procedureSymbolTypeId, symbol.id, symbol.data, symbol.data_size);

}

llvm::Type* IREmiterVisitor::newType(const Preambule& code, CompilerInterface* context)
{
	StructType* st = ((StructType*)(((StoredASTSumType*)code.ast)->ptr));
	return st->getLLVMType(this);
}

llvm::Value* IREmiterVisitor::convertCharToCharStr(llvm::Value* var_char)
{
	llvm::Value* charBuffer = Builder->CreateAlloca(Builder->getInt8Ty(), Builder->getInt32(2), "charBuffer");
	Builder->CreateStore(var_char, Builder->CreateConstGEP1_32(Builder->getInt8Ty(), charBuffer, 0), false);

	// Null-terminate the buffer
	Builder->CreateStore(Builder->getInt8(0), Builder->CreateConstGEP1_32(Builder->getInt8Ty(), charBuffer, 1), false);

	return charBuffer;
}

void IREmiterVisitor::finish()
{
	std::error_code EC;
	llvm::raw_fd_ostream file("output.ll", EC, llvm::sys::fs::OF_Text);
	TheModule->print(file, nullptr);
}

void IREmiterVisitor::visit(VaribleDeclaration* stmt)
{
	auto varible = this->Builder->CreateAlloca( this->getLLVMType(stmt->type), nullptr, stmt->varibleName.val);
	if (stmt->init != nullptr) {
		stmt->init->visit(this);
		auto val = this->last;
		this->Builder->CreateStore(val, varible);
	}
	varibles[varibles.size() - 1].emplace(stmt->varibleName.val, VariblesEntry{ stmt->varibleName,stmt->type->copy(),varible });
	last = nullptr;
}

llvm::Type* IREmiterVisitor::getLLVMType(IType* type)
{
	return type->getLLVMType(this);
}

llvm::Type* IREmiterVisitor::getLLVMType(TypeNameExpression* type)
{
	llvm::Type* ret_ty = Builder->getVoidTy();
	if (type->typeName.val == "Int") {
		ret_ty = Builder->getInt32Ty();
	}
	else if (type->typeName.val == "Float") {
		ret_ty = Builder->getFloatTy();
	}
	else if (type->typeName.val == "Double") {
		ret_ty = Builder->getDoubleTy();
	}
	else if (type->typeName.val == "String") {
		ret_ty = llvm::PointerType::get(Builder->getInt8Ty(), 0);
	}
	else if (type->typeName.val == "Char") {
		ret_ty = Builder->getInt8Ty();
	}
	else if (type->typeName.val == "Bool") {
		ret_ty = Builder->getInt1Ty();
	}
	else if (type->typeName.val == "Ptr") {
		ret_ty = Builder->getPtrTy();
	}
	else {
		auto symbol = meta_context->findSymbol(meta_context->context, typeSymbolTypeId,type->typeName.val.c_str());
		return ((IType*)symbol.data)->getLLVMType(this);
	}
	return ret_ty;
}

llvm::Type* IREmiterVisitor::getLLVMType(ArrayType* type)
{
	auto t = type->inner_type->getLLVMType(this);
	return llvm::VectorType::get(t, 0);
}

llvm::Type* IREmiterVisitor::getLLVMType(FunctionType* visitor)
{
	return nullptr;
}

llvm::Type* IREmiterVisitor::getLLVMType(PointerType* type)
{
	return type->inner_type->getLLVMType(this)->getPointerTo(0);
}

llvm::Type* IREmiterVisitor::getLLVMType(StructType* st)
{
	auto typeSymbolInfo = meta_context->findSymbolById(meta_context->context,typeSymbolTypeId,st->typeId );
	if (not typeSymbolInfo.found) {
		std::vector<llvm::Type*> v;
		for (const auto i : st->inner_type) {
			v.push_back(this->getLLVMType(i.first));
		}
		auto llvmType = llvm::StructType::create(v, st->name.val);
		st->llvmType = llvmType;
		return llvmType;
	}
	else {
		IType* t = (IType*)typeSymbolInfo.data;
		if(t->llvmType) return t->llvmType;
		else {
			std::vector<llvm::Type*> v;
			for (const auto i : st->inner_type) {
				v.push_back(this->getLLVMType(i.first));
			}
			auto llvmType = llvm::StructType::create(v, st->name.val);
			t->llvmType = llvmType;
			return llvmType;
		}
	}
}

VariblesEntry lookInTables(const VaribleTable& base, std::string varname) {
	for (size_t i = base.size(); i > 0; i--) {
		size_t i_ = i - 1;
		auto it = base[i_].find(varname);
		if (it != base[i_].end()) {
			return it->second;
		}
	}
	return VariblesEntry{};
}

void IREmiterVisitor::visit(VaribleExpression* stmt)
{
	if (checkField) {
		this->lastId = stmt->varibleName;
		return;
	}
	auto a = lookInTables(varibles, stmt->varibleName.val);
	if (a.found) {
		this->lastId = {};
		this->last = this->Builder->CreateLoad( this->getLLVMType(a.type), a.llvm_inst);
		this->varible_address = a.llvm_inst;
	}
	else {
		this->varible_address = nullptr;
		auto symbol = meta_context->findSymbol(meta_context->context, procedureSymbolTypeId, stmt->varibleName.val.c_str());
		if (symbol.found) {
			this->lastId = stmt->varibleName;
			this->last = nullptr;
		}
		else {
			assert(false && "Fix me add proper error with meta_context");
		}
	}
}

void IREmiterVisitor::visit(TypeNameExpression* stmt)
{
	lastType = stmt->typeName;
	last = nullptr;
}

int32_t stringToBool(std::string str) {
	if (str == "true") return 1;
	else if (str == "false") return 0;
	assert(false);
	return 0;
}

void IREmiterVisitor::visit(LiteralExpression* stmt)
{
	if (stmt->type->toString() == "Int") {
		last = llvm::ConstantInt::get(*TheContext, llvm::APInt(32, std::stoi(stmt->value.val)));
	}
	else if (stmt->type->toString() == "Float") {
		last = llvm::ConstantFP::get(*TheContext, llvm::APFloat(std::stod(stmt->value.val)));
	}
	else if (stmt->type->toString() == "Char") {
		last = llvm::ConstantInt::get(*TheContext, llvm::APInt(8, stmt->value.val[0]));
	}
	else if (stmt->type->toString() == "Bool") {
		last = llvm::ConstantInt::get(*TheContext, llvm::APInt(1, stringToBool(stmt->value.val)));
	}
	else if (stmt->type->toString() == "String") {
		last = Builder->CreateGlobalStringPtr(stmt->value.val);
	}
	else {
		last = nullptr;
	}
	varible_address = nullptr;
}

void IREmiterVisitor::visit(PrefixOperatorExpression* stmt)
{
	if (stmt->op.val == "not") {
		stmt->right->visit(this);
		auto right = this->last;
		this->last = Builder->CreateXor(right, llvm::ConstantInt::get(right->getType(), 1));
	}
	else if (stmt->op.val == "ref") {
		stmt->right->visit(this);
		this->last = varible_address;
	}
	else if (stmt->op.val == "-") {
		stmt->right->visit(this);
		auto right = this->last;
		this->last = Builder->CreateNeg(right);
	}
	else {
		assert(false);
	}
	varible_address = nullptr;
}



void IREmiterVisitor::visit(BinaryOperatorExpression* exp)
{
	this->varible_address = nullptr;
	this->last = nullptr;
	this->lastId = {};
	if (exp->op.val == "+") {
		exp->left->visit(this);
		auto left = this->last;
		exp->right->visit(this);
		auto right = this->last;
		this->last = this->Builder->CreateAdd(left, right, "addtmp");
	}
	else if (exp->op.val == "-") {
		exp->left->visit(this);
		auto left = this->last;
		exp->right->visit(this);
		auto right = this->last;
		this->last = this->Builder->CreateSub(left, right, "subtmp");
	}
	else if (exp->op.val == "*") {
		exp->left->visit(this);
		auto left = this->last;
		exp->right->visit(this);
		auto right = this->last;
		this->last = this->Builder->CreateMul(left, right, "multmp");
	}
	else if (exp->op.val == "/") {
		exp->left->visit(this);
		auto left = this->last;
		exp->right->visit(this);
		auto right = this->last;
		this->last = this->Builder->CreateExactSDiv(left, right, "divtmp");
	}
	else if (exp->op.val == "%") {
		exp->left->visit(this);
		auto left = this->last;
		exp->right->visit(this);
		auto right = this->last;
		this->last = this->Builder->CreateSRem(left, right, "modulotemp");
	}
	else if (exp->op.val == "==") {
		exp->left->visit(this);
		auto left = this->last;
		exp->right->visit(this);
		auto right = this->last;
		this->last = this->Builder->CreateCmp(llvm::CmpInst::ICMP_EQ, left, right, "cmptmp");
	}
	else if (exp->op.val == "!=") {
		exp->left->visit(this);
		auto left = this->last;
		exp->right->visit(this);
		auto right = this->last;
		this->last = this->Builder->CreateCmp(llvm::CmpInst::ICMP_NE, left, right, "cmptmp");
	}
	else if (exp->op.val == ">") {
		exp->left->visit(this);
		auto left = this->last;
		exp->right->visit(this);
		auto right = this->last;
		this->last = this->Builder->CreateICmpSGT(left, right, "gttmp"); // Greater than
	}
	else if (exp->op.val == "<") {
		exp->left->visit(this);
		auto left = this->last;
		exp->right->visit(this);
		auto right = this->last;
		this->last = this->Builder->CreateICmpSLT(left, right, "lttmp"); // Less than
	}
	else if (exp->op.val == "<=") {
		exp->left->visit(this);
		auto left = this->last;
		exp->right->visit(this);
		auto right = this->last;
		this->last = this->Builder->CreateICmpSLE(left, right, "letmp"); // Less than or equal
	}
	else if (exp->op.val == ">=") {
		exp->left->visit(this);
		auto left = this->last;
		exp->right->visit(this);
		auto right = this->last;
		this->last = this->Builder->CreateICmpSGE(left, right, "getmp"); // Greater than or equal
	}
	else if (exp->op.val == "as") {
		exp->left->visit(this);
		auto left = this->last;
		exp->right->visit(this);
		IType* t = dynamic_cast<IType*>(exp->right.get());
		if (t != nullptr) {
			this->last = Builder->CreateBitOrPointerCast(left, this->getLLVMType(t), "casted_value");
		}
		else {
			assert(false);
		}
	}
	else if (exp->op.val == "|") {
		exp->left->visit(this);
		auto left = this->last;
		exp->right->visit(this);
		auto right = this->last;
		if (exp->left->type->toString() == "Char") {
			left = convertCharToCharStr(left);
		}
		if (exp->right->type->toString() == "Char") {
			right = convertCharToCharStr(right);
		}

		llvm::Value* concatenatedString = Builder->CreateCall(concatFunc, { left, right });
		this->last = concatenatedString;
	}
	else if (exp->op.val == "=") {
		exp->right->visit(this);
		auto right = this->last;
		exp->left->visit(this);
		auto left = this->last;

		this->last = this->Builder->CreateStore(right, varible_address);
	}
	else if (exp->op.val == "/=") {
		exp->right->visit(this);
		auto right = this->last;
		exp->left->visit(this);
		auto left = this->last;
		auto res = this->Builder->CreateExactSDiv(left, right, "divtmp");
		exp->left->visit(this);
		//if (left->getType()->isPointerTy()) {
		this->last = this->Builder->CreateStore(res, this->varible_address);
		//}
		//else {
		//	assert(false);
		//}
	}
	else if (exp->op.val == "+=") {
		exp->right->visit(this);
		auto right = this->last;
		exp->left->visit(this);
		auto left = this->last;
		auto res = this->Builder->CreateAdd(left, right, "addtemp");
		exp->left->visit(this);
		this->last = this->Builder->CreateStore(right, this->varible_address);
	}
	else if (exp->op.val == ".") {
		//add namespaces
		//add subtitution for foo(a) = a.foo();
		exp->left->visit(this);
		auto left = this->varible_address;
		checkField = true;
		exp->right->visit(this);
		auto right = this->lastId;
		checkField = false;

		//assert(left->getType()->isStructTy() &&"only struct type add error");

		auto leftSymbolType = meta_context->findSymbol(meta_context->context,typeSymbolTypeId,exp->left->type->toString().c_str());

		StructType* st = dynamic_cast<StructType*>((IType*)leftSymbolType.data);
		if (st) {
			for (int i = 0;i<st->inner_type.size();i++) {
				if (st->inner_type[i].second.val == right.val) {
					auto fieldLLVMType = st->inner_type[i].first->getLLVMType(this);
					this->varible_address = Builder->CreateGEP(fieldLLVMType, left, { Builder->getInt32(i) });
					this->last = Builder->CreateLoad(fieldLLVMType, this->varible_address);
					return;
				}
			}
		}
	}
	else if (exp->op.val == "or") {
		// Visit the right expression
		exp->right->visit(this);
		auto rightValue = last; // Assuming lastId is of type llvm::Value

		// Visit the left expression
		exp->left->visit(this);
		auto leftValue = last; // Assuming lastId is of type llvm::Value

		// Perform logical OR operation
		auto result = Builder->CreateLogicalOr(leftValue, rightValue, "or_result");

		// Update lastId
		last = result;
	}
	else if (exp->op.val == "and") {
		// Visit the right expression
		exp->right->visit(this);
		auto rightValue = last; // Assuming lastId is of type llvm::Value

		// Visit the left expression
		exp->left->visit(this);
		auto leftValue = last; // Assuming lastId is of type llvm::Value

		// Perform logical OR operation
		auto result = Builder->CreateLogicalAnd(leftValue, rightValue, "and_result");

		// Update lastId
		last = result;
	}
	else {
		last = nullptr;
		assert(false);
	}
	varible_address = nullptr;
}

void IREmiterVisitor::visit(PostfixOperatorExpression* stmt)
{
	if (stmt->op.val == "++") {
		stmt->left->visit(this);
		auto val = Builder->CreateAdd(this->last, Builder->getInt32(1));
		Builder->CreateStore(val, this->varible_address);
	}
	else if (stmt->op.val == "--") {
		stmt->left->visit(this);
		auto val = Builder->CreateSub(this->last, Builder->getInt32(1));
		Builder->CreateStore(val, this->varible_address);
	}
	last = nullptr;
	//assert(false);
}

void IREmiterVisitor::visit(ProcedureCallExpression* stmt)
{
	last = nullptr;
	stmt->function->visit(this);

	auto symbol = meta_context->findSymbol(meta_context->context, procedureSymbolTypeId, lastId.val.c_str());
	if (lastId.val == "printf") {

		std::vector<llvm::Value*> printfArgs;

		for (const auto& i : stmt->args) {
			i->visit(this);
			printfArgs.push_back(this->last);
		}

		Builder->CreateCall(printfFunc, printfArgs);
	}
	else if (lastId.val == "parseInt") {

		auto ftm = Builder->CreateGlobalStringPtr("%d\00");

		std::vector<llvm::Value*> scanfArgsCall;
		scanfArgsCall.push_back(ftm);
		stmt->args[0]->visit(this);
		scanfArgsCall.push_back(this->last);

		this->last = Builder->CreateCall(scanfFunc, scanfArgsCall);

	}
	else if (symbol.found) {

		std::vector<llvm::Value*> llvmArgs;

		for (const auto& i : stmt->args) {
			i->visit(this);
			llvmArgs.push_back(this->last);
		}

		SymbolInfo_Procedure* p = (SymbolInfo_Procedure*)symbol.data;
		llvm::Function* llvm_p = (llvm::Function*)p->llvm_func;
		assert(llvm_p);
		this->last = Builder->CreateCall(llvm_p, llvmArgs);
	}
	else {
		assert(false && "FIX Me add proper error info with meta_context");
	}
	varible_address = nullptr;
}

void IREmiterVisitor::visit(ArrayAcceseExpression* stmt)
{
	stmt->array->visit(this);
	auto arrayPtr = this->last;

	stmt->args->visit(this);
	auto index = this->last;

	// Assuming the array elements are of integer type (adjust as needed)
	auto elementType = this->getLLVMType(stmt->type);

	// Create a GEP (GetElementPtr) instruction to calculate the address of the element
	//std::vector<llvm::Value*> indices = { llvm::ConstantInt::get(Type::getInt32Ty(Context), 0), index };
	auto elementPtr = Builder->CreateGEP(elementType, arrayPtr, index);

	// Load the value from the calculated address
	this->last = Builder->CreateLoad(elementType, elementPtr);
	varible_address = elementPtr;
}
void IREmiterVisitor::visit(IfStatement* stmt)
{
	stmt->condition->visit(this);

	llvm::Function* function = Builder->GetInsertBlock()->getParent();
	llvm::BasicBlock* ifBlock = llvm::BasicBlock::Create(*TheContext, "if", function);
	llvm::BasicBlock* mergeBlock;
	auto cond = this->last;
	if (stmt->ifFalse != nullptr) {
		llvm::BasicBlock* elseBlock = llvm::BasicBlock::Create(*TheContext, "if.else", function);
		Builder->CreateCondBr(cond, ifBlock, elseBlock);
		Builder->SetInsertPoint(ifBlock);
		stmt->ifTrue->visit(this); // Generate IR for the 'if' body
		mergeBlock = llvm::BasicBlock::Create(*TheContext, "if.end", function);
		Builder->CreateBr(mergeBlock); // Jump to merge after the 'if' body
		Builder->SetInsertPoint(elseBlock);
		stmt->ifFalse->visit(this); // Generate IR for the 'else' body
		Builder->CreateBr(mergeBlock);
	}
	else {
		mergeBlock = llvm::BasicBlock::Create(*TheContext, "if.end", function);
		Builder->CreateCondBr(cond, ifBlock, mergeBlock);
		Builder->SetInsertPoint(ifBlock);
		stmt->ifTrue->visit(this); // Generate IR for the 'if' body
		Builder->CreateBr(mergeBlock);
	}
	Builder->SetInsertPoint(mergeBlock);
	last = nullptr;
	lastReturn = false;
	varible_address = nullptr;
}

void IREmiterVisitor::visit(WhileStatement* stmt)
{

	llvm::Function* function = Builder->GetInsertBlock()->getParent();

	// Create the blocks for the 'while' loop
	llvm::BasicBlock* conditionBlock = llvm::BasicBlock::Create(*TheContext, "while.cond", function);
	llvm::BasicBlock* loopBlock = llvm::BasicBlock::Create(*TheContext, "while.body", function);
	llvm::BasicBlock* afterLoopBlock = llvm::BasicBlock::Create(*TheContext, "while.end", function);

	// Set up the jump to the condition block
	Builder->CreateBr(conditionBlock);
	Builder->SetInsertPoint(conditionBlock);

	// Generate the condition
	stmt->condition->visit(this);
	llvm::Value* condValue = this->last;

	// Create the conditional branch
	Builder->CreateCondBr(condValue, loopBlock, afterLoopBlock);

	// Emit code for the loop block
	//function->getBasicBlockList().push_back(loopBlock);
	Builder->SetInsertPoint(loopBlock);
	stmt->body->visit(this); // Generate IR for the loop body
	Builder->CreateBr(conditionBlock); // Jump back to the condition check

	// Set insertion point for the block after the loop
	//function->getBasicBlockList().push_back(afterLoopBlock);
	Builder->SetInsertPoint(afterLoopBlock);
	last = nullptr;
	lastReturn = false;
	varible_address = nullptr;
}

void IREmiterVisitor::visit(ReturnStatement* stmt)
{
	stmt->exp->visit(this);
	Builder->CreateRet(last);
	last = nullptr;
	lastReturn = true;
	varible_address = nullptr;
}

void IREmiterVisitor::visit(ExpressionStatement* stmt)
{
	stmt->exp->visit(this);
	last = nullptr;
	lastReturn = false;
	varible_address = nullptr;
}

void IREmiterVisitor::visit(BlockStatement* stmt)
{
	varibles.push_back({});
	for (const auto& i : stmt->body) {
		i->visit(this);
	}
	varibles.pop_back();
	last = nullptr;
	lastReturn = false;
	varible_address = nullptr;
}


