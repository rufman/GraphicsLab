/*
 * particleEngine.hpp
 *See particleEngine.cpp
 */

#ifndef GAME_PARTICLEENGINE_HPP
#define GAME_PARTICLEENGINE_HPP

//std includes
#include <vector>
#include <string>

//common includes
#include "../../common/Definitions.hpp"
#include "../../common/GLIncludes.hpp"
#include "../../common/TGATexture.hpp"
#include "../../common/Utils.hpp"
#include "../../common/Camera3D.hpp"

//particleEngine include
#include "Particle.hpp"

namespace game_space {

template <class T>
class ParticleEngine {
public:
	ParticleEngine(Camera3D* camera):_numberOfRenderedParticles(200) {
		_startPosition = Point(0,0,0);
		_startAcceleration = Vector3D(0,1,0);
		_engineActive = false;

		_camera = camera;
		Point from(_camera->getLookAt().from);
		_directionOfCamera = from - _startPosition;

		_maxNumberOfParticles = 300;

		for (int i = 0; i < _maxNumberOfParticles; i++) // Initialize all the particles
		{
			T* particle = new T();
			particle->resetParticle(_startPosition,_startAcceleration);
			particle->active = false;
			_particles.push_back(particle);
		}

		loadParticleTexture(_particles[0]->_particleTexturePath);

		_maxSize =10;
		_minSize = 1;
	}

	virtual ~ParticleEngine()
	{
		for (std::vector<Particle*>::iterator particleIterator = _particles.begin();particleIterator != _particles.end(); ++particleIterator) {
			Particle* particle = *particleIterator;
			delete particle;
		}
	}

	void loadParticleTexture(const std::string &textureFilePath) {
		_particleTexture = new TGATexture(textureFilePath);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	void setStartPosition(Point position)
	{
		_startPosition = position;
		Point from(_camera->getLookAt().from);
		_directionOfCamera = from - _startPosition;
	}

	Point getStartPosition()
	{
		return _startPosition;
	}

	void setStartAcceleration(Vector3D acceleration)
	{
		_startAcceleration = acceleration;
	}

	Vector3D getStartAcceleration()
	{
		return _startAcceleration;
	}

	void setMaxSize(float maxSize)
	{
		_maxSize = maxSize;
	}

	float getMaxSize() const
	{
		return _maxSize;
	}

	void setMinSize(float minSize)
	{
		_minSize = minSize;
	}

	float getMinSize() const
	{
		return _minSize;
	}

	Vector3D getDirectionOfCamera()
	{
		return Vector3D(_directionOfCamera.x,_directionOfCamera.y,_directionOfCamera.z);
	}

	void setCamera(Camera3D* camera)
	{
		_camera = camera;
	}

	void setActive(bool active)
	{
		_engineActive = active;
	}

	void update(float seconds)
	{
		// Loop through all the particles
		for (std::vector<Particle*>::iterator particleIterator = _particles.begin();particleIterator != _particles.end(); ++particleIterator) {
			Particle* particle = *particleIterator;

			if(_engineActive)
			{
				particle->active = true;
			}

			if(particle->timeToLive < 0)
			{
				particle->resetParticle(_startPosition,_startAcceleration);
				if(!_engineActive)
				{
					particle->active = false;
				}
			}
			particle->update(seconds);
		}
	}

	void draw()
	{

		glShadeModel(GL_SMOOTH); // Enable Smooth Shading
		glClearDepth(1.0f);                         // Depth Buffer Setup
		glDepthMask(GL_FALSE);                      // Disables Depth Testing
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);// Black background
		glEnable(GL_BLEND);// Enable blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// Type of blending to perform
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);// Really nice perspective calculations
		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);// Really nice point smoothing

		_numberOfRenderedParticles = 0;
		// loop through all the particles
		for (std::vector<Particle*>::iterator particleIterator = _particles.begin();particleIterator != _particles.end(); ++particleIterator) {
			Particle* particle = *particleIterator;
			// If the particle is active
			if (particle->active) {
				// Draw the particle using the RGB values, fade the particle based on its time to live
				glColor4f(particle->r, particle->g,particle->b,particle->timeToLive);

				float size = _minSize + rand()%1 * (_maxSize - _minSize);

				glTranslatef(particle->x,particle->y,particle->z);
				//rotating the sprite towards the camera
				Utils::applyGLRotation(Vector3D(0,0,1),getDirectionOfCamera());

				_particleTexture->setActive(true);
				// Build quad from a triangle strip
				glBegin(GL_TRIANGLE_STRIP);
				glTexCoord2d(_particleTexture->getWidth(), _particleTexture->getHeight());
				glVertex3f(size, size, 0);// Top right
				glTexCoord2d(0, _particleTexture->getHeight());
				glVertex3f( - size, size, 0);// Top left
				glTexCoord2d(_particleTexture->getWidth(), 0);
				glVertex3f(size, - size, 0);// Bottom right
				glTexCoord2d(0, 0);
				glVertex3f( - size, - size, 0);// Bottom left
				glEnd();
				_particleTexture->setActive(false);

				Utils::applyGLRotation(getDirectionOfCamera(),Vector3D(0,0,1));
				glTranslatef(-particle->x,-particle->y,-particle->z);
				_numberOfRenderedParticles++;
			}
		}
		glDepthMask(GL_TRUE);
	}

	int getNumberOfRenderedParticles()
	{
		return _numberOfRenderedParticles;
	}

private:
	int _maxNumberOfParticles;
	std::vector<Particle*> _particles;

	float _minSize;
	float _maxSize;

	Point _directionOfCamera;
	Camera3D* _camera;

	Point _startPosition;
	Vector3D _startAcceleration;

	TGATexture* _particleTexture;
	Particle* _particleType;
	bool _engineActive;

	int _numberOfRenderedParticles;
};

}
#endif
