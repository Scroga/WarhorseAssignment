#include "game_application.h"

#include "utils/custom_vertex.h" 

LRESULT GameApplication::handle_message(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	// msg:
	// WM_PAINT		needs repaint
	// WM_SIZE		resized
	// WM_DESTROY   window is being destroyed
	// WM_KEYDOWN / WM_KEYUP
	// WM_MOUSEMOVE, WM_LBUTTONDOWN

	switch (msg) {
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		input_handler_.on_key_down((int)wParam);
		return 0;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		input_handler_.on_key_up((int)wParam);
		return 0;

	case WM_LBUTTONDOWN:
		SetCapture(hWnd);
		input_handler_.on_mouse_down(VK_LBUTTON);
		return 0;

	case WM_LBUTTONUP:
		ReleaseCapture();
		input_handler_.on_mouse_up(VK_LBUTTON);
		return 0;

	case WM_RBUTTONDOWN:
		SetCapture(hWnd);
		input_handler_.on_mouse_down(VK_RBUTTON);
		return 0;

	case WM_RBUTTONUP:
		ReleaseCapture();
		input_handler_.on_mouse_up(VK_RBUTTON);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	// default message handling
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/// Creates the Direct3D9 interface and rendering device bound to the given window
/// Configures basic parameters (windowed/fullscreen, backbuffer size, vsync) 
/// and stores the created objects in class pointers
/// @param hWnd Target window handle for rendering
/// @return S_OK on success, E_FAIL on failure
HRESULT GameApplication::init_D3D(HWND hWnd) {
	// Create the D3D object
	if (NULL == (pD3D_ = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	// Set up the structure used to create the D3DDevice

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	D3DDISPLAYMODE dm;
	pD3D_->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dm);

	if (fullscreen_) {
		window_width_ = dm.Width;
		window_height_ = dm.Height;
	}

	d3dpp.Windowed = !fullscreen_;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	d3dpp.BackBufferFormat = fullscreen_ ? dm.Format : D3DFMT_UNKNOWN;
	d3dpp.BackBufferWidth = window_width_;
	d3dpp.BackBufferHeight = window_height_;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.FullScreen_RefreshRateInHz = 0;

	// Create the D3DDevice
	if (FAILED(pD3D_->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &pd3dDevice_)))
	{
		return E_FAIL;
	}

	return S_OK;
}

/// Creates a small dynamic vertex buffer used for drawing 2D quads(sprites)
/// The buffer holds 4 CUSTOMVERTEX vertices (one screen-space quad) and is
/// intended to be updated frequently (lock/discard) before drawing
/// @return S_OK if the vertex buffer was created successfuly, otherwise E_FAIL
HRESULT GameApplication::init_VB() {
	if (FAILED(pd3dDevice_->CreateVertexBuffer(4 * sizeof(CustomVertex),
		D3DUSAGE_DYNAMIC, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &pVB_, NULL)))
	{
		return E_FAIL;
	}
	return S_OK;
}

/// Returns "now" as raw QPC ticks
LARGE_INTEGER GameApplication::now_ticks() {
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	return t;
}

/// Convert ticks tp milliseconds since start
long long GameApplication::ticks_to_ms(LARGE_INTEGER t) const {
	return ((t.QuadPart - start_time_.QuadPart) * 1000LL) / freq_.QuadPart;
}

/// Processes pending Windows messages, clears the backbuffer to `clear_color`,
/// sets render states, and begins the D3D scene
/// @param clear_color Color used to clear the backbuffer at the start of the frame
/// @return true if a quit message (WM_QUIT) was received and the main loop should exit, otherwise false
bool GameApplication::begin_frame(DWORD clear_color) {
	// enter the message loop
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT) return true;
	}

	// clear the frame buffer
	pd3dDevice_->Clear(0, NULL, D3DCLEAR_TARGET, clear_color, 1.0f, 0);

	// setup render state for this frame
	pd3dDevice_->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	pd3dDevice_->SetRenderState(D3DRS_ZENABLE, false);

	pd3dDevice_->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pd3dDevice_->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pd3dDevice_->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	pd3dDevice_->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	pd3dDevice_->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	pd3dDevice_->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	pd3dDevice_->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	pd3dDevice_->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	pd3dDevice_->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pd3dDevice_->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pd3dDevice_->SetRenderState(D3DRS_LIGHTING, false);

	pd3dDevice_->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// setup viewport
	D3DVIEWPORT9 vp{};
	vp.X = 0; vp.Y = 0;
	vp.Width = window_width_;
	vp.Height = window_height_;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	pd3dDevice_->SetViewport(&vp);

	pd3dDevice_->SetFVF(D3DFVF_CUSTOMVERTEX);

	pd3dDevice_->BeginScene();

	return false;
}

