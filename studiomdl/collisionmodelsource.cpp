#include <string.h>
#include "studiomdl/collisionmodelsource.h"
#include "studiomdl/studiomdl.h"
#include "mathlib/mathlib.h"
#include "tier1/strtools.h"

extern int FindLocalBoneNamed( const s_source_t *pSource, const char *pName );
extern const char *RenameBone( const char *pName );

void GlobalFace( s_face_t *out, s_mesh_t *pmesh, s_face_t *face )
{
	out->a = face->a + pmesh->vertexoffset;
	out->b = face->b + pmesh->vertexoffset;
	out->c = face->c + pmesh->vertexoffset;
	out->d = face->d;
}

void CCollisionModelSource::InitBoneMap()
{
	if ( !m_pModel )
		return;
	m_bonemap.SetSize( m_pModel->numbones );
	for ( int i = 0; i < m_pModel->numbones; i++ )
		m_bonemap[i] = i;
}

void CCollisionModelSource::Simplify()
{
	// Bone merging / bone-map fixup has already been done through the
	// MergeBones calls during QC parsing.  Nothing further to do here.
}

void CCollisionModelSource::SkipBone( int boneIndex )
{
	if ( boneIndex >= 0 && boneIndex < m_bonemap.Count() )
		m_bonemap[boneIndex] = -1;
}

int CCollisionModelSource::FindLocalBoneNamed( const char *pName )
{
	return ::FindLocalBoneNamed( m_pModel, pName );
}

bool CCollisionModelSource::ShouldProcessBone( int boneIndex )
{
	if ( boneIndex < 0 || boneIndex >= m_bonemap.Count() )
		return false;
	return m_bonemap[boneIndex] == boneIndex;
}

void CCollisionModelSource::MergeBones( int parentIndex, int childIndex )
{
	if ( !m_pModel || parentIndex < 0 || childIndex < 0 )
		return;
	if ( childIndex >= m_bonemap.Count() )
		return;
	// Remap child to parent so ProcessJointedModel skips child and attributes
	// its verts to the parent collision model.
	m_bonemap[childIndex] = parentIndex;
}

void CCollisionModelSource::MergeBones( const char *pParent, const char *pChild )
{
	int parentIndex = FindLocalBoneNamed( pParent );
	int childIndex  = FindLocalBoneNamed( pChild );
	if ( parentIndex >= 0 && childIndex >= 0 )
		MergeBones( parentIndex, childIndex );
}

bool CCollisionModelSource::FaceHasVertOnBone( const s_face_t &face, int boneIndex )
{
	auto checkVert = [&]( uint32_t vi ) -> bool
	{
		if ( vi >= (uint32_t)m_pModel->numvertices )
			return false;
		const s_boneweight_t &bw = m_pModel->vertex[vi].boneweight;
		for ( int k = 0; k < bw.numbones; k++ )
		{
			int mapped = ( bw.bone[k] < m_bonemap.Count() ) ? m_bonemap[bw.bone[k]] : bw.bone[k];
			if ( mapped == boneIndex )
				return true;
		}
		return false;
	};
	return checkVert( face.a ) || checkVert( face.b ) || checkVert( face.c );
}

void CCollisionModelSource::ConvertToWorldSpace( CUtlVector<Vector> &worldVerts, s_source_t *pSrc )
{
	// Vertex positions in an SMD are already in model/pose space (world space
	// for the T-pose), so just copy them directly.
	worldVerts.SetCount( pSrc->numvertices );
	for ( int i = 0; i < pSrc->numvertices; i++ )
		worldVerts[i] = pSrc->vertex[i].position;
}

void CCollisionModelSource::ConvertToBoneSpace( int boneIndex, CUtlVector<Vector> &boneVerts )
{
	// Transform from pose/world space to the local space of boneIndex.
	// boneToPose maps bone-local -> pose; its inverse maps pose -> bone-local.
	matrix3x4_t poseFromBone = m_pModel->boneToPose[boneIndex];
	// Build inverse: for a rigid transform, inv = transpose(rot) with negated trans
	matrix3x4_t bonePoseInv;
	// MatrixInvert works for general 3x4 rigid transforms
	MatrixInvert( poseFromBone, bonePoseInv );

	boneVerts.SetCount( m_pModel->numvertices );
	for ( int i = 0; i < m_pModel->numvertices; i++ )
		VectorTransform( m_pModel->vertex[i].position, bonePoseInv, boneVerts[i] );
}

s_face_t CCollisionModelSource::GetGlobalFace( s_mesh_t *pmesh, int faceIndex )
{
	s_face_t *face = m_pModel->face + pmesh->faceoffset + faceIndex;
	s_face_t globalFace;
	GlobalFace( &globalFace, pmesh, face );
	return globalFace;
}

void CCollisionModelSource::FindBoundBones( s_mesh_t *pmesh, CUtlVector<int> &bones )
{
	for ( int i = 0; i < pmesh->numfaces; i++ )
	{
		s_face_t globalFace = GetGlobalFace( pmesh, i );
		uint32_t verts[3] = { globalFace.a, globalFace.b, globalFace.c };
		for ( int j = 0; j < 3; j++ )
		{
			const s_boneweight_t &bw = m_pModel->vertex[verts[j]].boneweight;
			for ( int k = 0; k < bw.numbones; k++ )
			{
				if ( bones.Find( bw.bone[k] ) == -1 )
					bones.AddToTail( bw.bone[k] );
			}
		}
	}
}

void CCollisionModelSource::FindBoundBones( s_boneweight_t &bw, CUtlVector<int> &bones )
{
	for ( int k = 0; k < bw.numbones; k++ )
	{
		if ( bones.Find( bw.bone[k] ) == -1 )
			bones.AddToTail( bw.bone[k] );
	}
}

int CCollisionModelSource::RemapBone( int bone ) const
{
	if ( bone < 0 || bone >= m_bonemap.Count() )
		return bone;
	return m_bonemap[bone];
}
