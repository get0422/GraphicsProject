// Graphics2Project.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Graphics2Project.h"
#include "ObjLoader.h"

#define MAX_LOADSTRING 100
#define BACKBUFFER_WIDTH	500 // 729
#define BACKBUFFER_HEIGHT	500 // 640
#define ColorCube			0
#define TextureCube			1

//----------------------------------------------------------------------------------------------------------
// Global Variables:
//----------------------------------------------------------------------------------------------------------
#pragma region
HWND							hWnd;
HINSTANCE						hInst;                                    // current instance
WCHAR							szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR							szWindowClass[MAX_LOADSTRING];            // the main window class name
ID3D11Device*					m_pDevice				= nullptr;
ID3D11DeviceContext*			m_pDeviceContext		= nullptr;
IDXGISwapChain*					m_pSwapChain			= nullptr;
ID3D11RenderTargetView*			m_pRenderTargetView 	= nullptr;
ID3D11InputLayout*				m_pInput				= nullptr;
ID3D11Resource*					m_pBackBuffer			= nullptr;

ID3D11Buffer*					m_pVertexBuffer			= nullptr;
ID3D11Buffer*					m_pIndexBuffer			= nullptr;
ID3D11Buffer*					m_pConstantBuffer		= nullptr;
ID3D11Texture2D*				m_pTexture2D			= nullptr;

ID3D11DepthStencilView*			m_pDepthStencil			= nullptr;
ID3D11SamplerState*				m_pSamplerState			= nullptr;

ID3D11ShaderResourceView*		m_pTextureRV			= nullptr;

ID3D11VertexShader* 			m_pVertexShader			= nullptr;
ID3D11PixelShader*				m_pPixelShader			= nullptr;

//ID3D11Debug*					debug					= nullptr;

D3D_DRIVER_TYPE					m_DriverType;
D3D_FEATURE_LEVEL				m_FeatureLevel;
D3D11_VIEWPORT					m_ViewPort;

XMMATRIX						WorldMatrix;
XMMATRIX						ViewMatrix;
XMMATRIX						ProjectionMatrix;

//View Martix Vectors
XMVECTOR Eye					= XMVectorSet(0.0f, 1.5f, -5.0f, 0.0f);
XMVECTOR Focus					= XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
XMVECTOR Up						= XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
XMVECTOR ResetEye = Eye;

// Need for Loading Pryamid
Object							Pryamid;
XMMATRIX						PryamidMatrix;
ID3D11Buffer*					PryamidVertexBuffer		= nullptr;
ID3D11Buffer*					PryamidIndexBuffer		= nullptr;
ID3D11Buffer*					PryamidConstantBuffer	= nullptr;
ID3D11ShaderResourceView*		PryamidTexture			= nullptr;

#pragma endregion





//----------------------------------------------------------------------------------------------------------
// Forward declarations of functions included in this code module:
//----------------------------------------------------------------------------------------------------------
#pragma region
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
HRESULT Initialize();
void Shutdown();
bool Run();
void DrawCube();
void SetCube();
void VertexIndexConstBuffers(const char * filename, Object & model, ID3D11Buffer *& vertbuffer, ID3D11Buffer *& indexbuffer, ID3D11Buffer *& world);
void DrawObject(Object & model, ID3D11Buffer * vertexbuffer, ID3D11Buffer * indexbuffer, ID3D11Buffer * worldbuffer, ID3D11ShaderResourceView * texture);

// Microsoft::WRL::ComPtr<var> name;
#pragma endregion


#pragma region
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
#pragma endregion

