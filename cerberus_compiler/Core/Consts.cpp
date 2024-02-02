#include "pch.h"
#include "Consts.h"

const std::string seperators = "+-*/%|!=<>.,;:";
const std::string parentheses = "(){}[]";

const std::vector<std::string> keywords = {
	"return",
	"true",
	"false",
	"Let",
	"copy",
	"move",
	"if",
	"else",
	"while",
	"..."
};

std::vector<Operator_t> operators = {
	{".",1,true,false,true,false},
	{"->",2,true,false,true,false},
	//{"__operator_fcall",2,true},
	//{"__operator_subscript",2,true},
	{"as",3,true,false,true,false}, // cast
	{"ref",3,true,true,false,false},
	{"++",3,true,true,false,true},
	{"--",3,true,true,false,true},
	{"not",3,false,true,false,true},
	{"*",4,true,false,true,false},
	{"/",4,true,false,true,false},
	{"%",4,true,false,true,false},
	{"+",5,true,true,true,false},
	{"-",5,true,true,true,false},
	{"|",5,true,false,true,false},
	{"<=>",6,true,false,true,false},
	{">=",7,true,false,true,false},
	{"<=",7,true,false,true,false},
	{"<",7,true,false,true,false},
	{">",7,true,false,true,false},
	{"==",8,true,false,true,false},
	{"!=",8,true,false,true,false},
	{"and",9,true,false,true,false},
	{"or",10,true,false,true,false},
	{"xor",10,true,false,true,false},
	{"+=",11,false,false,true,false},
	{"-=",11,false,false,true,false},
	{"*=",11,false,false,true,false},
	{"/=",11,false,false,true,false},
	{"%=",11,false,false,true,false},
	{"|=",11,false,false,true,false},
	{"=",11,false,false,true,false},
	{",",12,true,false,true,false},
	{";",12,true,false,false,true},
	{":",12,true,false,true,false},
};

std::vector<CompilerDefinedOperators> operatorsResults = {
	CompilerDefinedOperators(">","Int","Int","Bool"),
	CompilerDefinedOperators("<","Int","Int","Bool"),
	CompilerDefinedOperators("==","Int","Int","Bool"),
	CompilerDefinedOperators("!=","Int","Int","Bool"),
	CompilerDefinedOperators(">=","Int","Int","Bool"),
	CompilerDefinedOperators("<=","Int","Int","Bool"),

	CompilerDefinedOperators(">","Float","Float","Bool"),
	CompilerDefinedOperators("<","Float","Float","Bool"),
	CompilerDefinedOperators("==","Float","Float","Bool"),
	CompilerDefinedOperators("!=","Float","Float","Bool"),
	CompilerDefinedOperators(">=","Float","Float","Bool"),
	CompilerDefinedOperators("<=","Float","Float","Bool"),

	CompilerDefinedOperators("-","Char","Char","Int"),
	CompilerDefinedOperators("+","Char","Char","Int"),

	CompilerDefinedOperators(">","Char","Char","Bool"),
	CompilerDefinedOperators("<","Char","Char","Bool"),
	CompilerDefinedOperators("==","Char","Char","Bool"),
	CompilerDefinedOperators("!=","Char","Char","Bool"),
	CompilerDefinedOperators(">=","Char","Char","Bool"),
	CompilerDefinedOperators("<=","Char","Char","Bool"),

	CompilerDefinedOperators("|","Char","String","String"),
	CompilerDefinedOperators("|","Char","Char","String"),
	CompilerDefinedOperators("|","String","Char","String")
};
//unary operators 
// [] array -> element

//std::vector<std::string> hidenOperators = {
//	"__operator_fcall",
//	"__operator_subscript",
//};

std::vector<std::string> prefixOperators = {
	"++",
	"--",
	"not",
	"ref",
	"+",
	"-",
};
std::vector<std::string> infixOperators = {
	"==",
	"!=",
	">=",
	"<=",
	"->",
	"+=",
	"-=",
	"*=",
	"/=",
	"%=",
	"|=",
	"or",
	"<=>",
	"and",
	"xor",
	"as",
	"+",
	"-",
	"*",
	"/",
	"%",
	"|",
	"=",
	"<",
	">",
	".",
	","
};
std::vector<std::string> sufixOperators = {
	"++",
	"--",
};