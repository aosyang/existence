/*******************************************************************
*         Advanced 3D Game Programming using DirectX 9.0
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*    Desc: Sampe application for Direct3D
*
* copyright (c) 2002 by Peter A Walsh and Adrian Perez
* See license.txt for modification and distribution information
******************************************************************/

//#include <template.h>
#include "BspTree.h"
#include "MeshElement.h"

#include <assert.h>

namespace Gen
{
	BspTree::BspTree()
		: m_pHead(NULL)
	{
	}

	BspTree::~BspTree()
	{
		// destroy the tree
		delete m_pHead;
	}
	void BspTree::AddTriangle( const BspTriangle& in )
	{
		if( !m_pHead )
		{
			// if there's no tree, make a new one
			m_pHead = new BspNode( in );
		}
		else
		{
			// otherwise add it to the tree
			m_pHead->AddTriangle( in );
		}
	}

	void BspTree::AddPolygonList( vector< BspTriangle >& in )
	{
		if( !m_pHead )
		{
			// if there's no tree, make a new one
			m_pHead = new BspNode( in );
		}
		else
		{
			/**
			* Adding a list of polygons to
			* an existing tree is unimplemented
			* (exercise to the reader)
			*/
			assert( false );
		}
	}

	void BspTree::TraverseTree(
		vector<BspTriangle*>* polyList,
		const Vector3f& loc )
	{
		if( m_pHead )
		{
			// drop it down
			m_pHead->TraverseTree( polyList, loc );
		}
	}

	bool BspTree::LineOfSight( const Vector3f& a, const Vector3f& b )
	{
		assert( m_pHead ); // make sure there is a tree to test against

		return m_pHead->LineOfSight( a, b );
	}

	bool BspTree::IsPointInSolid(const Vector3f& point)
	{
		return m_pHead->TestPoint(point);
		//return false;
	}

	bool BspTree::Intersects(const Ray& ray, Vector3f* point, BspTriangle** triangle)
	{
		if (m_pHead)
			return m_pHead->Intersects(ray.origin, ray.origin + ray.direction * ray.distance, ray.direction, point, triangle);

		return false;
	}

	bool BspTree::PushSphere(const Vector3f& pos, Vector3f& newpos, float radius) const
	{
		if (m_pHead)
			return m_pHead->PushSphere(pos, newpos, radius);

		return false;
	}


	BspTree::BspNode::~BspNode()
	{
		delete m_pFront;
		delete m_pBack;
	}

	BspTree::BspNode::BspNode( bool bIsSolid )
		: m_bIsLeaf( true )
		, m_bIsSolid( bIsSolid )
		, m_pFront( NULL )
		, m_pBack( NULL )
	{
		// all done.
	}

	BspTree::BspNode::BspNode( const BspTriangle& in )
		: m_bIsLeaf( false )
		, m_Triangle( in )
		, m_plane( in )
		, m_pFront( new BspNode( false ) )
		, m_pBack( new BspNode( true ) )
	{
		// all done.
	}

	BspTree::BspNode::BspNode( vector< BspTriangle >& in )
		: m_bIsLeaf( false )
	{
		// if the list is empty, we're bombing out.
		assert( in.size() );

		// get the best index to use as a splitting plane
		int bestIndex = BestIndex( in );

		// we could remove the index from the vector, but that's slow.
		// instead we'll just kind of ignore it during the next phase.
		// remove the best index
		BspTriangle splitPoly = in[bestIndex];

		m_plane = Plane3( splitPoly );
		m_Triangle = splitPoly;

		// take the rest of the polygons and divide them.
		vector< BspTriangle > frontList, backList;

		unsigned int i;
		for( i=0; i<in.size(); i++ )
		{
			// ignore the BspTriangle if it's the one
			// we're using as the splitting plane
			if( i == bestIndex ) continue;

			// test the BspTriangle against this node.
			PointListLoc res = m_plane.TestPoly( in[i] );
			//BspTriangle front, back; // used in plistSPLIT

			switch( res )
			{
			case plistFront:
				// drop down the front
				frontList.push_back( in[i] );
				break;
			case plistBack:
				// drop down the back
				backList.push_back( in[i] );
				break;
			case plistSplit:
				// split the BspTriangle, drop the halves down.
				m_plane.Split( in[i], frontList, backList );
				//frontList.push_back( front );
				//backList.push_back( back );
				break;
			case plistCoplanar:
				// add the BspTriangle to this node's list
				m_coplanarList.push_back( in[i] );

				// hack : 对于位于平面上的点，放到front列表中
				//frontList.push_back( in[i] );

				break;
			}
		}

		// we're done processing the BspTriangle list. Deal with them.
		if( frontList.size() )
		{
			m_pFront = new BspNode( frontList );
		}
		else
		{
			m_pFront = new BspNode( false );
		}
		if( backList.size() )
		{
			m_pBack = new BspNode( backList );
		}
		else
		{
			m_pBack = new BspNode( true );
		}
	}

