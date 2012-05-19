#ifndef GAME_ROUNDTREE_HPP
#define GAME_ROUNDTREE_HPP
#include "Tree.hpp"

namespace game_space {
class Scene;

class RoundTree: public Tree {
public:
	RoundTree(Scene &scene);
};

}
#endif
