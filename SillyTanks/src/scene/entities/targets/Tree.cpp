#include "../../../common/Definitions.hpp"

//common includes
#include "../../../common/Utils.hpp"
#include "../../../common/GLIncludes.hpp"
#include "../../../common/Types.hpp"
#include "../../../common/TGATexture.hpp"

//scene includes
#include "../../../scene/Scene.hpp"
#include "../../../scene/Terrain.hpp"
#include "Tree.hpp"

namespace game_space {

Tree::Tree(Scene & scene, const std::string &file) :
		PLYModel(scene) {
	load(file);
}

Tree::Tree(Scene & scene, const std::string &modelFile, const std::string &textureFile) : PLYModel(scene){
	load(modelFile,textureFile);
}

Tree::~Tree() {
}

void Tree::draw() const {

//	// activate sperical EM texture coord generation
//	// for this to work properly, you need to specify almost one normal per face
//	// since the sphere mapping algorythm need to know how your faces are oriented about the viewer (origin)
//	glEnable(GL_TEXTURE_GEN_S);
//	glEnable(GL_TEXTURE_GEN_T);
//	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
//	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
//
//	// activate screen blending mode, don't light primitives
//	glDisable(GL_LIGHTING);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_ONE,GL_ONE);
//	glEnable(GL_TEXTURE_2D);
//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	TGATexture *environment = new TGATexture(
//			"resources/GFX/levels/terrain_tex.tga");
//	environment->setActive(true);
//	glColor3f(1,1,1);
	//GL_REFLECTION_MAP for s,t,r texture coordinates
//
//	glBindTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X, cubeMap[0]);
//	glBindTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, cubeMap[1]);
//	glBindTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, cubeMap[2]);
//	glBindTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, cubeMap[3]);
//	glBindTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, cubeMap[4]);
//	glBindTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, cubeMap[5]);
//
//	glEnable(GL_TEXTURE_CUBE_MAP);
//
//	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
//	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
//	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
//
//	glEnable(GL_TEXTURE_GEN_S);
//	glEnable(GL_TEXTURE_GEN_T);
//	glEnable(GL_TEXTURE_GEN_R);
	//Texture use for automatic coordinate generation
	//glBindTexture(GL_TEXTURE_2D, environment);
//	glEnable(GL_TEXTURE_2D);

	//Sphere mapping and enable s & t texture generation
//	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
//	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
//	glEnable(GL_TEXTURE_GEN_S);
//	glEnable(GL_TEXTURE_GEN_T);
	glPushMatrix();
	glTranslatef(_position.x, _position.y, _position.z);
	Utils::applyGLRotation(Vector3D(0.0f, 1.0f, 0.0f),
			_scene.getTerrain().getNormal(_position));
	glScalef(5, 5, 5);
	glRotatef(-90, 1, 0, 0);
	PLYModel::draw();
	glPopMatrix();

//	glDisable(GL_TEXTURE_GEN_S);
//	glDisable(GL_TEXTURE_GEN_T);
//	glDisable(GL_TEXTURE_GEN_R);
//
//	glDisable(GL_TEXTURE_CUBE_MAP);
//	glDisable(GL_TEXTURE_2D);
//	glDisable(GL_BLEND);
//	glEnable(GL_LIGHTING);
//	glDisable(GL_TEXTURE_GEN_T);
//	glDisable(GL_TEXTURE_GEN_S);
}

void Tree::drawShadow(const Point &lightPosition) {
	glPushMatrix();
	glTranslatef(_position.x, _position.y, _position.z);
	Utils::applyGLRotation(Vector3D(0.0f, 1.0f, 0.0f),
			_scene.getTerrain().getNormal(_position));
	glScalef(5, 5, 5);
	glRotatef(-90, 1, 0, 0);
	castShadow(&lightPosition);
	glPopMatrix();
}

}
