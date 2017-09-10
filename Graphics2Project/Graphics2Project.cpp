// Graphics2Project.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Graphics2Project.h"
#include "ObjLoader.h"

#define MAX_LOADSTRING 100
#define BACKBUFFER_WIDTH	729
#define BACKBUFFER_HEIGHT	640
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
ID3D11VertexShader* 			m_pGeometryVertexShader = nullptr;
ID3D11VertexShader* 			m_pSkyBoxVertexShader	= nullptr;
ID3D11PixelShader*				m_pSkyBoxPixelShader	= nullptr;
ID3D11PixelShader*				m_pPixelShader			= nullptr;
ID3D11GeometryShader*			m_pGeometryShader		= nullptr;

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

XMMATRIX						CubeMatrix;


// Need for Loading Pryamid
ObjLoader						Pryamid;
XMMATRIX						PryamidMatrix;
ID3D11Buffer*					PryamidVertexBuffer		= nullptr;
ID3D11Buffer*					PryamidIndexBuffer		= nullptr;
ID3D11Buffer*					PryamidConstantBuffer	= nullptr;
ID3D11ShaderResourceView*		PryamidTexture			= nullptr;

// Need for Loading Floor
XMMATRIX						FloorMatrix;
ID3D11Buffer*					FloorVertexBuffer		= nullptr;
ID3D11Buffer*					FloorIndexBuffer		= nullptr;
ID3D11Buffer*					FloorConstantBuffer		= nullptr;
ID3D11ShaderResourceView*		FloorTexture2D			= nullptr;

// Need For Geometry
XMMATRIX						GeometryMatrix;
ID3D11Buffer*					GeometryVertexBuffer	= nullptr;
ID3D11Buffer*					GeometryIndexBuffer		= nullptr;
ID3D11Buffer*					GeometryConstantBuffer	= nullptr;
ID3D11ShaderResourceView*		GeometryTexture2D		= nullptr;

// Need For Lighting
Lighting Lights[3];
XMMATRIX						LightMatrix;
ID3D11Buffer*					LightConstantBuffer		= nullptr;

// Need For Skybox
ID3D11Buffer*					SkyBoxVertexBuffer		= nullptr;
ID3D11Buffer*					SkyBoxIndexBuffer		= nullptr;
ID3D11Buffer*					SkyBoxConstantBuffer	= nullptr;
ID3D11ShaderResourceView*		SkyBoxTexture2D			= nullptr;
ID3D11InputLayout*				SkyBoxInputLayout		= nullptr;

