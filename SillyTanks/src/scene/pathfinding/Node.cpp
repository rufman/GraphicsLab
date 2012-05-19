/*
 * dot.cpp
 * Show the state of the grid points
 */

#include "Node.hpp"

namespace game_space {

Node::Node(Point position,Scene & scene):
Drawable(scene),
_nodeState(Node::FREE),
_pathState(Node::NOTHING),
_position(position) {
	_freeMaterial.setAmbient(Color::GRAY );
	_freeMaterial.setDiffuse( Color::GRAY );
	_freeMaterial.setSpecular( Color( 0.5, 0.5, 0.5 ) );
	_freeMaterial.setEmission( Color( 0.0, 0.0, 0.0 ) );
	_freeMaterial.setShininess( 15 );

	_openMaterial.setAmbient( Color::SKY_BLUE );
	_openMaterial.setDiffuse(Color::SKY_BLUE );
	_openMaterial.setSpecular( Color( 0.5, 0.5, 0.5 ) );
	_openMaterial.setEmission( Color( 0.0, 0.0, 0.0 ) );
	_openMaterial.setShininess( 15 );

	_closedMaterial.setAmbient( Color::BLACK );
	_closedMaterial.setDiffuse( Color::BLACK );
	_closedMaterial.setSpecular( Color( 0.5, 0.5, 0.5 ) );
	_closedMaterial.setEmission( Color( 0.0, 0.0, 0.0 ) );
	_closedMaterial.setShininess( 15 );

	_blockedMaterial.setAmbient( Color::SADDLE_BROWN );
	_blockedMaterial.setDiffuse( Color::SADDLE_BROWN );
	_blockedMaterial.setSpecular( Color( 0.5, 0.5, 0.5 ) );
	_blockedMaterial.setEmission( Color( 0.0, 0.0, 0.0 ) );
	_blockedMaterial.setShininess( 15 );

	_startMaterial.setAmbient( Color::RED );
	_startMaterial.setDiffuse( Color::RED );
	_startMaterial.setSpecular( Color( 1.0, 1.0, 1.0 ) );
	_startMaterial.setEmission( Color( 0.0, 0.0, 0.0 ) );
	_startMaterial.setShininess( 15 );

	_endMaterial.setAmbient( Color::GREEN );
	_endMaterial.setDiffuse( Color::GREEN);
	_endMaterial.setSpecular( Color( 1.0, 1.0, 1.0 ) );
	_endMaterial.setEmission( Color( 0.0, 0.0, 0.0 ) );
	_endMaterial.setShininess( 100 );

	_partOfPathMaterial.setAmbient( Color::YELLOW );
	_partOfPathMaterial.setDiffuse( Color( 0.5, 0.3, 0.0 ) );
	_partOfPathMaterial.setSpecular( Color( 0.5, 0.5, 0.5 ) );
	_partOfPathMaterial.setEmission( Color( 0.0, 0.0, 0.0 ) );
	_partOfPathMaterial.setShininess( 15 );
}

Node::~Node() {

}

void Node::draw() const {
	glPolygonMode( GL_FRONT_AND_BACK, ( _renderingParameters.drawMode == RenderingParameters::WIREFRAME ) ? GL_LINE : GL_FILL );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glTranslatef(_position.x,_position.y+1,_position.z);

	switch(_pathState)
	{
		case NOTHING:
		{
			switch(_nodeState)
			{
				case FREE:
				{
					_freeMaterial.setActive();
					break;
				}
				case OPEN:
				{
					_openMaterial.setActive();
					break;
				}
				case CLOSED:
				{
					_closedMaterial.setActive();
					break;
				}
				case BLOCKED:
				{
					_blockedMaterial.setActive();
					break;
				}
			}
			break;
		}
		case STARTPOINT:
		{
			_startMaterial.setActive();
			glutSolidCube(0.5);
			break;
		}
		case ENDPOINT:
		{
			_endMaterial.setActive();
			glutSolidCube(0.5);
			break;
		}
		case PARTOFPATH:
		{
			_partOfPathMaterial.setActive();
			glutSolidCube(0.5);
			break;
		}
	}
	glPopMatrix();
}

bool Node::equals(Node node) const {
	// a node is equal to another node if x,y,z,f_score (therefore g and h), nodeState and pathState are equal
	return (_position.x == node._position.x)
	&& (_position.z == node._position.y)
	&& (_position.z == node._position.z);
}

void Node::setNodeState(NodeState nodestate)
{
	_nodeState = nodestate;
}

}

