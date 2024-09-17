#pragma warning(disable : 6011)

#include <Windows.h>
#include <ShlObj.h>
#include <cinttypes>
#include <filesystem>
#include <string>
#include "MinHook.h"
#include "fnv_hash.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "vmt_smart_hook.hpp"

#include "CheatManager.hpp"
#include "Hook.hpp"

static ID3D11Device* g_pd3dDevice = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static ID3D11DeviceContext* g_pd3dContext = nullptr;
static ID3D11RenderTargetView* view = nullptr;
static HWND g_hwnd = nullptr;
void* origin_present = nullptr;
using Present = HRESULT(__stdcall*)(IDXGISwapChain*, UINT, UINT);

WNDPROC origin_wndProc;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

LRESULT __stdcall WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam)) { return true; }
	return CallWindowProc(origin_wndProc, hwnd, uMsg, wParam, lParam);
}

static void __stdcall init_imgui(IDXGISwapChain* device) noexcept
{
	g_pSwapChain = device;
	g_pSwapChain->GetDevice(__uuidof(g_pd3dDevice), reinterpret_cast<void**>(&(g_pd3dDevice)));
	g_pd3dDevice->GetImmediateContext(&g_pd3dContext);

	DXGI_SWAP_CHAIN_DESC sd;
	g_pSwapChain->GetDesc(&sd);
	g_hwnd = sd.OutputWindow;

	ID3D11Texture2D* buf{};
	device->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buf);
	g_pd3dDevice->CreateRenderTargetView(buf, nullptr, &view);
	buf->Release();

	origin_wndProc = WNDPROC(::SetWindowLongPtr(g_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc)));

	ImGui::CreateContext();
	ImGui_ImplWin32_Init(g_hwnd);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dContext);
	ImGui_ImplDX11_CreateDeviceObjects();
}




bool init = true;
long __stdcall my_present(IDXGISwapChain* device, UINT a, UINT b) {
	if (init) {
		init = false;
		init_imgui(device);
	}
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	cheatManager.gui->render();
	ImGui::EndFrame();
	ImGui::Render();
	g_pd3dContext->OMSetRenderTargets(1, &view, nullptr);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return ((Present)origin_present)(device, a, b);
}


void Hooks::init() noexcept{
	const unsigned level_count = 2;
	D3D_FEATURE_LEVEL levels[level_count] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
	DXGI_SWAP_CHAIN_DESC sd{};
	sd.BufferCount = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = GetForegroundWindow();
	sd.SampleDesc.Count = 1;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	auto hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		levels,
		level_count,
		D3D11_SDK_VERSION,
		&sd,
		&g_pSwapChain,
		&g_pd3dDevice,
		nullptr,
		nullptr);
}

void Hooks::install() noexcept
{
	this->init();
	if (g_pSwapChain) {
		auto vtable_ptr = (void***)(g_pSwapChain);
		auto vtable = *vtable_ptr;
		auto present = vtable[8];
		MH_Initialize();
		MH_CreateHook(present, my_present, &origin_present);
		MH_EnableHook(present);
		// 看看是否需要hook 13 
		g_pd3dDevice->Release();
		g_pSwapChain->Release();
	}
}

void Hooks::uninstall() noexcept
{
	//::SetWindowLongW(cheatManager.memory->window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(originalWndProc));
	//swap_chain_vmt->unhook();
	//cheatManager.cheatState = false;
}
