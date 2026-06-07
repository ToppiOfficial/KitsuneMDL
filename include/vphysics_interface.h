// Minimal vphysics interface declarations needed by studiomdl collision model building.
#ifndef VPHYSICS_INTERFACE_H
#define VPHYSICS_INTERFACE_H
#pragma once

#include "mathlib/vector.h"
#include "mathlib/mathlib.h"
#include "tier1/interface.h"

// Opaque physics types
class CPhysConvex;
class CPhysCollide;
class CPhysPolysoup;
class ICollisionQuery;
class IVPhysicsKeyParser;
class CPolyhedron;
class IPhysicsObject;
struct vcollide_t;
struct surfacedata_t;
struct surfacephysicsparams_t;

// Layout must match Source SDK 2013 vphysics.dll exactly — 3 bools + float = 8 bytes
struct convertconvexparams_t
{
	bool	buildOuterConvexHull;
	bool	buildDragAxisAreas;
	bool	checkOptimalTracing;
	float	dragAreaEpsilon;

	void Defaults()
	{
		buildOuterConvexHull = false;
		buildDragAxisAreas   = false;
		checkOptimalTracing  = false;
		dragAreaEpsilon      = 0.25f;
	}
};

// Source SDK 2013 / TF2 64-bit vphysics.dll uses version 007
#undef  VPHYSICS_COLLISION_INTERFACE_VERSION
#define VPHYSICS_COLLISION_INTERFACE_VERSION	"VPhysicsCollision007"
#define VPHYSICS_SURFACEPROPS_INTERFACE_VERSION	"VPhysicsSurfaceProps001"

#ifndef abstract_class
#define abstract_class struct
#endif

abstract_class IPhysicsCollision
{
public:
	virtual ~IPhysicsCollision( void ) {}

	virtual CPhysConvex		*ConvexFromVerts( Vector **pVerts, int vertCount ) = 0;
	virtual CPhysConvex		*ConvexFromPlanes( float *pPlanes, int planeCount, float mergeDistance ) = 0;
	virtual float			ConvexVolume( CPhysConvex *pConvex ) = 0;
	virtual float			ConvexSurfaceArea( CPhysConvex *pConvex ) = 0;
	virtual void			SetConvexGameData( CPhysConvex *pConvex, unsigned int gameData ) = 0;
	virtual void			ConvexFree( CPhysConvex *pConvex ) = 0;
	virtual CPhysConvex		*BBoxToConvex( const Vector &mins, const Vector &maxs ) = 0;
	virtual CPhysConvex		*ConvexFromConvexPolyhedron( const CPolyhedron &poly ) = 0;
	virtual void			ConvexesFromConvexPolygon( const Vector &vPolyNormal, const Vector *pPoints, int iPointCount, CPhysConvex **pOutput ) = 0;

	virtual CPhysPolysoup	*PolysoupCreate( void ) = 0;
	virtual void			PolysoupDestroy( CPhysPolysoup *pSoup ) = 0;
	virtual void			PolysoupAddTriangle( CPhysPolysoup *pSoup, const Vector &a, const Vector &b, const Vector &c, int materialIndex7bits ) = 0;
	virtual CPhysCollide	*ConvertPolysoupToCollide( CPhysPolysoup *pSoup, bool useMOPP ) = 0;

	virtual CPhysCollide	*ConvertConvexToCollide( CPhysConvex **pConvex, int convexCount ) = 0;
	virtual CPhysCollide	*ConvertConvexToCollideParams( CPhysConvex **pConvex, int convexCount, const convertconvexparams_t &convertParams ) = 0;
	virtual void			DestroyCollide( CPhysCollide *pCollide ) = 0;

	virtual int				CollideSize( CPhysCollide *pCollide ) = 0;
	virtual int				CollideWrite( char *pDest, CPhysCollide *pCollide, bool bSwap = false ) = 0;
	virtual CPhysCollide	*UnserializeCollide( char *pBuffer, int size, int index ) = 0;

	virtual float			CollideVolume( CPhysCollide *pCollide ) = 0;
	virtual float			CollideSurfaceArea( CPhysCollide *pCollide ) = 0;

	virtual Vector			CollideGetExtent( const CPhysCollide *pCollide, const Vector &origin, const QAngle &angles, const Vector &direction ) = 0;
	virtual void			CollideGetAABB( Vector *pMins, Vector *pMaxs, const CPhysCollide *pCollide, const Vector &origin, const QAngle &angles ) = 0;
	virtual void			CollideGetMassCenter( CPhysCollide *pCollide, Vector *pOutMassCenter ) = 0;
	virtual void			CollideSetMassCenter( CPhysCollide *pCollide, const Vector &massCenter ) = 0;
	virtual Vector			CollideGetOrthographicAreas( const CPhysCollide *pCollide ) = 0;
	virtual void			CollideSetOrthographicAreas( CPhysCollide *pCollide, const Vector &areas ) = 0;
	virtual int				CollideIndex( const CPhysCollide *pCollide ) = 0;
	virtual CPhysCollide	*BBoxToCollide( const Vector &mins, const Vector &maxs ) = 0;
	virtual int				GetConvexesUsedInCollideable( const CPhysCollide *pCollideable, CPhysConvex **pOutputArray, int iOutputArrayLimit ) = 0;

	virtual IPhysicsCollision *ThreadContextCreate( void ) = 0;
	virtual void			ThreadContextDestroy( IPhysicsCollision *pThreadContext ) = 0;

	virtual CPhysCollide	*CreateVirtualMesh( const void *params ) = 0;
	virtual bool			SupportsVirtualMesh() = 0;
	virtual bool			GetBBoxCacheSize( int *pCachedSize, int *pCachedCount ) = 0;

	virtual CPolyhedron		*PolyhedronFromConvex( CPhysConvex * const pConvex, bool bUseTempPolyhedron ) = 0;
	virtual void			OutputDebugInfo( const CPhysCollide *pCollide ) = 0;
	virtual unsigned int	ReadStat( int statID ) = 0;
	virtual float			CollideGetRadius( const CPhysCollide *pCollide ) = 0;
	virtual void			*VCollideAllocUserData( vcollide_t *pVCollide, size_t userDataSize ) = 0;
	virtual void			VCollideFreeUserData( vcollide_t *pVCollide ) = 0;
	virtual void			VCollideCheck( vcollide_t *pVCollide, const char *pName ) = 0;
};

abstract_class IPhysicsSurfaceProps
{
public:
	virtual ~IPhysicsSurfaceProps( void ) {}

	virtual int		ParseSurfaceData( const char *pFilename, const char *pTextfile ) = 0;
	virtual int		SurfacePropCount( void ) const = 0;
	virtual int		GetSurfaceIndex( const char *pSurfacePropName ) const = 0;
	virtual void	GetPhysicsProperties( int surfaceDataIndex, float *density, float *thickness, float *friction, float *elasticity ) const = 0;
	virtual surfacedata_t	*GetSurfaceData( int surfaceDataIndex ) = 0;
	virtual const char		*GetString( unsigned short stringTableIndex ) const = 0;
	virtual const char		*GetPropName( int surfaceDataIndex ) const = 0;
	virtual void	SetWorldMaterialIndexTable( int *pMapArray, int mapSize ) = 0;
	virtual void	GetPhysicsParameters( int surfaceDataIndex, surfacephysicsparams_t *pParamsOut ) const = 0;
};

#endif // VPHYSICS_INTERFACE_H