int DirectionCount = 0;
int SpotCount = 0;
float Zoom = 2;
float NearPlane = 0.01;
float FarPlane = 200;
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
void SetCube();
void SetFloorAndGeometry();
void SetSkyBox();
void DrawCube();
void DrawFloor();
void DrawGeometry();
void DrawSkyBox();
void VertexIndexConstBuffers(const char * filename, ObjLoader & model, ID3D11Buffer *& vertbuffer, ID3D11Buffer *& indexbuffer, ID3D11Buffer *& world);
void DrawObject(ObjLoader & model, ID3D11Buffer * vertexbuffer, ID3D11Buffer * indexbuffer, ID3D11Buffer * worldbuffer, ID3D11ShaderResourceView * texture);

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
	swapdesc.BufferCount		= 1;
	swapdesc.BufferDesc.Format	= DXGI_FORMAT_R8G8B8A8_UNORM;
	swapdesc.BufferUsage		= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapdesc.Flags				= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapdesc.OutputWindow		= hWnd;
	swapdesc.SampleDesc.Count	= 1;
	swapdesc.SampleDesc.Quality	= NULL;
	swapdesc.Windowed			= true;

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
	texturedesc.Width				= width;
	texturedesc.Height				= height;
	texturedesc.MipLevels			= 1;
	texturedesc.ArraySize			= 1;
	texturedesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
	texturedesc.SampleDesc.Count	= 1;
	texturedesc.SampleDesc.Quality	= NULL;
	texturedesc.Usage				= D3D11_USAGE_DEFAULT;
	texturedesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	texturedesc.CPUAccessFlags		= NULL;
	texturedesc.MiscFlags			= NULL;
	m_pDevice->CreateTexture2D(&texturedesc, NULL, &m_pTexture2D);

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format				= texturedesc.Format;
	descDSV.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice	= NULL;
	m_pDevice->CreateDepthStencilView(m_pTexture2D, &descDSV, &m_pDepthStencil);

	// Initializing the Viewport
	m_ViewPort.Width	= static_cast<float>(width);
	m_ViewPort.Height	= static_cast<float>(height);
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;

	// Setting Indexed Geometry
	SetCube();
	SetFloorAndGeometry();
	SetSkyBox();

	// Setting Onject
	VertexIndexConstBuffers("Files/Crystal.obj", Pryamid, PryamidVertexBuffer, PryamidIndexBuffer, PryamidConstantBuffer);
	
	// Loading Indexed Geometry Textures
	CreateDDSTextureFromFile(m_pDevice, L"files/Box_Circuit.dds", NULL, &m_pTextureRV);
	CreateDDSTextureFromFile(m_pDevice, L"files/bownCartoonGround_seamless.dds", NULL, &FloorTexture2D);
	CreateDDSTextureFromFile(m_pDevice, L"files/greendragon.dds", NULL, &GeometryTexture2D);

	// Loading Object Textures 
	CreateDDSTextureFromFile(m_pDevice, L"files/icium.dds", NULL, &PryamidTexture);

	// Loading SkyBox Texture
	CreateDDSTextureFromFile(m_pDevice, L"files/SKYBOX.dds", NULL, &SkyBoxTexture2D);

	/* Setting Lighting */
	ZeroMemory(&Lights, sizeof(Lighting) * 3);

	// Spot Light
	Lights[0].Direction = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
	Lights[0].Color		= XMFLOAT4(0.0f, 0.0f, 255.0f, 1.0f);
	Lights[0].Position  = XMFLOAT4(0.0f, 7.0f, 0.0f, 0.0f);
	Lights[0].Radius	= XMFLOAT4(0.923f, 0.707f, 10.0f, 0.0f);

	// Point Light
	Lights[1].Position	= XMFLOAT4(6.0f, 1.0f, 0.0f, 0.0f);
	Lights[1].Color		= XMFLOAT4(255.0f, 0.0f, 255.0f, 1.0f);
	Lights[1].Radius	= XMFLOAT4(4.0f, 0.0f, 0.0f, 1.0f);

	// Directional Lighting
	Lights[2].Direction = XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f);
	Lights[2].Color		= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// Setting up the Light Buffer
	D3D11_BUFFER_DESC lightbuffdesc;
	ZeroMemory(&lightbuffdesc, sizeof(D3D11_BUFFER_DESC));
	lightbuffdesc.Usage		= D3D11_USAGE_DEFAULT;
	lightbuffdesc.BindFlags	= D3D11_BIND_CONSTANT_BUFFER;
	lightbuffdesc.ByteWidth	= sizeof(Lighting) * 3;
	m_pDevice->CreateBuffer(&lightbuffdesc, nullptr, &LightConstantBuffer);



	// Creating the sample state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU		= D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV		= D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW		= D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc	= D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD			= 0;
	sampDesc.MaxLOD			= D3D11_FLOAT32_MAX;
	m_pDevice->CreateSamplerState(&sampDesc, &m_pSamplerState);


	// Decleraing Shaders
	m_pDevice->CreateVertexShader(Trivial_VS, sizeof(Trivial_VS), NULL, &m_pVertexShader);
	m_pDevice->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), NULL, &m_pPixelShader);
	m_pDevice->CreateGeometryShader(Trivial_GS, sizeof(Trivial_GS), NULL, &m_pGeometryShader);
	m_pDevice->CreateVertexShader(GS_VS, sizeof(GS_VS), NULL, &m_pGeometryVertexShader);
	m_pDevice->CreatePixelShader(SkyBox_PS, sizeof(SkyBox_PS), NULL, &m_pSkyBoxPixelShader);
	m_pDevice->CreateVertexShader(SkyBox_VS, sizeof(SkyBox_VS), NULL, &m_pSkyBoxVertexShader);



	// Defining the Input Layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", NULL, DXGI_FORMAT_R32G32B32A32_FLOAT, NULL, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, NULL },
		{ "COLOR", NULL, DXGI_FORMAT_R32G32B32A32_FLOAT, NULL, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, NULL },
		{ "UV", NULL, DXGI_FORMAT_R32G32_FLOAT, NULL, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, NULL },
		{ "NORMAL", NULL, DXGI_FORMAT_R32G32B32A32_FLOAT, NULL, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, NULL },
	};
	// Number of Elements in the Layout
	UINT numberOfElements = ARRAYSIZE(layout);

	// Creating the Input Layout
	m_pDevice->CreateInputLayout(layout, numberOfElements, Trivial_VS, sizeof(Trivial_VS), &m_pInput);
	
	D3D11_INPUT_ELEMENT_DESC vskyLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	// Number of Elements in the Layout
	UINT numberOfElements2 = ARRAYSIZE(vskyLayout);

	m_pDevice->CreateInputLayout(vskyLayout, numberOfElements2, SkyBox_VS, sizeof(SkyBox_VS), &SkyBoxInputLayout);

	// Initializing the world matrix
	WorldMatrix		= XMMatrixIdentity();

	CubeMatrix		= XMMatrixIdentity();
	
	FloorMatrix		= XMMatrixIdentity();
	FloorMatrix		= XMMatrixTranslation(0, -0.5f, 0);

	PryamidMatrix	= XMMatrixIdentity();
	PryamidMatrix	= XMMatrixTranslation(5, -0.5f, 15.0f);

	GeometryMatrix = XMMatrixIdentity();

	// Initializing the view matrix
	ViewMatrix = XMMatrixLookAtLH(Eye, Focus, Up);

	// Initializing the projection matrix
	ProjectionMatrix = XMMatrixPerspectiveFovLH(XM_PI/Zoom, BACKBUFFER_WIDTH / static_cast<float>(BACKBUFFER_HEIGHT), NearPlane, FarPlane);


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

	#pragma region Window Clipping
	// Near Plane
	if (GetAsyncKeyState(VK_NUMPAD1) & 0x1) { NearPlane += 0.01f; }
	if (GetAsyncKeyState(VK_NUMPAD2) & 0x1) { if (FarPlane > 0.02) { NearPlane -= 0.01f; } }
	// Fare Plane
	if (GetAsyncKeyState(VK_NUMPAD4) & 0x1) { if (FarPlane > 1) { FarPlane -= 1.0f; } }
	if (GetAsyncKeyState(VK_NUMPAD5) & 0x1) { FarPlane += 1.0f; }
	// Zoom
	if (GetAsyncKeyState(VK_NUMPAD7) & 0x1) { Zoom += 0.05;}
	if (GetAsyncKeyState(VK_NUMPAD8) & 0x1) { if (Zoom > 1.1) { Zoom -= 0.05; } }

	// Initializing the projection matrix
	ProjectionMatrix = XMMatrixPerspectiveFovLH(XM_PI/Zoom, BACKBUFFER_WIDTH / static_cast<float>(BACKBUFFER_HEIGHT), NearPlane, FarPlane);
	#pragma endregion


	#pragma region Light Movement
	if (GetAsyncKeyState('1') & 0x1) {
		if (DirectionCount == 0) {
			Lights[2].Direction = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
			DirectionCount++;
		}
		else if (DirectionCount == 1) {
			Lights[2].Direction = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
			DirectionCount++;
		}
		else if (DirectionCount == 2) {
			Lights[2].Direction = XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f);
			DirectionCount++;
		}
		else if (DirectionCount == 3) {
			Lights[2].Direction = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
			DirectionCount++;
		}
		else if (DirectionCount == 4) {
			Lights[2].Direction = XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f);
			DirectionCount = 0;
		}
	}
	if (GetAsyncKeyState('2') & 0x1) {
		if (SpotCount == 0) {
			Lights[0].Direction = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
			SpotCount++;
		}
		else if (SpotCount == 1) {
			Lights[0].Direction = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
			SpotCount++;
		}
		else if (SpotCount == 2) {
			Lights[0].Direction = XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f);
			SpotCount++;
		}
		else if (SpotCount == 3) {
			Lights[0].Direction = XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f);
			SpotCount++;
		}
		else if (SpotCount == 4) {
			Lights[0].Direction = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
			SpotCount++;
		}
		else if (SpotCount == 5) {
			Lights[0].Direction = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
			SpotCount = 0;
		}
	}

	if (GetAsyncKeyState('G')) { Lights[0].Position.y -= 0.005; }
	if (GetAsyncKeyState('T')) { Lights[0].Position.y += 0.005; }
	if (GetAsyncKeyState('3')) { Lights[0].Position.x -= 0.005; }
	if (GetAsyncKeyState('4')) { Lights[0].Position.x += 0.005; }
	if (GetAsyncKeyState('5')) { Lights[0].Position.z -= 0.005; }
	if (GetAsyncKeyState('6')) { Lights[0].Position.z += 0.005; }


	if (GetAsyncKeyState('H')) { Lights[1].Position.y -= 0.005; }
	if (GetAsyncKeyState('Y')) { Lights[1].Position.y += 0.005; }
	if (GetAsyncKeyState('7')) { Lights[1].Position.x -= 0.005; }
	if (GetAsyncKeyState('8')) { Lights[1].Position.x += 0.005; }
	if (GetAsyncKeyState('9')) { Lights[1].Position.z -= 0.005; }
	if (GetAsyncKeyState('0')) { Lights[1].Position.z += 0.005; }
	#pragma endregion

	// ViewMatrix/ViewPort Movement/Rotation
	#pragma region Camera Movement
	if (GetAsyncKeyState('R')) { ViewMatrix = XMMatrixLookAtLH(ResetEye, Focus, Up); }
	#if 1
	// ViewPort/Camera Zoom In
	if (GetAsyncKeyState('Q')) { ViewMatrix = XMMatrixMultiply(ViewMatrix, XMMatrixTranslation(0, 0, -0.01f)); }
	// ViewPort/Camera Zoom Out
	if (GetAsyncKeyState('E')) { ViewMatrix = XMMatrixMultiply(ViewMatrix, XMMatrixTranslation(0, 0, 0.01f)); }
	#endif
	// ViewPort/Camera movement Up
	if (GetAsyncKeyState('W')) { ViewMatrix = XMMatrixMultiply(ViewMatrix, XMMatrixTranslation(0, -0.01f, 0)); }
	// ViewPort/Camera movement Down
	if (GetAsyncKeyState('S')) { ViewMatrix = XMMatrixMultiply(ViewMatrix, XMMatrixTranslation(0, 0.01f, 0)); }
	// ViewPort/Camera movement Right
	if (GetAsyncKeyState('D')) { ViewMatrix = XMMatrixMultiply(ViewMatrix, XMMatrixTranslation(-0.01f, 0, 0)); }
	// ViewPort/Camera movement Left
	if (GetAsyncKeyState('A')) { ViewMatrix = XMMatrixMultiply(ViewMatrix, XMMatrixTranslation(0.01f, 0, 0)); }
	// ViewPort/Camera rotate Up
	if (GetAsyncKeyState('I')) { ViewMatrix = XMMatrixMultiply(ViewMatrix, XMMatrixRotationX(0.001)); }
	// ViewPort/Camera rotate Down
	if (GetAsyncKeyState('K')) { ViewMatrix = XMMatrixMultiply(ViewMatrix, XMMatrixRotationX(-0.001)); }
	// ViewPort/Camera rotate Left
	if (GetAsyncKeyState('J')) { ViewMatrix = XMMatrixMultiply(ViewMatrix, XMMatrixRotationY(0.001)); }
	// ViewPort/Camera rotate Right
	if (GetAsyncKeyState('L')) { ViewMatrix = XMMatrixMultiply(ViewMatrix, XMMatrixRotationY(-0.001)); }
	#pragma endregion

	// Setting Target View
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencil);
	
	// Setting Viewport
	m_pDeviceContext->RSSetViewports(1, &m_ViewPort);
	
	// Setting Sampler State
	m_pDeviceContext->PSSetSamplers(NULL, 1, &m_pSamplerState);

	// Setting Light Buffer
	m_pDeviceContext->PSSetConstantBuffers(NULL, 1, &LightConstantBuffer);

	// Rotating Cube
	CubeMatrix = XMMatrixRotationY(t);

	// Clearing Back Buffer
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, Colors::DarkCyan);

	// Clearing Depth Buffer
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencil, D3D11_CLEAR_DEPTH, 1.0f, NULL);

	// Update variables
	ConstantMatrix constantM;
	constantM.ObjectMatrix				= XMMatrixTranspose(CubeMatrix);
	constantM.View						= XMMatrixTranspose(ViewMatrix);
	constantM.Projection				= XMMatrixTranspose(ProjectionMatrix);
	m_pDeviceContext->UpdateSubresource(m_pConstantBuffer, NULL, NULL, &constantM, NULL, NULL);

	ConstantMatrix constantPrymid;
	constantPrymid.ObjectMatrix			= XMMatrixTranspose(PryamidMatrix);
	constantPrymid.View					= XMMatrixTranspose(ViewMatrix);
	constantPrymid.Projection			= XMMatrixTranspose(ProjectionMatrix);
	m_pDeviceContext->UpdateSubresource(PryamidConstantBuffer, NULL, NULL, &constantPrymid, NULL, NULL);

	ConstantMatrix constantFloor;
	constantFloor.ObjectMatrix			= XMMatrixTranspose(FloorMatrix);
	constantFloor.View					= XMMatrixTranspose(ViewMatrix);
	constantFloor.Projection			= XMMatrixTranspose(ProjectionMatrix);
	m_pDeviceContext->UpdateSubresource(FloorConstantBuffer, NULL, NULL, &constantFloor, NULL, NULL);

	ConstantMatrix constantGeometry;
	constantGeometry.ObjectMatrix = XMMatrixTranspose(GeometryMatrix);
	constantGeometry.View = XMMatrixTranspose(ViewMatrix);
	constantGeometry.Projection = XMMatrixTranspose(ProjectionMatrix);
	m_pDeviceContext->UpdateSubresource(GeometryConstantBuffer, NULL, NULL, &constantGeometry, NULL, NULL);

	ConstantMatrix constantSkyBox;
	constantSkyBox.ObjectMatrix = XMMatrixTranspose(GeometryMatrix);
	constantSkyBox.View = XMMatrixTranspose(ViewMatrix);
	constantSkyBox.Projection = XMMatrixTranspose(ProjectionMatrix);
	m_pDeviceContext->UpdateSubresource(SkyBoxConstantBuffer, NULL, NULL, &constantSkyBox, NULL, NULL);

	Lighting constantLight[3];
	// Spot Light
	constantLight[0].Color		= Lights[0].Color;
	constantLight[0].Direction  = Lights[0].Direction;
	constantLight[0].Position	= Lights[0].Position;
	constantLight[0].Radius		= Lights[0].Radius;
	// Point Light
	constantLight[1].Color		= Lights[1].Color;
	constantLight[1].Position	= Lights[1].Position;
	constantLight[1].Radius		= Lights[1].Radius;
	// Directional Light
	constantLight[2].Color		= Lights[2].Color;
	constantLight[2].Direction	= Lights[2].Direction;

	// Updating the Light Buffer
	m_pDeviceContext->UpdateSubresource(LightConstantBuffer, NULL, NULL, &constantLight, NULL, NULL);


	// Drawing Objects
	DrawSkyBox();
	DrawFloor();
	DrawGeometry();
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

	if (PryamidVertexBuffer) { PryamidVertexBuffer->Release(); }
	if (PryamidIndexBuffer) { PryamidIndexBuffer->Release(); }
	if (PryamidConstantBuffer) { PryamidConstantBuffer->Release(); }
	if (PryamidTexture) { PryamidTexture->Release(); }

	if (FloorVertexBuffer) { FloorVertexBuffer->Release(); }
	if (FloorIndexBuffer) { FloorIndexBuffer->Release(); }
	if (FloorConstantBuffer) { FloorConstantBuffer->Release(); }
	if (FloorTexture2D) { FloorTexture2D->Release(); }

	if (LightConstantBuffer) { LightConstantBuffer->Release(); }
	
	if (GeometryVertexBuffer) { GeometryVertexBuffer->Release(); }
	if (GeometryIndexBuffer) { GeometryIndexBuffer->Release(); }
	if (GeometryConstantBuffer) { GeometryConstantBuffer->Release(); }
	if (GeometryTexture2D) { GeometryTexture2D->Release(); }

	if (m_pSkyBoxVertexShader) { m_pSkyBoxVertexShader->Release(); }
	if (m_pSkyBoxPixelShader) { m_pSkyBoxPixelShader->Release(); }
	if (SkyBoxVertexBuffer) { SkyBoxVertexBuffer->Release(); }
	if (SkyBoxIndexBuffer) { SkyBoxIndexBuffer->Release(); }
	if (SkyBoxConstantBuffer) { SkyBoxConstantBuffer->Release(); }
	if (SkyBoxTexture2D) { SkyBoxTexture2D->Release(); }
	if (SkyBoxInputLayout) { SkyBoxInputLayout->Release(); }

	if (m_pVertexShader) { m_pVertexShader->Release(); }
	if (m_pPixelShader) { m_pPixelShader->Release(); }
	if (m_pGeometryVertexShader) { m_pGeometryVertexShader->Release(); }
	if (m_pGeometryShader) { m_pGeometryShader->Release(); }

}


