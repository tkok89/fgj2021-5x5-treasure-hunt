#include "workingdir.h"

#include <stdio.h>
#ifdef _WIN32
	#include <direct.h>
#else
	#include <unistd.h>
#endif

char path[FILENAME_MAX];

std::string getWorkingDir()
{
	#ifdef _WIN32
		_getcwd(path, sizeof(path));
	#else
		getcwd(path, sizeof(path));
	#endif

	return std::string(path);
}
