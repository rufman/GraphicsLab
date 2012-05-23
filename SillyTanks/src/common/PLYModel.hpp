/**
 * plyModel.hpp
* See plyModel.cpp
 */

#ifndef GRAPHICSLAB_PLYMODEL_HPP
#define GRAPHICSLAB_PLYMODEL_HPP


// common includes
#include "Drawable.hpp"
#include "Material.hpp"
#include "TGATexture.hpp"

#include <string>
#include <vector>
#include <cfloat>


namespace game_space {

/** PLY Model */
class PLYModel : public Drawable
{
public:
    /** 
	 * Constructor
	 *
	 * @param[in, out]	scene	Owning scene
	 */
    PLYModel( Scene &scene );
    
    /** Destructor */
    ~PLYModel();
    
	/**
	 * Load ply data from a file.
	 *
	 * @param[in]	file	File to load PLY data from
	 */
    void load( const std::string &modelFile);
	void load( const std::string &modelFile, const std::string &textureFile);

	/** Draw the model to the screen. */
	void draw() const;
	void setPosition(Point &position);
	Point &getPosition();
	std::vector<Point>* getVertices();
	void setNeighbors();
	void castShadow(const Point *lightPosition);

protected:
	void buildDisplayLists();
	
	void processFile( const std::string &file );
	
protected:
	uint _numDisplayLists;
	GLuint _displayLists;

	Material _material;

	Point _position;

public:
	struct PLYData
	{
		std::vector<Point> vertices;
		bool textured;
		std::vector<Point> textureCoords;
		struct Triangle 
		{ 
			uint vertex1, vertex2, vertex3;
			
			Triangle( uint vertex1_, uint vertex2_, uint vertex3_ ) :
				vertex1( vertex1_ ), vertex2( vertex2_ ), vertex3( vertex3_ )
			{}
		};
		std::vector<Triangle> triangles;
		
		std::vector<Vector3D> vertexNormals;
		std::vector<Vector3D> triangleNormals;
		std::vector<bool> triangleVisible;
		struct Neighbors // a basic vector class
		{
			int neigh[3];
		};
		std::vector<Neighbors> triangleNeighbors;

		Point min, max;

		void reset() {
			vertices.clear();
			triangles.clear();
			vertexNormals.clear();
			triangleNormals.clear();
			
			static const float minFloat = FLT_MIN;
			static const float maxFloat = FLT_MAX;
			min = Point( maxFloat, maxFloat, maxFloat );
			max = Point( minFloat, minFloat, minFloat );
		}
		
	} _plyData;
	
private:
	float _angle;
	TGATexture* _texture;

}; // class PLYModel


}


#endif // GRAPHICSLAB_PLYMODEL_HPP
