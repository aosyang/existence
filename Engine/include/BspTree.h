/*******************************************************************
*            Advanced 3D Game Programming using DirectX 9.0
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*     Desc: Sample application for Direct3D
*
* copyright (c) 2002 by Peter A Walsh and Adrian Perez
* See license.txt for modification and distribution information
******************************************************************/

#ifndef _BSPTREE_H
#define _BSPTREE_H

#include "Vector3f.h"
#include "Plane3.h"
#include "BaseMesh.h"
#include "BspTriangle.h"

#include <vector>


namespace Gen
{
	const float percentageToCheck = .5f; // 10%

	/**
	* This code expects the set of polygons we're giving it to be
	* closed, forming a continuous skin. If it's not, weird things
	* may happen.
	*/
	class BspTree
	{
	public:

		// construction/destruction
		BspTree();
		~BspTree();

		// we need to handle copying
		BspTree( const BspTree &in );
		BspTree& operator=( const BspTree &in );

		// add a BspTriangle to the tree
		void AddTriangle( const BspTriangle& in );
		void AddPolygonList( std::vector< BspTriangle >& in );

		void TraverseTree(
			std::vector< BspTriangle* >* polyList,
			const Vector3f& loc );

		// 线段与BSP树是否有交点
		// 如果两个点分别处于solid和非solid空间，则始终返回false
		bool LineOfSight( const Vector3f& a, const Vector3f& b );

		bool IsPointInSolid(const Vector3f& point);

		bool Intersects(const Ray& ray, Vector3f* point, BspTriangle** triangle = NULL);

		bool PushSphere(const Vector3f& pos, Vector3f& newpos, float radius) const;

	protected:

	private:

		class BspNode
		{

			BspNode*         m_pFront;              // pointer to front subtree
			BspNode*         m_pBack;               // pointer to back subtree

			BspTriangle     m_Triangle;
			Plane3         m_plane;
			bool           m_bIsLeaf;
			bool           m_bIsSolid;

			std::vector< BspTriangle > m_coplanarList;

			static int BestIndex( std::vector< BspTriangle >& polyList );

		public:
			BspNode( bool bIsSolid );                            // leaf constructor
			BspNode( const BspTriangle& in );                // node constructor
			BspNode( std::vector< BspTriangle >& in );            // node constructor
			~BspNode();

			// we need to handle copying
			BspNode( const BspNode &in );
			BspNode& operator=( const BspNode &in );

			void AddTriangle( const BspTriangle& in );

			void TraverseTree(
				std::vector< BspTriangle* >* polyList,
				const Vector3f& loc );

			bool IsLeaf()
			{
				return m_bIsLeaf;
			}

			bool LineOfSight( const Vector3f& a, const Vector3f& b );

			bool Intersects(const Vector3f& v0, const Vector3f& v1, const Vector3f& dir, Vector3f* point, BspTriangle** triangle = NULL);

			bool PushSphere(const Vector3f& pos, Vector3f& newpos, float radius) const;

			bool TestPoint(const Vector3f& point);
		};

		BspNode* m_pHead; // root node of the tree

	};

	inline BspTree::BspTree( const BspTree &in )
	{
		// clone the tree
		if( in.m_pHead )
			m_pHead = new BspNode( *in.m_pHead );
		else
			m_pHead = NULL;
	}

	inline BspTree& BspTree::operator=( const BspTree &in )
	{
		if( &in != this )
		{
			// delete the tree if we have one already
			if( m_pHead )
				delete m_pHead;
			// clone the tree
			if( in.m_pHead )
				m_pHead = new BspNode( *in.m_pHead );
			else
				m_pHead = NULL;
		}
		return *this;
	}

	inline BspTree::BspNode::BspNode( const BspNode &in )
	{
		m_Triangle = in.m_Triangle;
		m_plane = in.m_plane;
		m_bIsLeaf = in.m_bIsLeaf;
		m_bIsSolid = in.m_bIsSolid;

		// clone the trees
		m_pFront = NULL;
		if( in.m_pFront )
			m_pFront = new BspNode( *in.m_pFront );

		m_pBack = NULL;
		if( in.m_pBack )
			m_pBack = new BspNode( *in.m_pBack );
	}
	inline BspTree::BspNode& BspTree::BspNode::operator=( const BspNode &in )
	{
		if( &in != this )
		{
			// delete the subtrees if we have them already
			if( m_pFront )
				delete m_pFront;
			if( m_pBack )
				delete m_pBack;

			// copy all the data over
			m_Triangle = in.m_Triangle;
			m_plane = in.m_plane;
			m_bIsLeaf = in.m_bIsLeaf;
			m_bIsSolid = in.m_bIsSolid;

			// clone the trees
			m_pFront = NULL;
			if( in.m_pFront )
				m_pFront = new BspNode( *in.m_pFront );

			m_pBack = NULL;
			if( in.m_pBack )
				m_pBack = new BspNode( *in.m_pBack );
		}
		return *this;
	}

	bool BuildBspTreeFromMesh(BspTree* bsp, BaseMesh* mesh);
}

#endif //_BSPTREE_H