//----------------------------------------------------------------------------------------------------------
// Functions: Creating/Initializing Stuff Needed by DirectX
//----------------------------------------------------------------------------------------------------------
void SetCube() {
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
////////////////
		{ XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),	XMFLOAT2(0.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },
		{ XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),	XMFLOAT2(1.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },
		{ XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),	XMFLOAT2(1.0f, 0.0f),	XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },
		{ XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),	XMFLOAT2(0.0f, 0.0f),	XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },
////////////////
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

	// Initializing/Creating Constant Buffer
	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffdesc.ByteWidth = sizeof(ConstantMatrix);
	m_pDevice->CreateBuffer(&buffdesc, nullptr, &m_pConstantBuffer);
}

void SetFloorAndGeometry()
{
	// Creating Floor Vertex
	SIMPLE_VERTEX Vertex[] = {
		{ XMFLOAT4(-30.0f, -0.5f, -30.0f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT2(0.0f, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(30.0f, -0.5f, -30.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT2(1.0f, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(30.0f, -0.5f,  30.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT2(1.0f, 0.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(-30.0f, -0.5f,  30.0f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT2(0.0f, 0.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
	};

	// Initializing Vertex Buffer
	D3D11_BUFFER_DESC		buffdesc;
	ZeroMemory(&buffdesc, sizeof(D3D11_BUFFER_DESC));
	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffdesc.ByteWidth = sizeof(SIMPLE_VERTEX) * 4;

	// Initializing SubSource
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = Vertex;

	// Creating Vertex Buffer
	m_pDevice->CreateBuffer(&buffdesc, &data, &FloorVertexBuffer);
	m_pDevice->CreateBuffer(&buffdesc, &data, &GeometryVertexBuffer);

	// Creating Floor Index
	DWORD32 Indexes[] = {
		3,1,0,
		2,1,3,
	};

	// Initializing/Creating Index Buffer
	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffdesc.ByteWidth = sizeof(DWORD32) * 6;
	data.pSysMem = Indexes;
	m_pDevice->CreateBuffer(&buffdesc, &data, &FloorIndexBuffer);
	m_pDevice->CreateBuffer(&buffdesc, &data, &GeometryIndexBuffer);

	// Initializing/Creating Constant Buffer
	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffdesc.ByteWidth = sizeof(ConstantMatrix);
	m_pDevice->CreateBuffer(&buffdesc, nullptr, &FloorConstantBuffer);
	m_pDevice->CreateBuffer(&buffdesc, nullptr, &GeometryConstantBuffer);

}

void SetSkyBox()
{
	SkyboxVertex Vertex[] = {
		#pragma region SkyVerts
		{ XMFLOAT4(-50.0f, 50.0f, -50.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)	 },
		{ XMFLOAT4(50.0f, 50.0f, -50.0f, 1.0f),		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)	 },
		{ XMFLOAT4(50.0f, 50.0f, 50.0f, 1.0f),		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)	 },
		{ XMFLOAT4(-50.0f, 50.0f, 50.0f, 1.0f),		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)	 },

		{ XMFLOAT4(-50.0f, -50.0f, -50.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)	 }, 
		{ XMFLOAT4(50.0f, -50.0f, -50.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)	 }, 
		{ XMFLOAT4(50.0f, -50.0f, 50.0f, 1.0f),		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)	 },
		{ XMFLOAT4(-50.0f, -50.0f, 50.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)	 },

		{ XMFLOAT4(-50.0f, -50.0f, 50.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)	 }, 
		{ XMFLOAT4(-50.0f, -50.0f, -50.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)	 }, 
		{ XMFLOAT4(-50.0f, 50.0f, -50.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)	 }, 
		{ XMFLOAT4(-50.0f, 50.0f, 50.0f, 1.0f),		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)	 },

		{ XMFLOAT4(50.0f, -50.0f, 50.0f, 1.0f),		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)	 },
		{ XMFLOAT4(50.0f, -50.0f, -50.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)	 },
		{ XMFLOAT4(50.0f, 50.0f, -50.0f, 1.0f),		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)	 },
		{ XMFLOAT4(50.0f, 50.0f, 50.0f, 1.0f),		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)	 },

		{ XMFLOAT4(-50.0f, -50.0f, -50.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)	 },
		{ XMFLOAT4(50.0f, -50.0f, -50.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)	 }, 
		{ XMFLOAT4(50.0f, 50.0f, -50.0f, 1.0f),		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)	 },
		{ XMFLOAT4(-50.0f, 50.0f, -50.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)	 },

		{ XMFLOAT4(-50.0f, -50.0f, 50.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)	 },
		{ XMFLOAT4(50.0f, -50.0f, 50.0f, 1.0f),		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)	 },
		{ XMFLOAT4(50.0f, 50.0f, 50.0f, 1.0f),		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)	 },
		{ XMFLOAT4(-50.0f, 50.0f, 50.0f, 1.0f),		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)	 },
		#pragma endregion

	};
	//SkyboxVertex
	// Initializing Buffer
	D3D11_BUFFER_DESC		buffdesc;
	ZeroMemory(&buffdesc, sizeof(D3D11_BUFFER_DESC));
	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffdesc.ByteWidth = sizeof(SkyboxVertex) * 24;

	// Initializing SubSource
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = Vertex;

	// Creating Vertex Buffer
	m_pDevice->CreateBuffer(&buffdesc, &data, &SkyBoxVertexBuffer);

	// Creating Index
	DWORD32 Indexes[] = {
		#pragma region SkyIndexs
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
		#pragma endregion
	};

	// Creating Index buffer
	buffdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffdesc.ByteWidth = sizeof(DWORD32) * 36;
	data.pSysMem = Indexes;
	m_pDevice->CreateBuffer(&buffdesc, &data, &SkyBoxIndexBuffer);

	// Initializing/Creating Constant Buffer
	buffdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffdesc.ByteWidth = sizeof(ConstantMatrix);
	m_pDevice->CreateBuffer(&buffdesc, nullptr, &SkyBoxConstantBuffer);
}

