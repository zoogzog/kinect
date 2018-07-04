#include "Particle.h"

using namespace rndp;

//--------------------------------------------------------------------------------

Particle::Particle()
{
	pXYZ = ofVec3f(0, 0, 0);
	pVelocityDirection = ofVec3f(0, 0, 0);
	pVeloctiyMagnitude = 0;

	double norm = sqrt(pVelocityDirection.x * pVelocityDirection.x + pVelocityDirection.y * pVelocityDirection.y + pVelocityDirection.z * pVelocityDirection.z);

	pVelocityDirection.x /= norm;
	pVelocityDirection.y /= norm;
	pVelocityDirection.z /= norm;
}

Particle::Particle(ofVec3f xyz)
{
	pXYZ = xyz;
	pVelocityDirection = ofVec3f(0, 0, 0);
	pVeloctiyMagnitude = 0;

	double norm = sqrt(pVelocityDirection.x * pVelocityDirection.x + pVelocityDirection.y * pVelocityDirection.y + pVelocityDirection.z * pVelocityDirection.z);

	pVelocityDirection.x /= norm;
	pVelocityDirection.y /= norm;
	pVelocityDirection.z /= norm;
}

Particle::Particle(ofVec3f xyz, ofVec3f v, double m, float s)
{
	pXYZ = xyz;
	pVelocityDirection = v;
	pVeloctiyMagnitude = m;
	pSize = s;

	double norm = sqrt(pVelocityDirection.x * pVelocityDirection.x + pVelocityDirection.y * pVelocityDirection.y + pVelocityDirection.z * pVelocityDirection.z);

	pVelocityDirection.x /= norm;
	pVelocityDirection.y /= norm;
	pVelocityDirection.z /= norm;
}

//--------------------------------------------------------------------------------

void Particle::updateVelocity(ofVec3f v)
{
	pVelocityDirection.x += v.x;
	pVelocityDirection.y += v.y;
	pVelocityDirection.z += v.z;

	double norm = sqrt(pVelocityDirection.x * pVelocityDirection.x + pVelocityDirection.y * pVelocityDirection.y + pVelocityDirection.z * pVelocityDirection.z);

	pVelocityDirection.x /= norm;
	pVelocityDirection.y /= norm;
	pVelocityDirection.z /= norm;
}

void Particle::updateVelocity(ofVec3f v, double m)
{
	updateVelocity(v);

	pVeloctiyMagnitude += m;
}

void Particle::updateMagnitude(double m)
{
	pVeloctiyMagnitude += m;
}

void Particle::updateCoordinates(ofVec3f xyz)
{
	pXYZ.x += xyz.x;
	pXYZ.y += xyz.y;
	pXYZ.z += xyz.z;

	if (pXYZ.x > 1) { pXYZ.x = 1; } if (pXYZ.x < 0) { pXYZ.x = 0; }
	if (pXYZ.y > 1) { pXYZ.y = 1; } if (pXYZ.y < 0) { pXYZ.y = 0; }
	if (pXYZ.z > 1) { pXYZ.z = 1; } if (pXYZ.z < 0) { pXYZ.z = 0; }
}

void Particle::update()
{
	pVeloctiyMagnitude = 0.003;
	pXYZ.x += pVelocityDirection.x * pVeloctiyMagnitude;
	pXYZ.y += pVelocityDirection.y * pVeloctiyMagnitude;
	pXYZ.z += pVelocityDirection.z * pVeloctiyMagnitude;

	if (pXYZ.x > 1) { pXYZ.x = 1; pVelocityDirection.x *= -1; }
	if (pXYZ.x < 0) { pXYZ.x = 0; pVelocityDirection.x *= -1; }

	if (pXYZ.y > 1) { pXYZ.y = 1; pVelocityDirection.y *= -1; }
	if (pXYZ.y < 0) { pXYZ.y = 0; pVelocityDirection.y *= -1; }

	if (pXYZ.z > 1) { pXYZ.z = 1; pVelocityDirection.z *= -1; }
	if (pXYZ.z < 0) { pXYZ.z = 0; pVelocityDirection.z *= -1; }

}

//--------------------------------------------------------------------------------

void Particle::setVelocity(ofVec3f v)
{
	pVelocityDirection = v;
}

void Particle::setVelocity(ofVec3f v, double m)
{
	pVelocityDirection = v;
	pVeloctiyMagnitude = m;
}

void Particle::setCoordinates(ofVec3f xyz)
{
	pXYZ = xyz;
}

//--------------------------------------------------------------------------------

ofVec3f Particle::getVelocity()
{
	return pVelocityDirection;
}

ofVec3f Particle::getCoordinates()
{
	return pXYZ;
}

double Particle::getVelocityMagnitude()
{
	return pVeloctiyMagnitude;
}

//--------------------------------------------------------------------------------
