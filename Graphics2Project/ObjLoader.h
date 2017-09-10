#pragma once
#include "stdafx.h"

class ObjLoader
{
	vector<SIMPLE_VERTEX> model;
	vector<unsigned int> index;

public:

	ObjLoader() = default;
	~ObjLoader() = default;

	vector<SIMPLE_VERTEX> & GetModel()  { return model; }
	vector<unsigned int> & GetIndex() { return index; }

	bool Load(const char * filepath);
};

bool ObjLoader::Load(const char * filepath)
{
	FILE * file = fopen(filepath, "r");

	if (file == nullptr)
		return false;

	vector<XMFLOAT4> points, normals, tempVertices, tempNormals;
	vector<XMFLOAT3> uv, tempUV;
	vector<unsigned int> posIndices, uvIndices, normalIndices;

	while (true)
	{
		char lineheader[128];
		int result = fscanf(file, "%s", lineheader);
		if (result == EOF)
			break;

		if (strcmp(lineheader, "v") == 0)
		{
			XMFLOAT4 position;
			fscanf(file, "%f %f %f\n", &position.x, &position.y, &position.z);
			tempVertices.push_back(position);
		}
		else if (strcmp(lineheader, "vt") == 0)
		{
			XMFLOAT3 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			tempUV.push_back(uv);
		}
		else if (strcmp(lineheader, "vn") == 0)
		{
			XMFLOAT4 normals;
			fscanf(file, "%f %f %f\n", &normals.x, &normals.y, &normals.z);
			tempNormals.push_back(normals);
		}
		else if (strcmp(lineheader, "f") == 0)
		{
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
			&vertexIndex[0], &uvIndex[0], &normalIndex[0],
			&vertexIndex[1], &uvIndex[1], &normalIndex[1],
			&vertexIndex[2], &uvIndex[2], &normalIndex[2]);

			posIndices.push_back(vertexIndex[0]);
			posIndices.push_back(vertexIndex[1]);
			posIndices.push_back(vertexIndex[2]);

			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);

			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	points = tempVertices;
	uv = tempUV;
	normals = tempNormals;

	for (unsigned int i = 0; i < posIndices.size(); i++)
	{
		SIMPLE_VERTEX temp;
		temp.points = points[posIndices[i] - 1];
		temp.uvs = uv[uvIndices[i] - 1];
		temp.uvs.y = 1.0f - temp.uvs.y;
		temp.normals = normals[normalIndices[i] - 1];
		model.push_back(temp);
		index.push_back(i);
	}

	return true;
}