void VertexIndexConstBuffers(const char * fileName, ObjLoader & model, ID3D11Buffer *& vertBuffer, ID3D11Buffer *& indexBuffer, ID3D11Buffer *& constantBuffer)
{
	ObjLoader mesh;
	mesh.Load(fileName);
	model = mesh;

	SIMPLE_VERTEX* Vertex = new SIMPLE_VERTEX[model.GetModel().size()];
	unsigned int objIcount = model.GetIndex().size();
	unsigned int * Indexes = new unsigned int[objIcount];

	// Creating Object Vertex
	for (unsigned int i = 0; i < model.GetModel().size(); i++) {
		Vertex[i] = model.GetModel()[i];
	}
	// Creating Object Index
	for (unsigned int i = 0; i < model.GetIndex().size(); i++) {
		Indexes[i] = model.GetIndex()[i];
	}

	// Initializing Vertex Buffer
	D3D11_BUFFER_DESC objVertbuffdesc;
	ZeroMemory(&objVertbuffdesc, sizeof(D3D11_BUFFER_DESC));
	objVertbuffdesc.Usage = D3D11_USAGE_DEFAULT;
	objVertbuffdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	objVertbuffdesc.ByteWidth = sizeof(SIMPLE_VERTEX) * objIcount;

	// Initializing SubSource
	D3D11_SUBRESOURCE_DATA objsubResource;
	ZeroMemory(&objsubResource, sizeof(D3D11_SUBRESOURCE_DATA));
	objsubResource.pSysMem = Vertex;

	// Creating Vertex Buffer
	m_pDevice->CreateBuffer(&objVertbuffdesc, &objsubResource, &vertBuffer);

	// Initializing/Creating Index Buffer
	objVertbuffdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	objVertbuffdesc.ByteWidth = sizeof(DWORD32) * objIcount;
	objsubResource.pSysMem = Indexes;
	m_pDevice->CreateBuffer(&objVertbuffdesc, &objsubResource, &indexBuffer);

	// Initializing/Creating Constant Buffer
	objVertbuffdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	objVertbuffdesc.ByteWidth = sizeof(ConstantMatrix);
	m_pDevice->CreateBuffer(&objVertbuffdesc, NULL, &constantBuffer);

	delete[] Vertex;
	delete[] Indexes;
}


