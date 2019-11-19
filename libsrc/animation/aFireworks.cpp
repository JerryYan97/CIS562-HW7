// ParticleSystem.cpp: implementation of the AFireworks class.
//
//////////////////////////////////////////////////////////////////////

#include "aFireworks.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <time.h>


#ifndef RAD
#define PI 3.14159265358979f
#define RAD (PI / 180.0f)
#endif
#ifndef GRAVITY
#define GRAVITY 9.8f
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AFireworks::AFireworks()
{
    m_deltaT = 0.033f;
	m_rocketMass = 50.0;
	m_sparkMass = 1.0;

	m_attractorPos = vec3(0.0, 500.0, 0.0);
	m_repellerPos = vec3(0.0, 500.0, 0.0);
	m_windForce = vec3(250.0, 0.0, 0.0);

}

AFireworks::~AFireworks()
{

	for (unsigned int i = 0; i < sparks.size(); i++)
	{
		ASpark* pSpark = sparks[i];
		delete pSpark;
	}
	for (unsigned int i = 0; i < rockets.size(); i++)
	{
		ARocket* pRocket = rockets[i];
		delete pRocket;
	}

	rockets.clear();
	sparks.clear();

}

void AFireworks::display()
{

	for (unsigned int i = 0; i < sparks.size(); i++)
	{
		ASpark* pSpark = sparks[i];
		pSpark->display();
	}
	
	for (unsigned int i = 0; i < rockets.size(); i++)
	{
		ARocket* pRocket = rockets[i];
		pRocket->display();
	}

}

int AFireworks::getNumParticles()
{
	return sparks.size() + rockets.size();
}


void AFireworks::fireRocket(float posx, float* color)
	/*	fireRocket is called by AFireworksViewer::onKeyboard() function, when user presses the space key.
	*  Input:	float posx. X launch position of the rocket. Y launch position is always set to 0.
	*			float* color. color[0], color[1] and color[2] are the RGB color of this rocket.
	*                        It changes according to posx and it is provided for you.
	*  In this function, you want to generate a ARocket object and add it to the  rockets vector
	*  The initial state of the ARocket object is as follows:
	*      Position is posx and posy.
	*      launch angle is a random value between 80 and 110 degrees, assume the X axis is 0 degrees.
	*      Speed is a random number from 60 to 120.
	*      vertical force is mass * GRAVITY.
	*      rocket mass is 50.
	*      timeSpan (i.e. time to live) is 10.0
	*/
{
	float posy = 0.0f;
	ARocket *newRocket = new ARocket(color);

	// need to compute and set values for initial state of rocket  (including time to live)
	float stateVals[12] = { 0.0 };
	
	//TODO: Add your code here
	
	vec3 velocityDir = vec3(1, 0, 0);
	srand(time(0));
	double randomZeroToOne = ((rand()) % 10) / 10.f;
	double launchAngleOffset = 30 * randomZeroToOne;
	double launchAngle = 80 + launchAngleOffset;
	double speedOffset = 60 * randomZeroToOne;
	double speed = 60 + speedOffset;

	mat3 rotationMat; 
	rotationMat.FromAxisAngle(vec3(0, 0, 1), Deg2Rad * launchAngle);
	velocityDir = rotationMat * velocityDir;

	// Now the velocityDir's representation is changed.
	velocityDir *= speed;

	// pos:
	stateVals[0] = posx;
	// velocity:
	stateVals[3] = velocityDir[0];
	stateVals[4] = velocityDir[1];
	stateVals[5] = velocityDir[2];
	// vertical force:
	stateVals[7] =  - m_rocketMass * GRAVITY;
	// mass:
	stateVals[9] = m_rocketMass;
	std::cout << "m_rocketMass: " << m_rocketMass << std::endl;
	// time to live:
	stateVals[10] = 10.f;
	
	newRocket->setState(stateVals);
	newRocket->setAttractor(m_attractorPos);
	newRocket->setRepeller(m_repellerPos);
	newRocket->setWind(m_windForce);
	rockets.push_back(newRocket);
}


