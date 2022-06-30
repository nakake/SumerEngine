#pragma once


#include "IFManager.h"
#include "ShaderSturuct.h"
#include "StringManager.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


struct Mesh;
struct Vertex;

struct aiMesh;
struct aiMaterial;

struct ImportSettings
{
	const wchar_t*		filename = nullptr;
	std::vector<Mesh>&	meshes;
	bool				inverseU;
	bool				inverseV;
};

class AssimpLoader
{
public:
	bool Load(ImportSettings setting);
private:
	void LoadMesh(Mesh& dst, const aiMesh* src, bool inverseU, bool inversV);
	void LoadTexture(const wchar_t* filename, Mesh& dst, const aiMaterial* src);
};

