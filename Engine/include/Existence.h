//-----------------------------------------------------------------------------------
/// Existence.h 预包含头文件
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _EXISTENCE_H
#define _EXISTENCE_H

namespace Gen
{
	// Interfaces
	class IAudioSystem;
	class IRenderDevice;
	class IVertexBuffer;
	class BaseTexture;
	class IGame;
	class ISceneObject;

	// Math
	class Vector3f;
	class Matrix4;
	class Matrix3;
	class Frustum;
	class Ray;
	//class BspTriangle;

	class Platform;

	// Lighting
	class LightingManager;
	class ILight;
	class Light;
	class LightableObject;
	class ShadowManager;

	class Color4f;
	class CameraShakeEffect;
	class Material;
	class Input;
	class Timer;
	class System;
	class Font;
	class FontManager;
	class NullAudioSystem;
	class BspTree;
	class Log;
	class Image;
	class String;

	class TextureManager;
	class MeshManager;
	class SkeletonManager;
	class Renderer;
	class AudioManager;

	class BaseMesh;
	class EmdMesh;
	class Ms3dMesh;

	class SceneGraph;
	class SceneSerializer;

	class SceneObject;
	class SceneRootObject;
	class Camera;
	class MeshObject;
	class SkeletalMeshObject;
	class DistantViewObject;
	class Billboard;
	class Decal;
	class ParticlePool;
	class ParticleEmitter;
	class AudioListener;
	class AudioSourceObject;
	class BspObject;

	class FileSystem;

	class GameObjectBase;
	class Actor;

	class EGUIManager;
	class UIRootObject;
	class BaseUIObject;
	class TextUIControl;
	class ImageUIControl;
}


#include "Platform.h"
#include "Debug.h"

#include "Texture.h"
#include "IRenderDevice.h"
#include "IVertexBuffer.h"
#include "MathUtil.h"
#include "Vector3f.h"
#include "Matrix4.h"
#include "Matrix3.h"
#include "Frustum.h"
#include "Ray.h"
#include "BspTriangle.h"
#include "Color4f.h"
#include "IAudioSystem.h"
#include "CameraShake.h"
#include "ILight.h"
#include "Light.h"
#include "LightingManager.h"
#include "ShadowManager.h"
#include "LightableObject.h"
#include "Material.h"
#include "Singleton.h"
#include "Input.h"
#include "Timer.h"
#include "IGame.h"
#include "System.h"
#include "Font.h"
#include "NullAudioSystem.h"
#include "AudioListener.h"
#include "AudioSourceObject.h"
#include "BspTree.h"
#include "Log.h"
#include "Skeleton.h"
#include "Image.h"
#include "EString.h"
#include "StringConverter.h"

#include "MeshManager.h"
#include "TextureManager.h"
#include "SkeletonManager.h"
#include "Renderer.h"
#include "AudioManager.h"

#include "BaseMesh.h"
#include "EmdMesh.h"
#include "Ms3dMesh.h"

#include "SceneGraph.h"
#include "SceneSerializer.h"

#include "ISceneObject.h"
#include "SceneObject.h"
#include "SceneRootObject.h"
#include "Camera.h"
#include "MeshObject.h"
#include "SkeletalMeshObject.h"
#include "DistantViewObject.h"
#include "Billboard.h"
#include "Decal.h"
#include "BspObject.h"
#include "ParticlePool.h"
#include "ParticleEmitter.h"

#include "ResourceManager.h"

#include "FileSystem.h"

#include "GameObjectBase.h"
#include "Actor.h"

#include "EGUIManager.h"
#include "UIRootObject.h"
#include "BaseUIObject.h"
#include "TextUIControl.h"
#include "ImageUIControl.h"

#endif
