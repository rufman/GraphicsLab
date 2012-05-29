/**
 * robot.hpp
 * 
 * Author   :   Fatih Erol
 * Date     :   19.03.2012
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_ROBOT_HPP
#define GRAPHICSLAB_ROBOT_HPP

// Base class include
#include "../collisiondetection/Projectile.hpp"

// Includes
#include "../../../common/Material.hpp"

//particle engine includes
#include "../../particleEngine/ParticleEngine.hpp"
#include "../../particleEngine/Sparkle.hpp"

#include <string>
#include <map>

namespace game_space {

/** Robot */
class Robot : public Projectile
{
public:
	/**
	 * Constructor
	 *
	 * @param[in, out]	scene	Owning scene
	 */
	Robot( Scene &scene );

	/** Destructor */
	~Robot();

	/** Draw the object to the screen. */
	void draw() const;

	/** Reset the parameters */
	void reset();

	/** 
	 * Update robot for given amount of time
	 *
	 * @parma[in]	seconds		Seconds to update robot for
	 */
	void move( float seconds );

	/**
	 * Load animation from a file.
	 *
	 * @param[in]	animationFile	File to load
	 */
	void loadAnimation( const std::string &animationFile );

	bool isDetonated();

	void setVelocity( const Vector3D &velocity );

private:
	struct Animation
	{
		struct Frame
		{
			uint id;

			//movement variables
			//move body
			float mBodyX;
			float mBodyY;
			float mBodyZ;

			//rotate body
			float rBodyX;
			float rBodyY;
			float rBodyZ;

			float generalSize;

			//move head
			float mHeadX;
			float mHeadY;
			float mHeadZ;

			//rotate head
			float rHeadX;
			float rHeadY;
			float rHeadZ;

			//move left eye
			float mLeftEyeZ;

			//move right eye
			float mRightEyeZ;

			//move left mustache
			float mLeftMustacheX;
			float mLeftMustacheY;
			float mLeftMustacheZ;

			//rotate left mustache
			float rLeftMustacheY;

			//move left mustache
			float mRightMustacheX;
			float mRightMustacheY;
			float mRightMustacheZ;

			//rotate left mustache
			float rRightMustacheY;

			//move top hat
			float mTopHatY;

			//rotate top hat
			float rTopHatX;

			//rotate the left arm
			float rLeftArmX;
			float rLeftArmY;
			float rLeftArmZ;

			//rotate the right arm
			float rRightArmX;
			float rRightArmY;
			float rRightArmZ;

			//rotate the left leg
			float rLeftLegX;
			float rLeftLegY;
			float rLeftLegZ;

			//rotate the right leg
			float rRightLegX;
			float rRightLegY;
			float rRightLegZ;
		};

		typedef std::map<uint, Frame> FrameVector;
		FrameVector frames; // Vector of frames
		uint currentFrame;//<	Current frame ID

		void reset()
		{
			frames.clear();
			currentFrame = 0;
		}
	};
	Animation _animation;

private:
	/** Surface materials */
	Material _bodyMaterial;
	Material _eyeMaterial;
	Material _topHatMaterial;
	Material _mustacheMaterial;
	Material _antennaMaterial;
	Material _spaceMaterial;

	Vector3D _velocity;
	Vector3D _acceleration;

	float _timeToLive;

	ParticleEngine<Sparkle> *_sparkleParticleEngine;


}; // class Robot

}

#endif // GRAPHICSLAB_ROBOT_HPP
