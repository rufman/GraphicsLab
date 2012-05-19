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
//	TGATexture *environment = new TGATexture("resources/GFX/textures/dusk.tga");
//	environment->setActive(true);
//	glColor3f(1,1,1);

	glPushMatrix();
	glTranslatef(_position.x, _position.y, _position.z);
	Utils::applyGLRotation(Vector3D(0.0f, 1.0f, 0.0f),
			_scene.getTerrain().getNormal(_position));
	glScalef(5, 5, 5);
	glRotatef(-90, 1, 0, 0);
	PLYModel::draw();
	glPopMatrix();

//	glDisable(GL_TEXTURE_2D);
//	glDisable(GL_BLEND);
//	glEnable(GL_LIGHTING);
//	glDisable(GL_TEXTURE_GEN_T);
//	glDisable(GL_TEXTURE_GEN_S);
}

}
