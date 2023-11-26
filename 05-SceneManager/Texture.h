#pragma once

#include <d3dx9.h>
#include <unordered_map>
//
// Warpper class to simplify texture manipulation. See also CGame::LoadTexture
//
class CTextures
{
	static CTextures* __instance;

	unordered_map<int, LPDIRECT3DTEXTURE9> textures;

public:
	CTextures();
	void Add(int id, LPCWSTR filePath, D3DCOLOR transparentColor);
	LPDIRECT3DTEXTURE9 Get(unsigned int i);

	void Clear();
	static CTextures* GetInstance();
};

