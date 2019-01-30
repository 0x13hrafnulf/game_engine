#pragma once
#include <GL/glew.h>
namespace game_engine
{
	struct Position //composition
	{
		float x;
		float y;
	};
	struct ColorRGBA8
	{
		ColorRGBA8() : r(0), g(0), b(0), a(0) { /*Empty*/ };
		ColorRGBA8(GLubyte R, GLubyte G, GLubyte B, GLubyte A) : r(R), g(G), b(B), a(A) { /*Empty*/ };
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	};
	struct UV
	{
		float u;
		float v;
	};
	struct vertex
	{
		Position position;

		ColorRGBA8 color;

		UV uv;
		void setPosition(float x, float y)
		{
			position.x = x;
			position.y = y;
		}
		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
		{
			color = ColorRGBA8(r, g, b, a);
		}
		void setUV(float u, float v)
		{
			uv.u = u;
			uv.v = v;
		}
	};
}