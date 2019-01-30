#pragma once
#include <vector>
namespace game_engine
{
	class IOmanager
	{
	public:
		static bool readFileToBuffer(const std::string& filePath, std::vector<unsigned char>& buffer);
	};

}