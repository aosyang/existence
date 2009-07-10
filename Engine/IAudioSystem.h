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

#include <string>

using namespace std;

//-----------------------------------------------------------------------------------
/// \brief
/// 音源接口
/// 
/// 保存已读取的音源
/// 
/// \remarks
/// 音源需要使用AudioSystem进行卸载，不能直接删除
//-----------------------------------------------------------------------------------
class IAudioSource
{
public:
	virtual void Play() = 0;
	virtual void Stop() = 0;

	virtual void SetTransform(const Matrix4& transform) = 0;
protected:
	IAudioSource() {}
	virtual ~IAudioSource() {}
};

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
	virtual bool LoadAudioFromFile(const string& sourceName, const string& filename) = 0;

	// 根据音频名返回一格音源
	virtual IAudioSource* GetAudioSource(const string& sourceName) = 0;

	// 指定听者的位置
	virtual void SetListenerTransform(const Matrix4& transform) = 0;

	// 播放一个指定的音源
	virtual void PlaySource(IAudioSource* source) = 0;
	//-----------------------------------------------------------------------------------
	/// \brief
	/// 播放一个指定名称的音源
	/// 
	/// \param sourceName
	/// 音源名称
	/// 
	/// \remarks
	/// 这个方法相当于分别调用GetAudioSource(const string& sourceName)和PlaySource(IAudioSource* source)
	//-----------------------------------------------------------------------------------
	virtual void PlaySource(const string& sourceName) = 0;
	// 停止指定音源的播放
	virtual void StopSource(IAudioSource* source) = 0;

};

// 音频系统创建函数，用于插件的生成函数
typedef IAudioSystem*(*AudioSystemFactoryCreateFunc)();

#endif