// This only produce a ring ?
void AFireworks::explode(float rocketPosx, float rocketPosy, float rocketPosz, 
	                     float rocketVelx, float rocketVely, float rocketVelz, float* rocketColor)

	/*	explode is called in AFireworks::update() when a rocket reaches its top height.
	*  It is called ARocket::TOTALEXPLOSIONS times to generate a series of rings of sparks.
	*  Input: float posx. X position where a ring of sparks are generated.
	*		   float posy. Y position where a ring of sparks are generated.
	*		   float posy. Z position where a ring of sparks are generated.
	*  Input: float velx. X velocity of rocket when sparks are generated.
	*		   float vely. Y velocity of rocket when sparks are generated.
	*		   float velz. Z velocity of rocket when sparks are generated.
	*         float* color. color[0], color[1] and color[2] are the RGB color of the rocket. It will also be the color of the sparks it generates.
	*
	*  In this function, you want to generate a number of sparks that are uniformily distributed on a ring at [posx, posy, posz]
	*  then append them to the sparks vector using a push_back function call. Since this is a 2D demo you can set posz = 0.0
	*  The initial state vector for each spark should accommodate the constraints below:
	*   They should be evenly distribute on a ring.
	*
	*  At the time of the explosion:
	*      the number of sparks generated should be based on a random number between 10 and 60.
	*      the position of the sparks is determined by [posx, posy, posz]
	*      the magnitude of the inital velocity of each spark should  be based on a random value between 20 and 40
	*      the direction of the initial velocity of each spark should be uniformly distributed between 0 and 360 degrees
	*      the total velocity of the spark at the time of its creation is the sum of the rocket velocity and the intial spark velocity
	*  force on every spark is just the gravity.
	*  spark mass is 50.
	*  Total timeToLive of every spark is 10.
	*/
		
	// The default mass of the rocket is 50.0, it should explode for a total of explosionCount = TOTALEXPLOSIONS time steps
{
	float stateVals[12] = { 0.0 };
	int numSparks = MAXSPARKS;
	float relaVelocityMag = MAXVELOCITY;

	// TODO: Add  code here to randomize the number of sparks and their initial velocity
	double randomZeroToOne = ((rand()) % 10) / 10.f;
	numSparks += 10 * randomZeroToOne;
	relaVelocityMag += 20 * randomZeroToOne;
	float degreeOffset = 360 / numSparks;
	float currDeg = 0;

	for (int i = 0; i < numSparks; i++)
	{
		ASpark* newSpark = new ASpark(rocketColor);
		// TODO: Add your code here
		// pos init.
		stateVals[0] = rocketPosx;
		stateVals[1] = rocketPosy;
		stateVals[2] = rocketPosz;
		
		// velocity init.
		vec3 rocketVelocity = vec3(rocketVelx, rocketVely, rocketVelz);
		vec3 dir = vec3(1, 0, 0);
		mat3 rMat;
		currDeg += (degreeOffset + ((rand()) % 10) / 5.f);
		rMat.FromAxisAngle(vec3(0, 0, 1), currDeg * Deg2Rad);
		dir = rMat * dir;
		vec3 sparkVelocity = dir * relaVelocityMag + rocketVelocity;
		stateVals[3] = sparkVelocity[0];
		stateVals[4] = sparkVelocity[1];
		stateVals[5] = sparkVelocity[2];

		// force init.
		stateVals[7] = - 50 * GRAVITY;

		// mass init.
		stateVals[9] = 50.f;

		// time to live init.
		stateVals[10] = 15.f;

		newSpark->setState(stateVals);
		newSpark->setAttractor(m_attractorPos);
		newSpark->setRepeller(m_repellerPos);
		newSpark->setWind(m_windForce);
		sparks.push_back(newSpark);
	}
}


