#pragma once
#include "GLTexture.h"
#include <string>
namespace game_engine
{
	class ImageLoader
	{
	public:
		static GLTexture loadPNG(std::string filePath);
	};

}