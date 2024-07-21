#include <cstdlib>
#include <iostream>

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

static void setEnvironmentVariables(int DEBUG=0)
{
    setEnvironmentVariable("CORE_NAME", "CoreEngine");
    setEnvironmentVariable("CORE_VERSION", "0.1.1");
    setEnvironmentVariable("CORE_DEBUG", DEBUG ? "1" : "0");
}