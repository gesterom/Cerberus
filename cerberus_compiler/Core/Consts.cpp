#include "pch.h"
#include "Consts.h"

const std::string seperators = "+-*/%|!=<>.,;:";
const std::string parentheses = "(){}[]";

const std::vector<std::string> keywords = {
	"return",
	"Let",
	"copy",
	"move",
	"ref",
	"if",
	"else",
	"while",
	"..."
};

std::vector<Operator_t> operators = {
	{".",1,true},
	{"->",2,true},
	//{"__operator_fcall",2,true},
	//{"__operator_subscript",2,true},
	{"as",3,true}, // cast
	{"++",3,true},
	{"--",3,true},
	{"not",3,false},
	{"*",4,true},
	{"/",4,true},
	{"%",4,true},
	{"+",5,true},
	{"-",5,true},
	{"|",5,true},
	{"<=>",6,true},
	{">=",7,true},
	{"<=",7,true},
	{"<",7,true},
	{">",7,true},
	{"==",8,true},
	{"!=",8,true},
	{"and",9,true},
	{"or",10,true},
	{"xor",10,true},
	{"+=",11,false},
	{"-=",11,false},
	{"*=",11,false},
	{"/=",11,false},
	{"%=",11,false},
	{"|=",11,false},
	{"=",11,false},
	{",",12,true},
	{";",12,true},
	{":",12,true},
};

//std::vector<std::string> hidenOperators = {
//	"__operator_fcall",
//	"__operator_subscript",
//};

std::vector<std::string> prefixOperators = {
	"++",
	"--",
	"not",
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