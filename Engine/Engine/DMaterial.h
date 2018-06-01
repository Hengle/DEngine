#pragma once
#include "DShader.h"

class DMaterial : public DResObject
{
public:
	DMaterial(DShader*);
	~DMaterial();
	DShader* GetShader() const;
	template<typename  T>
	void SetCBuffer(const LPCSTR buffername, T buffer) const;
	bool HasCBuffer(LPCSTR buffername);
	virtual void Destroy();


private:
	DShader* m_shader;
	DShaderParam* m_params;
};

template<typename  T>
inline void DMaterial::SetCBuffer(const LPCSTR buffername, T buffer) const
{
	int cbindex = m_shader->GetCBufferIndex(buffername);
	if (cbindex >= 0)
	{
		void* pbf;
		m_params->BeginSetParam(cbindex, &pbf);
		T* tbf = (T*)pbf;
		(*tbf) = buffer;
		m_params->EndSetParam(cbindex);
	}
}
