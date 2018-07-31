#pragma once
#include "DResObject.h"
#include <fstream>
//#include <map>

/*资源项*/
class DResItem
{
public:
	DResItem();
	void Load();
	void Unload();
	virtual void Release();
	DResObject* GetRes();

protected:
	virtual DResObject* OnLoad() = 0;

private:
	bool m_isLoaded;
	DResObject* m_res;
};

/*shader资源项*/
class DShaderResItem : public DResItem
{
public:
	DShaderResItem(char* path);
	virtual void Release();
	static DShaderResItem* LoadManifest(std::ifstream&);

protected:
	virtual DResObject* OnLoad();

private:
	char* m_path;
};

/*纹理资源项*/
class DTexture2DResItem : public DResItem
{
public:
	DTexture2DResItem(char* path, int wrapMode);
	virtual void Release();
	static DTexture2DResItem* LoadManifest(std::ifstream&);

protected:
	virtual DResObject* OnLoad();

private:
	char* m_path;
	int m_wrapMode;
};

/*立方体贴图资源*/
class DTextureCubeResItem :public DResItem
{
public:
	DTextureCubeResItem();
	void SetFace(int, int, int);
	virtual void Release();
	static DTextureCubeResItem* LoadManifest(std::ifstream&);

protected:
	virtual DResObject* OnLoad();

private:
	int m_rightGroupId, m_rightId;
	int m_leftGroupId, m_leftId;
	int m_topGroupId, m_topId;
	int m_bottomGroupId, m_bottomId;
	int m_frontGroupId, m_frontId;
	int m_backGroupId, m_backId;
};

/*材质资源项*/
class DMaterialResItem : public DResItem
{
public:
	DMaterialResItem(char* path);
	//void SetPath(char* path);
	virtual void Release();
	static DMaterialResItem* LoadManifest(std::ifstream&);

protected:
	virtual DResObject* OnLoad();

private:
	DResObject* LoadMaterial();
	void LoadShader(std::ifstream&, DResObject**);
	void LoadTexture(std::ifstream&, DResObject*);
	void LoadNumbers(std::ifstream&, DResObject*, int);

private:
	//unsigned int m_shaderId;
	//unsigned int m_shaderGroupId;
	char* m_path;
	//std::map<std::string, MaterialResTexDesc> m_textureIds;
};

class DGeometryResItem : public DResItem
{
public:
	DGeometryResItem(char* path);
	virtual void Release();
	static DGeometryResItem* LoadManifest(std::ifstream&);

protected:
	virtual DResObject* OnLoad();

private:
	char* m_path;
};

