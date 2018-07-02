#pragma once
#include "DResObject.h"
#include <fstream>
#include <map>

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

class DMeshResItem : public DResItem
{
public:
	DMeshResItem(char* path);
	virtual void Release();
	static DMeshResItem* LoadManifest(std::ifstream&);

protected:
	virtual DResObject* OnLoad();

private:
	char* m_path;
};

