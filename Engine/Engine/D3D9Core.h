#pragma once
#include "DGLCore.h"
#include "DMath.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <map>
#include <vector>
#include <string>

//class DMeshBuffer9 : public DMeshBuffer
//{
//public:
//	DMeshBuffer9();
//	void Init(LPDIRECT3DDEVICE9, int vertexCount, int indexCount, int bufferLength, int dataSize, const float* vertices, const unsigned long* indices);
//	void Draw(LPDIRECT3DDEVICE9);
//	virtual void Release();
//
//private:
//	/*IDirect3DVertexBuffer9* m_vertexBuffer;
//	IDirect3DIndexBuffer9* m_indexBuffer;*/
//	ID3DXMesh* m_mesh;
//	int m_dataSize;
//	int m_vertexCount;
//	int m_indexCount;
//	int m_pimCount;
//};

//class DTextureBuffer9 : public DTextureBuffer
//{
//public:
//	virtual void Release();
//};

//class DShaderBuffer9 : public DShaderBuffer
//{
//private:
//	class ShaderParam9
//	{
//	public:
//		ShaderParam9()
//		{
//			bufferIndex = -1;
//			bufferOffset = -1;
//			bufferLength = 0;
//			shaderType = 0;
//			paramLength = 0;
//			paramOffset = -1;
//		}
//
//		ShaderParam9(int bufferIndex, int bufferOffset, int bufferLength, int paramOffset, int paramLength, int shaderType)
//		{
//			this->bufferIndex = bufferIndex;
//			this->bufferOffset = bufferOffset;
//			this->bufferLength = bufferLength;
//			this->shaderType = shaderType;
//			this->paramOffset = paramOffset;
//			this->paramLength = paramLength;
//		}
//	public:
//		int bufferIndex, bufferOffset, bufferLength;
//		int paramOffset, paramLength;
//		int shaderType;
//	};
//
//public:
//	DShaderBuffer9();
//	~DShaderBuffer9();
//	void Init(LPDIRECT3DDEVICE9, WCHAR * vertexShader, WCHAR * pixelShader);
//	virtual unsigned int GetCBufferCount() const;
//	virtual unsigned int GetPropertyCount() const;
//	virtual void GetPropertyInfo(const LPCSTR key, int & cindex, int & coffset, int & clength, int& poffset, int& plength, int& stype) const;
//	virtual bool HasProperty(const LPCSTR key) const;
//	virtual void Release();
//	void SetMatrix(const LPCSTR key, LPDIRECT3DDEVICE9 device, DMatrix4x4);
//	void ApplyBuffer(LPDIRECT3DDEVICE9, int cindex, int csize, int stype, float* params);
//	void Draw(LPDIRECT3DDEVICE9);
//
//private:
//	bool InitShader(LPDIRECT3DDEVICE9, WCHAR*, WCHAR*);
//	HRESULT InitVertexShader();
//	HRESULT InitPixelShader();
//
//private:
//	IDirect3DVertexShader9* m_vertexShader;
//	IDirect3DPixelShader9* m_pixelShader;
//	ID3DXConstantTable* m_vertexConstable;
//	ID3DXConstantTable* m_pixelConstable;
//	int m_propertyCount;
//	std::map<std::string, ShaderParam9> m_params;
//	std::vector<D3DXHANDLE> m_handles;
//};

class D3D9Core : public DGLCore
{
public:
	D3D9Core();
	~D3D9Core();

	virtual bool Init(int, int, bool, HWND);
	virtual void Destroy();
	virtual void BeginRender();
	virtual void EndRender();
	virtual DMeshRes* CreateMeshRes();
	virtual DTextureRes* CreateTextureRes();
	virtual DShaderRes* CreateShaderRes();
	//virtual void ApplyShaderParams(DShaderBuffer * shaderBuffer, int cindex, int coffset, int csize, int stype, float* params);
	//virtual void ApplyTextureParams(DTextureBuffer* textureBuffer);
	//virtual void DrawMesh(const DMeshBuffer*, int);
	//virtual void DrawShader(const DShaderBuffer*, int);

	//virtual void SetBackBufferRenderTarget();

	LPDIRECT3DDEVICE9 GetDevice()
	{
		return m_device;
	}

private:
	LPDIRECT3D9 m_d3d;
	LPDIRECT3DDEVICE9 m_device;
	D3DVIEWPORT9 m_viewPort;
};