//----------------------------------------------------------------------------------------------------------
// Functions: Rendering Frame by frame
//----------------------------------------------------------------------------------------------------------
void DrawCube() {
	/* Renders the Triangles for the Cube */
	unsigned int	strides = sizeof(SIMPLE_VERTEX);
	unsigned int	offsets = 0;
	// Setting VertexBuffer
	m_pDeviceContext->IASetVertexBuffers(NULL, 1, &m_pVertexBuffer, &strides, &offsets);
	// Setting Input Layout
	m_pDeviceContext->IASetInputLayout(m_pInput);
	// Setting Index Buffer
	m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, NULL);
	// Setting Topology
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	/* Setting Vertex Shader */
	m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, NULL);
	// Setting Constant Buffer
	m_pDeviceContext->VSSetConstantBuffers(NULL, 1, &m_pConstantBuffer);

	/* Setting Pixel Shader */
	m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, NULL);
	// Setting Texture Resource
	m_pDeviceContext->PSSetShaderResources(NULL, 1, &m_pTextureRV);

	// Drawing Indexed Cube
	m_pDeviceContext->DrawIndexed(36, 0, 0);
	//m_pDeviceContext->DrawIndexedInstanced(36, 3, 0, 0, 0);
}

void DrawFloor()
{
	/* Renders the Triangles for the Floor */
	unsigned int	strides = sizeof(SIMPLE_VERTEX);
	unsigned int	offsets = 0;
	// Setting VertexBuffer
	m_pDeviceContext->IASetVertexBuffers(NULL, 1, &FloorVertexBuffer, &strides, &offsets);
	// Setting Input Layout
	m_pDeviceContext->IASetInputLayout(m_pInput);
	// Setting Index Buffer
	m_pDeviceContext->IASetIndexBuffer(FloorIndexBuffer, DXGI_FORMAT_R32_UINT, NULL);
	// Setting Topology
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	/* Setting Vertex Shader */
	m_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, NULL);
	// Setting Constant Buffer
	m_pDeviceContext->VSSetConstantBuffers(NULL, 1, &FloorConstantBuffer);

	/* Setting Pixel Shader */
	m_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, NULL);
	// Setting Texture Resource
	m_pDeviceContext->PSSetShaderResources(NULL, 1, &FloorTexture2D);

	// Drawing Indexed Quad
	m_pDeviceContext->DrawIndexed(6, 0, 0);
}

