#include "DTextureRes10.h"
#include "DSystem.h"

DTextureRes10::DTextureRes10(ID3D10Device * device, WCHAR * filename)
{
	/*HRESULT result;

	m_device = device;

	m_isSuccess = false;

	result = D3DX10CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_texture, NULL);
	if (FAILED(result))
	{
		return;
	}
	m_isSuccess = true;*/
}

DTextureRes10::~DTextureRes10()
{
}

void DTextureRes10::Apply(UINT, UINT, DWarpMode)
{
}

void DTextureRes10::Release()
{
}
