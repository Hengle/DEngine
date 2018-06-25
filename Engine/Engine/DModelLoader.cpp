#include "DModelLoader.h"
#include "DMath.h"
#include <string>
#include <vector>
#include <map>
#include <fstream>

using namespace std;

bool DModelLoader::LoadObj(const char * file, float** buffer, unsigned long ** indexBuffer, int & vertexCount, int & indexCount, int& bufferLength, int& dataSize)
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

	dataSize = sizeof(float) * 5;
	bufferLength = 5;

	(*buffer) = new float[vertexCount * 5];
	(*indexBuffer) = new unsigned long[indexCount];

	int i, j, k;
	for (i = 0, j = 0, k = 0; i < vertexCount; i++, j += 3, k += 2)
	{
		(*buffer)[i * 5] = outvertexes[j];
		(*buffer)[i * 5 + 1] = outvertexes[j + 1];
		(*buffer)[i * 5 + 2] = outvertexes[j + 2];

		(*buffer)[i * 5 + 3] = outuvs[k];
		(*buffer)[i * 5 + 4] = outuvs[k + 1];

		//(*buffer)[i * 8 + 5] = outnormals[j];
		//(*buffer)[i * 8 + 6] = outnormals[j + 1];
		//(*buffer)[i * 8 + 7] = outnormals[j + 2];
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

bool DModelLoader::CreateCube(float** buffer, unsigned long ** indexBuffer, int & vertexCount, int & indexCount, int& bufferLength, int& dataSize)
{
	dataSize = sizeof(float) * 5;
	vertexCount = 24;
	indexCount = 36;
	bufferLength = 5;

	(*buffer) = new float[vertexCount * 5];
	(*indexBuffer) = new unsigned long[indexCount];

	(*buffer)[0] = -0.5f; (*buffer)[1] = -0.5f; (*buffer)[2] = -0.5f; (*buffer)[3] = 0; (*buffer)[4] = 0;
	(*buffer)[5] = -0.5f; (*buffer)[6] = 0.5f; (*buffer)[7] = -0.5f; (*buffer)[8] = 0; (*buffer)[9] = 1;
	(*buffer)[10] = 0.5f; (*buffer)[11] = 0.5f; (*buffer)[12] = -0.5f; (*buffer)[13] = 1; (*buffer)[14] = 1;
	(*buffer)[15] = 0.5f; (*buffer)[16] = -0.5f; (*buffer)[17] = -0.5f; (*buffer)[18] = 1; (*buffer)[19] = 0;

	(*buffer)[20] = -0.5f; (*buffer)[21] = 0.5f; (*buffer)[22] = -0.5f; (*buffer)[23] = 0; (*buffer)[24] = 0;
	(*buffer)[25] = -0.5f; (*buffer)[26] = 0.5f; (*buffer)[27] = 0.5f; (*buffer)[28] = 0; (*buffer)[29] = 1;
	(*buffer)[30] = 0.5f; (*buffer)[31] = 0.5f; (*buffer)[32] = 0.5f; (*buffer)[33] = 1; (*buffer)[34] = 1;
	(*buffer)[35] = 0.5f; (*buffer)[36] = 0.5f; (*buffer)[37] = -0.5f; (*buffer)[38] = 1; (*buffer)[39] = 0;

	(*buffer)[40] = -0.5f; (*buffer)[41] = 0.5f; (*buffer)[42] = 0.5f; (*buffer)[43] = 0; (*buffer)[44] = 1;
	(*buffer)[45] = -0.5f; (*buffer)[46] = -0.5f; (*buffer)[47] = 0.5f; (*buffer)[48] = 0; (*buffer)[49] = 0;
	(*buffer)[50] = 0.5f; (*buffer)[51] = -0.5f; (*buffer)[52] = 0.5f; (*buffer)[53] = 1; (*buffer)[54] = 0;
	(*buffer)[55] = 0.5f; (*buffer)[56] = 0.5f; (*buffer)[57] = 0.5f; (*buffer)[58] = 1; (*buffer)[59] = 1;

	(*buffer)[60] = -0.5f; (*buffer)[61] = -0.5f; (*buffer)[62] = 0.5f; (*buffer)[63] = 0; (*buffer)[64] = 1;
	(*buffer)[65] = -0.5f; (*buffer)[66] = -0.5f; (*buffer)[67] = -0.5f; (*buffer)[68] = 0; (*buffer)[69] = 0;
	(*buffer)[70] = 0.5f; (*buffer)[71] = -0.5f; (*buffer)[72] = -0.5f; (*buffer)[73] = 1; (*buffer)[74] = 0;
	(*buffer)[75] = 0.5f; (*buffer)[76] = -0.5f; (*buffer)[77] = 0.5f; (*buffer)[78] = 1; (*buffer)[79] = 1;

	(*buffer)[80] = 0.5f; (*buffer)[81] = -0.5f; (*buffer)[82] = -0.5f; (*buffer)[83] = 0; (*buffer)[84] = 0;
	(*buffer)[85] = 0.5f; (*buffer)[86] = 0.5f; (*buffer)[87] = -0.5f; (*buffer)[88] = 0; (*buffer)[89] = 1;
	(*buffer)[90] = 0.5f; (*buffer)[91] = 0.5f; (*buffer)[92] = 0.5f; (*buffer)[93] = 1; (*buffer)[94] = 1;
	(*buffer)[95] = 0.5f; (*buffer)[96] = -0.5f; (*buffer)[97] = 0.5f; (*buffer)[98] = 1; (*buffer)[99] = 0;

	(*buffer)[100] = -0.5f; (*buffer)[101] = -0.5f; (*buffer)[102] = 0.5f; (*buffer)[103] = 0; (*buffer)[104] = 0;
	(*buffer)[105] = -0.5f; (*buffer)[106] = 0.5f; (*buffer)[107] = 0.5f; (*buffer)[108] = 0; (*buffer)[109] = 1;
	(*buffer)[110] = -0.5f; (*buffer)[111] = 0.5f; (*buffer)[112] = -0.5f; (*buffer)[113] = 1; (*buffer)[114] = 1;
	(*buffer)[115] = -0.5f; (*buffer)[116] = -0.5f; (*buffer)[117] = -0.5f; (*buffer)[118] = 1; (*buffer)[119] = 0;

	(*indexBuffer)[0] = 0; (*indexBuffer)[1] = 1; (*indexBuffer)[2] = 2;
	(*indexBuffer)[3] = 0; (*indexBuffer)[4] = 2; (*indexBuffer)[5] = 3;
	(*indexBuffer)[6] = 4; (*indexBuffer)[7] = 5; (*indexBuffer)[8] = 6;
	(*indexBuffer)[9] = 4; (*indexBuffer)[10] = 6; (*indexBuffer)[11] = 7;
	(*indexBuffer)[12] = 8; (*indexBuffer)[13] = 9; (*indexBuffer)[14] = 10;
	(*indexBuffer)[15] = 8; (*indexBuffer)[16] = 10; (*indexBuffer)[17] = 11;
	(*indexBuffer)[18] = 12; (*indexBuffer)[19] = 13; (*indexBuffer)[20] = 14;
	(*indexBuffer)[21] = 12; (*indexBuffer)[22] = 14; (*indexBuffer)[23] = 15;
	(*indexBuffer)[24] = 16; (*indexBuffer)[25] = 17; (*indexBuffer)[26] = 18;
	(*indexBuffer)[27] = 16; (*indexBuffer)[38] = 18; (*indexBuffer)[29] = 19;
	(*indexBuffer)[30] = 20; (*indexBuffer)[31] = 21; (*indexBuffer)[32] = 22;
	(*indexBuffer)[33] = 20; (*indexBuffer)[34] = 22; (*indexBuffer)[35] = 23;

	return true;
}

bool DModelLoader::CreatePlane(float** buffer, unsigned long ** indexBuffer, int & vertexCount, int & indexCount, int& bufferLength, int& dataSize)
{
	dataSize = sizeof(float) * 5;

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

	bufferLength = 5;

	(*buffer) = new float[vertexCount*5];
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

			(*buffer)[index * 5] = x;
			(*buffer)[index * 5 + 1] = 0.0f;
			(*buffer)[index * 5 + 2] = z;

			(*buffer)[index * 5 + 3] = u;
			(*buffer)[index * 5 + 4] = v;

			/*(*buffer)[index * 8 + 5] = 0;
			(*buffer)[index * 8 + 6] = 1.0f;
			(*buffer)[index * 8 + 7] = 0;*/

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
