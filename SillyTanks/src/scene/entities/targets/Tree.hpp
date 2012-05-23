#ifndef GAME_TREE_HPP
#define GAME_TREE_HPP

#include "../../../common/PLYModel.hpp"
// Includes
#include <string>

namespace game_space {

class Scene;

class Tree: public PLYModel {
public:
	Tree(Scene &scene, const std::string &file);
	virtual ~Tree();
	void draw() const;
	void drawShadow(const Point &lightPosition);
};

}
#endif
