#pragma once
#include "DShader.h"

class DMaterial : public DResObject
{
public:
	DMaterial(DShader*);
	~DMaterial();
	DShader* GetShader() const;
	template<class T>
	void SetCBuffer(LPCSTR buffername, T buffer);
	bool HasCBuffer(LPCSTR buffername);
	void Draw();
	virtual void Destroy();

private:
	DShader* m_shader;
	void** m_cbuffers;
};

template<class T>
inline void DMaterial::SetCBuffer(LPCSTR buffername, T buffer)
{
	int cbindex = m_shader->GetCBufferIndex(buffername);
	if (cbindex >= 0)
	{
		T* pbf = (T*)m_cbuffers[cbindex];
		&pbf = buffer;
	}
}
