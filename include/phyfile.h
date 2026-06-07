#ifndef PHYFILE_H
#define PHYFILE_H
#pragma once

typedef struct phyheader_s
{
	int size;
	int id;
	int solidCount;
	int checkSum;
} phyheader_t;

#endif // PHYFILE_H
