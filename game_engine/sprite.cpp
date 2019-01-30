#include "sprite.h"
#include "vertex.h"
#include <cstddef>
#include "ResourceManager.h"
namespace game_engine
{
	sprite::sprite()
		: _vboID(0)
	{

	}


	sprite::~sprite()
	{
		if (_vboID != 0)
		{
			glDeleteBuffers(1, &_vboID);
		}
	}



	void sprite::init(float x, float y, float width, float height, std::string texturePath)
	{
		_x = x;
		_y = y;
		_width = width;
		_height = height;

		_texture = ResourceManager::getTexture(texturePath);


		if (_vboID == 0)
		{
			glGenBuffers(1, &_vboID);
		}

		vertex vertexData[6]; // 6 vertices * 2 triangles = quad

		//one triangle
		vertexData[0].setPosition(x + width, y + height);
		vertexData[0].setUV(1.0f, 1.0f);

		vertexData[1].setPosition(x, y + height);
		vertexData[1].setUV(0.0f, 1.0f);

		vertexData[2].setPosition(x, y);
		vertexData[2].setUV(0.0f, 0.0f);

		//second triangle
		vertexData[3].setPosition(x, y);
		vertexData[3].setUV(0.0f, 0.0f);

		vertexData[4].setPosition(x + width, y);
		vertexData[4].setUV(1.0f, 0.0f);

		vertexData[5].setPosition(x + width, y + height);
		vertexData[5].setUV(1.0f, 1.0f);



		for (int i = 0; i < 6; ++i)
		{
			vertexData[4].setColor(255, 0, 255, 255);
		}
		vertexData[1].setColor(0, 0, 255, 255);
		vertexData[4].setColor(0, 255, 0, 255);

		glBindBuffer(GL_ARRAY_BUFFER, _vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	void sprite::draw()
	{


		glBindTexture(GL_TEXTURE_2D, _texture.id);
		//bind the buffer object
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);


		glEnableVertexAttribArray(0);
		//tell opengl to use 1st attrb array. 
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		//Position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, position));
		//Color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(vertex), (void*)offsetof(vertex, color));

		//this is UV attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, uv));

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		//unbind VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}