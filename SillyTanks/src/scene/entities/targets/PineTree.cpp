#include "../../../common/Definitions.hpp"

//scene includes
#include "../../../scene/Scene.hpp"

#include "PineTree.hpp"

namespace game_space {

PineTree::PineTree(Scene & scene) :
		Tree(scene, PINETREE_MODEL, PINETREE_TEXTURE) {
}

}
