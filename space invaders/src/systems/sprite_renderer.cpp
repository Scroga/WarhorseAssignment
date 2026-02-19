#include "systems/sprite_renderer.h"

#include "utils/utility.h"
#include "utils/custom_vertex.h"

SpriteRenderer::SpriteRenderer(LPDIRECT3DDEVICE9 pd3dDevice) : pd3dDevice_(pd3dDevice) {}

/// Binds a texture to texture stage 0
/// @param tex pointer returned by load_sprite() (IDirect3DTexture9*)
void SpriteRenderer::set_current_texture(void* tex) const {
	IDirect3DTexture9* t = (IDirect3DTexture9*)tex;
	pd3dDevice_->SetTexture(0, t);
}

/// Loads a sprite texture from file
/// @param fname Path to the texture file
/// @return texture pointer (IDirect3DTexture9*), or nullptr on failure
void* SpriteRenderer::load_sprite(const char* fname) const  {
	IDirect3DTexture9* tex = NULL;
	D3DXCreateTextureFromFile(pd3dDevice_, fname, &tex);
	return tex;

}

/// Draws a textured quad (sprite) centered at (xcentre, ycentre)
/// @param sprite Texture returned by load_sprite()
/// @param xcentre,ycentre Sprite center in screen space (pixels)
/// @param xsize,ysize Half-size of the sprite in pixels
/// @param angle Rotation in radians (clockwise for screen-space Y-down)
/// @param col ARGB tint color (multiplied with texture)
void SpriteRenderer::draw_sprite(
	void* sprite, float xcentre, float ycentre, float xsize, float ysize,  float angle, DWORD col) const {

	set_current_texture(sprite);

	Vec2 center{ xcentre , ycentre };
	Vec2 p0 = rotate_point(center, Vec2(-xsize, -ysize), angle); // top-left
	Vec2 p1 = rotate_point(center, Vec2(xsize, -ysize), angle);  // top-right
	Vec2 p2 = rotate_point(center, Vec2(-xsize, ysize), angle);  // bottom-left
	Vec2 p3 = rotate_point(center, Vec2(xsize, ysize), angle);   // bottom-right

	CustomVertex quadVertices[] =
	{
		{ p0.x, p0.y, 0.5f, 1.0f, col, 0, 0 },
		{ p1.x, p1.y, 0.5f, 1.0f, col, 1, 0 },
		{ p2.x, p2.y, 0.5f, 1.0f, col, 0, 1 },
		{ p3.x, p3.y, 0.5f, 1.0f, col, 1, 1 },
	};

	pd3dDevice_->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, quadVertices, sizeof(CustomVertex));
}

/// Draws an untextured circle outline (line strip)
/// @param cx,cy Center in screen space (pixels)
/// @param r Radius in pixels
/// @param col ARGB color
/// @param segments Number of line segments (min 3, max 512)
/// @note Temporarily unbinds the current texture stage 0 and restores it afterwards
void SpriteRenderer::draw_circle(float cx, float cy, float r, DWORD col, int segments) const {
	if (segments < 3) segments = 3;
	if (segments > 512) segments = 512;

	IDirect3DBaseTexture9* oldtex = NULL;
	pd3dDevice_->GetTexture(0, &oldtex);
	pd3dDevice_->SetTexture(0, NULL);

	pd3dDevice_->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);

	CustomVertex* v = (CustomVertex*)_malloca(sizeof(CustomVertex) * (segments + 1));
	if (v == NULL) return;

	const float twoPi = PI * 2.0f;
	for (int i = 0; i <= segments; i++) {
		float a = (twoPi * i) / segments;
		float x = cx + cosf(a) * r;
		float y = cy + sinf(a) * r;

		v[i].x = x;
		v[i].y = y;
		v[i].z = 0.5f;
		v[i].rhw = 1.0f;
		v[i].color = col;
		v[i].u = 0.0f;
		v[i].v = 0.0f;
	}

	// draws a connected chain of lines
	pd3dDevice_->DrawPrimitiveUP(D3DPT_LINESTRIP, segments, v, sizeof(CustomVertex));
	_freea(v);

	pd3dDevice_->SetTexture(0, oldtex);
	if (oldtex) oldtex->Release();
}

