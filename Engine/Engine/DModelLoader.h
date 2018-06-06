#pragma once

static class DModelLoader
{
public:
	static bool LoadObj(const char *, float**, unsigned long **, int &, int &, int&, int&);

	static bool CreateCube(float**, unsigned long **, int &, int &, int&, int&);

	static bool CreatePlane(float**, unsigned long **, int &, int &, int&, int&);
};

