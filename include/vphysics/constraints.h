#ifndef CONSTRAINTS_H
#define CONSTRAINTS_H
#pragma once

struct constraint_axislimit_t
{
	float minRotation;
	float maxRotation;
	float angularVelocity;
	float torque;

	inline void SetAxisFriction( float rmin, float rmax, float friction )
	{
		minRotation = rmin;
		maxRotation = rmax;
		angularVelocity = 0;
		torque = friction;
	}
	inline void Defaults()
	{
		SetAxisFriction( 0, 0, 0 );
	}
};

struct constraint_ragdollparams_t
{
	int parentIndex;
	int childIndex;
	constraint_axislimit_t axes[3];

	inline void Defaults()
	{
		parentIndex = -1;
		childIndex = -1;
		axes[0].Defaults();
		axes[1].Defaults();
		axes[2].Defaults();
	}
};

#endif // CONSTRAINTS_H
