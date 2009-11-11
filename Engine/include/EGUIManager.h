//-----------------------------------------------------------------------------------
/// EGUIManager.h ͼ�ν��������
///
/// File Encoding : GB2312
///
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _EGUIMANAGER_H
#define _EGUIMANAGER_H

#include "Singleton.h"
#include "Engine.h"
#include "UIRootObject.h"
#include "TextUIControl.h"
#include "ImageUIControl.h"

namespace Gen
{
	// ע��EGUI�ؼ����ɺ���
	#define REGISTER_EGUI_CLASS(class_name) \
		public:\
			class_name* Create##class_name() { \
				class_name* control = new class_name(); \
				AddUIObject(control); \
				return control; \
			}


	class EGUIManager : public Singleton<EGUIManager>
	{
		friend class Singleton<EGUIManager>;
		REGISTER_EGUI_CLASS(TextUIControl);
		REGISTER_EGUI_CLASS(ImageUIControl);
	public:
		void Initialize();
		void Shutdown();
		
		// UI�������
		//void OnMousePressed(MouseButtonID id);
		//void OnMouseReleased(MouseButtonID id);
		//void OnMouseMoved(int x, int y);

		void Update(unsigned long deltaTime);
		void RenderUI();

	protected:
		EGUIManager();

		// ���UI�ؼ�
		void AddUIObject(BaseUIObject* object);

	protected:
		UIRootObject*				m_UIRootObject;		///< UI������

	};
}

#endif
