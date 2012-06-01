/**
 * robot.cpp
*	Author: Benjamin Ellenberger
 */

// Class declaration include
#include "Robot.hpp"

// common includes
#include "../../../common/Exception.hpp"
#include "../../../common/Material.hpp"
#include "../../../common/Utils.hpp"

//scene includes
#include "../../Scene.hpp"
#include "../../Terrain.hpp"

#include <fstream>
#include <sstream>
#include <cmath>
#include <iostream>


namespace game_space{


Robot::Robot( Scene &scene ) :
	Projectile( scene,Projectile::ROBOT,0 ),_timeToLive(0),_velocity(Vector3D(0,0,0)),_acceleration(Vector3D(0,0,0))
{

	_bodyMaterial.setAmbient( Color( 0.0, 1.0, 0.1 ) );
	_bodyMaterial.setDiffuse( Color( 0.0, 1.0, 0.1 ) );
	_bodyMaterial.setSpecular( Color( 0.5, 0.5, 0.5 ) );
	_bodyMaterial.setEmission( Color( 0.0, 0.0, 0.0 ) );
	_bodyMaterial.setShininess( 15 );

	_antennaMaterial.setAmbient( Color( 0.0, 1.0, 0.1 ) );
	_antennaMaterial.setDiffuse( Color( 0.0, 1.0, 0.1 ) );
	_antennaMaterial.setSpecular( Color( 0.5, 0.5, 0.5 ) );
	_antennaMaterial.setEmission( Color( 0.0, 0.0, 0.0 ) );
	_antennaMaterial.setShininess( 15 );

	_eyeMaterial.setAmbient( Color( 0.01, 0.01, 0.01 ) );
	_eyeMaterial.setDiffuse( Color( 0.01, 0.01, 0.01 ) );
	_eyeMaterial.setSpecular( Color( 1.0, 1.0, 1.0 ) );
	_eyeMaterial.setEmission( Color( 0.01, 0.01, 0.01 ) );
	_eyeMaterial.setShininess( 15 );

	_topHatMaterial.setAmbient( Color( 0.05, 0.05, 0.05 ) );
	_topHatMaterial.setDiffuse( Color( 1.0, 1.0, 1.0) );
	_topHatMaterial.setSpecular( Color( 1.0, 1.0, 1.0 ) );
	_topHatMaterial.setEmission( Color( 0.0, 0.0, 0.0 ) );
	_topHatMaterial.setShininess( 100 );

	_mustacheMaterial.setAmbient( Color( 0.5, 0.3, 0.0 ) );
	_mustacheMaterial.setDiffuse( Color( 0.5, 0.3, 0.0 ) );
	_mustacheMaterial.setSpecular( Color( 0.5, 0.5, 0.5 ) );
	_mustacheMaterial.setEmission( Color( 0.0, 0.0, 0.0 ) );
	_mustacheMaterial.setShininess( 15 );


	_spaceMaterial.setAmbient( Color( 1.0, 1.0, 1.0 ) );
	_spaceMaterial.setDiffuse( Color( 1.0, 1.0, 1.0 ) );
	_spaceMaterial.setSpecular( Color( 1.0, 1.0, 1.0 ) );
	_spaceMaterial.setEmission( Color( 0.0, 0.0, 0.0 ) );
	_spaceMaterial.setShininess( 15 );
	/////////////////////////////////////////////////////////////

	Animation::Frame frame;
	frame.id = 0;
	//body movement/rotation
	frame.mBodyX = 0; frame.mBodyY = 0.0; frame.mBodyZ = 0.0;frame.generalSize = 1.0;
	frame.rBodyX = 0; frame.rBodyY = 0; frame.rBodyZ = 0;

	//arm rotation
	frame.rLeftArmX = 0;frame.rLeftArmY = 0; frame.rLeftArmZ = 0;
	frame.rRightArmX = 0;frame.rRightArmY = 0; frame.rRightArmZ = 0;

	//leg rotation
	frame.rLeftLegX = 0;frame.rLeftLegY = 0; frame.rLeftLegZ = 0;
	frame.rRightLegX = 0;frame.rRightLegY = 0; frame.rRightLegZ = 0;

	// head movement/rotation
	frame.mHeadX = 0;frame.mHeadY = 0;frame.mHeadZ = 0;
	frame.rHeadX = 0;frame.rHeadY = 0;frame.rHeadZ = 0;

	//move eyes
	frame.mLeftEyeZ = 0;frame.mRightEyeZ  = 0;

	//mustache movement/rotation
	frame.mLeftMustacheX = 0;frame.mLeftMustacheY = 0;frame.mLeftMustacheZ = 0;
	frame.rLeftMustacheY = 0;
	frame.mRightMustacheX = 0;frame.mRightMustacheY = 0;frame.mRightMustacheZ = 0;
	frame.rRightMustacheY = 0;

	//top hat movement/rotation
	frame.rTopHatX = 0;
	frame.mTopHatY = 0;

	_animation.frames[frame.id] = frame;

    reset();

    _sparkleParticleEngine = new ParticleEngine<Sparkle>(_scene.getCurrentlyActiveCamera(), 30);
    _sparkleParticleEngine->setStartAcceleration(Vector3D(1, 1, 1));
    _sparkleParticleEngine->setActive(true);
}

Robot::~Robot()
{
	//nothing to do here!	0,,
	//9gag.com			   DO
	//					  *//
delete _sparkleParticleEngine;
}


void Robot::reset()
{
	_animation.currentFrame = 0;
}

void Robot::move( float seconds )
{
	_timeToLive += seconds;

	//ballistics of a projectile
	_position.x += _velocity.x*seconds;
	_position.y += _velocity.y*seconds + 0.5*GRAVITATIONAL_ACCELERATION*seconds*seconds;
	_position.z += _velocity.z*seconds;

	_velocity.y += GRAVITATIONAL_ACCELERATION*seconds;

	//if the robot hits the ground and the time to live is over it detonates
	if ((_position.y < _scene.getTerrain().getHeight(_position))) {
		if(_timeToLive > ROBOT_TIMETOLIVE)
		{
			//do area damage
			Point left,right,front,back;
			left = _position;
			left.x += 3;
			right = _position;
			right.x -= 3;
			front = _position;
			front.z += 3;
			back = _position;
			back.z -= 3;

		//supermassive blackhole
		_scene.getTerrain().doDamageAt(left, 0.75);
		_scene.getTerrain().doDamageAt(right, 0.75);
		_scene.getTerrain().doDamageAt(front, 0.75);
		_scene.getTerrain().doDamageAt(back, 0.75);
		_scene.getTerrain().doDamageAt(_position, 0.75);
		_detonated = true;
		}
		else
		{
			//time to live not finished, the robot bounces off the ground
			_velocity.y = -_velocity.y;
		}
	}

	uint lastFrame = _animation.frames.rbegin()->first;
	if ( _animation.currentFrame == lastFrame )
	{
		_animation.currentFrame = _animation.frames.begin()->first;
	}
	else
	{
		_animation.currentFrame++;
	}

	_sparkleParticleEngine->setStartPosition(_position);
	_sparkleParticleEngine->update(seconds);
}

void Robot::loadAnimation( const std::string &animationFile )
{
	std::ifstream file( animationFile.c_str() );	
    if ( !file.is_open() )
    {
        std::stringstream error;
        error << "Failed to open animation file \"" << animationFile << "\"";
        throw Exception( error.str() );
    }
	
	_animation.reset();

	//drop out the comment line
	char skip[400];
	file.getline(skip,400,'\n');;

	while ( !file.eof() )
	{
		Animation::Frame frame;
		file >> frame.id >> frame.mBodyX >> frame.mBodyY >> frame.mBodyZ >> frame.generalSize

		//rotate body
		>> frame.rBodyX >> frame.rBodyY >> frame.rBodyZ

		//move head
		>> frame.mHeadX >> frame.mHeadY >> frame.mHeadZ

		//rotate head
		>> frame.rHeadX >> frame.rHeadY >> frame.rHeadZ

		//move left eye
		>> frame.mLeftEyeZ

		//move right eye
		>> frame.mRightEyeZ

		//move left mustache
		>> frame.mLeftMustacheX >> frame.mLeftMustacheY >> frame.mLeftMustacheZ

		//rotate left mustache
		>> frame.rLeftMustacheY

		//move left mustache
		>> frame.mRightMustacheX >> frame.mRightMustacheY >> frame.mRightMustacheZ

		//rotate left mustache
		>> frame.rRightMustacheY

		//move top hat
		>> frame.mTopHatY

		//rotate top hat
		>> frame.rTopHatX

		//rotate the left arm
		>> frame.rLeftArmX >> frame.rLeftArmY >> frame.rLeftArmZ

		//rotate the right arm
		>> frame.rRightArmX >> frame.rRightArmY >> frame.rRightArmZ

		//rotate the left leg
		>> frame.rLeftLegX >> frame.rLeftLegY >> frame.rLeftLegZ

		//rotate the right leg
		>> frame.rRightLegX >> frame.rRightLegY >> frame.rRightLegZ;

		if ( frame.id == 0 )
		{
			_animation.frames[frame.id] = frame;
		}
		else
		{
			const Animation::Frame &lastFrame = _animation.frames.rbegin()->second;

			for ( uint id = ( lastFrame.id + 1 ); id <= frame.id; id++ )
			{
				Animation::Frame f;
				f.id = id;

				//movement of the body
				f.mBodyX = lastFrame.mBodyX + ( id - lastFrame.id )*( frame.mBodyX - lastFrame.mBodyX )/(frame.id - lastFrame.id );
				f.mBodyY = lastFrame.mBodyY + ( id - lastFrame.id )*( frame.mBodyY - lastFrame.mBodyY )/(frame.id - lastFrame.id );
				f.mBodyZ = lastFrame.mBodyZ + ( id - lastFrame.id )*( frame.mBodyZ - lastFrame.mBodyZ )/(frame.id - lastFrame.id );

				f.rBodyX = lastFrame.rBodyX + ( id - lastFrame.id )*( frame.rBodyX - lastFrame.rBodyX )/(frame.id - lastFrame.id );
				f.rBodyY = lastFrame.rBodyY + ( id - lastFrame.id )*( frame.rBodyY - lastFrame.rBodyY )/(frame.id - lastFrame.id );
				f.rBodyZ = lastFrame.rBodyZ + ( id - lastFrame.id )*( frame.rBodyZ - lastFrame.rBodyZ )/(frame.id - lastFrame.id );

				f.generalSize =  lastFrame.generalSize + ( id - lastFrame.id )*( frame.generalSize - lastFrame.generalSize )/(frame.id - lastFrame.id );

				//move head
				f.mHeadX = lastFrame.mHeadX + ( id - lastFrame.id )*( frame.mHeadX - lastFrame.mHeadX )/(frame.id - lastFrame.id );
				f.mHeadY = lastFrame.mHeadY + ( id - lastFrame.id )*( frame.mHeadY - lastFrame.mHeadY )/(frame.id - lastFrame.id );
				f.mHeadZ = lastFrame.mHeadZ + ( id - lastFrame.id )*( frame.mHeadZ - lastFrame.mHeadZ )/(frame.id - lastFrame.id );

				//rotate head
				f.rHeadX = lastFrame.rHeadX + ( id - lastFrame.id )*( frame.rHeadX - lastFrame.rHeadX )/(frame.id - lastFrame.id );
				f.rHeadY = lastFrame.rHeadY + ( id - lastFrame.id )*( frame.rHeadY - lastFrame.rHeadY )/(frame.id - lastFrame.id );
				f.rHeadZ = lastFrame.rHeadZ + ( id - lastFrame.id )*( frame.rHeadZ - lastFrame.rHeadZ )/(frame.id - lastFrame.id );

				//move left eye
				f.mLeftEyeZ = lastFrame.mLeftEyeZ + ( id - lastFrame.id )*( frame.mLeftEyeZ - lastFrame.mLeftEyeZ )/(frame.id - lastFrame.id );

				//move right eye
				f.mRightEyeZ = lastFrame.mRightEyeZ + ( id - lastFrame.id )*( frame.mRightEyeZ - lastFrame.mRightEyeZ )/(frame.id - lastFrame.id );

				//move left mustache
				f.mLeftMustacheX = lastFrame.mLeftMustacheX + ( id - lastFrame.id )*( frame.mLeftMustacheX - lastFrame.mLeftMustacheX )/(frame.id - lastFrame.id );
				f.mLeftMustacheY = lastFrame.mLeftMustacheY + ( id - lastFrame.id )*( frame.mLeftMustacheY - lastFrame.mLeftMustacheY )/(frame.id - lastFrame.id );
				f.mLeftMustacheZ = lastFrame.mLeftMustacheZ + ( id - lastFrame.id )*( frame.mLeftMustacheZ - lastFrame.mLeftMustacheZ )/(frame.id - lastFrame.id );

				//rotate left mustache
				f.rLeftMustacheY = lastFrame.rLeftMustacheY + ( id - lastFrame.id )*( frame.rLeftMustacheY - lastFrame.rLeftMustacheY )/(frame.id - lastFrame.id );

				//move left mustache
				f.mRightMustacheX = lastFrame.mRightMustacheX + ( id - lastFrame.id )*( frame.mRightMustacheX - lastFrame.mRightMustacheX )/(frame.id - lastFrame.id );
				f.mRightMustacheY = lastFrame.mRightMustacheY + ( id - lastFrame.id )*( frame.mRightMustacheY - lastFrame.mRightMustacheY )/(frame.id - lastFrame.id );
				f.mRightMustacheZ = lastFrame.mRightMustacheZ + ( id - lastFrame.id )*( frame.mRightMustacheZ - lastFrame.mRightMustacheZ )/(frame.id - lastFrame.id );

				//rotate left mustache
				f.rRightMustacheY = lastFrame.rRightMustacheY + ( id - lastFrame.id )*( frame.rRightMustacheY - lastFrame.rRightMustacheY )/(frame.id - lastFrame.id );

				//move top hat
				f.mTopHatY = lastFrame.mTopHatY + ( id - lastFrame.id )*( frame.mTopHatY - lastFrame.mTopHatY )/(frame.id - lastFrame.id );

				//rotate top hat
				f.rTopHatX = lastFrame.rTopHatX + ( id - lastFrame.id )*( frame.rTopHatX - lastFrame.rTopHatX )/(frame.id - lastFrame.id );

				//rotate the left arm
				f.rLeftArmX = lastFrame.rLeftArmX + ( id - lastFrame.id )*( frame.rLeftArmX - lastFrame.rLeftArmX )/(frame.id - lastFrame.id );
				f.rLeftArmY = lastFrame.rLeftArmY + ( id - lastFrame.id )*( frame.rLeftArmY - lastFrame.rLeftArmY )/(frame.id - lastFrame.id );
				f.rLeftArmZ = lastFrame.rLeftArmZ + ( id - lastFrame.id )*( frame.rLeftArmZ - lastFrame.rLeftArmZ )/(frame.id - lastFrame.id );

				//rotate the right arm
				f.rRightArmX = lastFrame.rRightArmX + ( id - lastFrame.id )*( frame.rRightArmX - lastFrame.rRightArmX )/(frame.id - lastFrame.id );
				f.rRightArmY = lastFrame.rRightArmY + ( id - lastFrame.id )*( frame.rRightArmY - lastFrame.rRightArmY )/(frame.id - lastFrame.id );
				f.rRightArmZ = lastFrame.rRightArmZ + ( id - lastFrame.id )*( frame.rRightArmZ - lastFrame.rRightArmZ )/(frame.id - lastFrame.id );

				//rotate the left leg
				f.rLeftLegX = lastFrame.rLeftLegX + ( id - lastFrame.id )*( frame.rLeftLegX - lastFrame.rLeftLegX )/(frame.id - lastFrame.id );
				f.rLeftLegY = lastFrame.rLeftLegY + ( id - lastFrame.id )*( frame.rLeftLegY - lastFrame.rLeftLegY )/(frame.id - lastFrame.id );
				f.rLeftLegZ = lastFrame.rLeftLegZ + ( id - lastFrame.id )*( frame.rLeftLegZ - lastFrame.rLeftLegZ )/(frame.id - lastFrame.id );

				//rotate the right leg
				f.rRightLegX = lastFrame.rRightLegX + ( id - lastFrame.id )*( frame.rRightLegX - lastFrame.rRightLegX )/(frame.id - lastFrame.id );
				f.rRightLegY = lastFrame.rRightLegY + ( id - lastFrame.id )*( frame.rRightLegY - lastFrame.rRightLegY )/(frame.id - lastFrame.id );
				f.rRightLegZ = lastFrame.rRightLegZ + ( id - lastFrame.id )*( frame.rRightLegZ - lastFrame.rRightLegZ )/(frame.id - lastFrame.id );
				_animation.frames[f.id] = f;
			}
		}
	}
}

void Robot::draw() const
{

	glPushMatrix();
	_sparkleParticleEngine->draw();
	glPopMatrix();



		glShadeModel( ( _renderingParameters.shadeMode == RenderingParameters::FLAT ) ? GL_FLAT : GL_SMOOTH );
		glPolygonMode( GL_FRONT_AND_BACK, ( _renderingParameters.drawMode == RenderingParameters::WIREFRAME ) ? GL_LINE : GL_FILL );

		const Animation::Frame &frame = ( _animation.frames.find( _animation.currentFrame ) )->second;

		//this is the overall size to resize the whole android
		float generalSize = frame.generalSize;

		float armLength = 0.6*generalSize;
		float armWidth = 0.1*generalSize;

		float legLength = 0.4*generalSize;
		float legWidth = 0.1*generalSize;

		float bodyWidth = 0.6*generalSize;
		float bodyHeight = 0.75*generalSize;

		float antennaLength = 0.4*generalSize;
		float antennaAngle = 20;

		float eyeSize = 0.04*generalSize;
		float eyeHeightRad = Utils::PI/10;
		float eyeDistanceDeg = 40;

		float mustacheHeightRad = Utils::PI/30;
		float mustacheSize = 0.07*generalSize;

		float topHatHeight = 0.7*generalSize;
		float topHatWidth  = 0.6*generalSize;
		float topHatBrimHeight = 0.05*generalSize;

		//movement variables
		//move body
		float mBodyX = frame.mBodyX;
		float mBodyY = frame.mBodyY;
		float mBodyZ = frame.mBodyZ;

		//rotate body
		float rBodyX = frame.rBodyX;
		float rBodyY = frame.rBodyY;
		float rBodyZ = frame.rBodyZ;

		//move head
		float mHeadX = frame.mHeadX;
		float mHeadY = frame.mHeadY;
		float mHeadZ = frame.mHeadZ;

		//rotate head
		float rHeadX = frame.rHeadX;
		float rHeadY = frame.rHeadY;
		float rHeadZ = frame.rHeadZ;

		//move left eye
		float mLeftEyeZ = frame.mLeftEyeZ;

		//move right eye
		float mRightEyeZ = frame.mRightEyeZ;

		//move left mustache
		float mLeftMustacheX = frame.mLeftMustacheX;
		float mLeftMustacheY = frame.mLeftMustacheY;
		float mLeftMustacheZ = frame.mLeftMustacheZ;

		//rotate left mustache
		float rLeftMustacheY  = frame.rLeftMustacheY;

		//move left mustache
		float mRightMustacheX = frame.mRightMustacheX;
		float mRightMustacheY = frame.mRightMustacheY;
		float mRightMustacheZ = frame.mRightMustacheZ;

		//rotate left mustache
		float rRightMustacheY = frame.rRightMustacheY;

		//move top hat
		float mTopHatY = frame.mTopHatY;

		//rotate top hat
		float rTopHatX = frame.rTopHatX;

		//rotate the left arm
		float rLeftArmX = frame.rLeftArmX;
		float rLeftArmY = frame.rLeftArmY;
		float rLeftArmZ = frame.rLeftArmZ;

		//rotate the right arm
		float rRightArmX = frame.rRightArmX;
		float rRightArmY = frame.rRightArmY;
		float rRightArmZ = frame.rRightArmZ;

		//rotate the left leg
		float rLeftLegX = frame.rLeftLegX;
		float rLeftLegY = frame.rLeftLegY;
		float rLeftLegZ = frame.rLeftLegZ;

		//rotate the right leg
		float rRightLegX = frame.rRightLegX;
		float rRightLegY = frame.rRightLegY;
		float rRightLegZ = frame.rRightLegZ;

		GLUquadric* quadric = gluNewQuadric();

		glMatrixMode( GL_MODELVIEW );



		//the body is the highest in the hierarchy
		_bodyMaterial.setActive();
		glPushMatrix();
			//ANIMATION OF THE WHOLE BODY (jumping,moving,rotation)
			glTranslatef(_position.x+mBodyX,_position.y+mBodyY,_position.z+mBodyZ);
			glRotatef(rBodyY,0,1.0,0);//to achieve that the mesh can rotate and bow
			glRotatef(rBodyX,1.0,0,0);
			glRotatef(rBodyZ,0,0,1.0);

			//draw the body
			glTranslatef(0.0,legLength,0.0);

			//translate to the ground first
			glTranslatef(0.0,bodyWidth*1.4,0.0);
			glRotatef(90,1.0,0.0,0.0);
			gluCylinder(quadric,bodyWidth,bodyWidth,bodyHeight,50,30);

			glPushMatrix();
				//ANIMATION OF THE HEAD (Tilt,rotate, look at, move up/down)
				glRotatef(rHeadX,1.0,0,0);
				glRotatef(rHeadY,0,0,1.0);
				glRotatef(rHeadZ,0,0,1.0);
				glTranslatef(mHeadX,mHeadZ,-mHeadY);

				//draw the head
				glRotatef(-90,1.0,0.0,0.0);
				glutSolidSphere(bodyWidth,30,30);


				_eyeMaterial.setActive();
				//draw the eyes
				glPushMatrix();
					//ANIMATION OF THE LEFT EYE (move in/out)
					glTranslatef(0,0,-mLeftEyeZ);

					glRotatef(eyeDistanceDeg/2,0.0,1.0,0.0);
					glTranslatef(0.0,bodyWidth*sin(eyeHeightRad),bodyWidth*cos(eyeHeightRad));
					glutSolidSphere(eyeSize,30,30);
				glPopMatrix();

				glPushMatrix();
					//ANIMATION OF THE RIGHT EYE (move in/out)
					glTranslatef(0,0,-mRightEyeZ);

					glRotatef(-eyeDistanceDeg/2,0.0,1.0,0.0);
					glTranslatef(0.0,bodyWidth*sin(eyeHeightRad),bodyWidth*cos(eyeHeightRad));
					glutSolidSphere(eyeSize,30,30);
				glPopMatrix();

				_antennaMaterial.setActive();
				//draw the first antenna
				glPushMatrix();
					glTranslatef(-antennaLength*1.5*sin(Utils::toRadian(antennaAngle)),antennaLength*1.5*cos(Utils::toRadian(antennaAngle)),0.0);
					glRotatef(antennaAngle,0.0,0.0,1.0);
					glRotatef(-90,1.0,0.0,0.0);
					gluCylinder(quadric,bodyWidth/15,bodyWidth/15,antennaLength,12,12);
				glPopMatrix();


				//draw the second antenna
				glPushMatrix();
					glTranslatef(antennaLength*1.5*sin(Utils::toRadian(antennaAngle)),antennaLength*1.5*cos(Utils::toRadian(antennaAngle)),0.0);
					glRotatef(-antennaAngle,0.0,0.0,1.0);
					glRotatef(-90,1.0,0.0,0.0);
					gluCylinder(quadric,bodyWidth/15,bodyWidth/15,antennaLength,12,12);
				glPopMatrix();



				_mustacheMaterial.setActive();
				//draw the mustache
				glPushMatrix();
				//ANIMATION OF THE LEFT MOUSTACHE (move up/down)
				glTranslatef(mLeftMustacheX,-mLeftMustacheY,mLeftMustacheZ);
				glRotatef(rLeftMustacheY,0,1.0,0);

				glTranslatef(-mustacheSize,bodyWidth*sin(mustacheHeightRad),bodyWidth*cos(mustacheHeightRad));
				glRotatef(90,0.0,1.0,0.0);
				glutSolidCone(mustacheSize,mustacheSize*2,10,10);
				glPushMatrix();
					glRotatef(-180,0.0,1.0,0.0);
					glutSolidCone(mustacheSize,mustacheSize*4,10,10);
				glPopMatrix();
				glPopMatrix();


				glPushMatrix();
				//ANIMATION OF THE RIGHT MOUSTACHE (move up/down)
				glTranslatef(mRightMustacheX,-mRightMustacheY,mRightMustacheZ);
				glRotatef(rRightMustacheY,0,1.0,0);

				glTranslatef(mustacheSize,bodyWidth*sin(mustacheHeightRad),bodyWidth*cos(mustacheHeightRad));
				glRotatef(-90,0.0,1.0,0.0);
				glutSolidCone(mustacheSize,mustacheSize*2,10,10);
				glPushMatrix();
					glRotatef(180,0.0,1.0,0.0);
					glutSolidCone(mustacheSize,mustacheSize*4,10,10);
				glPopMatrix();
				glPopMatrix();



				_topHatMaterial.setActive();
				//draw the tophat
				glPushMatrix();
					//ANIMATION OF THE TOP Hat (move up/down)
					glRotatef(rTopHatX,0,0,1.0);
					glTranslatef(0,mTopHatY,0);


					//brim
					glTranslatef(0,bodyWidth*7/8,0);
					glRotatef(90,1.0,0.0,0.0);
					gluCylinder(quadric,topHatWidth,topHatWidth,topHatBrimHeight,30,1);


					//close the brim on the upper side
					glPushMatrix();
						glTranslatef(0,topHatBrimHeight,0);
						glRotatef(90,0.0,0,1.0);
						glutSolidCone(topHatWidth,0.01,30,1);
					glPopMatrix();

					//close the hat on the lower side
					glPushMatrix();
						glRotatef(90,0,0,1.0);
						glutSolidCone(topHatWidth,0.01,30,1);
					glPopMatrix();

					//top part
					glPushMatrix();
						glTranslatef(0,0,-topHatHeight);
						glRotatef(90,0,0,1.0);
						gluCylinder(quadric,topHatWidth/2,topHatWidth/2,topHatHeight,30,30);
					glPopMatrix();

					//close the top
					glPushMatrix();
						glTranslatef(0,0,-topHatHeight);
						glRotatef(90,0,0,1.0);
						glutSolidCone(topHatWidth/2,0.01,30,1);
					glPopMatrix();

				glPopMatrix();
			glPopMatrix();




			_bodyMaterial.setActive();
			//draw the left arm
			glPushMatrix();
				glTranslatef(1.3*bodyWidth,0,0);
				glutSolidSphere(armWidth,30,30);
				glPushMatrix();
					//ANIMATION OF THE LEFT ARM (rotate in all directions)
					glRotatef(rLeftArmX,1.0,0,0);
					glRotatef(rLeftArmZ,0,1.0,0);
					glRotatef(rLeftArmY,0,0,1.0);
					gluCylinder(quadric,armWidth,armWidth,armLength,30,30);
					glPushMatrix();
						glTranslatef(0,0,armLength);
						glutSolidSphere(armWidth,30,30);
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();

			//draw the right arm
			glPushMatrix();
				glTranslatef(1.3*-bodyWidth,0,0);
				glutSolidSphere(armWidth,30,30);
				glPushMatrix();
					//ANIMATION OF THE RIGHT ARM (rotate in all directions)
					glRotatef(rRightArmX,1.0,0,0);
					glRotatef(rRightArmZ,0,1.0,0);
					glRotatef(rRightArmY,0,0,1.0);
					gluCylinder(quadric,armWidth,armWidth,armLength,30,30);
					glPushMatrix();
						glTranslatef(0,0,armLength);
						glutSolidSphere(armWidth,30,30);
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();

			//draw the left leg
			glPushMatrix();
				glTranslatef(bodyWidth*2/3,0,bodyHeight);
				glutSolidSphere(legWidth,30,30);
				glPushMatrix();
					//ANIMATION OF THE LEFT LEG (rotate in all directions)
					glRotatef(rLeftLegX,1.0,0,0);
					glRotatef(rLeftLegZ,0,1.0,0);
					glRotatef(rLeftLegY,0,0,1.0);
					gluCylinder(quadric,legWidth,legWidth,legLength,30,30);
					glPushMatrix();
						glTranslatef(0,0,legLength);
						glutSolidSphere(legWidth,30,30);
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();

			//draw the right leg
			glPushMatrix();
				glTranslatef(-bodyWidth*2/3,0,bodyHeight);
				glutSolidSphere(legWidth,30,30);
				glPushMatrix();
				//ANIMATION OF THE RIGHT LEG (rotate in all directions)
					glRotatef(rRightLegX,1.0,0,0);
					glRotatef(rRightLegZ,0,1.0,0);
					glRotatef(rRightLegY,0,0,1.0);
					gluCylinder(quadric,legWidth,legWidth,legLength,30,30);
					glPushMatrix();
						glTranslatef(0,0,legLength);
						glutSolidSphere(legWidth,30,30);
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();

		//FIXME:this should not be necessary
	glPopMatrix();

}

bool Robot::isDetonated()
{
	return _detonated;
}

void Robot::setVelocity( const Vector3D &velocity )
{
	_velocity = velocity;
}

}
