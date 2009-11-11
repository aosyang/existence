//-----------------------------------------------------------------------------------
/// Ms3dMesh.h ms3dÕ¯∏Ò¿‡
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _MS3DMESH_H
#define _MS3DMESH_H

#include "BaseMesh.h"

namespace Gen
{
	// byte-align structures
#ifdef _MSC_VER
#	pragma pack( push, packing )
#	pragma pack( 1 )
#	define PACK_STRUCT
#elif defined( __GNUC__ )
#	define PACK_STRUCT	__attribute__((packed))
#else
#	error you must byte-align these structures with the appropriate compiler directives
#endif

	typedef unsigned char byte;
	typedef unsigned short word;

	struct Ms3dHeader
	{
		char	id[10];
		int		version;
	} PACK_STRUCT;

	struct Ms3dVertex
	{
		byte		flags;
		float		pos[3];
		char		boneID;
		byte		refCount;
	} PACK_STRUCT;

	struct Ms3dTriangle
	{
		word	flags;
		word	vertexIndices[3];
		float	vertexNormals[3][3];
		float	u[3], v[3];
		byte	smoothingGroup;
		byte	groupIndex;
	} PACK_STRUCT;

	struct Ms3dMaterial
	{
		char	name[32];
		float	ambient[4];
		float	diffuse[4];
		float	specular[4];
		float	emissive[4];
		float	shininess;		// 0.0f - 128.0f
		float	transparency;	// 0.0f - 1.0f
		byte	mode;			// 0, 1, 2 is unused now
		char	texture[128];
		char	alphamap[128];
	} PACK_STRUCT;

	// Default alignment
#ifdef _MSC_VER
#	pragma pack( pop, packing )
#endif

#undef PACK_STRUCT

	class Ms3dMesh : public BaseMesh
	{
		friend class MeshManager;
	public:
		static IMesh* ManagerLoad(const String& filename);
		bool LoadFromFile(const String& filename);
	private:
		Ms3dMesh();
	};
}

#endif
