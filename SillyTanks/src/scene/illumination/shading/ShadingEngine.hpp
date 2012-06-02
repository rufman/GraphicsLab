/*
 * ShadingEngine.hpp
 */

#ifndef SHADINGENGINE_H_
#define SHADINGENGINE_H_

#include "../../../common/GLIncludes.hpp"

namespace game_space {

class ShadingEngine {
public:
	enum Shader{
		TOON
	};
public:
	ShadingEngine();
	virtual ~ShadingEngine();

	void applyToonShader();
	void clearShaders();
	void setTextureForShader(GLuint i);
	void compileShaders();
private:
	char* readInShader(char* fn);
	// printShaderInfoLog
	// From OpenGL Shading Language 3rd Edition, p215-216
	// Display (hopefully) useful error messages if shader fails to compile
	void printShaderInfoLog(GLint shader);

private:
	GLhandleARB toonShader;
	GLint loc;
};

}

#endif /* TOONSHADER_H_ */
