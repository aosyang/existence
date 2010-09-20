//-----------------------------------------------------------------------------------
/// SceneGraph.h ����ͼ��������ӵ������Ķ���
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _SCENEGRAPH_H
#define _SCENEGRAPH_H

#include "ISceneObject.h"
#include "SceneObject.h"
#include "Platform.h"
#include "Camera.h"
#include "Ray.h"
#include "Color4f.h"
#include "Collision.h"

#include <set>
#include <vector>



namespace Gen
{

	struct RenderView
	{
		Vector3f position;
		Matrix4 viewMatrix;
		Matrix4 projMatrix;
		Frustum* frustum;
	};

	class SceneSerializer;

	/// @brief
	/// ����ͼ
	/// @par
	///		������Ⱦ�����³�������ĳ���ͼ
	/// @remarks
	///		ÿ��������ӵ�����ͼ֮���ɳ���ͼ������¼���Ⱦ
	class SceneGraph
	{
		// �������л���Ҫ�Գ���ͼ����ȫ����Ȩ��
		friend class SceneSerializer;
	public:
		/// ���캯��
		SceneGraph();
		/// ��������
		~SceneGraph();

	private:
		/// @brief
		/// ��Ӷ��󵽳�����
		/// @param object
		///		Ҫ��ӵ������Ķ���
		/// @param autoDelete
		///		�ö����Ƿ��ڳ�������ʱͬʱ����
		/// @remarks
		///		�򳡾����ظ����ͬһ����������assert
		void AddObject(SceneObject* object, bool autoDelete=true);

	public:
		/// @brief
		/// �ӳ������Ƴ�����
		/// @param object
		///		Ҫ�ӳ������Ƴ��Ķ���
		/// @param deleteObj
		///		�Ƴ�������Ƿ����ٶ���
		/// @param recursive
		///		�Ƿ�ݹ��Ƴ�������Ӷ���
		/// @remarks
		///		�������������RemoveSceneObjectInternal��������ʵ�ʵ�ɾ������
		/// @see
		///		RemoveSceneObjectInternal()
		void RemoveObject(SceneObject* object, bool deleteObj=true, bool recursive=false);

		/// @brief
		/// ����һ����ע��ĳ�������
		/// @param className
		///		��������������
		/// @returns
		///		���سɹ������Ķ���ָ��
		SceneObject* CreateSceneObject(const String& className, bool autoDelete=true);

		// Update & Render

		/// @brief
		/// ���³���
		/// @par
		///		���������ж����Update������������
		/// @param deltaTime
		///		����һ֡��������������ʱ��(����)
		void UpdateScene(unsigned long deltaTime);

		/// @brief
		/// ������Ⱦ�ӵ�
		/// @param view
		///		��Ⱦ�ӵ����
		void SetupRenderView(const RenderView& view);

		/// @brief
		/// ʹ�������������Ⱦ�ӵ�
		/// @param camera
		///		Ҫ��Ⱦ�������
		void SetupRenderView(Camera* camera);

		/// @brief
		/// ��Ⱦ����
		void RenderScene();

		/// @brief
		/// ����ʰȡ��������
		/// @param ray
		///		ʰȡ������
		/// @param sceneObjects
		///		���ڼ�¼ʰȡ�Ķ����б�
		/// @param type
		///		����ʰȡ���������(��ʱû����;)
		/// @param collisionGroup
		///		��ײ������
		/// @param sortResult
		///		�Ƿ�Խ�������ɽ���Զ������
		/// @remarks
		///		Ĭ�϶��󲻻ᱻ�����κε���ײ�飬��Ҫ�ֶ�ָ������ʰȡ
		void RayPickingSceneObject(const Ray& ray, ObjectsCollisionInfos& sceneObjects, int type, int collisionGroup = COLLISION_GROUP_ALL, bool sortResult = false);

		/// @brief
		///	����������ײ
		/// @param pos
		///		��������
		/// @param newpos
		///		������ײ���������������
		/// @param radius
		///		����뾶
		/// @returns
		///		����������κ����巢����ײ����true�����򷵻�false
		/// @remarks
		///		����������������ײ���ݶ���д��PushSphere�������
		bool PushSphere(const Vector3f& pos, Vector3f& newpos, float radius);

		/// @brief
		/// �������ṹ�����log
		void DumpToLog();

	private:
		/// @brief
		///		�Ƴ���������(�ڲ�����)
		/// @param object
		///		Ҫ�Ƴ��ĳ�������
		/// @param deleteObj
		///		�Ƿ�ͬʱ���ٶ���
		/// @param recursive
		///		�Ƿ�ݹ��Ƴ�����
		void RemoveSceneObjectInternal(SceneObject* object, bool deleteObj, bool recursive);

		/// @brief
		/// ִ��ʵ�ʵ��Ƴ�����
		/// @par
		///		��������ÿ֡���º����
		/// @remarks
		///		��������������Ƴ���Ҫ�ӳ������Ƴ������ж���
		void ProcessRemove();

	private:
		//typedef std::vector<ISceneObject*> SceneObjects;
		//SceneObjects	m_SceneObjects;

		//SceneRootObject*			m_RootObject;		///< �������������

		SceneObjectSet				m_SceneObjects;		///< ���������б�
		SceneObjectSet				m_AutoDeleteList;	///< �Զ�ɾ�������б�

		/// @brief
		/// �Ƴ������б�Ԫ��
		struct RemoveListElement
		{
			SceneObject* obj;		///< Ҫ�Ƴ��Ķ���
			bool	deleteObj;		///< �Ƿ����Ƴ���ɾ������
			bool	recursive;		///< �Ƿ�ݹ�����Ӷ���
		};

		std::vector<RemoveListElement>	m_RemoveList;		///< �Ƴ��б���¼����һ֡���º���Ҫ�Ƴ��Ķ���

		//RenderableObjectSet			m_RenderableObjects;

		bool						m_ProcessingUpdate;	///< ��ǰ�Ƿ����ڸ��³�����״̬��ʶ

		RenderView					m_RenderView;		///< ��Ⱦ�ӵ���Ϣ

		SceneObjectFactory			m_ObjectFactory;	///< �������󹤳�
	};
}

#endif
