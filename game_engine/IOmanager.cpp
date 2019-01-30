#include "IOmanager.h"
#include <fstream>
namespace game_engine
{
	bool IOmanager::readFileToBuffer(const std::string& filePath, std::vector<unsigned char>& buffer)
	{
		std::ifstream file(filePath, std::ios::binary);//reading in binary format
		if (file.fail())
		{
			perror(filePath.c_str());
			return false;
		}

		//seek to the end

		file.seekg(0, std::ios::end);
		//get file size

		int fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		//reduce the filze size by the any header size
		fileSize -= file.tellg();

		buffer.resize(fileSize);

		file.read((char *)&(buffer[0]), fileSize);

		return true;
	}
}