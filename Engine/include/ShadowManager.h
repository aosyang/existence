//-----------------------------------------------------------------------------------
/// ShadowManager.h 阴影管理器，有光就有影……
///
/// File Encoding : GB2312
///
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _SHADOWMANAGER_H
#define _SHADOWMANAGER_H

#include "Singleton.h"
#include "Light.h"
#include "SceneRootObject.h"

class ShadowManager : public Singleton<ShadowManager>
{
	friend class Singleton<ShadowManager>;
public:
	void SetLight(Light* light) { m_Light = light; }
	Light* GetLight() { return m_Light; }

	//void RenderLightViewScene(SceneRootObject* rootObj);
	//void RenderUntexturedScene(SceneRootObject* rootObj);

private:
	ShadowManager();

	Light*		m_Light;
};

#endif
