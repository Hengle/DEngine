#include "DModelLoader.h"
#include "DMath.h"
#include "DSystem.h"
#include <string>
#include <vector>
#include <map>
#include <fstream>

using namespace std;

struct DModelLoaderTangent
{
public:
	float x = 0.0f, y = 0.0f, z = 0.0f;
	int count = 0;
};

bool DModelLoader::LoadObj(const char * file, DGeometryBufferDesc* desc)
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
				bool uvstartsAtTop = DSystem::GetGraphicsMgr()->GetGLCore()->IsUVStartsAtTop();
				if (uvstartsAtTop)
					uvs.push_back(1.0f - v[1]);
				else
					uvs.push_back(v[1]);
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

	desc->vertexCount = cindex;
	desc->indexCount = outindexes.size();
	desc->colors = 0;
	desc->indices = new unsigned int[desc->indexCount];
	desc->normals = new float[desc->vertexCount * 3];
	desc->uv2s = 0;
	desc->uv3s = 0;
	desc->uvs = new float[desc->vertexCount * 2];
	desc->vertices = new float[desc->vertexCount * 3];
	desc->tangents = new float[desc->vertexCount * 4];
	desc->boneWeights = 0;
	desc->boneIndices = 0;

	DModelLoaderTangent* tangents = new DModelLoaderTangent[desc->vertexCount];
	//dataSize = sizeof(float) * 5;
	//bufferLength = 5;

	//(*buffer) = new float[vertexCount * 5];
	//(*indexBuffer) = new unsigned long[indexCount];

	DVector3 tangent;

	int i, j, k;
	for (i = 0, j = 0, k = 0; i < desc->vertexCount; i++, j += 3, k += 2)
	{
		desc->vertices[i * 3] = outvertexes[j];
		desc->vertices[i * 3 + 1] = outvertexes[j + 1];
		desc->vertices[i * 3 + 2] = outvertexes[j + 2];

		desc->uvs[i * 2] = outuvs[k];
		desc->uvs[i * 2 + 1] = outuvs[k + 1];

		desc->normals[i * 3] = outnormals[j];
		desc->normals[i * 3 + 1] = outnormals[j + 1];
		desc->normals[i * 3 + 2] = outnormals[j + 2];

	}
	for (i = 0; i < desc->indexCount; i+=3) {
		if (i + 1 >= desc->indexCount || i + 2 >= desc->indexCount)
			continue;
		desc->indices[i] = outindexes[i];
		desc->indices[i + 1] = outindexes[i + 1];
		desc->indices[i + 2] = outindexes[i + 2];

		CalculateTangent(&tangent, outindexes[i], outindexes[i + 1], outindexes[i + 2], desc);

		tangents[outindexes[i]].x += tangent.x;
		tangents[outindexes[i]].y += tangent.y;
		tangents[outindexes[i]].z += tangent.z;
		tangents[outindexes[i]].count += 1;

		tangents[outindexes[i + 1]].x += tangent.x;
		tangents[outindexes[i + 1]].y += tangent.y;
		tangents[outindexes[i + 1]].z += tangent.z;
		tangents[outindexes[i + 1]].count += 1;

		tangents[outindexes[i + 2]].x += tangent.x;
		tangents[outindexes[i + 2]].y += tangent.y;
		tangents[outindexes[i + 2]].z += tangent.z;
		tangents[outindexes[i + 2]].count += 1;
	}
	for (i = 0; i < desc->vertexCount; i++)
	{
		DModelLoaderTangent t = tangents[i];
		float tx = t.x;
		float ty = t.y;
		float tz = t.z;
		if (t.count > 0)
		{
			tx /= t.count;
			ty /= t.count;
			tz /= t.count;
		}
		float len = sqrtf(tx*tx + ty*ty + tz*tz);
		if (IS_FLOAT_EQUAL(len, 0.0f) == false) {
			tx /= len;
			ty /= len;
			tz /= len;
		}
		desc->tangents[i * 4] = tx;
		desc->tangents[i * 4 + 1] = ty;
		desc->tangents[i * 4 + 2] = tz;
		desc->tangents[i * 4 + 3] = -1.0f;
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

	delete[] tangents;
	tangents = 0;

	return true;
}

