#ifndef COLLISIONMODEL_H
#define COLLISIONMODEL_H
#pragma once

// Entry point called from Cmd_CollisionModel / Cmd_CollisionJoints
// separateJoints == true  => ragdoll ($collisionjoints)
// separateJoints == false => single-body prop ($collisionmodel)
int  DoCollisionModel( bool separateJoints );

// Called after SimplifyModel(), before WriteModelFiles()
void CollisionModel_Build( void );

// Called from WriteModelFiles() after the MDL checksum is computed
void CollisionModel_Write( long checkSum );

// Adjust bounding box to include collision hull (for props only)
void CollisionModel_ExpandBBox( Vector &mins, Vector &maxs );

// Override the output .phy filename (called by $phyname)
void CollisionModel_SetName( const char *pName );

// Return the total mass of the collision model
float GetCollisionModelMass();

#endif // COLLISIONMODEL_H
