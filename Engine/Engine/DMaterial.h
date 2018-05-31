#pragma once
#include "DShader.h"

class DMaterial
{
public:
	DMaterial(DShader*);
	~DMaterial();
	DShader* GetShader() const;
	template<class T>
	void SetCBuffer(LPCSTR buffername, T buffer);
	bool HasCBuffer(LPCSTR buffername);

private:
	DShader* m_shader;
	void** m_cbuffers;
};

template<class T>
inline void DMaterial::SetCBuffer(LPCSTR buffername, T buffer)
{
}
