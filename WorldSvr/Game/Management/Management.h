#pragma once
#include "Core.h"

namespace Management
{
	void WriteLogs(std::string path, std::string log);
	std::string GetCurrentDate();
	void EnsureDir(std::string path);
}