void DrawGeometry() {
	/* Renders the Geometry for the Floor */
	unsigned int	strides = sizeof(SIMPLE_VERTEX);
	unsigned int	offsets = 0;
	// Setting VertexBuffer
	m_pDeviceContext->IASetVertexBuffers(0, 1, &GeometryVertexBuffer, &strides, &offsets);
	// Setting Input Layout
	m_pDeviceContext->IASetInputLayout(m_pInput);
	// Setting Index Buffer
	m_pDeviceContext->IASetIndexBuffer(GeometryIndexBuffer, DXGI_FORMAT_R32_UINT, NULL);
	// Setting Topology
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	/* Setting Vertex Shader */
	m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, NULL);
	// Setting Vertex Constant Buffer
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &GeometryConstantBuffer);

	/* Setting Pixel Shader */
	m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, NULL);
	// Setting Texture Resource
	m_pDeviceContext->PSSetShaderResources(0, 1, &GeometryTexture2D);
	
	/* Setting Geometry Shader */
	m_pDeviceContext->GSSetShader(m_pGeometryShader, NULL, NULL);
	// Setting Geometry Constant Buffer
	m_pDeviceContext->GSSetConstantBuffers(0, 1, &GeometryConstantBuffer);
	
	// Drawing Indexed Quad
	m_pDeviceContext->Draw(1, 0);

	/* Nulling the Geometry Shader */
	m_pDeviceContext->GSSetShader(NULL, NULL, NULL);
}