bool DModelLoader::LoadMesh(const char * path, DGeometryBufferDesc * desc, bool& dynamic)
{
	ifstream f(path, ios::binary);

	unsigned int vcount;
	unsigned int icount;

	f.read((char*)&vcount, sizeof(unsigned int));
	f.read((char*)&icount, sizeof(unsigned int));

	desc->vertexCount = vcount;
	desc->indexCount = icount;
	desc->colors = 0;
	desc->indices = new unsigned int[desc->indexCount];
	desc->normals = new float[desc->vertexCount * 3];
	desc->uv2s = 0;
	desc->uv3s = 0;
	desc->uvs = new float[desc->vertexCount * 2];
	desc->vertices = new float[desc->vertexCount * 3];
	desc->tangents = new float[desc->vertexCount * 4];
	desc->boneWeights = 0;
	desc->boneIndices = 0;

	char define;

	while (!f.eof())
	{
		define = 0;
		f.read((char*)&define, sizeof(char));
		if (define == 'v')
		{
			f.read((char*)desc->vertices, sizeof(float)*vcount * 3);
		}
		else if (define == 'u')
		{
			f.read((char*)desc->uvs, sizeof(float)*vcount * 2);
		}
		else if (define == 'n')
		{
			f.read((char*)desc->normals, sizeof(float)*vcount * 3);
		}
		else if (define == 'i')
		{
			f.read((char*)desc->indices, sizeof(unsigned int)*icount);
		}
		else if (define == 'b')
		{
			dynamic = true;
			if (desc->boneIndices == 0)
				desc->boneIndices = new unsigned int[desc->vertexCount * 4];
			f.read((char*)desc->boneIndices, sizeof(unsigned int)*vcount * 4);
		}
		else if (define == 'w')
		{
			dynamic = true;
			if (desc->boneWeights == 0)
				desc->boneWeights = new float[desc->vertexCount * 4];
			f.read((char*)desc->boneWeights, sizeof(float)*vcount*4);
		}
	}

	f.close();

	DModelLoaderTangent* tangents = new DModelLoaderTangent[desc->vertexCount];

	DVector3 tangent;

	int i;

	for (i = 0; i < desc->indexCount; i += 3) {
		if (i + 1 >= desc->indexCount || i + 2 >= desc->indexCount)
			continue;

		CalculateTangent(&tangent, desc->indices[i], desc->indices[i + 1], desc->indices[i + 2], desc);

		tangents[desc->indices[i]].x += tangent.x;
		tangents[desc->indices[i]].y += tangent.y;
		tangents[desc->indices[i]].z += tangent.z;
		tangents[desc->indices[i]].count += 1;

		tangents[desc->indices[i + 1]].x += tangent.x;
		tangents[desc->indices[i + 1]].y += tangent.y;
		tangents[desc->indices[i + 1]].z += tangent.z;
		tangents[desc->indices[i + 1]].count += 1;

		tangents[desc->indices[i + 2]].x += tangent.x;
		tangents[desc->indices[i + 2]].y += tangent.y;
		tangents[desc->indices[i + 2]].z += tangent.z;
		tangents[desc->indices[i + 2]].count += 1;
	}
	for (i = 0; i < desc->vertexCount; i++)
	{
		DModelLoaderTangent t = tangents[i];
		float tx = t.x;
		float ty = t.y;
		float tz = t.z;
		if (t.count > 0)
		{
			tx /= t.count;
			ty /= t.count;
			tz /= t.count;
		}
		float len = sqrtf(tx*tx + ty*ty + tz*tz);
		if (IS_FLOAT_EQUAL(len, 0.0f) == false) {
			tx /= len;
			ty /= len;
			tz /= len;
		}

		bool uvstartsAtTop = DSystem::GetGraphicsMgr()->GetGLCore()->IsUVStartsAtTop();
		if (uvstartsAtTop)
			desc->uvs[i * 2 + 1] = 1.0f - desc->uvs[i * 2 + 1];

		desc->tangents[i * 4] = tx;
		desc->tangents[i * 4 + 1] = ty;
		desc->tangents[i * 4 + 2] = tz;
		desc->tangents[i * 4 + 3] = -1.0f;
	}

	delete[] tangents;
	tangents = 0;

	return true;
}

