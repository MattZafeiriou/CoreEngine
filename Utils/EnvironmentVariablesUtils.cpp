#include <cstdlib>
#include <iostream>
#include <string>

static const char* getEnvironmentVariable(const char* name);

static void setEnvironmentVariable(const char* name, const char* value)
{
    if (_putenv_s(name, value) != 0)
    {
        std::cout << "Failed to set " << name << " environment variable" << std::endl;
        return;
    }
    bool DEBUG = std::stoi(getEnvironmentVariable("CORE_DEBUG"));
    if (DEBUG)
        std::cout << name << "=" << value << std::endl;
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
    if (DEBUG)
        std::cout << "Setting environment variables:" << std::endl;
    setEnvironmentVariable("CORE_DEBUG", DEBUG ? "1" : "0");
    setEnvironmentVariable("CORE_NAME", "CoreEngine");
    setEnvironmentVariable("CORE_VERSION", version);
    setEnvironmentVariable("CORE_ASSETS_PATH", (std::string(getEnvironmentVariable("USERPROFILE")) + std::string("\\Documents\\" + std::string(getEnvironmentVariable("CORE_NAME")) + "\\Assets\\")).c_str());
    setEnvironmentVariable("CORE_FILE_PATH", (std::string(getEnvironmentVariable("USERPROFILE")) + std::string("\\Documents\\" + std::string(getEnvironmentVariable("CORE_NAME")) + "\\")).c_str());
}