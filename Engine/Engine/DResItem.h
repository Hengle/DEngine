#pragma once
#include "DResObject.h"
#include <fstream>
#include <map>

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

/*材质资源项*/
class DMaterialResItem : public DResItem
{
private:
	struct MaterialResTexDesc
	{
	public:
		unsigned int groupId, resId;
	};

public:
	DMaterialResItem(unsigned int shaderGroupId, unsigned int shaderId);
	void AddTexture(char*, unsigned int, unsigned int);
	virtual void Release();
	static DMaterialResItem* LoadManifest(std::ifstream&);

protected:
	virtual DResObject* OnLoad();

private:
	unsigned int m_shaderId;
	unsigned int m_shaderGroupId;
	std::map<std::string, MaterialResTexDesc> m_textureIds;
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

