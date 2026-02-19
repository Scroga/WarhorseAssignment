#pragma once

#include "utils/vec2.h"

#include <d3dx9.h>

class SpriteRenderer {
private:
	LPDIRECT3DDEVICE9 pd3dDevice_;

	void set_current_texture(void* tex) const;
public:
	SpriteRenderer(LPDIRECT3DDEVICE9 pd3dDevice);

	void* load_sprite(const char* fname) const;
	void draw_sprite( void* sprite, float xcentre, float ycentre, float xsize, float ysize, 
		float angle = 0, DWORD col = 0xffffffff) const;

	void draw_circle(float cx, float cy, float r, DWORD col = 0xffffffff, int segments = 64) const;
};