
// Class declaration include
#include "Fog.hpp"

namespace game_space {


Fog::Fog(float fogDensity, float fogStart, float fogEnd, float fogRed, float fogGreen, float fogBlue){
	glFogi(GL_FOG_MODE, GL_EXP2);
	glFogf(GL_FOG_DENSITY, fogDensity);
	glHint(GL_FOG_HINT, GL_NICEST);
	glFogf(GL_FOG_START, fogStart); // Fog Start Depth
	glFogf(GL_FOG_END, fogEnd); // Fog End Depth
	GLfloat fogColor[4] = { fogRed, fogGreen, fogBlue, 1.0f };
	glFogfv(GL_FOG_COLOR, fogColor);
}


Fog::~Fog() {
}


void Fog::apply() {
	glEnable(GL_FOG);
}

void Fog::remove() {
	glDisable(GL_FOG);
}


}
