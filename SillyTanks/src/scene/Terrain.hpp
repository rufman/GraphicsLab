/**
 * terrain.hpp
* See terrain.cpp
 */

#ifndef GRAPHICSLAB_TERRAIN_HPP
#define GRAPHICSLAB_TERRAIN_HPP

//pathfinding includes
#include "pathfinding/Node.hpp"

//common includes
#include "../common/PLYModel.hpp"


//std includes
#include <sstream>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

namespace game_space {

/** Terrain */
class Terrain : public Drawable
{
public:
    /** 
	 * Constructor
	 *
	 * @param[in, out]	scene					Owning scene
	 * @param[in]		textureFilePrefix		Terrain texture file prefix
	 */
    Terrain( Scene &scene, const std::string &textureFilePrefix, float width, float length, uint widthResolution = 0, uint lengthResolution = 0 );
	
	/** Destructor */
	~Terrain();
	
	float getHeight(const Point &point) const;
	Vector3D getNormal(const Point &point) const;

    /** Draw the sky dome to the screen. */
    void draw() const;
    void drawShadows(const Point &lightPosition) const;

    int getNearestTriangleIndexAt( const Point &point) const;
    void doDamageAt( const Point &point,float damageStrength ) ;
    std::vector<Point>* findPath(Point start,Point goal);
    Node* getNodeFromPoint(Point point);
    Node * getNeighborOf(Point point,int px,int pz);
    float heuristicCostEstimate(Node from,Node to);
    float distBetween(Node from,Node to);
    std::vector<Node*> getNeighbors(Node node);
    Point getRandomPointOnMap();

    bool checkBorder(const Point &point) const;
	
private:
	void buildDisplayLists();
	
private:
	GLuint _displayLists;
	uint _numDisplayLists;
	
	Material _material;
	
	std::vector<Point> _vertices;
	std::vector<Point> _textureCoordinates;
	std::vector<Vector3D> _vertexNormals;
	
	struct Triangle
	{
		uint vertex1, vertex2, vertex3;
		
		Triangle( uint vertex1_, uint vertex2_, uint vertex3_ ) :
			vertex1( vertex1_ ), vertex2( vertex2_ ), vertex3( vertex3_ ) {}
	};
	std::vector<Triangle> _triangles;
	std::vector<Vector3D> _triangleNormals;
	std::vector<PLYModel*> _trees;

	float _width, _length;
	uint _widthResolution, _lengthResolution;
	
	TGAImage *_heightData;
	TGAImage *_objectData;
	TGATexture *_texture;
	
	//pathfinding
	std::vector<Node*> _nodes;
	bool _firstRun;

}; // class Terrain


// For sorting the heap the STL needs compare function that lets us compare
// the f value of two nodes

class HeapCompare_f
{
	public:

		bool operator() ( const Node *x, const Node *y ) const
		{
			return x->_f_score > y->_f_score;
		}
};

}


#endif // GRAPHICSLAB_TERRAIN_HPP
