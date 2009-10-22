//-----------------------------------------------------------------------------------
/// Prerequistites.h 预包含头文件
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _PREREQUISITES_H
#define _PREREQUISITES_H


// Interfaces
class IAudioSystem;
class IRenderer;
class IVertexBuffer;
class ITexture;
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
class TextureManager;
class Mesh;
class Input;
class Timer;
class System;
class Font;
class FontManager;
class NullAudioSystem;
class BspTree;
class Log;
class Skeletal;
class Image;
class String;

class SceneGraph;
class SceneSerializer;

class SceneObject;
class SceneRootObject;
class Camera;
class MeshObject;
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

class EGUIManager;
class UIRootObject;
class BaseUIObject;
class TextUIControl;
class ImageUIControl;


#include "Platform.h"
#include "Debug.h"

#include "ITexture.h"
#include "IRenderer.h"
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
#include "Mesh.h"
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
#include "Skeletal.h"
#include "Image.h"
#include "EString.h"
#include "StringConverter.h"

#include "SceneGraph.h"
#include "SceneSerializer.h"

#include "ISceneObject.h"
#include "SceneObject.h"
#include "SceneRootObject.h"
#include "Camera.h"
#include "MeshObject.h"
#include "DistantViewObject.h"
#include "Billboard.h"
#include "Decal.h"
#include "BspObject.h"
#include "ParticlePool.h"
#include "ParticleEmitter.h"

#include "ResourceManager.h"

#include "FileSystem.h"

#include "GameObjectBase.h"

#include "EGUIManager.h"
#include "UIRootObject.h"
#include "BaseUIObject.h"
#include "TextUIControl.h"
#include "ImageUIControl.h"

#endif