/// Finishes rendering of the current frame and displays it on screen
/// Additionally caps the frame rate to 60 fps using simple time check
void GameApplication::end_frame() {
	if (last_draw_.QuadPart == 0) 
		last_draw_ = now_ticks();

	pd3dDevice_->EndScene(); // end the rendering for this frame

	while (ticks_to_ms(now_ticks()) < ticks_to_ms(last_draw_) + 1000 / 60) { Sleep(0); }

	pd3dDevice_->Present(NULL, NULL, NULL, NULL);

	last_draw_ = now_ticks();

	input_handler_.clear_hits();
	SetCursor(LoadCursor(NULL, IDC_ARROW));
}

/// Creates and runs the main application: registers a Win32 window class,
/// creates the main window, initializes audio (FMOD) and Direct3D resources,
/// then enters the main game loop until the user quits
/// @param hInst Handle to this application's instance, used to load resources
/// @return exit code from the application.
int GameApplication::run(HINSTANCE hInst) {
	// calls message box
	int id = MessageBox(NULL, "fullscreen?", "answer me!", MB_YESNOCANCEL);
	if (id == IDCANCEL) return 0;
	fullscreen_ = (id == IDYES);

	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_CLASSDC;
	wc.hInstance = hInst;
	wc.lpfnWndProc = &GameApplication::MsgProc; // function that handles messages
	wc.lpszClassName = window_class_name;

	ATOM a = RegisterClassEx(&wc);
	if (!a) {
		MessageBox(nullptr, "RegisterClassEx failed", "Error", MB_OK);
		return 0;
	}

	RECT rect = {};
	rect.left = 0;
	rect.top = 0;
	rect.right = window_width_;
	rect.bottom = window_height_;

	// WS_POPUP - borderless window
	// WS_OVERLAPPEDWINDOW - window with borders
	DWORD style = fullscreen_ ? WS_POPUP : WS_OVERLAPPEDWINDOW;
	style |= WS_VISIBLE; // create the window already visible
	bool has_menu = false;
	AdjustWindowRect(&rect, style, has_menu);

	hWnd_ = CreateWindow(
		window_class_name, window_title,
		style, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
		NULL, NULL, wc.hInstance, this); // pass "this" to later recover it in MsgProc

	FSOUND_Init(44100, 42, 0);
	QueryPerformanceCounter(&start_time_);
	QueryPerformanceFrequency(&freq_);

	// initialize Direct3D
	if (SUCCEEDED(init_D3D(hWnd_)))
	{
		// create the vertex buffer
		if (SUCCEEDED(init_VB()))
		{
			SetWindowPos(hWnd_, NULL, 0, 0, window_width_, window_height_, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE | SWP_ASYNCWINDOWPOS);
			SetCursor(LoadCursor(NULL, IDC_ARROW));

			// show the window
			ShowWindow(hWnd_, fullscreen_ ? SW_SHOWMAXIMIZED : SW_SHOWDEFAULT);
			UpdateWindow(hWnd_);

			// setup game services
			GameServices game_services(pd3dDevice_, input_handler_, Vec2{ (float)window_width_, (float)window_height_ });
			game_services.asset_manager.preload();

			// setup and run game
			SpaceInvaders game(game_services);
			while (!begin_frame() && !input_handler_.is_key_down(VK_ESCAPE)) {
				game.in_loop();
				end_frame();
			}
		}
	}

	UnregisterClass(window_class_name, wc.hInstance);
	return 0;
}

/// Releases all previously initialized objects
void GameApplication::cleanup() {
	if (pVB_ != NULL)
		pVB_->Release();

	if (pd3dDevice_ != NULL)
		pd3dDevice_->Release();

	if (pD3D_ != NULL)
		pD3D_->Release();
}

/// Windows window procedure. It's the function Windows calls whenever it receives a message (keyboard, mouse, resize, close)
/// @param hWnd Handle to the window that is receiving the message
/// @param msg The message ID
/// @param wParam "Word parameter" - extra info for the message, meaning depends on msg
/// @param lParam "Long parameter" - more extra info, meaning depends on msg
/// @return LRESULT "Long result" the value returned back to Windows as the result of handling the message
LRESULT WINAPI GameApplication::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_NCCREATE) {
		auto* cs = reinterpret_cast<CREATESTRUCT*>(lParam); // cs points to a struct that contains window creation details
		auto* app = reinterpret_cast<GameApplication*>(cs->lpCreateParams); // take the pointer that was passed to CreateWindow

		// store the pointer inside the window object so it can be retrieved later
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)app);
	}

	auto* app = reinterpret_cast<GameApplication*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	return app ? app->handle_message(hWnd, msg, wParam, lParam) : DefWindowProc(hWnd, msg, wParam, lParam);
}

/// The application's entry point
// @param hInst Handle to this application's instance, used to load resources
// @return exit code from the application.
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT) {
	GameApplication app;
	return app.run(hInst);
}
