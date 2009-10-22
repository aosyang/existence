//-----------------------------------------------------------------------------------
/// IAudioSystem.h 音频系统接口，提供音频系统的公共操作方法
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _IAUDIOSYSTEM_H
#define _IAUDIOSYSTEM_H

#include "Matrix4.h"
#include "EString.h"
#include "IAudioSource.h"
#include "IAudioBuffer.h"

//-----------------------------------------------------------------------------------
/// \brief
/// 音频系统接口
/// 
/// 提供音频系统的基本操作接口
//-----------------------------------------------------------------------------------
class IAudioSystem
{
public:
	virtual ~IAudioSystem() {}

	// 初始化与关闭
	virtual bool Initialize() = 0;
	virtual void Shutdown() = 0;

	// 从文件载入一个音频
	virtual bool LoadAudioBufferFromFile(const String& bufferName, const String& filename) = 0;

	// 根据音频名返回一格音源
	virtual IAudioBuffer* GetAudioBuffer(const String& bufferName) = 0;

	// 指定听者的位置
	virtual void SetListenerTransform(const Matrix4& transform, const Vector3f& velocity) = 0;

	virtual IAudioSource* CreateSourceInstance(IAudioBuffer* buffer, const Vector3f& position, bool autoRemove=true) = 0;

	virtual void RemoveSource(IAudioSource* source) = 0;

	virtual void Update() = 0;
};

// 音频系统创建函数，用于插件的生成函数
typedef IAudioSystem*(*AudioSystemFactoryCreateFunc)();

#endif
