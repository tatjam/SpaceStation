#include "SquirrelScript.h"



void printfunc(HSQUIRRELVM v, const SQChar *s, ...)
{
	va_list vl;
	va_start(vl, s);
	scvprintf(stdout, s, vl);
	va_end(vl);
}

void errorfunc(HSQUIRRELVM v, const SQChar *s, ...)
{
	va_list vl;
	va_start(vl, s);
	scvprintf(stderr, s, vl);
	va_end(vl);
}


SQInteger sqReadFileChar(SQUserPointer file) 
{
	int ret;
	char c;
	if ((ret = fread(&c, sizeof(c), 1, (FILE *)file)>0))
		return c;
	return 0;
}


void sqCompileError(HSQUIRRELVM v, const SQChar* desc, const SQChar* source, SQInteger line, SQInteger column)
{
	printf("Failure while reading file (%s)\n", source);
	printf("(%i:%i): %s\n", line, column, desc);
}


sqArgument sqScript::call(std::string name, sqArgList args)
{
	SQInteger top = sq_gettop(vm);
	sq_pushroottable(vm);
	sq_pushstring(vm, name.c_str(), -1);
	sq_get(vm, -2);
	sq_pushroottable(vm);
	// Args
	for (int i = 0; i < args.args.size(); i++)
	{
		sqArgument* n = &args.args[i];
		if (n->type == OT_NULL)
		{
			sq_pushnull(vm);
		}
		else if (n->type == OT_BOOL)
		{
			sq_pushbool(vm, (bool)n->iVal);
		}
		else if (n->type == OT_FLOAT)
		{
			sq_pushfloat(vm, n->fVal);
		}
		else if (n->type == OT_INTEGER)
		{
			sq_pushinteger(vm, n->iVal);
		}
		else if (n->type == OT_STRING)
		{
			sq_pushstring(vm, n->sVal.c_str(), -1);
		}
		else
		{
			sq_pushnull(vm);
		}
	}

	sq_call(vm, args.args.size() + 1, SQFalse, SQTrue);
	sq_settop(vm, top); //restores the original stack size

	return sqArgument();

}

void sqScript::run()
{
}


bool sqScript::load(char* path, bool registerlib)
{

	if (registerlib)
	{
		sq_pushroottable(vm);
		sqstd_register_mathlib(vm);
		sqstd_register_stringlib(vm);
		sq_pop(vm, 1);

		sq_setprintfunc(vm, printfunc, errorfunc); //sets the print function

	}

	sqstd_seterrorhandlers(vm); //registers the default error handlers


	sq_pushroottable(vm);

	if (SQ_SUCCEEDED(sqstd_dofile(vm, _SC(path), SQFalse, SQTrue)))
	{
		return true;
	}
	return false;
}

sqScript::sqScript()
{
	vm = sq_open(BASE_STACK_SIZE);
	sqstd_seterrorhandlers(vm); //registers the default error handlers

}


sqScript::~sqScript()
{
}