//----------------------------------------------------------------------------------------------------------
// Creating/Initializing Stuff Needed by DirectX
//----------------------------------------------------------------------------------------------------------
HRESULT Initialize() {

	// Getting The Current Width and Height of the Window
	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT width = rc.right;
	UINT height = rc.bottom;

	// Describing the SwapChain
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

	D3D_FEATURE_LEVEL FeatureLevels[4] = { D3D_FEATURE_LEVEL_10_0,D3D_FEATURE_LEVEL_10_1,D3D_FEATURE_LEVEL_11_0,D3D_FEATURE_LEVEL_11_1 };

	// Creating Device, Swap Chain and Device Conext
	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG, FeatureLevels,
		4, D3D11_SDK_VERSION, &swapdesc, &m_pSwapChain, &m_pDevice, &m_FeatureLevel, &m_pDeviceContext);

	// Initializing the SwapChain
	m_pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&m_pBackBuffer));
	m_pDevice->CreateRenderTargetView(m_pBackBuffer, NULL, &m_pRenderTargetView);
	
	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC texturedesc;
	ZeroMemory(&texturedesc, sizeof(texturedesc));
	texturedesc.Width = width;
	texturedesc.Height = height;
	texturedesc.MipLevels = 1;
	texturedesc.ArraySize = 1;
	texturedesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texturedesc.SampleDesc.Count = 1;
	texturedesc.SampleDesc.Quality = 0;
	texturedesc.Usage = D3D11_USAGE_DEFAULT;
	texturedesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texturedesc.CPUAccessFlags = 0;
	texturedesc.MiscFlags = 0;
	m_pDevice->CreateTexture2D(&texturedesc, NULL, &m_pTexture2D);

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = texturedesc.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	m_pDevice->CreateDepthStencilView(m_pTexture2D, &descDSV, &m_pDepthStencil);

	// Initializing the Viewport
	m_ViewPort.Width = static_cast<float>(width);
	m_ViewPort.Height = static_cast<float>(height);
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;

	#pragma region CUBE RELATED STUFF
	// Creating Cube Vertex
	SIMPLE_VERTEX Vertex[] = {
		#pragma region CubeVerts
		#if TextureCube
		{ XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT2(1.0f, 0.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT2(0.0f, 0.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT2(0.0f, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT2(1.0f, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },

		{ XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT2(0.0f, 0.0f),	XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT2(1.0f, 0.0f),	XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT2(1.0f, 1.0f),	XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT2(0.0f, 1.0f),	XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f) },

		{ XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f),	XMFLOAT2(0.0f, 1.0f),	XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f),	XMFLOAT2(1.0f, 1.0f),	XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f),	XMFLOAT2(1.0f, 0.0f),	XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f),	XMFLOAT2(0.0f, 0.0f),	XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f) },

		{ XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT2(1.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT2(0.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT2(0.0f, 0.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),		XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT2(1.0f, 0.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) },

		{ XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),	XMFLOAT2(0.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },
		{ XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),	XMFLOAT2(1.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },
		{ XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),	XMFLOAT2(1.0f, 0.0f),	XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },
		{ XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),	XMFLOAT2(0.0f, 0.0f),	XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },

		{ XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT2(1.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f) },
		{ XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT2(0.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f) },
		{ XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT2(0.0f, 0.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f) },
		{ XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT2(1.0f, 0.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f) },
		#endif // 0

		#if ColorCube
		{ XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f),		XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f) },
		{ XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f),		XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),			XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f),		XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f),		XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f) },
		{ XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f),		XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f),		XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f),		XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
		#endif // 0
		#pragma endregion
	};

	// Initializing Buffer
	D3D11_BUFFER_DESC		buffdesc;
	ZeroMemory(&buffdesc, sizeof(D3D11_BUFFER_DESC));
	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffdesc.ByteWidth = sizeof(SIMPLE_VERTEX) * 24;

	// Initializing SubSource
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = Vertex;

	// Creating Vertex Buffer
	m_pDevice->CreateBuffer(&buffdesc, &data, &m_pVertexBuffer);

	// Creating Index
	DWORD32 Indexes[] = {
		#pragma region CubeIndexs
		#if TextureCube
		3,1,0,
		2,1,3,

		6,4,5,
		7,4,6,

		11,9,8,
		10,9,11,

		14,12,13,
		15,12,14,

		19,17,16,
		18,17,19,

		22,20,21,
		23,20,22
		#endif // 0

		#if ColorCube
		3,1,0,
		2,1,3,

		0,5,4,
		1,5,0,

		3,4,7,
		0,4,3,

		1,6,5,
		2,6,1,

		2,7,6,
		3,7,2,

		6,4,5,
		7,4,6,
		#endif // 0
		#pragma endregion
	};

	// Creating Index buffer
	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffdesc.ByteWidth = sizeof(DWORD32) * 36;
	data.pSysMem = Indexes;
	m_pDevice->CreateBuffer(&buffdesc, &data, &m_pIndexBuffer);

	// Load the Texture
	CreateDDSTextureFromFile(m_pDevice, L"files/Box_Circuit.dds", NULL, &m_pTextureRV);
	CreateDDSTextureFromFile(m_pDevice, L"files/Box_Circuit.dds", NULL, &PryamidTexture);

	// Creating the sample state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	m_pDevice->CreateSamplerState(&sampDesc, &m_pSamplerState);
	#pragma endregion


	// Decleraing Shaders
	m_pDevice->CreateVertexShader(Trivial_VS, sizeof(Trivial_VS), NULL, &m_pVertexShader);
	m_pDevice->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), NULL, &m_pPixelShader);

	// Defining the Input Layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	// Number of Elements in the Layout
	UINT numberOfElements = ARRAYSIZE(layout);

	// Creating the Input Layout
	m_pDevice->CreateInputLayout(layout, numberOfElements, Trivial_VS, sizeof(Trivial_VS), &m_pInput);

	// Initializing/Creating Constant Buffer
	buffdesc.Usage						= D3D11_USAGE_DEFAULT;
	buffdesc.BindFlags					= D3D11_BIND_CONSTANT_BUFFER;
	buffdesc.ByteWidth					= sizeof(ConstantMatrix);
	m_pDevice->CreateBuffer(&buffdesc, nullptr, &m_pConstantBuffer);

	// Initializing the world matrix
	WorldMatrix = XMMatrixIdentity();
	PryamidMatrix = XMMatrixIdentity();
	PryamidMatrix = XMMatrixTranslation(3, 0, 0);

	// Initializing the view matrix
	ViewMatrix = XMMatrixLookAtLH(Eye, Focus, Up);

	// Initializing the projection matrix
	ProjectionMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV2, width / static_cast<float>(height), 0.01f, 100.0f);

	VertexIndexConstBuffers("testpyramid.obj", Pryamid, PryamidVertexBuffer, PryamidIndexBuffer, PryamidConstantBuffer);

	return S_OK;
}


