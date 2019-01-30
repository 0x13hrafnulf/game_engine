#pragma once
#include <string>
#include <GL/glew.h>

namespace game_engine
{
	class GLSLProgram
	{
	public:
		GLSLProgram();

		~GLSLProgram();

		void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

		void linkShaders();
		GLint getUniformLocation(const std::string& uniformName);
		void addAttribute(const std::string& attributeName);
		void use();
		void unuse();
	private:
		GLuint _programID;
		GLuint _vertexShaderID;
		GLuint _fragmentShaderID;
		int _numAttributes;

		void compileShader(const std::string& FilePath, GLuint id);



	};

}