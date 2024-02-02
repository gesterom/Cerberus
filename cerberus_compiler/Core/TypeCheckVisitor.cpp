#include "pch.h"
#include "TypeCheckVisitor.h"
#include "Consts.h"
#include <cassert>

void produceError_Type(CompilerInterface* context, BinaryOperatorExpression* exp) {
	std::string msg = std::string() + "Incorect Types of operator " + exp->toString() + "\n Get : \nleft ->  " + exp->left->type->toString() + "\nright -> " + exp->right->type->toString() + "\n";
	context->error_msg(context->context,
		ErrorType::SyntaxError,
		exp->pos,
		msg.c_str()
	);
}

void produceError_UnknownVarible(CompilerInterface* context, String varname) {
	std::string msg = std::string() + "Unknown varible " + varname.val + "\n";
	context->error_msg(context->context,
		ErrorType::UnknownVarible,
		varname.pos,
		msg.c_str()
	);
}

void produceError_UnknownProcedure(CompilerInterface* context, String varname) {
	std::string msg = std::string() + "Unknown procedure " + varname.val + "\n";
	context->error_msg(context->context,
		ErrorType::UnknownProcedure,
		varname.pos,
		msg.c_str()
	);
}

void produceError_ArrayAcceseOnNonArrayType(CompilerInterface* context, std::string type, Position pos) {
	std::string msg = std::string() + "Incorect type for array accese : " + type;
	context->error_msg(context->context,
		ErrorType::MultipleDeclaration,
		pos,
		msg.c_str()
	);
}


void produceError_MultipleDeclarationVarible(CompilerInterface* context, String varname, Position prev) {
	std::string msg = std::string() + "Multiple declaration of varible " + varname.val + "\n Previus declaration: " + prev.filename + " " + std::to_string(prev.line) + ":" + std::to_string(prev.character);
	context->error_msg(context->context,
		ErrorType::MultipleDeclaration,
		varname.pos,
		msg.c_str()
	);
}

void produceError_EmptyLet(CompilerInterface* context, String varname) {
	std::string msg = std::string() + "Using Let without initialization of varible " + varname.val;
	context->error_msg(context->context,
		ErrorType::Other,
		varname.pos,
		msg.c_str()
	);
}

VariblesEntry lookInTables(const VaribleTable& base, String varname) {

	for (size_t oi = base.size(); oi > 0; oi--) {
		auto i = oi - 1;
		auto it = base[i].find(varname.val);
		if (it != base[i].end()) {
			return it->second;
		}
	}
	return VariblesEntry{};
}

void TypeCheckVisitor::check(const Preambule& code, CompilerInterface* context)
{
	this->meta_context = context;
	if (code.ast == nullptr) return;
	Procedure* p = (Procedure*)((StoredASTSumType*)code.ast)->ptr;
	varibles.clear();
	varibles.push_back({});

	for (const auto& i : p->args) {
		varibles[varibles.size() - 1].emplace(std::make_pair(i.second.val, VariblesEntry{ i.second,i.first->copy(),nullptr }));
	}

	for (const auto& i : p->code) {
		i->visit(this);
	}
}

void TypeCheckVisitor::visit(VaribleExpression* stmt)
{
	if (checkField) {
		this->lastId = stmt->varibleName;
		return;
	}

	auto a = lookInTables(varibles, stmt->varibleName);
	if (a.found) {
		stmt->type = a.type;
	}
	else {
		auto info = meta_context->findSymbol(meta_context->context, procedureSymbolTypeId, stmt->varibleName.val.c_str());
		if (info.found) {
			stmt->type = new FunctionType(info.symbolName);
			this->lastId = stmt->varibleName;
		}
		else {
			produceError_UnknownVarible(meta_context, stmt->varibleName);
		}
	}
}

void TypeCheckVisitor::visit(TypeNameExpression* stmt)
{
	lastType = stmt;
}

void TypeCheckVisitor::visit(LiteralExpression*)
{
}

void TypeCheckVisitor::visit(PrefixOperatorExpression* stmt)
{
	stmt->right->visit(this);
	if (stmt->op.val == "ref") {
		stmt->type = stmt->right->type;
	}
	else {
		stmt->type = stmt->right->type;
	}
}

void TypeCheckVisitor::visit(BinaryOperatorExpression* stmt)
{
	stmt->left->visit(this);
	if (stmt->op.val == "as") {
		if (stmt->right->type->toString() != "@Type") {
			produceError_Type(meta_context, stmt);
			stmt->type = new InternalType("Error");
		}
		else {
			stmt->right->visit(this);
			stmt->type = this->lastType->copy();
		};
	}
	else if (stmt->op.val == ".") {
		checkField = true;
		stmt->right->visit(this);
		std::string field = this->lastId.val;
		checkField = false;
		assert(field!="" && "add error for .(exp) ");
		auto symbol = meta_context->findSymbol(meta_context->context,typeSymbolTypeId,stmt->left->type->toString().c_str());
		assert(symbol.found);
		IType* symbolData = (IType*)symbol.data;
		StructType* st = dynamic_cast<StructType*>(symbolData);
		if (st) {
			for (const auto& i: st->inner_type) {
				if (i.second.val == field) {
					stmt->type = i.first;
					break;
				}
			}
		}
	}
	else {
		stmt->right->visit(this);
		bool flag = false;
		for (const auto& i : operatorsResults) {
			if (i.op == stmt->op.val and i.left == stmt->left->type->toString() and i.right == stmt->right->type->toString()) {
				flag = true;
				auto symbolTypeID =  meta_context->findSymbol(meta_context->context,typeSymbolTypeId,i.ret.c_str()).id;
				stmt->type = new TypeNameExpression(i.ret, symbolTypeID);
				break;
			}
		}
		if (not flag) {
			if (stmt->left->type->toString() == stmt->right->type->toString()) {
				stmt->type = stmt->left->type->copy();
			}
			else {
				produceError_Type(meta_context, stmt);
				stmt->type = new InternalType("Error");
			}
		}
	}
}