//----------------------------------------------------------------------------------------------------------
// Constant Frame Rendering
//----------------------------------------------------------------------------------------------------------
bool Run() {

	// Time Per Frame
	static float t = 0.0f;
	static ULONGLONG timeStart = 0;
	ULONGLONG timeCur = GetTickCount64();
	if (timeStart == 0)
		timeStart = timeCur;
	t = (timeCur - timeStart) / 1000.0f;

	// ViewMatrix/ViewPort Movement/Rotation
	#pragma region Camera Movement
	if (GetAsyncKeyState('R')) {
		ViewMatrix = XMMatrixLookAtLH(ResetEye, Focus, Up);
	}
	// ViewPort/Camera Zoom In
	if (GetAsyncKeyState('Q')) {
		XMMATRIX lo = XMMatrixTranslation(0, 0, -0.001f);
		ViewMatrix = XMMatrixMultiply(ViewMatrix, lo);
	}
	// ViewPort/Camera Zoom Out
	if (GetAsyncKeyState('E')) {
		XMMATRIX lo = XMMatrixTranslation(0, 0, 0.001f);
		ViewMatrix = XMMatrixMultiply(ViewMatrix, lo);
	}
	// ViewPort/Camera movement Up
	if (GetAsyncKeyState('W')) {
		XMMATRIX lo = XMMatrixTranslation(0, -0.001f, 0);
		ViewMatrix = XMMatrixMultiply(ViewMatrix, lo);
	}
	// ViewPort/Camera movement Down
	if (GetAsyncKeyState('S')) {
		XMMATRIX lo = XMMatrixTranslation(0, 0.001f, 0);
		ViewMatrix = XMMatrixMultiply(ViewMatrix, lo);
	}
	// ViewPort/Camera movement Right
	if (GetAsyncKeyState('D')) {
		XMMATRIX lo = XMMatrixTranslation(-0.001f, 0, 0);
		ViewMatrix = XMMatrixMultiply(ViewMatrix, lo);
	}
	// ViewPort/Camera movement Left
	if (GetAsyncKeyState('A')) {
		XMMATRIX lo = XMMatrixTranslation(0.001f, 0, 0);
		ViewMatrix = XMMatrixMultiply(ViewMatrix, lo);
	}
	// ViewPort/Camera rotate Up
	if (GetAsyncKeyState('I')) {
		XMMATRIX lo = XMMatrixRotationX(0.0001);
		ViewMatrix = XMMatrixMultiply(ViewMatrix, lo);
	}
	// ViewPort/Camera rotate Down
	if (GetAsyncKeyState('K')) {
		XMMATRIX lo = XMMatrixRotationX(-0.0001);
		ViewMatrix = XMMatrixMultiply(ViewMatrix, lo);
	}
	// ViewPort/Camera rotate Left
	if (GetAsyncKeyState('J')) {
		XMMATRIX lo = XMMatrixRotationY(0.0001);
		ViewMatrix = XMMatrixMultiply(ViewMatrix, lo);
	}
	// ViewPort/Camera rotate Right
	if (GetAsyncKeyState('L')) {
		XMMATRIX lo = XMMatrixRotationY(-0.0001);
		ViewMatrix = XMMatrixMultiply(ViewMatrix, lo);
	}
	#pragma endregion


	// Setting Target View
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencil);
	
	// Setting Viewport
	m_pDeviceContext->RSSetViewports(1, &m_ViewPort);
	
	// Rotating Cube
	WorldMatrix = XMMatrixRotationY(t);

	// Clearing Back Buffer
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, Colors::DarkCyan);

	// Clearing Depth Buffer
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencil, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Update variables
	ConstantMatrix constantM;
	constantM.World						= XMMatrixTranspose(WorldMatrix);
	constantM.View						= XMMatrixTranspose(ViewMatrix);
	constantM.Projection				= XMMatrixTranspose(ProjectionMatrix);
	m_pDeviceContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &constantM, 0, 0);

	ConstantMatrix constantPrymid;
	constantPrymid.World = XMMatrixTranspose(PryamidMatrix);
	constantPrymid.View = XMMatrixTranspose(ViewMatrix);
	constantPrymid.Projection = XMMatrixTranspose(ProjectionMatrix);
	m_pDeviceContext->UpdateSubresource(PryamidConstantBuffer, 0, NULL, &constantPrymid, 0, 0);



	DrawCube();
	DrawObject(Pryamid, PryamidVertexBuffer, PryamidIndexBuffer, PryamidConstantBuffer, PryamidTexture);

	/* Presenting our back buffer to our front buffer */
	m_pSwapChain->Present(0, 0);

	return true;
}


