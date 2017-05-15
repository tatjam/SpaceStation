#pragma once

#include <stdarg.h>

extern "C"
{
#include "../include/squirrel.h"
#include "../include/sqstdio.h"
#include "../include/sqstdmath.h"
#include "../include/sqstdstring.h"
#include "../include/sqstdaux.h"
}

#ifdef SQUNICODE

#define scvprintf vfwprintf
#else

#define scvprintf vfprintf
#endif

#include <vector>
#include <unordered_map>

#define BASE_STACK_SIZE 512


// TODO: EXTEND THIS PLEASE! 




struct sqArgument
{
	SQObjectType type;
	
	int iVal;
	float fVal;
	std::string sVal;
};



class sqArgList
{
public:

	std::vector<sqArgument> args;

};

typedef sqArgument(*sqFunction)(sqArgList);


class sqUtil
{
public:
	
	static float getFloat(sqArgument arg);
	static int getInt(sqArgument arg);
	static std::string getString(sqArgument arg);
	static bool getBool(sqArgument arg);

};

SQInteger sqReadFileChar(SQUserPointer file);

void sqCompileError(HSQUIRRELVM v, const SQChar* desc, const SQChar* source, SQInteger line, SQInteger column);

void printfunc(HSQUIRRELVM v, const SQChar *s, ...);

void errorfunc(HSQUIRRELVM v, const SQChar *s, ...);



class sqScript
{

public:

	HSQUIRRELVM vm;

	// Registers a function
	void createFunction(sqFunction fun);

	// Calls function by name, feed arguments
	sqArgument call(std::string name, sqArgList args);

	// Runs script root
	void run();

	// Loads AND COMPILES script.
	// RUNS ROOT! Returns true if success
	// and false if failure
	bool load(char* path, bool registerlib = true);


	sqScript();
	~sqScript();
};