	void BspTree::BspNode::AddTriangle( const BspTriangle& in )
	{
		if( m_bIsLeaf )
		{
			// reinitialize ourselves as a node
			*this = BspNode( in );
		}
		else
		{
			// test the BspTriangle against this node.
			PointListLoc res = this->m_plane.TestPoly( in );

			BspTriangle front, back; // used in plistSPLIT
			switch( res )
			{
			case plistFront:
				// drop down the front
				m_pFront->AddTriangle( in );
				break;
			case plistBack:
				// drop down the back
				m_pBack->AddTriangle( in );
				break;
			case plistSplit:
				{
					vector<BspTriangle> vFront;
					vector<BspTriangle> vBack;

					// split the BspTriangle, drop the halves down.
					m_plane.Split( in, vFront, vBack);
					for (unsigned int i=0; i<vFront.size(); i++)
						m_pFront->AddTriangle( vFront[i] );
					for (unsigned int i=0; i<vBack.size(); i++)
						m_pBack->AddTriangle( vBack[i] );
					break;
				}
			case plistCoplanar:
				// add the BspTriangle to this node's list
				m_coplanarList.push_back( in );
				break;
			}
		}
	}

	void BspTree::BspNode::TraverseTree( vector< BspTriangle* >* polyList,
		const Vector3f& loc )
	{
		if( m_bIsLeaf )
		{
			// do nothing.
		}
		else
		{
			// test the loc against the current node
			PointLoc res = m_plane.TestPoint( loc );

			unsigned int i;
			switch( res )
			{
			case ptFront:
				// get back, us, front
				m_pBack->TraverseTree( polyList, loc );
				polyList->push_back( &m_Triangle ); // the triangle at this node
				for( i=0; i<m_coplanarList.size(); i++ )
				{
					polyList->push_back( &m_coplanarList[i] );
				}
				m_pFront->TraverseTree( polyList, loc );
				break;

			case ptBack:
				// get front, us, back
				m_pFront->TraverseTree( polyList, loc );
				polyList->push_back( &m_Triangle ); // the triangle at this node
				for( i=0; i<m_coplanarList.size(); i++ )
				{
					polyList->push_back( &m_coplanarList[i] );
				}
				m_pBack->TraverseTree( polyList, loc );
				break;

			case ptCoplanar:
				// get front, back, us
				m_pFront->TraverseTree( polyList, loc );
				m_pBack->TraverseTree( polyList, loc );
				polyList->push_back( &m_Triangle ); // the triangle at this node
				for( i=0; i<m_coplanarList.size(); i++ )
				{
					polyList->push_back( &m_coplanarList[i] );
				}
				break;

			}
		}
	}

	// TODO: 最优分割面选择需要考虑到二叉树的平衡性
	// weight = fabs(左数目-右数目)+分割数目
	// 越小越优先考虑
	int BspTree::BspNode::BestIndex( vector< BspTriangle >& polyList )
	{
		/**
		* The current hueristic is blind least-split
		*/
		// run through the list, searching for the best one.
		// the highest BspTriangle we'll bother testing (10% of total)
		int maxCheck;
		maxCheck = (int)(polyList.size() * percentageToCheck);
		if( !maxCheck ) maxCheck = 1;

		int bestSplits = 100000;
		int bestIndex = -1;
		int currSplits;				// 这个平面总共分割了多少平面
		int frontCount, backCount;
		Plane3 currPlane;
		for(int i=0; i<maxCheck; i++ )
		{
			currSplits = 0;
			frontCount = backCount = 0;
			currPlane = Plane3( polyList[i] );
			PointListLoc res;

			for(unsigned int i2=0; i2< polyList.size(); i2++ )
			{
				if( i == i2 ) continue;

				res = currPlane.TestPoly( polyList[i2] );
				switch(res)
				{
				case plistSplit:
					currSplits++;
					break;
				case plistFront:
					frontCount++;
					break;
				case plistBack:
					backCount++;
					break;
				}
			}

			int weight = abs(frontCount - backCount) + currSplits;
			//int weight = currSplits;

			if( weight < bestSplits )
			{
				bestSplits = weight;
				bestIndex = i;
			}
		}
		assert( bestIndex >= 0 );
		return bestIndex;
	}

