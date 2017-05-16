#pragma once

#include <stdarg.h>

extern "C"
{
#include "../dep/squirrel/include/squirrel.h"
#include "../dep/squirrel/include/sqstdio.h"
#include "../dep/squirrel/include/sqstdmath.h"
#include "../dep/squirrel/include/sqstdstring.h"
#include "../dep/squirrel/include/sqstdaux.h"
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

#define sqNoArgument sqArgument();

struct sqArgument
{
	bool empty = true;
	SQObjectType type = OT_NULL;
	
	int iVal;
	float fVal;
	std::string sVal;

	sqArgument() { empty = true; }
	sqArgument(SQObjectType type) { this->type = type; empty = false; }
	sqArgument(SQObjectType type, int iVal, float fVal, std::string sVal)
	{
		this->type = type; this->iVal = iVal; this->fVal = fVal, this->sVal = sVal; empty = false;
	};
};



class sqArgList
{
public:

	std::vector<sqArgument> args;

};

typedef SQInteger(*sqFunction)(HSQUIRRELVM);


SQInteger sqReadFileChar(SQUserPointer file);

void sqCompileError(HSQUIRRELVM v, const SQChar* desc, const SQChar* source, SQInteger line, SQInteger column);

void printfunc(HSQUIRRELVM v, const SQChar *s, ...);

void errorfunc(HSQUIRRELVM v, const SQChar *s, ...);


// This is messy, but the system is not gonna be
// threaded so this CAN be done. Could be changed!

class sqScript
{

public:

	HSQUIRRELVM vm;

	// Registers a function
	void regFunction(sqFunction fun, std::string fname);

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