void TypeCheckVisitor::visit(PostfixOperatorExpression* stmt)
{
	stmt->left->visit(this);
	stmt->type = stmt->left->type->copy();
}

void TypeCheckVisitor::visit(ProcedureCallExpression* stmt)
{
	stmt->function->visit(this);
	auto func_name = this->lastId;
	//as symbol table about this function 
	SymbolInfo symbol = meta_context->findSymbol(meta_context->context,procedureSymbolTypeId,func_name.val.c_str());
	if (not symbol.found) {
		produceError_UnknownProcedure(this->meta_context,func_name);
		return;
	}
	SymbolInfo_Procedure* data = (SymbolInfo_Procedure*)symbol.data;

	auto typeId = meta_context->getSymbolTypeInfo(meta_context->context,"type");
	stmt->type = (IType*)meta_context->findSymbolById(meta_context->context,typeId.id, data->return_type).data;

	for (const auto& i : stmt->args) {
		i->visit(this);
	}

	//stmt->type = data->return_type;
	//assert(false);
	//meta_context->findSymbol(meta_context->context,function,stmt->function);
}

void TypeCheckVisitor::visit(ArrayAcceseExpression* exp)
{
	exp->array->visit(this);
	exp->args->visit(this);

	ArrayType* arr = dynamic_cast<ArrayType*>(exp->array->type);
	PointerType* ptr = dynamic_cast<PointerType*>(exp->array->type);

	if (arr != nullptr) {
		// The cast is successful, and you can use derivedPtr.
		exp->type = arr->inner_type;
	}
	else if (ptr != nullptr) {
		exp->type = ptr->inner_type;
	}
	else if (exp->array->type->toString() == "String") {
		exp->type = new TypeNameExpression("Char");
	}
	else {
		produceError_ArrayAcceseOnNonArrayType(this->meta_context,exp->array->type->toString(),exp->array->pos);
		exp->type = new InternalType("Error");
	}
}

void TypeCheckVisitor::visit(IfStatement* stmt)
{
	stmt->condition->visit(this);
	stmt->ifTrue->visit(this);
	if (stmt->ifFalse)
		stmt->ifFalse->visit(this);
}

void TypeCheckVisitor::visit(WhileStatement* stmt)
{
	stmt->condition->visit(this);
	stmt->body->visit(this);
}

void TypeCheckVisitor::visit(ReturnStatement* stmt)
{
	stmt->exp->visit(this);
}

void TypeCheckVisitor::visit(ExpressionStatement* stmt)
{
	stmt->exp->visit(this);
}

void TypeCheckVisitor::visit(BlockStatement* stmt)
{
	varibles.push_back({});
	for (const auto& i : stmt->body) {
		i->visit(this);
	}
	varibles.pop_back();
}

void TypeCheckVisitor::visit(VaribleDeclaration* stmt)
{
	auto a = lookInTables(varibles, stmt->varibleName);
	if (a.found) {
		produceError_MultipleDeclarationVarible(meta_context, stmt->varibleName, a.pos);
	}
	else {
		if (stmt->type->toString() == "Let" and stmt->init) {
			stmt->init->visit(this);
			varibles[varibles.size() - 1].emplace(std::make_pair(stmt->varibleName.val, VariblesEntry{ stmt->varibleName,stmt->init->type->copy(),nullptr }));//
		}
		else if (stmt->type->toString() == "Let") {
			produceError_EmptyLet(meta_context, stmt->varibleName);
		}
		else {
			varibles[varibles.size() - 1].emplace(std::make_pair(stmt->varibleName.val, VariblesEntry{ stmt->varibleName,stmt->type->copy(),nullptr }));
			if (stmt->init) {
				stmt->init->visit(this);
				if (stmt->type->toString() != stmt->init->type->toString()) {
					std::string msg = std::string() + "Varible declaration have difrent type then initialization expression \nType: " + stmt->type->toString() + "\nExpression: " + stmt->init->type->toString() + "\n";
					meta_context->error_msg(meta_context->context, ErrorType::TypeSystemError, stmt->varibleName.pos, msg.c_str());
				}
			}
		}
	}
}

llvm::Type* TypeCheckVisitor::getLLVMType(IType* visitor)
{
	return nullptr;
}

llvm::Type* TypeCheckVisitor::getLLVMType(TypeNameExpression* visitor)
{
	return nullptr;
}

llvm::Type* TypeCheckVisitor::getLLVMType(ArrayType* visitor)
{
	return nullptr;
}

llvm::Type* TypeCheckVisitor::getLLVMType(FunctionType* visitor)
{
	return nullptr;
}

llvm::Type* TypeCheckVisitor::getLLVMType(PointerType* visitor)
{
	return nullptr;
}

llvm::Type* TypeCheckVisitor::getLLVMType(StructType* visitor)
{
	return nullptr;
}