	bool BspTree::BspNode::LineOfSight( const Vector3f& a, const Vector3f& b )
	{
		if( m_bIsLeaf )
		{
			// if we land in a solid node, then there is no line of sight
			return !m_bIsSolid;
		}

		PointLoc aLoc = m_plane.TestPoint( a );
		PointLoc bLoc = m_plane.TestPoint( b );

		Vector3f split;

		if( aLoc == ptCoplanar && bLoc == ptCoplanar )
		{
			// for sake of something better to do, be conservative
			//return false;
			return m_pFront->LineOfSight( a, b );
		}

		if( aLoc == ptFront && bLoc == ptBack )
		{
			//split, then return the logical 'or' of both sides
			split = m_plane.Split( a, b );

			return m_pFront->LineOfSight( a, split )
				&& m_pBack->LineOfSight( b, split );
		}

		if( aLoc == ptBack && bLoc == ptFront )
		{
			// split, then return the logical 'or' of both sides
			split = m_plane.Split( a, b );

			return m_pFront->LineOfSight( b, split )
				&& m_pBack->LineOfSight( a, split );
		}

		// the other == POINT_COLPLANAR or POINT_FRONT
		if( aLoc == ptFront || bLoc == ptFront )
		{
			// drop down the front
			return m_pFront->LineOfSight( a, b );
		}

		else // they're both on the back side
		{
			// drop down the front
			return m_pBack->LineOfSight( a, b );
		}

		return true;
	}

	bool BspTree::BspNode::Intersects(const Vector3f& v0, const Vector3f& v1, const Vector3f& dir, Vector3f* point, BspTriangle** triangle)
	{
		float d0 = v0 * m_plane.n + m_plane.d;
		float d1 = v1 * m_plane.n + m_plane.d;

		Vector3f pos;

		if (d0 > 0){
			if (d1 <= 0){
				pos = v0 - (d0 / (m_plane.n * dir)) * dir;
			}

			if (m_pFront != NULL && m_pFront->Intersects(v0, (d1 <= 0)? pos : v1, dir, point, triangle)) return true;

			if (d1 <= 0){

				// 只检测一个多边形？
				if (m_Triangle.IsAbove(pos)){
					if (point)
						*point = pos;
					if (triangle)
						*triangle = &m_Triangle;
					return true;
				}

				vector< BspTriangle >::iterator iter;
				for (iter=m_coplanarList.begin(); iter!=m_coplanarList.end(); iter++)
				{
					if (iter->IsAbove(pos))
					{
						if (point)
							*point = pos;
						if (triangle)
							*triangle = &(*iter);
						return true;
					}
				}

				if (m_pBack != NULL && m_pBack->Intersects(pos, v1, dir, point, triangle)) return true;
			}
		} else {
			if (d1 > 0){
				pos = v0 - (d0 / (m_plane.n * dir)) * dir;
			}

			if (m_pBack != NULL && m_pBack->Intersects(v0, (d1 > 0)? pos : v1, dir, point, triangle)) return true;

			if (d1 > 0){
				if (m_Triangle.IsAbove(pos)){
					if (point)
						*point = pos;
					if (triangle)
						*triangle = &m_Triangle;
					return true;
				}

				vector< BspTriangle >::iterator iter;
				for (iter=m_coplanarList.begin(); iter!=m_coplanarList.end(); iter++)
				{
					if (iter->IsAbove(pos))
					{
						if (point)
							*point = pos;
						if (triangle)
							*triangle = &(*iter);
						return true;
					}
				}


				if (m_pFront != NULL && m_pFront->Intersects(pos, v1, dir, point, triangle)) return true;
			}
		}

		return false;
	}

