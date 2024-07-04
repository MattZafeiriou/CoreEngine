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