#pragma once

#include "ofMain.h"

namespace rndp
{
	class Particle
	{
	private:
		//---- Coordinates of this particle
		ofVec3f pXYZ;

		//---- Velocity of this particle
		ofVec3f pVelocityDirection;
		double pVeloctiyMagnitude;

		//---- Size of this particle 
		float pSize;
	public:
		Particle();
		Particle(ofVec3f xyz);
		Particle(ofVec3f xyz, ofVec3f v, double m, float s);

		void updateVelocity(ofVec3f v);
		void updateVelocity(ofVec3f v, double m);
		void updateMagnitude(double m);
		void updateCoordinates(ofVec3f xyz);
	
		void setVelocity(ofVec3f v);
		void setVelocity(ofVec3f v, double m);
		void setCoordinates(ofVec3f xyz);

		ofVec3f getVelocity();
		ofVec3f getCoordinates();
		double getVelocityMagnitude();

		void update();

	};
}