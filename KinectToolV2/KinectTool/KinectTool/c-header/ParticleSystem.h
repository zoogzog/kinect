#pragma once

#include <vector>

#include "ofMain.h"

#include "Particle.h"

using namespace std;

namespace rndp
{
	struct Node
	{
		int particleID1;
		int particleID2;

		Node();
		Node(int id1, int id2);
	};

	class ParticleSystem
	{
	private:
		vector <Particle> systemParticles;
		vector <Node> systemNodes;

	public:
		ParticleSystem();
		ParticleSystem(int count);

		void setup(int count);

		void update();
		void draw();

	};
}