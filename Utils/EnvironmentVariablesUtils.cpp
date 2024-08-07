#include <cstdlib>
#include <iostream>
#include <string>

static void setEnvironmentVariable(const char* name, const char* value)
{
    if (_putenv_s(name, value) != 0)
    {
        std::cout << "Failed to set " << name << " environment variable" << std::endl;
    }
}

static const char* getEnvironmentVariable(const char* name)
{
    char* envValue = nullptr;
	size_t len;
	_dupenv_s(&envValue, &len, name);
	return envValue;
}

/*
 This function sets the environment variables for the engine
 arguments:
     version - the version of the engine, e.g. "0.1.2"
     DEBUG - 1 if the engine is in debug mode, 0 otherwise
 */
static void setEnvironmentVariables(const char* version, int DEBUG=0)
{
    setEnvironmentVariable("CORE_NAME", "CoreEngine");
    setEnvironmentVariable("CORE_VERSION", version);
    setEnvironmentVariable("CORE_DEBUG", DEBUG ? "1" : "0");
    setEnvironmentVariable("CORE_FILE_PATH", (std::string(getEnvironmentVariable("USERPROFILE")) + std::string("\\Documents\\CoreEngine\\")).c_str());
    if (DEBUG)
    {
        std::cout << "Setting environment variables:" << std::endl;
        std::cout << "CORE_NAME=" << getEnvironmentVariable("CORE_NAME") << std::endl;
        std::cout << "CORE_VERSION=" << getEnvironmentVariable("CORE_VERSION") << std::endl;
        std::cout << "CORE_DEBUG=" << getEnvironmentVariable("CORE_DEBUG") << std::endl;
        std::cout << "CORE_FILE_PATH=" << getEnvironmentVariable("CORE_FILE_PATH") << std::endl;
    }
}