// One simulation step 
void AFireworks::update(float deltaT, int extForceMode)
{
	//Step 1. Iterate through every ASpark in sparks. If the spark is dead(life is 0), erase it from sparks.
	//        Code is given. It is also an example of erasing an element from a vector.
	ASpark* pSpark;
	int index = 0;
	m_deltaT = deltaT;

	for (unsigned int i = 0; i < sparks.size(); i++)
	{
		pSpark = sparks[index];
		if (!pSpark->m_alive)
		{
			sparks.erase(sparks.begin() + index);
			delete pSpark;
		}
		else index++;
	}


	//Step 2. TODO: Iterate through every ARocket in rockets. If the rocket is dead, erase it from rockets.
	          //If the rocket explosionCount does not equal - 1 then explode another ring of sparks.
	
	ARocket* pRocket;	
	index = 0;

	// Add you code here
	// Iterate through every ARocket in rockets. If the rocket is dead, erase it from rockets.
	for (unsigned int i = 0; i < rockets.size(); i++)
	{
		pRocket = rockets[index];
		
		if (!pRocket->m_alive)
		{
			rockets.erase(rockets.begin() + index);
			delete pRocket;
		}
		else 
		{
			// If the rocket explosionCount does not equal - 1 then explode another ring of sparks.
			if (pRocket->m_mode == EXPLOSION && pRocket->m_explosionCount != -1)
			{
				//float color[3] = {pRocket->mColor[0], pRocket->mColor[1], pRocket->mColor[2]};
				explode(pRocket->m_state[0], pRocket->m_state[1], pRocket->m_state[2],
						pRocket->m_state[3], pRocket->m_state[4], pRocket->m_state[5],
						pRocket->m_color);
				pRocket->m_explosionCount--;
			}

			if (pRocket->m_mode == FLYING)
			{
				float stateVals[12] = { 0.0 };
				int numSparks = 1;

				// TODO: Add  code here to randomize the number of sparks and their initial velocity
				double randomZeroToOne = ((rand()) % 10) / 10.f;
				

				//numSparks += 10 * randomZeroToOne;


				for (int i = 0; i < numSparks; i++)
				{
					ASpark* newSpark = new ASpark(pRocket->m_color);
					// TODO: Add your code here
					// pos init.
					stateVals[0] = pRocket->m_state[0];
					stateVals[1] = pRocket->m_state[1];
					stateVals[2] = pRocket->m_state[2];

					// velocity init.
					vec3 rocketVelocity = vec3(pRocket->m_state[3], pRocket->m_state[4], pRocket->m_state[5]);
					double randomSignX = (rand()) % 10;
					if (randomSignX > 5)
					{
						randomSignX = 1;
					}
					else
					{
						randomSignX = -1;
					}
					double randomSignY = (rand()) % 10;
					if (randomSignY > 5)
					{
						randomSignY = 1;
					}
					else
					{
						randomSignY = -1;
					}
					double randomSignZ = (rand()) % 10;
					if (randomSignZ > 5)
					{
						randomSignZ = 1;
					}
					else
					{
						randomSignZ = -1;
					}

					vec3 sparkVelocityOciliation = vec3(randomSignX * ((rand()) % 10), randomSignY * ((rand()) % 10), randomSignZ * ((rand()) % 10));
					vec3 sparkVelocity = sparkVelocityOciliation + rocketVelocity / 10;
					stateVals[3] = sparkVelocity[0];
					stateVals[4] = sparkVelocity[1];
					stateVals[5] = sparkVelocity[2];

					// force init.
					stateVals[7] = -50 * GRAVITY;

					// mass init.
					stateVals[9] = 50.f;

					// time to live init.
					stateVals[10] = 10.f;

					newSpark->setState(stateVals);
					newSpark->setAttractor(m_attractorPos);
					newSpark->setRepeller(m_repellerPos);
					newSpark->setWind(m_windForce);
					sparks.push_back(newSpark);
				}
			}
			index++;
		}
	}
	


	//Step 3. update valid sparks and rockets.
	//        Code is given.

	for (unsigned int i = 0; i < sparks.size(); i++)
	{
		ASpark* pSpark = sparks[i];
		pSpark->update(m_deltaT, extForceMode);
	}
	
	for (unsigned int i = 0; i < rockets.size(); i++)
	{
		ARocket* pRocket = rockets[i];
		pRocket->update(m_deltaT, extForceMode);
	}
}
