#pragma once
#include <GL/glew.h>
#include "GLTexture.h"
#include <string>
namespace game_engine
{
	class sprite
	{
	public:
		sprite();
		~sprite();

		void draw();
		void init(float x, float y, float width, float height, std::string texturePath);


	private:

		float _y;
		float _x;
		float _width;
		float _height;
		GLuint _vboID; //unsignet32 int, vertex buffer object

		GLTexture _texture;
	};

}