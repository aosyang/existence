//-----------------------------------------------------------------------------------
/// SceneObject.h ����������ӵ������Ķ���̳��Դ��࣬�����˰��Ӷ���ķ���
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _SCENEOBJECT_H
#define _SCENEOBJECT_H

#include "MovableObjectBase.h"
#include "Frustum.h"
#include "Collision.h"
#include "EString.h"
#include "SceneSerializer.h"

#include <set>
#include <list>
#include <map>

using namespace std;

namespace Gen
{
	class SceneGraph;
	class SceneObject;
	class RenderableObjectBase;
	class SceneSerializer;
	class SceneSerializerNode;




	typedef SceneObject* (*FactoryCreateFuncPtr)(SceneGraph*);

	// ��ÿ��SceneObject������������������������ȴ����������Ĺ���
	// ���������ִ�
	// ͬʱ�������������������Ϊ˽�У���ֹ����ֱ�ӹ���
	// ����������������Ķ������ʵ�ִ�һ������SceneGraph*�Ĺ��캯������������

	/// @def DECLARE_FACTORY_OBJECT(className, baseClassName)
	/// ��һ��������Ϊ���������û��Ժ����ʹ��FACTORY_CREATE�괴���������
	/// @note
	///		�ڴ�������֮ǰ��������ʹ��REGISTER_FACTORYע����
	#define DECLARE_FACTORY_OBJECT(className, baseClassName) \
		typedef baseClassName BaseClass; \
		public:	\
			static SceneObject* FactoryCreateSceneObject(SceneGraph* scene) { \
				return new className(scene); \
			} \
			const String GetTypeName() const { return #className; } \
		protected: \
			className(SceneGraph* scene); \
			~className(); \
		private: \
			className();

	// �ڳ����д���һ������ת��Ϊ��Ӧ���͵�ָ��
	#define FACTORY_CREATE(scenePtr, className) static_cast<className*>(scenePtr->CreateSceneObject(#className))

	// ע��һ�����͵Ĺ�������
	#define REGISTER_FACTORY(className) SceneObjectFactory::RegisterFactoryCreator(#className, &className::FactoryCreateSceneObject);

	typedef map<const String, FactoryCreateFuncPtr>	SceneObjectFactoryCreators;

	// �������󹤳���ʹ�ù���������������
	class SceneObjectFactory
	{
	public:
		static void RegisterFactoryCreator(const String& className, FactoryCreateFuncPtr ptrFunc);
		static void RegisterDefaultFactories();

		SceneObject* CreateSceneObject(SceneGraph* scene, const String& className);
	protected:
		static SceneObjectFactoryCreators	m_FactoryCreators;
	};





	typedef set<RenderableObjectBase*> RenderableObjectSet;
	typedef set<SceneObject*>	SceneObjectSet;
	typedef list<SceneObject*>	SceneObjectList;

	/// @brief
	/// ��������
	/// @par
	///		ÿһ���ܹ��ڳ����и��µĶ��󶼼̳��Գ���������
	///		���������ṩ��ʰȡ�ж������ÿ���״�������ӽ��ȹ���
	/// @see
	///		SceneGraph
	class SceneObject : public MovableObjectBase
	{
		friend class SceneSerializer;
	public:
		/// @brief
		/// ���캯��
		/// @param scene
		///		����Ҫ��ӵ��ĳ���
		SceneObject(SceneGraph* scene);
		~SceneObject();

		// ----- Overwrite IObject

		/// @copydoc MovableObjectBase::Update()
		void Update(unsigned long deltaTime);

		/// @copydoc IObject::GetTypeName()
		const String GetTypeName() const { return "SceneObject"; }

		// ----- Overwrite MovableObjectBase

		/// @copydoc MovableObjectBase::IsCulled()
		/// ���������޳�m_Visible=false�Ķ���
		bool IsCulled(Frustum* frustum);

		// ----- SceneObject methods

		/// @brief
		/// ��Ⱦ������Ϣ
		/// @remarks
		///		���าд���������ʵ����Ⱦ��ͬ�ĵ�����Ϣ
		/// @note
		///		������������屾����Ⱦ֮�����
		/// @see
		///		SceneGraph::RenderScene()
		virtual void DebugRender();

	protected:
		/// @brief
		/// ��ȾAABB
		/// @remarks
		///		���������Ĭ�ϼ̳й�ϵ����DebugRender����
		//void DebugRenderAABB();

		/// @brief
		///	��ȾOBB
		/// @remarks
		///		���������Ĭ�ϼ̳й�ϵ����DebugRender����
		//void DebugRenderOBB();

	public:
		// ��ײ���

		// AABB��ײ��ѯ
		// virtual void QueryAABB

		// �������ж�

		/// @brief
		/// ���õ�ǰ�����Ƿ�ɼ�
		/// @param visible
		///		trueΪ�ɼ���falseΪ���ɼ�
		void SetVisible(bool visible) { m_Visible = visible; }

		/// @brief
		/// ��ȡ��ǰ�����Ƿ�ɼ�
		/// @returns
		///		trueΪ�ɼ���falseΪ���ɼ�
		bool GetVisible() const { return m_Visible; }

		/// @brief
		/// ָ���ö������ڵĳ���
		/// @param scene
		///		����ĳ���
		/// @remarks
		///		��������Ҫ��֪�������ڵĳ������Խ�����������
		void SetSceneGraph(SceneGraph* scene) { m_Scene = scene; }

		/// @brief
		/// �ݹ�ָ���������Ӷ������ڵĳ���
		/// @param scene
		///		����ĳ���
		/// @remarks
		///		����������ö����������Ӷ���ĳ���ָ��Ϊ��������
		void SetSceneGraphRecursively(SceneGraph* scene);

		/// @brief
		/// ��ȡ����ǰ���еĳ���ָ��
		/// @returns
		///		���ض���ǰ�ĳ���ָ��
		SceneGraph* GetSceneGraph() { return m_Scene; }

		/// @brief
		/// �ݹ��ռ���������Ҫ��Ⱦ�Ķ���
		/// @param renderableObjs
		///		��Ⱦ�����б���Ҫ��Ⱦ�Ķ���ӵ�����б���
		/// @param frustum
		///		��ǰ��Ⱦʹ�õ�ƽͷ�ӽ���
		virtual void CollectRenderableObject(RenderableObjectList& renderableObjs, Frustum* frustum);


		// Collision

		/// @brief
		/// ���ߵݹ�ʰȡ��������
		/// @param ray
		///		ʰȡ�жϵ�����
		///	@param sceneObjects
		///		����ཻ�жϳ�������������ö������ײ��Ϣ��ӵ�����б���
		/// @param type
		///		(��ʱ����)
		/// @param collisionGroup
		///		��ײ����
		void RayPickingSceneObject(const Ray& ray, ObjectsCollisionInfos& sceneObjects, int type, int collisionGroup);

		/// @brief
		/// ����������ཻ�ж�
		/// @param ray
		///		���жϵ�����
		/// @param info
		///		������Ϣ
		/// @returns
		///		����ཻ������true�����򷵻�false
		virtual bool IntersectsRay(const Ray& ray, CollisionResult& info);

		virtual bool PushSphere(const Vector3f& pos, Vector3f& newpos, float radius);

		/// ��ȡ��ײ������
		int GetCollisionGroupMask() const { return m_CollisionGroupMask; }

		/// ָ����ײ������
		void SetCollisionGroupMask(int mask) { m_CollisionGroupMask = mask; }

		// Attachable

		/// @brief
		/// ������������Ϊ�ö�����Ӷ���
		/// @param child
		///		�Ӷ���
		/// @param keepRotation
		///		�Ƿ�ά���Ӷ������ת���ܵ������Ӱ��
		/// @param keepWorldTransform
		///		�Ƿ��ڸ��ӵ�ʱ�򱣳��Ӷ�����������겻��
		void AttachChildObject(SceneObject* child, bool keepRotation = false, bool keepWorldTransform = false);

		/// @brief
		/// ���ö���Ӹ�����ϲ���
		/// @param keepWorldTransform
		///		�Ƿ�ά�ֶ���Ӹ��������Ժ����������
		void DetachFromParent(bool keepWorldTransform);

		/// ��ȡ������
		inline SceneObject* GetParent() const { return m_ParentObject; }

		/// @brief
		/// ɾ�������Ӷ���
		/// @remarks
		///		��������Ὣ�Ӷ���ӳ�����ɾ��
		void DeleteAllChildren();

		// ���л�
		virtual bool OnSave(SceneSerializerNode* node);
		virtual void OnRestore(SceneSerializerNode* node);

		void SetSerializable(bool serializable) { m_Serializable = serializable; }
		bool GetSerializable() const { return m_Serializable; }

		// Debug

		/// @brief
		/// ������������Ϣ�ݹ鵼������־
		/// @param level
		///		������ȣ���������ı�������
		void DumpToLogRecursively(int level=0);

	protected:
		/// @brief
		/// ָ��������
		/// @param parent
		///		������
		/// @param keepRotation
		///		���ֵ�ǰ�������ת��Ϣ���ܵ�������Ӱ��
		void SetParentObject(SceneObject* parent, bool keepRotation);

	protected:
		//// ���һ������Ⱦ����
		//void AddRenderableObject(RenderableObjectBase* renderable);

		//// ɾ��һ������Ⱦ����
		//void RemoveRenderableObject(RenderableObjectBase* renderable);

	protected:
		bool							m_Visible;					///< �ɼ���
		SceneGraph*						m_Scene;					///< ���������ĳ���ָ��


		SceneObjectSet					m_ChildrenObjects;			///< �ӽ����󼯺�
		SceneObject*					m_ParentObject;				///< ������

		bool							m_KeepRotation;				///< ��ǰ�������ת�����ܵ��������ת��Ӱ��
		int								m_CollisionGroupMask;		///< ��ײ������
		//RenderableObjectSet				m_RenderableObjects;
		bool							m_Serializable;				///< �ö����Ƿ���Ա��浽����
	};
}

#endif

