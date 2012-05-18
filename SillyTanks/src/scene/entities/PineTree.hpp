#ifndef GAME_PINETREE_HPP
#define GAME_PINETREE_HPP

#include "../../common/Definitions.hpp"

//common includes
#include "../../common/Utils.hpp"
#include "../../common/GLIncludes.hpp"
#include "../../common/Types.hpp"
#include "../../common/TGATexture.hpp"
//scene includes
#include "../../scene/Scene.hpp"
#include "../../scene/Terrain.hpp"

namespace game_space {

class PineTree:public PLYModel {
public:
	PineTree(Scene &scene);
	virtual ~PineTree();
	void draw() const;
};

}
#endif
