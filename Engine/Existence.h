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
class BaseLightableObject;
class ShadowManager;

class Color4f;
class Camera;
class Material;
class TextureManager;
class Mesh;
class MeshManager;
class Input;
class Timer;
class System;
class SceneGraph;
class BaseSceneObject;
class BaseUIObject;
class TextUIControl;
class SceneRootObject;
class UIRootObject;
class MeshObject;
class DistantViewObject;
class Billboard;
class Decal;
class Font;
class FontManager;
class NullAudioSystem;
class AudioListener;
class BspTree;
class BspObject;
class Log;
class Skeletal;
class Image;

#pragma warning ( disable : 4819 )


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
#include "Camera.h"
#include "ILight.h"
#include "Light.h"
#include "LightingManager.h"
#include "ShadowManager.h"
#include "BaseLightableObject.h"
#include "Material.h"
#include "Singleton.h"
#include "Mesh.h"
#include "MeshManager.h"
#include "Input.h"
#include "Timer.h"
#include "IGame.h"
#include "System.h"
#include "ISceneObject.h"
#include "SceneGraph.h"
#include "BaseSceneObject.h"
#include "BaseUIObject.h"
#include "TextUIControl.h"
#include "SceneRootObject.h"
#include "UIRootObject.h"
#include "MeshObject.h"
#include "DistantViewObject.h"
#include "Billboard.h"
#include "Decal.h"
#include "Font.h"
#include "NullAudioSystem.h"
#include "AudioListener.h"
#include "BspTree.h"
#include "BspObject.h"
#include "Log.h"
#include "Skeletal.h"
#include "Image.h"

#include "ResourceManager.h"

#endif
