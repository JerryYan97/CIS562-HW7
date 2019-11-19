// Spark.cpp: implementation of the ASpark class.
//
//////////////////////////////////////////////////////////////////////

#include "aSpark.h"
#include <math.h>
#include <time.h>

#ifndef GRAVITY
#define GRAVITY 9.8f
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ASpark::ASpark()
{
	//coefficients of restitution equals 0.25
	m_COR = 0.25f;
	m_mass = 1.0;
}

ASpark::ASpark(float* color): AParticle()
{
	for (int i = 0; i < 3; i++)
		m_color[i] = color[i];
 
	//coefficients of restitution equals 0.25
	m_COR = 0.25f;
}

ASpark::~ASpark()
{

}

//Set attractor position
void ASpark::setAttractor(vec3 position)
{
	m_attractorPos = position;
}

//Set repeller position
void ASpark::setRepeller(vec3 position)
{
	m_repellerPos = position;
}

void ASpark::setWind(vec3 wind)
{
	m_windForce = wind;
}

void ASpark::display()
{
	float fadeTime = 3.0;
	if (m_alive)
	{
		float alpha = 1.0;
		if (m_state[10] < fadeTime)
		{
			alpha = m_state[10] / 10.0f;
		}
		float scale = 1.0;

		glPushMatrix();
		glColor4f(m_color[0], m_color[1], m_color[2], alpha);
		glTranslatef(m_state[0], m_state[1], m_state[2]);
		glScalef(scale, scale, scale);
		glutSolidSphere(1.0, 10, 10);
		glPopMatrix();
	}

}
	


void ASpark::update(float deltaT, int extForceMode)
{
	m_deltaT = deltaT;
	if (m_state[10] <= 0.0)
	{
		m_alive = false;
		return;
	}

	if (!(extForceMode & EXT_SPARKFORCES_ACTIVE))
		extForceMode = 0;
	
	computeForces(extForceMode);
	
	updateState(deltaT, EULER);

	resolveCollisions();
	
	
}


 
void ASpark::computeForces(int extForceMode)
//	computes the forces applied to this spark
{
	// zero out all forces
	m_state[6] = 0.0;
	m_state[7] = 0.0;
	m_state[8] = 0.0;

	// gravity force
	addForce(m_mass*m_gravity);

	//std::cout << "gravity force" << m_mass * m_gravity << std::endl;
	//std::cout << "ASpark::computeForces::extForceMode" << extForceMode << std::endl;

	// wind force
	if (extForceMode & WIND_ACTIVE)
	{
		//TODO: Add your code here
		addForce(m_windForce);
		//std::cout << "m_windForce" << m_windForce << std::endl;
	}

	if (extForceMode & DRAG_ACTIVE)
	{
		//TODO: Add your code here
		// Air Friction:
		vec3 velocity = vec3(m_state[3], m_state[4], m_state[5]);
		vec3 airFrictionForce = -velocity;
		addForce(airFrictionForce * 20);
	}


	// attractor force
	if (extForceMode & ATTRACTOR_ACTIVE)
	{
		//TODO: Add your code here
		// We have a pos of an attractor.
		// Then, we need a default scalar force and a real-time direction to this attractor.
		// Finally, we can get the real force.
		vec3 particlePos = vec3(m_state[0], m_state[1], m_state[2]);
		//std::cout << "particlePos:" << particlePos << std::endl;
		vec3 attDir = m_attractorPos - particlePos;
		double attForceCof = 500;
		attDir = attDir.Normalize();
		vec3 attForce = attForceCof * attDir;
		addForce(attForce);
	}

	// repeller force
	if (extForceMode & REPELLER_ACTIVE)
	{
		//TODO: Add your code here
		vec3 particlePos = vec3(m_state[0], m_state[1], m_state[2]);
		vec3 repDir = -(m_repellerPos - particlePos);
		repDir = repDir.Normalize();
		double repForceCof = 300;
		vec3 repForce = repForceCof * repDir;
		addForce(repForce);
	}

	// random force
	if (extForceMode & RANDOM_ACTIVE)
	{
		//TODO: Add your code here
		//srand(time(0));
		double randomZeroToOne = ((rand()) % 10) / 10.f;
		double randomForceMag = 500 * randomZeroToOne;
		vec3 randomForceDir = vec3(rand(), rand(), rand());
		randomForceDir = randomForceDir.Normalize();
		vec3 randomForce = randomForceMag * randomForceDir;
		//std::cout << "randomForce:" << randomForce << std::endl;
		addForce(randomForce);
	}

}

void ASpark::resolveCollisions()
// resolves collisions of the spark with the ground
{
	//TODO: Add  code here that reverses the y value of the spark velocity vector when the y position value of the spark is < 0
	if (m_state[1] > 0)
	{
		return;
	}

	// reflect a vector:
	m_state[4] = -m_state[4];

	//std::cout << "ASpark::resolveCollisions(), x:" << m_state[0] << std::endl;
}