//----------------------------------------------------------------------------------------------------------
// Clean/Release the Onjects we have Created in Memory
//----------------------------------------------------------------------------------------------------------
void Shutdown() {

	if (m_pSwapChain) { m_pSwapChain->Release(); }
	if (m_pDevice) { m_pDevice->Release(); }
	if (m_pRenderTargetView) { m_pRenderTargetView->Release(); }
	if (m_pDeviceContext) { m_pDeviceContext->Release(); }
	if (m_pConstantBuffer) { m_pConstantBuffer->Release(); }
	if (m_pBackBuffer) { m_pBackBuffer->Release(); }
	if (m_pInput) { m_pInput->Release(); }
	if (m_pVertexBuffer) { m_pVertexBuffer->Release(); }
	if (m_pIndexBuffer) { m_pIndexBuffer->Release(); }
	if (m_pTexture2D) { m_pTexture2D->Release(); }
	if (m_pDepthStencil) { m_pDepthStencil->Release(); }
	if (m_pSamplerState) { m_pSamplerState->Release(); }
	if (m_pTextureRV) { m_pTextureRV->Release(); }

	if(PryamidVertexBuffer) { PryamidVertexBuffer->Release(); }
	if(PryamidIndexBuffer) { PryamidIndexBuffer->Release(); }
	if(PryamidConstantBuffer) { PryamidConstantBuffer->Release(); }

	if (m_pVertexShader) { m_pVertexShader->Release(); }
	if (m_pPixelShader) { m_pPixelShader->Release(); }
}

void SetCube() {


}

void DrawCube() {
	/* Renders the Triangles for the Cube */
	unsigned int	strides = sizeof(SIMPLE_VERTEX);
	unsigned int	offsets = 0;
	// Setting VertexBuffer
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &strides, &offsets);
	// Setting Input Layout
	m_pDeviceContext->IASetInputLayout(m_pInput);
	// Setting Index Buffer
	m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	// Setting Topology
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	/* Setting Vertex Shader */
	m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	// Setting Constant Buffer
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	/* Setting Pixel Shader */
	m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	// Setting Texture Resource
	m_pDeviceContext->PSSetShaderResources(0, 1, &m_pTextureRV);
	// Setting Sampler State
	m_pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerState);
	// Setting Drawing Indexed Cube
	m_pDeviceContext->DrawIndexed(36, 0, 0);

}

void DrawFloor()
{
	//m_pDeviceContext->PSSetShaderResources(0, 1, &floortexture);

	//m_pDeviceContext->VSSetShader(vertexshader, nullptr, NULL);
	//m_pDeviceContext->PSSetShader(pixelshader, nullptr, NULL);
	//const unsigned int vertSize = sizeof(Vertex);
	//const unsigned int nxtvertbuffinMem = 0;
	//m_pDeviceContext->IASetVertexBuffers(0, 1, &vertexbuffer, &vertSize, &nxtvertbuffinMem);

	//m_pDeviceContext->IASetIndexBuffer(floorindexbuffer, DXGI_FORMAT_R32_UINT, NULL);

	//m_pDeviceContext->IASetInputLayout(InputLayout);
	//// Tells Input Asembler how to draw lines
	//m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//// Sets World Const Buffer
	//m_pDeviceContext->VSSetConstantBuffers(0, 1, &floorworldbuffer);
	//// Sets Scene Const Buffer
	//m_pDeviceContext->VSSetConstantBuffers(1, 1, &m_pConstantBuffer);

	//m_pDeviceContext->PSSetConstantBuffers(0, 1, &lightbuffer);
	//floorcount = ARRAYSIZE(Floor_indicies);



	//m_pDeviceContext->DrawIndexed(floorcount, 0, 0);
}

