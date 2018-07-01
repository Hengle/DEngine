#pragma once
#include "DResObject.h"
#include <fstream>

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
	DShaderResItem(char* vsfile, char* psfile);
	virtual void Release();
	static DShaderResItem* LoadManifest(std::ifstream&);

protected:
	virtual DResObject* OnLoad();

private:
	char* m_vsfile, *m_psfile;
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
public:
	DMaterialResItem(char* path, int wrapMode);
	virtual void Release();
	static DMaterialResItem* LoadManifest(std::ifstream&);

protected:
	virtual DResObject* OnLoad();
};