	bool BspTree::BspNode::PushSphere(const Vector3f& pos, Vector3f& newpos, float radius) const
	{
		newpos = pos;
		float d = pos * m_Triangle.normal + m_Triangle.offset;

		// 先判断点是否位于平面正上方，如果是则判断点和平面的距离是否小于半径
		// 如果不是则将点与三角形的各个边缘进行比较

		bool pushed = false;
		float abs_d = fabsf(d);
		if (abs_d < radius){

			// 检查是否在节点每一个平面上
			if (m_Triangle.IsAbove(pos))
			{
				newpos += (radius - abs_d) * m_Triangle.normal; 
				pushed = true;
			}
			else
			{
				//vector< BspTriangle >::iterator iter;

				//for (iter=m_coplanarList.begin(); iter!=m_coplanarList.end(); iter++)
				for (unsigned int i=0; i<m_coplanarList.size(); i++)
				{
					if (m_coplanarList[i].IsAbove(pos))
					{
						newpos += (radius - abs_d) * m_coplanarList[i].normal; 
						pushed = true;
						break;
					}
				}
			}

			// 如果不在三角形正上方，按照边缘push
			if (!pushed)
			{
				Vector3f v1 = m_Triangle.vertices[2];
				for (int i = 0; i < 3; i++){
					Vector3f v0 = v1;
					v1 = m_Triangle.vertices[i];

					Vector3f diff = v1 - v0;

					float t = diff * (pos - v0);
					if (t > 0){
						float f = diff * diff;
						if (t < f){
							Vector3f v = v0 + (t / f) * diff;
							Vector3f dir = pos - v;
							if ((dir * dir) < radius * radius){
								dir.normalize();
								newpos = v + radius * dir;
								pushed = true;
								break;
							}
						}
					}
				}

				//vector< BspTriangle >::iterator iter;
				//for (iter=m_coplanarList.begin(); iter!=m_coplanarList.end()&&(!done); iter++)
				for (unsigned int j=0; j<m_coplanarList.size() && (!pushed); j++)
				{
					Vector3f v1 = m_coplanarList[j].vertices[2];
					for (int i = 0; i < 3; i++){
						Vector3f v0 = v1;
						v1 = m_coplanarList[j].vertices[i];

						Vector3f diff = v1 - v0;

						float t = diff * (pos - v0);
						if (t > 0){
							float f = diff * diff;
							if (t < f){
								Vector3f v = v0 + (t / f) * diff;
								Vector3f dir = pos - v;
								if ((dir * dir) < radius * radius){
									dir.normalize();
									newpos = v + radius * dir;
									pushed = true;
									break;
								}
							}
						}
					}
				}
			}

			// 如果也不在边缘上方，按照顶点push
			if (!pushed)
			{
				for(int i=0; (i<3)&&(!pushed); i++)
				{
					Vector3f v = m_Triangle.vertices[i];
					Vector3f t = pos - v;
					if (t.SquaredLength() < radius * radius)
					{
						// 从顶点向球心所在点移动
						t.normalize();
						newpos = v + t * radius;

						pushed = true;
						break;
					}
				}

				for (unsigned int j=0; j<m_coplanarList.size() && (!pushed); j++)
				{
					for(int i=0; (i<3)&&(!pushed); i++)
					{
						Vector3f v = m_coplanarList[j].vertices[i];
						Vector3f t = pos - v;
						if (t.SquaredLength() < radius * radius)
						{
							// 从顶点向球心所在点移动
							t.normalize();
							newpos = v + t * radius;

							pushed = true;
							break;
						}
					}
				}
			}

		}

		if (m_pFront != NULL && d > -radius) pushed |= m_pFront->PushSphere(newpos, newpos, radius);
		if (m_pBack  != NULL && d <  radius) pushed |= m_pBack ->PushSphere(newpos, newpos, radius);

		return pushed;
	}

	bool BspTree::BspNode::TestPoint(const Vector3f& point)
	{
		if (m_bIsLeaf)
		{
			if (m_bIsSolid) return true;
			return false;
		}

		PointLoc res = m_plane.TestPoint(point);

		switch(res)
		{
		case ptFront:
			return m_pFront->TestPoint(point);
		case ptBack:
			return m_pBack->TestPoint(point);
			//case ptCoplanar:
			//	return m_pBack->TestPoint(point);
		}

		return m_pBack->TestPoint(point);
	}


	bool BuildBspTreeFromMesh(BspTree* bsp, BaseMesh* mesh)
	{
		if (!bsp || !mesh)
			return false;

		BspTriangle triangle;
		vector<BspTriangle> triList;
		for (int n=0; n<mesh->GetElementCount(); n++)
		{
			MeshElement* elem = mesh->GetElement(n);
			for (unsigned int i=0; i<elem->GetFaceNum(); i++)
			{
				unsigned int v1, v2, v3;
				elem->GetFaceByIndex(i, v1, v2, v3);

				triangle.vertices[0] = mesh->GetVertexByIndex(v1);
				triangle.vertices[1] = mesh->GetVertexByIndex(v2);
				triangle.vertices[2] = mesh->GetVertexByIndex(v3);
				triangle.Setup();

				triList.push_back(triangle);

				//bsp->AddTriangle(triangle);
			}
		}
		bsp->AddPolygonList(triList);

		return true;
	}
}