void VertexIndexConstBuffers(const char * filename, Object & model, ID3D11Buffer *& vertbuffer, ID3D11Buffer *& indexbuffer, ID3D11Buffer *& constantBuffer)
{
	Object mesh;
	mesh.Load(filename);
	model = mesh;

	SIMPLE_VERTEX * vertices = new SIMPLE_VERTEX[model.GetModel().size()];
	unsigned int objIcount = model.GetIndex().size();
	unsigned int * indicies = new unsigned int[objIcount];

	for (unsigned int i = 0; i < model.GetModel().size(); i++)
		vertices[i] = model.GetModel()[i];

	for (unsigned int i = 0; i < model.GetIndex().size(); i++)
		indicies[i] = model.GetIndex()[i];

	// Describes Object Vertex Buffer //
	D3D11_BUFFER_DESC objVertbuffdesc;
	ZeroMemory(&objVertbuffdesc, sizeof(D3D11_BUFFER_DESC));
	objVertbuffdesc.Usage = D3D11_USAGE_DEFAULT;
	objVertbuffdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	objVertbuffdesc.CPUAccessFlags = NULL;
	objVertbuffdesc.ByteWidth = sizeof(SIMPLE_VERTEX) * objIcount;

	D3D11_SUBRESOURCE_DATA objsubResource;
	ZeroMemory(&objsubResource, sizeof(D3D11_SUBRESOURCE_DATA));
	objsubResource.pSysMem = vertices;

	// Creating Cube Vertex Buffer // 
	m_pDevice->CreateBuffer(&objVertbuffdesc, &objsubResource, &vertbuffer);

	D3D11_BUFFER_DESC objIndexbuffdesc;
	ZeroMemory(&objIndexbuffdesc, sizeof(D3D11_BUFFER_DESC));
	objIndexbuffdesc.Usage = D3D11_USAGE_DEFAULT;
	objIndexbuffdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	objIndexbuffdesc.CPUAccessFlags = NULL;
	objIndexbuffdesc.ByteWidth = sizeof(unsigned int) * objIcount;

	objsubResource.pSysMem = indicies;

	m_pDevice->CreateBuffer(&objIndexbuffdesc, &objsubResource, &indexbuffer);

	D3D11_BUFFER_DESC constobjScenebuffdesc;
	ZeroMemory(&constobjScenebuffdesc, sizeof(D3D11_BUFFER_DESC));
	constobjScenebuffdesc.Usage = D3D11_USAGE_DEFAULT;
	constobjScenebuffdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constobjScenebuffdesc.ByteWidth = sizeof(ConstantMatrix);

	m_pDevice->CreateBuffer(&constobjScenebuffdesc, NULL, &constantBuffer);

	delete[] vertices;
	delete[] indicies;
}

void DrawObject(Object & model, ID3D11Buffer * vertexbuffer, ID3D11Buffer * indexbuffer, ID3D11Buffer * constantBuffer, ID3D11ShaderResourceView * texture)
{

	unsigned int	strides = sizeof(SIMPLE_VERTEX);
	unsigned int	offsets = 0;
	// Setting VertexBuffer
	m_pDeviceContext->IASetVertexBuffers(0, 1, &vertexbuffer, &strides, &offsets);
	// Setting Input Layout
	m_pDeviceContext->IASetInputLayout(m_pInput);
	// Setting Index Buffer
	m_pDeviceContext->IASetIndexBuffer(indexbuffer, DXGI_FORMAT_R32_UINT, NULL);
	// Setting Topology
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	/* Setting Vertex Shader */
	m_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, NULL);
	// Setting Constant Buffer
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &constantBuffer);
	/* Setting Pixel Shader */
	m_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, NULL);
	// Setting Texture Resource
	m_pDeviceContext->PSSetShaderResources(0, 1, &texture);
	// Setting Drawing Indexed Cube
	m_pDeviceContext->DrawIndexed(model.GetIndex().size(), 0, 0);
}