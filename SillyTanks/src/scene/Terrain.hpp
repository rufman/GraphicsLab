/**
 * terrain.hpp
* See terrain.cpp
 */

#ifndef GRAPHICSLAB_TERRAIN_HPP
#define GRAPHICSLAB_TERRAIN_HPP

// common include
#include "../common/Drawable.hpp"
#include "../common/GLIncludes.hpp"
#include "../common/Material.hpp"

//pathfinding include
#include "pathfinding/Node.hpp"

#include <string>
#include <vector>

GAME_NAMESPACE_BEGIN

// Forward declarations
class TGAImage;
class TGATexture;

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

    int getNearestTriangleIndexAt( const Point &point) const;
    void doDamageAt( const Point &point ) ;
    void findPath(Point start,Point goal);
    Node* getNodeFromPoint(Point point);
    Node * getNeighborOf(Point point,int px,int pz);
    float heuristicCostEstimate(Node from,Node to);
    float distBetween(Node from,Node to);
    std::vector<Node*> getNeighbors(Node node);
	
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

	float _width, _length;
	uint _widthResolution, _lengthResolution;
	
	TGAImage *_heightData;
	TGATexture *_texture;
	
	//pathfinding debugging
	std::vector<Node*> _nodes;

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

GAME_NAMESPACE_END


#endif // GRAPHICSLAB_TERRAIN_HPP
