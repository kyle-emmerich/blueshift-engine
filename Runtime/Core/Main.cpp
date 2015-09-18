#include <bgfx.h>
#include <bgfxplatform.h>

#include <Windows.h>
#include <thread>

struct Context {
	int width;
	int height;

	bool stop = false;

	void* WindowHandle;
};

struct PosColorVertex 
{
	float x;
	float y;
	float z;

	uint32_t color;

	static void init() 
	{
		ms_decl.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
			.end();
	}
	static bgfx::VertexDecl ms_decl;
};
bgfx::VertexDecl PosColorVertex::ms_decl;

static PosColorVertex cube_test[8] =
{
	{ -1.0f,	1.0f,  1.0f, 0xfff0f0f0 },
	{  1.0f,	1.0f,  1.0f, 0xfff0f0f0 },
	{ -1.0f,   -1.0f,  1.0f, 0xfff0f0f0 },
	{  1.0f,   -1.0f,  1.0f, 0xfff0f0f0 },
	{ -1.0f,	1.0f, -1.0f, 0xfff0f0f0 },
	{  1.0f,	1.0f, -1.0f, 0xfff0f0f0 },
	{ -1.0f,   -1.0f, -1.0f, 0xfff0f0f0 },
	{  1.0f,   -1.0f, -1.0f, 0xfff0f0f0 },
};


LRESULT CALLBACK WndProc(HWND handle, UINT id, WPARAM high, LPARAM low);
void main_thread(Context* ctx);

int main(int argc, char* argv[]) {
	Context* ctx = new Context;
	ctx->width = 1280;
	ctx->height = 720;

	WNDCLASSEX wnd;
	ZeroMemory(&wnd, sizeof(wnd));
	wnd.cbSize = sizeof(wnd);
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.lpfnWndProc = WndProc;
	wnd.hInstance = (HINSTANCE)GetModuleHandle(NULL);
	wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd.lpszClassName = L"Astronomer Engine";
	wnd.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wnd);

	RECT size = { 0, 0, ctx->width, ctx->height }; AdjustWindowRect(&size, WS_OVERLAPPEDWINDOW, false);
	HWND handle = CreateWindow(L"Astronomer Engine", L"NoxAstra", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, size.right, size.bottom, NULL, NULL, GetModuleHandle(NULL), 0);
	SetWindowLongPtr(handle, GWLP_USERDATA, (LONG_PTR)ctx);

	bgfx::winSetHwnd(handle);
	
	ctx->WindowHandle = (void*)handle;

	std::thread main(main_thread, ctx);

	MSG msg;
	while (!ctx->stop) {
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT) {
				ctx->stop = true;
			}
		}
	}

	main.join();
	
	DestroyWindow(handle);
	delete ctx;
	return 0;
}

LRESULT CALLBACK WndProc(HWND handle, UINT id, WPARAM high, LPARAM low) {
	switch (id) {
	case WM_DESTROY:
		break;
	case WM_QUIT:
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		Context* ctx = (Context*)GetWindowLongPtr(handle, GWLP_USERDATA);
		if (ctx) 
		{
			RECT rect; GetClientRect(handle, &rect);

			ctx->width = rect.right - rect.left;
			ctx->height = rect.bottom - rect.top;
		}
		break;
	}
	return DefWindowProc(handle, id, high, low);
}

void main_thread(Context* ctx) {
	bgfx::init();
	bgfx::reset(ctx->width, ctx->height, BGFX_RESET_VSYNC);

	bgfx::setDebug(BGFX_DEBUG_TEXT);

	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

	int old_width = ctx->width; 
	int old_height = ctx->height;

	while (!ctx->stop) {
		if (old_width != ctx->width || old_height != ctx->height) {
			bgfx::reset(ctx->width, ctx->height, BGFX_RESET_VSYNC);
		}
		bgfx::setViewRect(0, 0, 0, ctx->width, ctx->height);
		bgfx::touch(0);

		bgfx::dbgTextClear();
		bgfx::dbgTextPrintf(0, 0, 0x4f, "testing my butthole");

		bgfx::frame();
	}
	bgfx::shutdown();
}