//bool DModelLoader::CreateCube(DGeometryBufferDesc*)
//{
//	/*dataSize = sizeof(float) * 5;
//	vertexCount = 24;
//	indexCount = 36;
//	bufferLength = 5;
//
//	(*buffer) = new float[vertexCount * 5];
//	(*indexBuffer) = new unsigned long[indexCount];
//
//	(*buffer)[0] = 0.5f; (*buffer)[1] = -0.5f; (*buffer)[2] = 0.5f; (*buffer)[3] = 0.0f; (*buffer)[4] = 0.0f;
//	(*buffer)[5] = -0.5f; (*buffer)[6] = -0.5f; (*buffer)[7] = 0.5f; (*buffer)[8] = 1.0f; (*buffer)[9] = 0.0f;
//	(*buffer)[10] = 0.5f; (*buffer)[11] = 0.5f; (*buffer)[12] = 0.5f; (*buffer)[13] = 0.0f; (*buffer)[14] = 1.0f;
//	(*buffer)[15] = -0.5f; (*buffer)[16] = 0.5f; (*buffer)[17] = 0.5f; (*buffer)[18] = 1.0f; (*buffer)[19] = 1.0f;
//
//	(*buffer)[20] = 0.5f; (*buffer)[21] = 0.5f; (*buffer)[22] = -0.5f; (*buffer)[23] = 0.0f; (*buffer)[24] = 1.0f;
//	(*buffer)[25] = -0.5f; (*buffer)[26] = 0.5f; (*buffer)[27] = -0.5f; (*buffer)[28] = 1.0f; (*buffer)[29] = 1.0f;
//	(*buffer)[30] = 0.5f; (*buffer)[31] = -0.5f; (*buffer)[32] = -0.5f; (*buffer)[33] = 0.0f; (*buffer)[34] = 1.0f;
//	(*buffer)[35] = -0.5f; (*buffer)[36] = -0.5f; (*buffer)[37] = -0.5f; (*buffer)[38] = 1.0f; (*buffer)[39] = 1.0f;
//
//	(*buffer)[40] = 0.5f; (*buffer)[41] = 0.5f; (*buffer)[42] = 0.5f; (*buffer)[43] = 0.0f; (*buffer)[44] = 0.0f;
//	(*buffer)[45] = -0.5f; (*buffer)[46] = 0.5f; (*buffer)[47] = 0.5f; (*buffer)[48] = 1.0f; (*buffer)[49] = 0.0f;
//	(*buffer)[50] = 0.5f; (*buffer)[51] = 0.5f; (*buffer)[52] = -0.5f; (*buffer)[53] = 0.0f; (*buffer)[54] = 0.0f;
//	(*buffer)[55] = -0.5f; (*buffer)[56] = 0.5f; (*buffer)[57] = -0.5f; (*buffer)[58] = 1.0f; (*buffer)[59] = 0.0f;
//
//	(*buffer)[60] = 0.5f; (*buffer)[61] = -0.5f; (*buffer)[62] = -0.5f; (*buffer)[63] = 0.0f; (*buffer)[64] = 0.0f;
//	(*buffer)[65] = 0.5f; (*buffer)[66] = -0.5f; (*buffer)[67] = 0.5f; (*buffer)[68] = 0.0f; (*buffer)[69] = 1.0f;
//	(*buffer)[70] = -0.5f; (*buffer)[71] = -0.5f; (*buffer)[72] = 0.5f; (*buffer)[73] = 1.0f; (*buffer)[74] = 1.0f;
//	(*buffer)[75] = -0.5f; (*buffer)[76] = -0.5f; (*buffer)[77] = -0.5f; (*buffer)[78] = 1.0f; (*buffer)[79] = 0.0f;
//
//	(*buffer)[80] = -0.5f; (*buffer)[81] = -0.5f; (*buffer)[82] = 0.5f; (*buffer)[83] = 0.0f; (*buffer)[84] = 0.0f;
//	(*buffer)[85] = -0.5f; (*buffer)[86] = 0.5f; (*buffer)[87] = 0.5f; (*buffer)[88] = 0.0f; (*buffer)[89] = 1.0f;
//	(*buffer)[90] = -0.5f; (*buffer)[91] = 0.5f; (*buffer)[92] = -0.5f; (*buffer)[93] = 1.0f; (*buffer)[94] = 1.0f;
//	(*buffer)[95] = -0.5f; (*buffer)[96] = -0.5f; (*buffer)[97] = -0.5f; (*buffer)[98] = 1.0f; (*buffer)[99] = 0.0f;
//
//	(*buffer)[100] = 0.5f; (*buffer)[101] = -0.5f; (*buffer)[102] = -0.5f; (*buffer)[103] = 0.0f; (*buffer)[104] = 0.0f;
//	(*buffer)[105] = 0.5f; (*buffer)[106] = 0.5f; (*buffer)[107] = -0.5f; (*buffer)[108] = 0.0f; (*buffer)[109] = 1.0f;
//	(*buffer)[110] = 0.5f; (*buffer)[111] = 0.5f; (*buffer)[112] = 0.5f; (*buffer)[113] = 1.0f; (*buffer)[114] = 1.0f;
//	(*buffer)[115] = 0.5f; (*buffer)[116] = -0.5f; (*buffer)[117] = 0.5f; (*buffer)[118] = 1.0f; (*buffer)[119] = 0.0f;
//
//	(*indexBuffer)[0] = 0; (*indexBuffer)[1] = 2; (*indexBuffer)[2] = 3;
//	(*indexBuffer)[3] = 0; (*indexBuffer)[4] = 3; (*indexBuffer)[5] = 1;
//	(*indexBuffer)[6] = 8; (*indexBuffer)[7] = 4; (*indexBuffer)[8] = 5;
//	(*indexBuffer)[9] = 8; (*indexBuffer)[10] = 5; (*indexBuffer)[11] = 9;
//	(*indexBuffer)[12] = 10; (*indexBuffer)[13] = 6; (*indexBuffer)[14] = 7;
//	(*indexBuffer)[15] = 10; (*indexBuffer)[16] = 7; (*indexBuffer)[17] = 11;
//	(*indexBuffer)[18] = 12; (*indexBuffer)[19] = 13; (*indexBuffer)[20] = 14;
//	(*indexBuffer)[21] = 12; (*indexBuffer)[22] = 14; (*indexBuffer)[23] = 15;
//	(*indexBuffer)[24] = 16; (*indexBuffer)[25] = 17; (*indexBuffer)[26] = 18;
//	(*indexBuffer)[27] = 16; (*indexBuffer)[38] = 18; (*indexBuffer)[29] = 19;
//	(*indexBuffer)[30] = 20; (*indexBuffer)[31] = 21; (*indexBuffer)[32] = 22;
//	(*indexBuffer)[33] = 20; (*indexBuffer)[34] = 22; (*indexBuffer)[35] = 23;*/
//
//	return true;
//}
//
//bool DModelLoader::CreatePlane(DGeometryBufferDesc* desc)
//{
//	//dataSize = sizeof(float) * 5;
//
//	float width = 20.0f;
//	float height = 20.0f;
//
//	float bx = -width*0.5f;
//	float bz = -height*0.5f;
//
//	int step = 10;
//
//	float deltax = width / step;
//	float deltaz = height / step;
//
//	int i, j;
//
//	desc->vertexCount = (step + 1)*(step + 1);
//	desc->indexCount = (step)*(step)* 6;
//
//	//bufferLength = 5;
//
//	desc->vertices = new float[desc ->vertexCount*3];
//	desc->colors = 0;
//	desc->normals = new float[desc->vertexCount * 3];
//	desc->uv2s = 0;
//	desc->uv3s = 0;
//	desc->uvs = new float[desc->vertexCount * 2];
//	desc->indices = new unsigned int[desc->indexCount];
//	desc->tangents = new float[desc->vertexCount * 4];
//
//	float x, z, u, v;
//	int index, id;
//	
//	for (i = 0; i <= step; i++)
//	{
//		for (j = 0; j <= step; j++)
//		{
//			x = bx + i*deltax;
//			z = bz + j*deltaz;
//
//			u = ((float)i) / step;
//			v = ((float)j) / step;
//
//			index = i*(step + 1) + j;
//			id = i*step + j;
//
//			desc->vertices[index * 3] = x;
//			desc->vertices[index * 3 + 1] = 0.0f;
//			desc->vertices[index * 3 + 2] = z;
//
//			desc->uvs[index * 2] = u;
//			bool uvstartsAtTop = DSystem::GetGraphicsMgr()->GetGLCore()->IsUVStartsAtTop();
//			if(uvstartsAtTop)
//				desc->uvs[index * 2 + 1] = 1.0f - v;
//			else
//				desc->uvs[index * 2 + 1] = v;
//
//			desc->normals[index * 3] = 0;
//			desc->normals[index * 3 + 1] = 1.0f;
//			desc->normals[index * 3 + 2] = 0;
//
//			if (i != step&&j != step)
//			{
//				desc->indices[id *6] = i*(step + 1) + j;
//				desc->indices[id *6+1] = i*(step + 1) + j+1;
//				desc->indices[id *6+2] = (i+1)*(step + 1) + j+1;
//
//				desc->indices[id *6+3] = i*(step + 1) + j;
//				desc->indices[id *6+4] = (i+1)*(step + 1) + j+1;
//				desc->indices[id *6+5] = (i+1)*(step + 1) + j;
//			}
//		}
//	}
//
//	DModelLoaderTangent* tangents = new DModelLoaderTangent[desc->vertexCount];
//	DVector3 tangent;
//
//	for (i = 0; i < desc->indexCount; i += 3) {
//		if (i + 1 >= desc->indexCount || i + 2 >= desc->indexCount)
//			continue;
//
//		CalculateTangent(&tangent, desc->indices[i], desc->indices[i + 1], desc->indices[i + 2], desc);
//
//		tangents[desc->indices[i]].x += tangent.x;
//		tangents[desc->indices[i]].y += tangent.y;
//		tangents[desc->indices[i]].z += tangent.z;
//		tangents[desc->indices[i]].count += 1;
//
//		tangents[desc->indices[i + 1]].x += tangent.x;
//		tangents[desc->indices[i + 1]].y += tangent.y;
//		tangents[desc->indices[i + 1]].z += tangent.z;
//		tangents[desc->indices[i + 1]].count += 1;
//
//		tangents[desc->indices[i + 2]].x += tangent.x;
//		tangents[desc->indices[i + 2]].y += tangent.y;
//		tangents[desc->indices[i + 2]].z += tangent.z;
//		tangents[desc->indices[i + 2]].count += 1;
//	}
//	for (i = 0; i < desc->vertexCount; i++)
//	{
//		DModelLoaderTangent t = tangents[i];
//		float tx = t.x;
//		float ty = t.y;
//		float tz = t.z;
//		if (t.count > 0)
//		{
//			tx /= t.count;
//			ty /= t.count;
//			tz /= t.count;
//		}
//		float len = sqrtf(tx*tx + ty*ty + tz*tz);
//		if (IS_FLOAT_EQUAL(len, 0.0f) == false) {
//			tx /= len;
//			ty /= len;
//			tz /= len;
//		}
//		desc->tangents[i * 4] = tx;
//		desc->tangents[i * 4 + 1] = ty;
//		desc->tangents[i * 4 + 2] = tz;
//		desc->tangents[i * 4 + 3] = -1.0f;
//	}
//
//	delete[] tangents;
//	tangents = 0;
//
//	return true;
//}
//
//bool DModelLoader::CreateSphere(DGeometryBufferDesc * desc)
//{
//	int tri = 9;
//
//	desc->vertexCount = (tri + 1)*(tri + 1) * 6;
//	desc->indexCount = tri*tri * 2 * 3 * 6;
//
//	desc->vertices = new float[desc->vertexCount * 3];
//	desc->colors = 0;
//	desc->normals = new float[desc->vertexCount * 3];
//	desc->uv2s = 0;
//	desc->uv3s = 0;
//	desc->uvs = 0;
//	desc->indices = new unsigned int[desc->indexCount];
//	desc->tangents = new float[desc->vertexCount * 4];
//
//	SetSphereFace(0, tri, DVEC3_RIGHT, DVEC3_UP, DVEC3_FORWARD, desc);
//	SetSphereFace(1, tri, DVEC3_BACK, DVEC3_UP, DVEC3_RIGHT, desc);
//	SetSphereFace(2, tri, DVEC3_LEFT, DVEC3_UP, DVEC3_BACK, desc);
//	SetSphereFace(3, tri, DVEC3_FORWARD, DVEC3_UP, DVEC3_LEFT, desc);
//	SetSphereFace(4, tri, DVEC3_RIGHT, DVEC3_BACK, DVEC3_UP, desc);
//	SetSphereFace(5, tri, DVEC3_RIGHT, DVEC3_FORWARD, DVEC3_DOWN, desc);
//
//	DModelLoaderTangent* tangents = new DModelLoaderTangent[desc->vertexCount];
//	DVector3 tangent;
//
//	int i;
//	for (i = 0; i < desc->indexCount; i += 3) {
//		if (i + 1 >= desc->indexCount || i + 2 >= desc->indexCount)
//			continue;
//
//		CalculateTangent(&tangent, desc->indices[i], desc->indices[i + 1], desc->indices[i + 2], desc);
//
//		tangents[desc->indices[i]].x += tangent.x;
//		tangents[desc->indices[i]].y += tangent.y;
//		tangents[desc->indices[i]].z += tangent.z;
//		tangents[desc->indices[i]].count += 1;
//
//		tangents[desc->indices[i + 1]].x += tangent.x;
//		tangents[desc->indices[i + 1]].y += tangent.y;
//		tangents[desc->indices[i + 1]].z += tangent.z;
//		tangents[desc->indices[i + 1]].count += 1;
//
//		tangents[desc->indices[i + 2]].x += tangent.x;
//		tangents[desc->indices[i + 2]].y += tangent.y;
//		tangents[desc->indices[i + 2]].z += tangent.z;
//		tangents[desc->indices[i + 2]].count += 1;
//	}
//	for (i = 0; i < desc->vertexCount; i++)
//	{
//		DModelLoaderTangent t = tangents[i];
//		float tx = t.x;
//		float ty = t.y;
//		float tz = t.z;
//		if (t.count > 0)
//		{
//			tx /= t.count;
//			ty /= t.count;
//			tz /= t.count;
//		}
//		float len = sqrtf(tx*tx + ty*ty + tz*tz);
//		if (IS_FLOAT_EQUAL(len, 0.0f) == false) {
//			tx /= len;
//			ty /= len;
//			tz /= len;
//		}
//		desc->tangents[i * 4] = tx;
//		desc->tangents[i * 4 + 1] = ty;
//		desc->tangents[i * 4 + 2] = tz;
//		desc->tangents[i * 4 + 3] = -1.0f;
//	}
//
//	delete[] tangents;
//	tangents = 0;
//
//	return true;
//}
//
//void DModelLoader::SetSphereFace(int face, int triangleCount, DVector3 & right, DVector3 & up, DVector3 & forward, DGeometryBufferDesc * desc)
//{
//	DMatrix4x4 matrix = DMatrix4x4(right.x, right.y, right.z, 0.0f,
//		up.x, up.y, up.z, 0.0f,
//		forward.x, forward.y, forward.z, 0.0f,
//		0.0f, 0.0f, 0.0f, 1.0f);
//
//	int index = 0, i, j;
//	for (i = 0; i <= triangleCount; i++)
//	{
//		for (j = 0; j <= triangleCount; j++)
//		{
//			float delta = 1.0f / triangleCount;
//			DVector3 pos;
//			matrix.TransformPoint(DVector3(-0.5f + delta*j, -0.5f + delta*i, -0.5f), pos);
//			DVector3 normal;
//			pos.GetNormalized(normal);
//			//pos = normal*0.5f;
//			//Vector3 dir = pos.normalized;
//			int vindex = face * (triangleCount + 1) * (triangleCount + 1) + i * (triangleCount + 1) + j;
//			desc->vertices[vindex * 3] = normal.x * 0.5f;
//			desc->vertices[vindex * 3 + 1] = normal.y * 0.5f;
//			desc->vertices[vindex * 3 + 2] = normal.z * 0.5f;
//			desc->normals[vindex * 3] = normal.x;
//			desc->normals[vindex * 3 + 1] = normal.y;
//			desc->normals[vindex * 3 + 2] = normal.z;
//
//			if (i != triangleCount && j != triangleCount)
//			{
//				desc->indices[face*triangleCount*triangleCount * 2 * 3 + index] = face * (triangleCount + 1) * (triangleCount + 1) + i * (triangleCount + 1) + j;
//				desc->indices[face * triangleCount * triangleCount * 2 * 3 + index + 1] = face * (triangleCount + 1) * (triangleCount + 1) + (i + 1) * (triangleCount + 1) + j;
//				desc->indices[face * triangleCount * triangleCount * 2 * 3 + index + 2] = face * (triangleCount + 1) * (triangleCount + 1) + (i + 1) * (triangleCount + 1) + j + 1;
//
//				desc->indices[face * triangleCount * triangleCount * 2 * 3 + index + 3] = face * (triangleCount + 1) * (triangleCount + 1) + i * (triangleCount + 1) + j;
//				desc->indices[face * triangleCount * triangleCount * 2 * 3 + index + 4] = face * (triangleCount + 1) * (triangleCount + 1) + (i + 1) * (triangleCount + 1) + j + 1;
//				desc->indices[face * triangleCount * triangleCount * 2 * 3 + index + 5] = face * (triangleCount + 1) * (triangleCount + 1) + i * (triangleCount + 1) + j + 1;
//
//				index += 6;
//			}
//		}
//	}
//}

