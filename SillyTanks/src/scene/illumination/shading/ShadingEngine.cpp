/*
 * ShadingEngine.cpp
 */

#include "ShadingEngine.hpp"
#include "../../../common/Definitions.hpp"
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

namespace game_space {

ShadingEngine::ShadingEngine() {

}

ShadingEngine::~ShadingEngine() {

}

void ShadingEngine::compileShaders() {
	//Toonshading
	GLhandleARB toonVertexShader, toonFragmentShader;
	toonVertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	toonFragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	char *vs = NULL, *fs = NULL;
	vs = readInShader(TOON_VERTEX_SHADER);
	fs = readInShader(TOON_FRAGMENT_SHADER);
	const char* vv =vs;
	const char* ff = fs;

	glShaderSourceARB(toonVertexShader, 1, &vv, NULL);
	glShaderSourceARB(toonFragmentShader, 1, &ff, NULL);

	GLint compiled;
	glCompileShaderARB(toonVertexShader);
	glGetShaderiv(toonVertexShader, GL_COMPILE_STATUS, &compiled);
		if(compiled==FALSE)
		{
			std::cout << "Vertex shader not compiled." << std::endl;
			printShaderInfoLog(toonVertexShader);

			glDeleteShader(toonVertexShader);
			toonVertexShader=0;
			glDeleteShader(toonFragmentShader);
			toonFragmentShader=0;

			return;
		}
	glCompileShaderARB(toonFragmentShader);
	glGetShaderiv(toonFragmentShader, GL_COMPILE_STATUS, &compiled);
		if(compiled==FALSE)
		{
			std::cout << "Fragment shader not compiled." << std::endl;
			printShaderInfoLog(toonFragmentShader);

			glDeleteShader(toonVertexShader);
			toonVertexShader=0;
			glDeleteShader(toonFragmentShader);
			toonFragmentShader=0;

			return;
		}

	toonShader = glCreateProgramObjectARB();
	glAttachObjectARB(toonShader, toonVertexShader);
	glAttachObjectARB(toonShader, toonFragmentShader);

	glLinkProgramARB(toonShader);
}

void ShadingEngine::applyToonShader()
{
	glUseProgramObjectARB(toonShader);

}

void ShadingEngine::setTextureForShader(GLuint i)
{

	//glActiveTexture(GL_TEXTURE0 + i);

	glUniform1iARB(glGetUniformLocationARB(toonShader, "Texture0"), i);
}

void ShadingEngine::clearShaders()
{
	glUseProgramObjectARB(0);
}

char *ShadingEngine::readInShader(char *fn) {


	FILE *fp;
	char *content = NULL;

	int count=0;

	if (fn != NULL) {
		fp = fopen(fn,"rt");

		if (fp != NULL) {

      fseek(fp, 0, SEEK_END);
      count = ftell(fp);
      rewind(fp);

			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}

// printShaderInfoLog
// From OpenGL Shading Language 3rd Edition, p215-216
// Display (hopefully) useful error messages if shader fails to compile
void ShadingEngine::printShaderInfoLog(GLint shader)
{
	int infoLogLen = 0;
	int charsWritten = 0;
	GLchar *infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

	if (infoLogLen > 0)
	{
		infoLog = new GLchar[infoLogLen];
		// error check for fail to allocate memory omitted
		glGetShaderInfoLog(shader, infoLogLen, &charsWritten, infoLog);
		std::cout << "InfoLog : " << std::endl << infoLog << std::endl;
		delete [] infoLog;
	}
}
}