void DrawSkyBox() {
	/* Renders the Triangles for the Cube */
	unsigned int	strides = sizeof(SkyboxVertex);
	unsigned int	offsets = 0;
	// Setting VertexBuffer
	m_pDeviceContext->IASetVertexBuffers(NULL, 1, &SkyBoxVertexBuffer, &strides, &offsets);
	// Setting Input Layout
	m_pDeviceContext->IASetInputLayout(SkyBoxInputLayout);
	// Setting Index Buffer
	m_pDeviceContext->IASetIndexBuffer(SkyBoxIndexBuffer, DXGI_FORMAT_R32_UINT, NULL);
	// Setting Topology
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	/* Setting Vertex Shader */
	m_pDeviceContext->VSSetShader(m_pSkyBoxVertexShader, NULL, NULL);
	// Setting Constant Buffer
	m_pDeviceContext->VSSetConstantBuffers(NULL, 1, &SkyBoxConstantBuffer);

	/* Setting Pixel Shader */
	m_pDeviceContext->PSSetShader(m_pSkyBoxPixelShader, NULL, NULL);
	// Setting Texture Resource
	m_pDeviceContext->PSSetShaderResources(NULL, 1, &SkyBoxTexture2D);
	
	// Drawing Indexed Cube
	m_pDeviceContext->DrawIndexed(36, 0, 0);
}

void DrawObject(ObjLoader & model, ID3D11Buffer * vertexBuffer, ID3D11Buffer * indexBuffer, ID3D11Buffer * constantBuffer, ID3D11ShaderResourceView * texture)
{
	unsigned int	strides = sizeof(SIMPLE_VERTEX);
	unsigned int	offsets = 0;
	// Setting VertexBuffer
	m_pDeviceContext->IASetVertexBuffers(NULL, 1, &vertexBuffer, &strides, &offsets);
	// Setting Input Layout
	m_pDeviceContext->IASetInputLayout(m_pInput);
	// Setting Index Buffer
	m_pDeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, NULL);
	// Setting Topology
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	/* Setting Vertex Shader */
	m_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, NULL);
	// Setting Constant Buffer
	m_pDeviceContext->VSSetConstantBuffers(NULL, 1, &constantBuffer);
	/* Setting Pixel Shader */
	m_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, NULL);
	// Setting Texture Resource
	m_pDeviceContext->PSSetShaderResources(NULL, 1, &texture);

	// Drawing Indexed Model
	m_pDeviceContext->DrawIndexed(model.GetIndex().size(), 0, 0);
}