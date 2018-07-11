#pragma once
class DObject
{
public:
	DObject();
	~DObject();
	void Destroy();

protected:
	virtual void OnDestroy() = 0;
};

