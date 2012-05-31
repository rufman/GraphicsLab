/**
 * terrain.cpp
 * This class creates a terrain that is read from a heightmap.
 */

// Class declaration include
#include "Terrain.hpp"

// common includes
#include "../common/GLIncludes.hpp"
#include "../common/Utils.hpp"
#include "../common/TGAImage.hpp"
#include "../common/TGATexture.hpp"

//pathfinding includes
#include "pathfinding/Node.hpp"

//targets includes
#include "entities/targets/PineTree.hpp"
#include "entities/targets/RoundTree.hpp"
#include "entities/targets/PalmTree.hpp"

//scene includes
#include "Scene.hpp"
#include "Water.hpp"

//std includes
#include <sstream>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>

namespace game_space {

Terrain::Terrain(Scene &scene, const std::string &textureFilePrefix, float width, float length, uint widthResolution, uint lengthResolution) :
		Drawable(scene), _numDisplayLists(2), _width(width), _length(length), _widthResolution(widthResolution), _lengthResolution(lengthResolution),_firstRun(true) {
	_displayLists = glGenLists(_numDisplayLists);

	_material.setAmbient(Color(0.2, 0.2, 0.2));
	_material.setDiffuse(Color(0.4, 0.4, 0.4));
	_material.setSpecular(Color(0.6, 0.6, 0.6));
	_material.setEmission(Color(0.0, 0.0, 0.0));
	_material.setShininess(32);

	std::stringstream heightFieldFileName;
	heightFieldFileName << textureFilePrefix << "_hf.tga";
	std::stringstream objectDataFileName;
	objectDataFileName << textureFilePrefix << "_od.tga";
	std::stringstream textureFileName;
	textureFileName << textureFilePrefix << "_tex.tga";

	_heightData = new TGAImage();
	_heightData->load(heightFieldFileName.str());

	_objectData = new TGAImage();
	_objectData->load(objectDataFileName.str());

	_texture = new TGATexture(textureFileName.str());

	if (_widthResolution == 0)
		_widthResolution = _heightData->getWidth();
	if (_lengthResolution == 0)
		_lengthResolution = _heightData->getHeight();

	int numVertices = _widthResolution * _lengthResolution;
	for (int i = 0; i < numVertices; i++) {
		_vertices.push_back(Point());
		_vertexNormals.push_back(Vector3D(0,1,0));
	}

	int numTriangles = 2 * (_widthResolution - 1) * (_lengthResolution - 1);
	for (int i = 0; i < numTriangles; i++) {
		_triangles.push_back(Triangle(0, 0, 0));
		_triangleNormals.push_back(Vector3D(0,1,0));
	}

	// Calculate vertices
	uint xSlice = _texture->getWidth() / _widthResolution;
	uint zSlice = _texture->getHeight() / _lengthResolution;

	Tree *pineTreeCache = new PineTree(_scene);
	pineTreeCache->setNeighbors();
	Tree *roundTreeCache = new RoundTree(_scene);
	roundTreeCache->setNeighbors();
	Tree *palmTreeCache = new PalmTree(_scene);
	palmTreeCache->setNeighbors();

	for (uint widthPoint = 0; widthPoint < _widthResolution; widthPoint++) {

		for (uint lengthPoint = 0; lengthPoint < _lengthResolution; lengthPoint++) {
			Point &vertex = _vertices[_widthResolution * lengthPoint + widthPoint];
			vertex.x = -_width / 2.0 + widthPoint * (_width / (_widthResolution - 1));
			vertex.z = _length / 2.0 - lengthPoint * (_length / (_lengthResolution - 1));

			int heightDataIndex = _heightData->getWidth() * (lengthPoint * zSlice) * 4 + widthPoint * xSlice * 4;
			int objectDataIndex = _objectData->getWidth() * (lengthPoint * zSlice) * 4 + widthPoint * xSlice * 4;
			float heightScaling = 7.0f;
			vertex.y = ((_heightData->getData()[heightDataIndex]) / heightScaling) - 200 / heightScaling;

			//ObjectData mapping
			if (_objectData->getData()[objectDataIndex] == PINETREE_MAPNR) {
				Tree *treeModel = new PineTree(_scene);
				treeModel->setPosition(vertex);
				treeModel->_plyData.triangleNeighbors = pineTreeCache->_plyData.triangleNeighbors;
				//treeModel->setNeighbors();
				_trees.push_back(treeModel);
			} else if (_objectData->getData()[objectDataIndex] == ROUNDTREE_MAPNR) {
				Tree *treeModel = new RoundTree(_scene);
				treeModel->setPosition(vertex);
				treeModel->_plyData.triangleNeighbors = roundTreeCache->_plyData.triangleNeighbors;
				//treeModel->setNeighbors();
				_trees.push_back(treeModel);
			} else if (_objectData->getData()[objectDataIndex] == PALMTREE_MAPNR) {
				Tree *treeModel = new PalmTree(_scene);
				treeModel->setPosition(vertex);
				treeModel->_plyData.triangleNeighbors = palmTreeCache->_plyData.triangleNeighbors;
				//treeModel->setNeighbors();
				_trees.push_back(treeModel);
			}
		}
	}

	// Build triangles
	for (int widthPoint = 0; widthPoint < (_widthResolution - 1); widthPoint++) {
		for (int lengthPoint = 0; lengthPoint < (_lengthResolution - 1); lengthPoint++) {
			int t1 = 2 * lengthPoint * (_widthResolution - 1) + widthPoint * 2;
			int t2 = t1 + 1;
			Triangle &triangle1 = _triangles[t1];
			Triangle &triangle2 = _triangles[t2];

			int v1 = (lengthPoint) * _widthResolution + (widthPoint);
			int v2 = v1 + 1;
			int v3 = v2 + _widthResolution;
			int v4 = v3 - 1;
			triangle1.vertex1 = v1;
			triangle1.vertex2 = v2;
			triangle1.vertex3 = v3;
			triangle2.vertex1 = v1;
			triangle2.vertex2 = v3;
			triangle2.vertex3 = v4;
		}
	}

	// Calculate triangle normals
	for (int widthPoint = 0; widthPoint < (_widthResolution - 1); widthPoint++) {
		for (int lengthPoint = 0; lengthPoint < (_lengthResolution - 1); lengthPoint++) {
			int t1 = 2 * lengthPoint * (_widthResolution - 1) + widthPoint * 2;
			int t2 = t1 + 1;
			Triangle &triangle1 = _triangles[t1];
			Triangle &triangle2 = _triangles[t2];

			_triangleNormals[t1] = Utils::normal(_vertices[triangle1.vertex1], _vertices[triangle1.vertex2], _vertices[triangle1.vertex3]);
			_triangleNormals[t2] = Utils::normal(_vertices[triangle2.vertex1], _vertices[triangle2.vertex2], _vertices[triangle2.vertex3]);
		}
	}

	// Calculate vertex normals
	for (uint widthPoint = 0; widthPoint < _widthResolution; widthPoint++) {
		for (uint lengthPoint = 0; lengthPoint < _lengthResolution; lengthPoint++) {
			int v = _widthResolution * lengthPoint + widthPoint;
			Vector3D &normal = _vertexNormals[v];
			normal = Vector3D(0.0, 0.0, 0.0);

			int t1 = (widthPoint > 0 && lengthPoint > 0) ? (2 * (lengthPoint - 1) * (_widthResolution - 1) + 2 * (widthPoint - 1)) : -1, t2 = (widthPoint > 0 && lengthPoint > 0) ? (t1 + 1) : -1, t3 = (widthPoint < (_widthResolution - 1) && lengthPoint > 0) ? (2 * (lengthPoint - 1) * (_widthResolution - 1) + 2 * (widthPoint) + 1) : -1, t4 =
					(widthPoint > 0 && lengthPoint < (_lengthResolution - 1)) ? (2 * (lengthPoint) * (_widthResolution - 1) + 2 * (widthPoint - 1)) : -1, t5 = (widthPoint < (_widthResolution - 1) && lengthPoint < (_lengthResolution - 1)) ? (2 * (lengthPoint) * (_widthResolution - 1) + 2 * (widthPoint)) : -1, t6 =
					(widthPoint < (_widthResolution - 1) && lengthPoint < (_lengthResolution - 1)) ? (t5 + 1) : -1;
			if (t1 >= 0)
				normal += _triangleNormals[t1];
			if (t2 >= 0)
				normal += _triangleNormals[t2];
			if (t3 >= 0)
				normal += _triangleNormals[t3];
			if (t4 >= 0)
				normal += _triangleNormals[t4];
			if (t5 >= 0)
				normal += _triangleNormals[t5];
			if (t6 >= 0)
				normal += _triangleNormals[t6];

			Utils::normalize(normal);
		}
	}

	buildDisplayLists();

	for (std::vector<Point>::iterator vertexIter = _vertices.begin(); vertexIter != _vertices.end(); vertexIter++) {
		Point vertex = *vertexIter;
		Node* node = new Node(Point(vertex.x, vertex.y, vertex.z), _scene);

		_nodes.push_back(node);
	}
}

Terrain::~Terrain() {
	glDeleteLists(_displayLists, _numDisplayLists);
	delete _heightData;
	delete _texture;
}

void Terrain::buildDisplayLists() {
	uint xSlice = _texture->getWidth() / _widthResolution;
	uint zSlice = _texture->getHeight() / _lengthResolution;

	int numTriangles = 2 * (_widthResolution - 1) * (_lengthResolution - 1);

	// Flat shading
	glNewList(_displayLists, GL_COMPILE);

	glEnable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	glShadeModel(GL_FLAT);

	_material.setActive();

	glBegin(GL_TRIANGLES);

	for (int t = 0; t < numTriangles; t++) {

		const Vector3D &normal = _triangleNormals[t];
		glNormal3f(normal.x, normal.y, normal.z);

		const Point &vertex1 = _vertices[_triangles[t].vertex1];
		const Point &vertex2 = _vertices[_triangles[t].vertex2];
		const Point &vertex3 = _vertices[_triangles[t].vertex3];

		glTexCoord2f(((vertex1.x + _width / 2.0) / (_width / (_widthResolution - 1))) * xSlice, ((-vertex1.z + _length / 2.0) / (_length / (_lengthResolution - 1))) * zSlice);
		glVertex3f(vertex1.x, vertex1.y, vertex1.z);

		glTexCoord2f(((vertex2.x + _width / 2.0) / (_width / (_widthResolution - 1))) * xSlice, ((-vertex2.z + _length / 2.0) / (_length / (_lengthResolution - 1))) * zSlice);
		glVertex3f(vertex2.x, vertex2.y, vertex2.z);

		glTexCoord2f(((vertex3.x + _width / 2.0) / (_width / (_widthResolution - 1))) * xSlice, ((-vertex3.z + _length / 2.0) / (_length / (_lengthResolution - 1))) * zSlice);
		glVertex3f(vertex3.x, vertex3.y, vertex3.z);
	}

	glEnd();

	glEndList();

	// Smooth shading
	glNewList(_displayLists + 1, GL_COMPILE);


	glEnable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	glShadeModel(GL_SMOOTH);

	_material.setActive();

	glBegin(GL_TRIANGLES);

	for (int t = 0; t < numTriangles; t++) {

		const Point &vertex1 = _vertices[_triangles[t].vertex1];
		const Point &vertex2 = _vertices[_triangles[t].vertex2];
		const Point &vertex3 = _vertices[_triangles[t].vertex3];

		if (_renderingParameters.shadeMode == RenderingParameters::SMOOTH) {
			const Vector3D &normal = _vertexNormals[_triangles[t].vertex1];
			glNormal3f(normal.x, normal.y, normal.z);
		}
		glTexCoord2f(((vertex1.x + _width / 2.0) / (_width / (_widthResolution - 1))) * xSlice, ((-vertex1.z + _length / 2.0) / (_length / (_lengthResolution - 1))) * zSlice);
		glVertex3f(vertex1.x, vertex1.y, vertex1.z);

		if (_renderingParameters.shadeMode == RenderingParameters::SMOOTH) {
			const Vector3D &normal = _vertexNormals[_triangles[t].vertex2];
			glNormal3f(normal.x, normal.y, normal.z);
		}
		glTexCoord2f(((vertex2.x + _width / 2.0) / (_width / (_widthResolution - 1))) * xSlice, ((-vertex2.z + _length / 2.0) / (_length / (_lengthResolution - 1))) * zSlice);
		glVertex3f(vertex2.x, vertex2.y, vertex2.z);

		if (_renderingParameters.shadeMode == RenderingParameters::SMOOTH) {
			const Vector3D &normal = _vertexNormals[_triangles[t].vertex3];
			glNormal3f(normal.x, normal.y, normal.z);
		}
		glTexCoord2f(((vertex3.x + _width / 2.0) / (_width / (_widthResolution - 1))) * xSlice, ((-vertex3.z + _length / 2.0) / (_length / (_lengthResolution - 1))) * zSlice);
		glVertex3f(vertex3.x, vertex3.y, vertex3.z);
	}

	glEnd();
	glEndList();
}

void Terrain::draw() const {

	glPolygonMode(GL_FRONT_AND_BACK, (_renderingParameters.drawMode == RenderingParameters::WIREFRAME) ? GL_LINE : GL_FILL);

	for (uint i = 0; i < _trees.size(); i++) {
		_trees[i]->draw();
	}
	_texture->setActive(true);
	_scene.getShadingEngine().setTextureForShader(_texture->getID());
	glEnable(GL_LIGHTING);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glCallList((_renderingParameters.shadeMode == RenderingParameters::FLAT) ? (_displayLists) : (_displayLists + 1));

	glPopMatrix();

	if (_renderingParameters.normalMode == RenderingParameters::VERTEX) {
		glColor3f(Color::WHITE.r, Color::WHITE.g, Color::WHITE.b);
		glBegin(GL_LINES);
		for (uint widthPoint = 0; widthPoint < _widthResolution; widthPoint++) {
			for (uint lengthPoint = 0; lengthPoint < _lengthResolution; lengthPoint++) {
				int v = _widthResolution * lengthPoint + widthPoint;
				const Point &vertex = _vertices[v];
				const Vector3D &normal = _vertexNormals[v];
				glVertex3f(vertex.x, vertex.y, vertex.z);
				glVertex3f(vertex.x + normal.x, vertex.y + normal.y, vertex.z + normal.z);
			}
		}
		glEnd();
	} else if (_renderingParameters.normalMode == RenderingParameters::TRIANGLE) {
		int numTriangles = 2 * (_widthResolution - 1) * (_lengthResolution - 1);
		glColor3f(Color::TAN.r, Color::TAN.g, Color::TAN.b);
		glBegin(GL_LINES);
		for (int t = 0; t < numTriangles; t++) {
			const Point &vertex1 = _vertices[_triangles[t].vertex1];
			const Point &vertex2 = _vertices[_triangles[t].vertex2];
			const Point &vertex3 = _vertices[_triangles[t].vertex3];
			Point baryCenter((vertex1.x + vertex2.x + vertex3.x) / 3.0f, (vertex1.y + vertex2.y + vertex3.y) / 3.0f, (vertex1.z + vertex2.z + vertex3.z) / 3.0f);
			const Vector3D &normal = _triangleNormals[t];

			glVertex3f(baryCenter.x, baryCenter.y, baryCenter.z);
			glVertex3f(baryCenter.x + normal.x, baryCenter.y + normal.y, baryCenter.z + normal.z);
		}
		glEnd();
	}
	_texture->setActive(false);
}

void Terrain::drawShadows(const Point &lightPostion) const {
	for (uint i = 0; i < _trees.size(); i++) {
		glClear(GL_STENCIL_BUFFER_BIT);
		((Tree*) _trees[i])->drawShadow(lightPostion);
	}
}

float Terrain::getHeight(const Point &point) const {

	int triangleNumber = getNearestTriangleIndexAt(point);
	if (triangleNumber != -1) {
		Point firstPoint = _vertices[_triangles[triangleNumber].vertex1];
		Point secondPoint = _vertices[_triangles[triangleNumber].vertex2];
		Point thirdPoint = _vertices[_triangles[triangleNumber].vertex3];

		float y = (_triangleNormals[triangleNumber].x * (point.x - _vertices[_triangles[triangleNumber].vertex1].x) + _triangleNormals[triangleNumber].z * (point.z - _vertices[_triangles[triangleNumber].vertex1].z)) / (-_triangleNormals[triangleNumber].y);
		y += _vertices[_triangles[triangleNumber].vertex1].y;
		return y;
	} else {
		return 0;
	}
}

Vector3D Terrain::getNormal(const Point &point) const {
	int triangleNumber = getNearestTriangleIndexAt(point);
	//std::cout << "normal size:" << _vertexNormals.size()<< std::endl;
	//std::cout << "normal index:" <<_triangles[triangleNumber].vertex3<< std::endl;
	//std::cout << "triangle number:" <<triangleNumber<< std::endl;
	if (triangleNumber >= 0) {
		Vector3D norm1 = _vertexNormals[_triangles[triangleNumber].vertex1];
		Vector3D norm2 = _vertexNormals[_triangles[triangleNumber].vertex2];
		Vector3D norm3 = _vertexNormals[_triangles[triangleNumber].vertex3];

		Point firstPoint = _vertices[_triangles[triangleNumber].vertex1];
		Point secondPoint = _vertices[_triangles[triangleNumber].vertex2];
		Point thirdPoint = _vertices[_triangles[triangleNumber].vertex3];

		float dist1 = Utils::distance(point, firstPoint);
		float dist2 = Utils::distance(point, secondPoint);
		float dist3 = Utils::distance(point, thirdPoint);
		float totalDist = dist1 + dist2 + dist3;

		float ratio1 = dist1 / totalDist;
		float ratio2 = dist2 / totalDist;
		float ratio3 = dist3 / totalDist;

		//std::cout << "ratio1:" << ratio1<< std::endl;

		return Vector3D(norm1.x * ratio1 + norm2.x * ratio2 + norm3.x * ratio3, norm1.y * ratio1 + norm2.y * ratio2 + norm3.y * ratio3, norm1.z * ratio1 + norm2.z * ratio2 + norm3.z * ratio3);
	} else {
		return Vector3D(0, 1, 0);
	}
}

int Terrain::getNearestTriangleIndexAt(const Point &point) const {

	float widthVertDistance = _width / (_widthResolution - 1);
	float lengthVertDistance = _length / (_lengthResolution - 1);

	float slope = lengthVertDistance / widthVertDistance;

	float columnNumber = (point.x + (_width / 2)) / widthVertDistance;
	float rowNumber = (-point.z + (_length / 2)) / lengthVertDistance;

	float restColumn = columnNumber - (int) columnNumber;
	float restRow = rowNumber - (int) rowNumber;

	int triangleNumber = -1;
	if (restRow - slope * restColumn > 0) {
		//upper triangle
		triangleNumber = (int) rowNumber * ((_widthResolution - 1) * 2) + ((int) columnNumber) * 2 + 1;

	} else {
		//lower triangle
		triangleNumber = (int) rowNumber * ((_widthResolution - 1) * 2) + ((int) columnNumber) * 2;
	}

	//fix for stuff that is beyond the border of the map
	if(triangleNumber < _triangles.size())
	{
	return triangleNumber;
	}
	else
	{
		return -1;
	}
}

void Terrain::doDamageAt(const Point &point, float damageStrength) {
	int triangleIndex = getNearestTriangleIndexAt(point);
	if (triangleIndex != -1) {
		const Point firstPoint = _vertices[_triangles[triangleIndex].vertex1];
		const Point secondPoint = _vertices[_triangles[triangleIndex].vertex2];
		const Point thirdPoint = _vertices[_triangles[triangleIndex].vertex3];
		firstPoint.y -= damageStrength;
		secondPoint.y -= damageStrength;
		thirdPoint.y -= damageStrength;
		_vertices.at(_triangles[triangleIndex].vertex1) = firstPoint;
		_vertices.at(_triangles[triangleIndex].vertex2) = secondPoint;
		_vertices.at(_triangles[triangleIndex].vertex3) = thirdPoint;

		for(std::vector<PLYModel*>::iterator treeIter = _trees.begin();treeIter != _trees.end();++treeIter)
		{
			PLYModel* tree = *treeIter;
			if(Utils::distance(tree->getPosition(),point) < 50)
			{
				Point newPoint(tree->getPosition().x,getHeight(tree->getPosition()),tree->getPosition().z);
				tree->setPosition(newPoint);
			}
		}


		//refresh/redraw terrain
		buildDisplayLists();
	}
}

std::vector<Point>* Terrain::findPath(Point startPoint, Point goalPoint) {

	for (uint i = 0; i < _nodes.size(); i++) {
		_nodes.at(i)->_f_score = 0;
		_nodes.at(i)->_g_score = 0;
		if (_nodes.at(i)->_nodeState != Node::BLOCKED) {
			_nodes.at(i)->_nodeState = Node::FREE;
		}
		_nodes.at(i)->_pathState = Node::NOTHING;
		_nodes.at(i)->_nextNode = NULL;
	}

	//read in all the blocked nodes
	if(_firstRun)
	{
	for (uint i = 0; i < _vertices.size(); i++) {
		_nodes[i]->setNodeState(checkBorder(_nodes[i]->_position) ? Node::BLOCKED : Node::FREE);
	}
	_firstRun = false;
	}

	//Implemented the a-star algorithm as it is written in pseudo code on wikipedia.org
	//http://en.wikipedia.org/wiki/A*_search_algorithm
	//for the open set and closed set I use a vector sorted in-place as a heap
	// I had to find out that according to the wiki article that my estimate is non-monotonic and that is why
	// I have to update the nodes of the closed set too.

	Node* start = getNodeFromPoint(startPoint);
	Node* goal = getNodeFromPoint(goalPoint);

	//if start or end point could not be found on the map we can not find a path
	if(start == NULL || goal == NULL)
	{
		return NULL;
	}
	start->_pathState = Node::STARTPOINT;
	goal->_pathState = Node::ENDPOINT;

	// closed set := the empty set    // The set of nodes already evaluated.
	std::vector<Node*> closedSet;

	// open set := {start}    // The set of tentative nodes to be evaluated, initially containing the start node
	std::vector<Node*> openSet;

	// sort back start point into heap
	openSet.push_back(start);
	push_heap(openSet.begin(), openSet.end(), HeapCompare_f());

	// g_score[start] := 0    // Cost from start along best known path.
	start->_g_score = 0;

	// f_score[start] := g_score[start] + h_score[start]    // Estimated total cost from start to goal through y.
	start->_f_score = start->_g_score + heuristicCostEstimate(*start, *goal);

	// while open set is not empty
	while (!openSet.empty()) {
		// current := the node in open set having the lowest f_score[] value
		// remove current from open set
		Node * current = openSet.front();
		pop_heap(openSet.begin(), openSet.end(), HeapCompare_f());
		openSet.pop_back();

		// add current to closed set
		current->_nodeState = Node::CLOSED;

		// sort back element into heap
		closedSet.push_back(current);
		push_heap(closedSet.begin(), closedSet.end(), HeapCompare_f());

		// check if the current node that we examine is the goal node
		if (current->_pathState == Node::ENDPOINT) {
			std::vector<Point>* path = new std::vector<Point>();
			Node* node = goal;
			while (node->_nextNode != NULL) {
				node->_pathState = Node::PARTOFPATH;
				node = node->_nextNode;
				path->push_back(node->_position);
			}

			//return the generated path
			return path;
		}

		// for each neighbor in neighbor_nodes(current)
		std::vector<Node*> neighborhoodSet = getNeighbors(*current);
		for (std::vector<Node*>::iterator neighborIter = neighborhoodSet.begin(); neighborIter != neighborhoodSet.end(); neighborIter++) {
			Node* neighbor = *neighborIter;

			float tentative_g_score = current->_g_score + distBetween(*current, *neighbor);

			// if neighbor in closed set
			if (neighbor->_nodeState == Node::FREE || tentative_g_score < neighbor->_g_score) {
				neighbor->_nextNode = current;
				neighbor->_g_score = tentative_g_score;
				neighbor->_f_score = neighbor->_g_score + heuristicCostEstimate(*neighbor, *goal);

				if(neighbor->_nodeState == Node::FREE)
				{
					neighbor->_nodeState = Node::OPEN;
					// sort back element into heap
					openSet.push_back(neighbor);
					push_heap(openSet.begin(), openSet.end(), HeapCompare_f());
				}
			}
		}
	}
	return NULL;
}

float Terrain::heuristicCostEstimate(Node from, Node to) {
	return distBetween(from, to);
}

float Terrain::distBetween(Node from, Node to) {
	return sqrt(pow(from._position.x - to._position.x, 2) + pow(from._position.y - to._position.y, 2) + pow(from._position.z - to._position.z, 2));
}

Node* Terrain::getNodeFromPoint(Point point) {
	return getNeighborOf(point, 0, 0);

}

Node* Terrain::getNeighborOf(Point point, int px, int pz) {

	float sliceW = _width / (_widthResolution - 1);
	float sliceL = _length / (_lengthResolution - 1);

	int indexX = (point.x + _width / 2) / sliceW;
	int indexZ = -(point.z - _length / 2) / sliceL;

	return ((indexX + px) + _widthResolution * (indexZ + pz) <= _nodes.size() && (indexX + px) < _widthResolution && (indexZ + pz) < _lengthResolution) ? _nodes.at((indexX + px) + _widthResolution * (indexZ + pz)) : NULL;
}

std::vector<Node*> Terrain::getNeighbors(Node node) {
	std::vector<Node*> neighbors;
	// push each possible move except allowing the search to go backwards
	Node * neighbor = getNeighborOf(node._position, -1, 0);
	if (neighbor != NULL) {
		neighbors.push_back(neighbor);
	}

	neighbor = getNeighborOf(node._position, -1, -1);
	if (neighbor != NULL) {
		neighbors.push_back(neighbor);
	}

	neighbor = getNeighborOf(node._position, 0, -1);
	if (neighbor != NULL) {
		neighbors.push_back(neighbor);
	}

	neighbor = getNeighborOf(node._position, 1, -1);
	if (neighbor != NULL) {
		neighbors.push_back(neighbor);
	}

	neighbor = getNeighborOf(node._position, 1, 0);
	if (neighbor != NULL) {
		neighbors.push_back(neighbor);
	}

	neighbor = getNeighborOf(node._position, 1, 1);
	if (neighbor != NULL) {
		neighbors.push_back(neighbor);
	}

	neighbor = getNeighborOf(node._position, 0, 1);
	if (neighbor != NULL) {
		neighbors.push_back(neighbor);
	}

	neighbor = getNeighborOf(node._position, -1, 1);
	if (neighbor != NULL) {
		neighbors.push_back(neighbor);
	}
	return neighbors;
}

Point Terrain::getRandomPointOnMap() {
	float x, y, z;
	do {
		x = (-_width / 2.0) + (rand() % (int) _width);
		z = (_length / 2.0) - (rand() % (int) _length);
		y = getHeight(Point(x, 0, z));
	} while (checkBorder(Point(x, y, z)));

	return Point(x, y, z);
}

bool Terrain::checkBorder(const Point &point) const {
	float threshold = 3.0;
//	std::cout<<"Water Offset:"<<_scene.getWater()->getHeightOffset()<<std::endl;
//	std::cout<<"Water Height:"<<_scene.getWater()->getHeight(point)<<std::endl;
//	std::cout<<"Point Height"<<point.y<<std::endl;
	if(-1*(_scene.getWater()->getHeightOffset())>point.y){
		return true;
	}

	float angleGravityNormal = acos(Utils::dot(Vector3D(0, 1, 0), getNormal(point)));

	//PI/2 equals 45 degree, a tank should be able to do that.
	if (angleGravityNormal > Utils::PI / 2) {
		return true;
	}

//	std::cout<<"Checking trees"<<std::endl;
	for (uint i = 0; i < _trees.size(); i++) {
		if (Utils::distance(_trees[i]->getPosition(), point) < threshold) {
			return true;
		}
	}

//	std::cout<<"Returning false"<<std::endl;
	return false;
}

}
