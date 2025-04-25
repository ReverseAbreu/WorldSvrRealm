#include "Management.h"
#include "filesystem"
#include "Common/Json/json.hpp"

std::string Management::GetCurrentDate()
{
	char buffer[100] = {0};

	std::time_t time	= std::time(nullptr);
	std::tm*	tm		= std::localtime(&time);

	std::strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", tm);

	return buffer;
}

void Management::EnsureDir(std::string path)
{
	if (!std::filesystem::exists(path))
		std::filesystem::create_directories(path);
}

void Management::WriteLogs(std::string path, std::string log)
{
	std::ofstream out(path, std::ios_base::app);

	if (out.is_open())
	{
		out << "[" << GetCurrentDate() << "]: " << log << std::endl;
	}

	out.close();
}