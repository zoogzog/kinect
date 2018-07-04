#include <iostream>
using namespace std;

#include "ParticleSystem.h"

using namespace rndp;


Node::Node()
{
	particleID1 = 0;
	particleID2 = 0;
}

Node::Node(int id1, int id2)
{
	particleID1 = id1;
	particleID2 = id2;
}
//--------------------------------------------------------------------------------
ParticleSystem::ParticleSystem()
{
}

ParticleSystem::ParticleSystem(int count)
{
	ofVec3f startpoint = ofVec3f(0.5, 0.5, 0);

	for (int i = 0; i < count; i++)
	{
		ofVec3f startvelocity = ofVec3f(ofRandomf(), ofRandomf(), 0);

		systemParticles.push_back(Particle(startpoint, startvelocity, 1, 0.001));
	}
}

//--------------------------------------------------------------------------------
void ParticleSystem::setup(int count)
{
	ofVec3f startpoint = ofVec3f(0.5, 0.5, 0);

	for (int i = 0; i < count; i++)
	{
		ofVec3f startvelocity = ofVec3f(ofRandomf(), ofRandomf(), 0);
		systemParticles.push_back(Particle(startpoint, startvelocity, 1, 0.001));
	}
}

void ParticleSystem::update()
{
	systemNodes.clear();

	for (int i = 0; i < systemParticles.size(); i++)
	{
		systemParticles.at(i).update();
	}


	for (int i = 0; i < systemParticles.size(); i++)
	{
		for (int j = 0; j < systemParticles.size(); j++)
		{

			if (i != j)
			{
					//---- Calculate distance
					ofVec3f p1 = systemParticles.at(i).getCoordinates();
					ofVec3f p2 = systemParticles.at(j).getCoordinates();

					double distance = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z));

					if (distance < 0.25)
					{
						systemNodes.push_back(Node(i, j));
					}
			}
		}
	}
}

void ParticleSystem::draw()
{
	ofPushStyle();
	ofSetColor(ofColor::white);
	for (int i = 0; i < systemParticles.size(); i++)
	{
		ofVec3f xyz = systemParticles.at(i).getCoordinates();

		ofVec3f c = ofVec3f(xyz.x *ofGetWidth(), xyz.y * ofGetHeight(), 0);

		ofDrawCircle(c, 3);
	}

	for (int i = 0; i < systemNodes.size(); i++)
	{
		int id1 = systemNodes.at(i).particleID1;
		int id2 = systemNodes.at(i).particleID2;

		ofVec3f xyz1 = systemParticles.at(id1).getCoordinates();
		xyz1.x *= ofGetWidth();
		xyz1.y *= ofGetHeight();
		xyz1.z = 0;

		ofVec3f xyz2 = systemParticles.at(id2).getCoordinates();
		xyz2.x *= ofGetWidth();
		xyz2.y *= ofGetHeight();
		xyz2.z = 0;

		ofDrawLine(xyz1, xyz2);
	}

	ofPopStyle();
}
