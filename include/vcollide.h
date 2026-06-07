#ifndef VCOLLIDE_H
#define VCOLLIDE_H
#pragma once

class CPhysCollide;

struct vcollide_t
{
	unsigned short solidCount : 15;
	unsigned short isPacked : 1;
	unsigned short descSize;
	CPhysCollide  **solids;
	char           *pKeyValues;
	void           *pUserData;
};

#endif // VCOLLIDE_H
