#include "DModelLoader.h"
#include "DMath.h"
#include <string>
#include <vector>
#include <map>
#include <fstream>

using namespace std;

bool DModelLoader::LoadObj(const char * file, float** buffer, unsigned long ** indexBuffer, int & vertexCount, int & indexCount, int& dataSize)
//bool DObjModelLoader::LoadObj(const char*file)
{
	ifstream ifile;
	ifile.open(file);
	if (ifile.fail())
	{
		return false;
	}
	char input;
	vector<float> vertexes;
	vector<float> uvs;
	vector<float> normals;

	vector<float> outvertexes;
	vector<float> outuvs;
	vector<float> outnormals;
	vector<int> outindexes;

	map<unsigned long int, int> maps;
	ifile.get(input);

	int index0, index1, index2;
	int index, cindex;
	int ci = 0;

	float v[3];

	unsigned long int key;

	cindex = 0;

	while (!ifile.eof())
	{
		if (input == 'v')
		{
			ifile.get(input);

			if (input == ' ')
			{
				ifile >> v[0] >> v[1] >> v[2];
				vertexes.push_back(v[0]);
				vertexes.push_back(v[1]);
				vertexes.push_back(v[2]);
			}
			if (input == 't')
			{
				ifile >> v[0] >> v[1] >> v[2];
				uvs.push_back(v[0]);
				uvs.push_back(1.0f - v[1]);
			}
			if (input == 'n')
			{
				ifile >> v[0] >> v[1] >> v[2];
				normals.push_back(v[0]);
				normals.push_back(v[1]);
				normals.push_back(v[2]);
			}
		}
		if (input == 'f')
		{
			ifile.get(input);
			if (input == ' ')
			{
				char indchar[3][32] = { 0 };
				ifile >> indchar[0] >> indchar[1] >> indchar[2];

				for (ci = 0; ci < 3; ci++) {
					sscanf_s(indchar[ci], "%d/%d/%d", &index0, &index1, &index2);
					index0 -= 1;
					index1 -= 1;
					index2 -= 1;
					key = index0 * 1000000000 + index1 * 10000 + index2;
					if (maps.count(key) != 0) {
						outindexes.push_back(maps[key]);
					}
					else {
						index = cindex;

						outvertexes.push_back(vertexes[index0 * 3]);
						outvertexes.push_back(vertexes[index0 * 3 + 1]);
						outvertexes.push_back(vertexes[index0 * 3 + 2]);
						outuvs.push_back(uvs[index1 * 2]);
						outuvs.push_back(uvs[index1 * 2 + 1]);
						outnormals.push_back(normals[index2 * 3]);
						outnormals.push_back(normals[index2 * 3 + 1]);
						outnormals.push_back(normals[index2 * 3 + 2]);
						outindexes.push_back(index);
						maps[key] = index;
						cindex += 1;
					}
				}
			}
		}
		ifile.get(input);
	}

	vertexCount = cindex;
	indexCount = outindexes.size();

	dataSize = sizeof(float) * 8;

	(*buffer) = new float[vertexCount * 8];
	(*indexBuffer) = new unsigned long[indexCount];

	int i, j, k;
	for (i = 0, j = 0, k = 0; i < vertexCount; i++, j += 3, k += 2)
	{
		(*buffer)[i * 8] = outvertexes[j];
		(*buffer)[i * 8 + 1] = outvertexes[j + 1];
		(*buffer)[i * 8 + 2] = outvertexes[j + 2];

		(*buffer)[i * 8 + 3] = outuvs[k];
		(*buffer)[i * 8 + 4] = outuvs[k + 1];

		(*buffer)[i * 8 + 5] = outnormals[j];
		(*buffer)[i * 8 + 6] = outnormals[j + 1];
		(*buffer)[i * 8 + 7] = outnormals[j + 2];
	}
	for (i = 0; i < indexCount; i++) {
		(*indexBuffer)[i] = outindexes[i];
	}

	ifile.close();
	vertexes.clear();;
	uvs.clear();
	normals.clear();
	outvertexes.clear();
	outuvs.clear();
	outnormals.clear();
	outindexes.clear();

	maps.clear();

	return true;
}

bool DModelLoader::CreateCube(float** buffer, unsigned long ** indexBuffer, int & vertexCount, int & indexCount, int& dataSize)
{
	return false;
}

bool DModelLoader::CreatePlane(float** buffer, unsigned long ** indexBuffer, int & vertexCount, int & indexCount, int& dataSize)
{
	dataSize = sizeof(float) * 8;

	float width = 20.0f;
	float height = 20.0f;

	float bx = -width*0.5f;
	float bz = -height*0.5f;

	int step = 10;

	float deltax = width / step;
	float deltaz = height / step;

	int i, j;

	vertexCount = (step + 1)*(step + 1);
	indexCount = (step)*(step)* 6;

	(*buffer) = new float[vertexCount*8];
	(*indexBuffer) = new unsigned long[indexCount];

	float x, z, u, v;
	int index, id;
	
	for (i = 0; i <= step; i++)
	{
		for (j = 0; j <= step; j++)
		{
			x = bx + i*deltax;
			z = bz + j*deltaz;

			u = ((float)i) / step;
			v = ((float)j) / step;

			index = i*(step + 1) + j;
			id = i*step + j;

			(*buffer)[index * 8] = x;
			(*buffer)[index * 8 + 1] = 0.0f;
			(*buffer)[index * 8 + 2] = z;

			(*buffer)[index * 8 + 3] = u;
			(*buffer)[index * 8 + 4] = v;

			(*buffer)[index * 8 + 5] = 0;
			(*buffer)[index * 8 + 6] = 1.0f;
			(*buffer)[index * 8 + 7] = 0;

			if (i != step&&j != step)
			{
				(*indexBuffer)[id *6] = i*(step + 1) + j;
				(*indexBuffer)[id *6+1] = i*(step + 1) + j+1;
				(*indexBuffer)[id *6+2] = (i+1)*(step + 1) + j+1;

				(*indexBuffer)[id *6+3] = i*(step + 1) + j;
				(*indexBuffer)[id *6+4] = (i+1)*(step + 1) + j+1;
				(*indexBuffer)[id *6+5] = (i+1)*(step + 1) + j;
			}
		}
	}

	return true;
}
