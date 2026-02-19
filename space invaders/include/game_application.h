#pragma once

#include <memory>

#include "fmod/api/inc/fmod.h"
#pragma comment(lib,"lib/fmod/api/lib/fmodvc.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

#include "utils/game_services.h"
#include "space_invaders.h"

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

class GameApplication {
private:
	LPDIRECT3D9             pD3D_ = NULL; // Used to create the D3DDevice
	LPDIRECT3DDEVICE9       pd3dDevice_ = NULL; // Our rendering device
	LPDIRECT3DVERTEXBUFFER9 pVB_ = NULL; // Buffer to hold tea2

	LARGE_INTEGER start_time_{};
	LARGE_INTEGER last_draw_{};
	LARGE_INTEGER freq_{};

	HWND hWnd_{};
	
	InputHandler input_handler_{};

	int window_width_ = 1280;
	int window_height_ = 720;
	bool fullscreen_ = true;

	HRESULT init_D3D(HWND hWnd);
	HRESULT init_VB();
	LRESULT handle_message(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	static LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	long long ticks_to_ms(LARGE_INTEGER t) const;
	LARGE_INTEGER now_ticks();

	bool begin_frame(DWORD clear_color = 0);
	void end_frame();
	void cleanup();
public:
	static constexpr const char* window_class_name = "SpaceInvaders";
	static constexpr const char* window_title = "Space Invaders";

	int run(HINSTANCE hInst);
};