void DModelLoader::CalculateTangent(DVector3 * out, int index0, int index1, int index2, DGeometryBufferDesc * desc)
{
	if (desc->uvs == 0)
	{
		*out = DVector3(0.0f, 0.0f, 0.0f);
		return;
	}
	DVector3 v0 = DVector3(desc->vertices[index0 * 3], desc->vertices[index0 * 3 + 1], desc->vertices[index0 * 3 + 2]);
	DVector3 v1 = DVector3(desc->vertices[index1 * 3], desc->vertices[index1 * 3 + 1], desc->vertices[index1 * 3 + 2]);
	DVector3 v2 = DVector3(desc->vertices[index2 * 3], desc->vertices[index2 * 3 + 1], desc->vertices[index2 * 3 + 2]);

	float udelta20y = desc->uvs[index2 * 2 + 1] - desc->uvs[index0 * 2 + 1];
	float udelta10y = desc->uvs[index1 * 2 + 1] - desc->uvs[index0 * 2 + 1];
	float udelta10x = desc->uvs[index1 * 2] - desc->uvs[index0 * 2];
	float udelta20x = desc->uvs[index2 * 2] - desc->uvs[index0 * 2];

	float denominator = udelta10x * udelta20y - udelta20x * udelta10y;
	if (IS_FLOAT_EQUAL(denominator, 0.0f)) 
	{
		*out = DVector3(0.0f, 0.0f, 0.0f);
		return;
	}

	*out = (udelta20y * (v1 - v0) - udelta10y * (v2 - v0)) / denominator;
}

