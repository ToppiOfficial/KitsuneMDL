#ifndef COLLISION_MODEL_SOURCE_H
#define COLLISION_MODEL_SOURCE_H
#pragma once

#include "tier1/utlvector.h"
#include "studiomdl/studiomdl.h"

#define MAX_EXTRA_COLLISION_MODELS 24

struct extramodel_t
{
	s_source_t *m_pSrc;
	matrix3x4_t m_matOffset;
	bool m_bConcave;
};

class CCollisionModelSource
{
public:
	s_source_t  *m_pModel;
	extramodel_t m_ExtraModels[MAX_EXTRA_COLLISION_MODELS + 1];
	bool         m_isJointed;
	bool         m_bAssumeWorldspace;
	bool         m_allowConcave;
	int          m_maxConvex;
	char        *m_pOverrideName;
	CUtlVector<int> m_bonemap;
	char         m_rootName[128];
	bool         m_allowConcaveJoints;
	bool         m_bRootCollisionIsEmpty;

public:
	void ConvertToWorldSpace( CUtlVector<Vector> &worldVerts, s_source_t *pSrc );
	void ConvertToBoneSpace( int boneIndex, CUtlVector<Vector> &boneVerts );
	bool ShouldProcessBone( int boneIndex );
	void Simplify();
	void SkipBone( int boneIndex );
	void InitBoneMap();
	void MergeBones( int parentIndex, int childIndex );
	void MergeBones( const char *pParent, const char *pChild );
	int  FindLocalBoneNamed( const char *pName );
	bool FaceHasVertOnBone( const s_face_t &face, int boneIndex );
	s_face_t GetGlobalFace( s_mesh_t *pmesh, int faceIndex );
	void FindBoundBones( s_mesh_t *pmesh, CUtlVector<int> &bones );
	void FindBoundBones( s_boneweight_t &bw, CUtlVector<int> &bones );
	int  RemapBone( int bone ) const;
};

struct convexlist_t
{
	int firstVertIndex;
	int numVertIndex;
};

// Converts face-local vertex indices to global vertex indices within the source
extern void GlobalFace( s_face_t *out, s_mesh_t *pmesh, s_face_t *face );

#endif // COLLISION_MODEL_SOURCE_H
