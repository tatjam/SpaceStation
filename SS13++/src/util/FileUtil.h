#pragma once
#include <string>

#ifdef _WIN32

#include <Windows.h>

#endif


// The path to the location of our executable
extern std::string futil_exePath;

// The path from which the program was launched
extern std::string futil_runPath;

struct FileUtil
{
	// Converts '/' to '\'	
	static std::string windowPath(std::string path)
	{

	}

	// Converts '\' to '/'
	static std::string normalPath(std::string path)
	{

	}
	
	// Platform dependant
	static std::string autoPath(std::string path)
	{

	}

	// Windows only for now :(
	// TODO
	static std::string stripFilename(std::string path)
	{
		std::string::size_type pos = path.find_last_of("\\/");
		return path.substr(0, pos);
	}

	// TODO OSX and Linux
	static std::string getExecutablePath(bool executePath = false)
	{

		std::string out;

#ifdef _WIN32
		if (!executePath)
		{
			TCHAR pbuf[MAX_PATH];
			GetModuleFileName(NULL, pbuf, MAX_PATH);
			for (int i = 0; i < MAX_PATH; i++)
			{
				if (pbuf[i] == '\0')
				{
					break;
				}

				out += pbuf[i];
			}
		}
		else
		{
			TCHAR pbuf[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, pbuf);
			for (int i = 0; i < MAX_PATH; i++)
			{
				if (pbuf[i] == '\0')
				{
					break;
				}

				out += pbuf[i];
			}
		}

#endif

#ifdef _linux_
		// TODO we need to write this t-t
		return "SAD!";
#endif

		return out;
	}
	
};