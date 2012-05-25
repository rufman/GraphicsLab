#include "../../../common/Definitions.hpp"

//scene includes
#include "../../../scene/Scene.hpp"

#include "RoundTree.hpp"

namespace game_space {

RoundTree::RoundTree(Scene & scene) :
		Tree(scene, ROUNDTREE_MODEL, ROUNDTREE_TEXTURE) {
}

}
