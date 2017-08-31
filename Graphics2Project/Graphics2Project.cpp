// Graphics2Project.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Graphics2Project.h"

#define MAX_LOADSTRING 100
#define BACKBUFFER_WIDTH	500 // 729
#define BACKBUFFER_HEIGHT	500 // 640

//----------------------------------------------------------------------------------------------------------
// Global Variables:
//----------------------------------------------------------------------------------------------------------
HWND							hWnd;
HINSTANCE						hInst;                                    // current instance
WCHAR							szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR							szWindowClass[MAX_LOADSTRING];            // the main window class name
ID3D11Device*					m_pDevice				= nullptr;
ID3D11DeviceContext*			m_pDeviceContext		= nullptr;
IDXGISwapChain*					m_pSwapChain			= nullptr;
ID3D11RenderTargetView*			m_pRenderTargetView 	= nullptr;
ID3D11Resource*					m_pBackBuffer			= nullptr;
ID3D11Buffer*					m_pConstantbuffer		= nullptr;


ID3D11VertexShader* 			m_pVertexShader			= nullptr;
ID3D11PixelShader*				m_pPixelShader			= nullptr;

//ID3D11Debug*					debug					= nullptr;

D3D_DRIVER_TYPE					m_DriverType;
D3D_FEATURE_LEVEL				m_FeatureLevel;
D3D11_VIEWPORT					m_ViewPort;

XMMATRIX						WorldMatrix;
XMMATRIX						ViewMatrix;
XMMATRIX						ProjectionMatrix;


//----------------------------------------------------------------------------------------------------------
// Structures
//----------------------------------------------------------------------------------------------------------

// Vertex Loading
struct SIMPLE_VERTEX {
	XMFLOAT4					points;
	XMFLOAT4					Color;
};


struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};

// Constant Buffer
struct SEND_TO_VRAM {
	XMFLOAT4					ConstantColor;
	XMFLOAT2					ConstantOffset;
	XMFLOAT2					Padding;
};


//----------------------------------------------------------------------------------------------------------
// Forward declarations of functions included in this code module:
//----------------------------------------------------------------------------------------------------------
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
HRESULT Initialize();
void Shutdown();
bool Run();

// Microsoft::WRL::ComPtr<var> name;


//----------------------------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//----------------------------------------------------------------------------------------------------------
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.


    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GRAPHICS2PROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow)) {
        return FALSE;
    }

	if (FAILED(Initialize())) {
		Shutdown();
		return 0;
	}
	
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GRAPHICS2PROJECT));

    MSG msg; ZeroMemory(&msg, sizeof(msg));

    // Main message loop:
    while (msg.message != WM_QUIT && Run())
    {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
	
	Shutdown();

    return (int) msg.wParam;
}



//----------------------------------------------------------------------------------------------------------
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the windows class.
//----------------------------------------------------------------------------------------------------------
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style                     = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc               = WndProc;
    wcex.cbClsExtra                = 0;
    wcex.cbWndExtra                = 0;
    wcex.hInstance                 = hInstance;
    wcex.hIcon                     = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GRAPHICS2PROJECT));
    wcex.hCursor                   = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground             = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName              = NULL;
    wcex.lpszClassName             = szWindowClass;
    wcex.hIconSm                   = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//----------------------------------------------------------------------------------------------------------
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and  ----- Creates main window
//
//   COMMENTS:
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//----------------------------------------------------------------------------------------------------------
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindowW(szWindowClass, L"Graphics Project", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, NULL, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd) {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//----------------------------------------------------------------------------------------------------------
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//----------------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
// Message handler for about box.
//--------------------------------------------------------------------------------------
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

HRESULT Initialize() {

	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	DXGI_SWAP_CHAIN_DESC swapdesc;
	ZeroMemory(&swapdesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapdesc.BufferCount = 1;
	swapdesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapdesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapdesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapdesc.OutputWindow = hWnd;
	swapdesc.SampleDesc.Count = 1;
	swapdesc.SampleDesc.Quality = 0;
	swapdesc.Windowed = true;

	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG, nullptr,
		NULL, D3D11_SDK_VERSION, &swapdesc, &m_pSwapChain, &m_pDevice, &m_FeatureLevel, &m_pDeviceContext);

	// Initialize the SwapChain
	m_pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&m_pBackBuffer));
	m_pDevice->CreateRenderTargetView(m_pBackBuffer, NULL, &m_pRenderTargetView);
	m_pBackBuffer->Release();

	// Initialize the Viewport
	m_ViewPort.Width = width;
	m_ViewPort.Height = height;
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;




	// Shader Decleration
	m_pDevice->CreateVertexShader(Trivial_VS, sizeof(Trivial_VS), NULL, &m_pVertexShader);
	m_pDevice->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), NULL, &m_pPixelShader);

	// Initialize/Create Constant Buffer
	D3D11_BUFFER_DESC		Constantbuffdesc;
	ZeroMemory(&Constantbuffdesc, sizeof(D3D11_BUFFER_DESC));
	Constantbuffdesc.Usage = D3D11_USAGE_DYNAMIC;
	Constantbuffdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Constantbuffdesc.ByteWidth = sizeof(SEND_TO_VRAM);
	Constantbuffdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_pDevice->CreateBuffer(&Constantbuffdesc, nullptr, &m_pConstantbuffer);

	// Initialize the world matrix
	WorldMatrix = XMMatrixIdentity();

	// Initialize the view matrix
	XMVECTOR Eye = XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
	XMVECTOR Focus = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	ViewMatrix = XMMatrixLookAtLH(Eye, Focus, Up);

	// Initialize the projection matrix
	ProjectionMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV2, width / (FLOAT)height, 0.01f, 100.0f);

	return S_OK;
}

bool Run() {

	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);

	m_pDeviceContext->RSSetViewports(1, &m_ViewPort);

	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, Colors::DarkCyan);

	//// Update variables
	//ConstantBuffer cb;
	//cb.mWorld = XMMatrixTranspose(WorldMatrix);
	//cb.mView = XMMatrixTranspose(ViewMatrix);
	//cb.mProjection = XMMatrixTranspose(ProjectionMatrix);
	//m_pDeviceContext->UpdateSubresource(m_pConstantbuffer, 0, nullptr, &cb, 0, 0);
	//// Renders triangles
	//m_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	//m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantbuffer);
	//m_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
	//m_pDeviceContext->DrawIndexed(36, 0, 0);
	
	// Present our back buffer to our front buffer
	m_pSwapChain->Present(0, 0);

	return true;
}

void Shutdown() {

	if (m_pSwapChain) { m_pSwapChain->Release(); }
	if (m_pDevice) { m_pDevice->Release(); }
	if (m_pRenderTargetView) { m_pRenderTargetView->Release(); }
	if (m_pDeviceContext) { m_pDeviceContext->Release(); }
	if (m_pConstantbuffer) { m_pConstantbuffer->Release(); }

	if (m_pVertexShader) { m_pVertexShader->Release(); }
	if (m_pPixelShader) { m_pPixelShader->Release(); }
}