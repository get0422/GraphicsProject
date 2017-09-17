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
//ID3D11Debug*					debug					= nullptr;
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

// For Texturing
ID3D11DepthStencilView*			m_pDepthStencil			= nullptr;
ID3D11Texture2D*				m_pTexture2D			= nullptr;
ID3D11SamplerState*				m_pSamplerState			= nullptr;

// Need for Loading Cube
XMMATRIX						CubeMatrix;
ID3D11Buffer*					m_pVertexBuffer			= nullptr;
ID3D11Buffer*					m_pIndexBuffer			= nullptr;
ID3D11Buffer*					m_pConstantBuffer		= nullptr;
ID3D11ShaderResourceView*		m_pTexture				= nullptr;

// Needed For Loading All the Different Shaders
ID3D11VertexShader* 			m_pVertexShader			= nullptr;
ID3D11VertexShader* 			m_pGeometryVertexShader = nullptr;
ID3D11VertexShader* 			m_pSkyBoxVertexShader	= nullptr;
ID3D11PixelShader*				m_pSkyBoxPixelShader	= nullptr;
ID3D11PixelShader*				m_pPixelShader			= nullptr;
ID3D11GeometryShader*			m_pGeometryShader		= nullptr;

// Need for Loading Pryamid(Any Model)
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
ID3D11ShaderResourceView*		FloorTexture			= nullptr;

// Need for Loading Floor
ID3D11Buffer*					StincelVertexBuffer		= nullptr;
ID3D11Buffer*					StincelIndexBuffer		= nullptr;
ID3D11Buffer*					StincelConstantBuffer	= nullptr;
ID3D11ShaderResourceView*		StincelTexture			= nullptr;

// Need For Geometry
XMMATRIX						GeometryMatrix;
ID3D11Buffer*					GeometryVertexBuffer	= nullptr;
ID3D11Buffer*					GeometryIndexBuffer		= nullptr;
ID3D11Buffer*					GeometryConstantBuffer	= nullptr;
ID3D11ShaderResourceView*		GeometryTexture			= nullptr;

// Need For Lighting
Lighting Lights[3];
XMMATRIX						LightMatrix;
ID3D11Buffer*					LightConstantBuffer		= nullptr;

// Need For Skybox 1
XMMATRIX						SkyBoxMatrix;
ID3D11Buffer*					SkyBoxVertexBuffer		= nullptr;
ID3D11Buffer*					SkyBoxIndexBuffer		= nullptr;
ID3D11Buffer*					SkyBoxConstantBuffer	= nullptr;
ID3D11ShaderResourceView*		SkyBoxTexture			= nullptr;

//-Scene-2--------------------------------------------------------------------------------
ID3D11RenderTargetView*			RenderTemp				= nullptr;
IDXGISwapChain*					SwapTemp				= nullptr;
ID3D11Device*					DeviceTemp				= nullptr;
ID3D11DeviceContext*			DeviceContextTemp		= nullptr;
ID3D11Resource*					BackBufferTemp			= nullptr;
ID3D11InputLayout*				InputTemp				= nullptr;
ID3D11VertexShader* 			VertexShaderTemp		= nullptr;
ID3D11PixelShader*				PixelShaderTemp			= nullptr;
ID3D11VertexShader* 			SkyBoxVertexShaderTemp	= nullptr;
ID3D11PixelShader*				SkyBoxPixelShaderTemp	= nullptr;
ID3D11DepthStencilView*			DepthStencilTemp		= nullptr;
ID3D11SamplerState*				SamplerStateTemp		= nullptr;
ID3D11Texture2D*				Texture2DTemp			= nullptr;

// Need For Skybox 2
XMMATRIX						SkyBoxMatrix2;
ID3D11Buffer*					SkyBoxVertexBuffer2		= nullptr;
ID3D11Buffer*					SkyBoxIndexBuffer2		= nullptr;
ID3D11Buffer*					SkyBoxConstantBuffer2	= nullptr;
ID3D11ShaderResourceView*		SkyBoxTexture2			= nullptr;

// Need For Wall
XMMATRIX						WallMatrix;
ID3D11Buffer*					WallVertexBuffer1		= nullptr;
ID3D11Buffer*					WallIndexBuffer1		= nullptr;
ID3D11Buffer*					WallConstantBuffer1		= nullptr;
ID3D11ShaderResourceView*		WallTexture1			= nullptr;
ID3D11Buffer*					WallVertexBuffer2		= nullptr;
ID3D11Buffer*					WallIndexBuffer2		= nullptr;
ID3D11Buffer*					WallConstantBuffer2		= nullptr;
ID3D11ShaderResourceView*		WallTexture2			= nullptr;
ID3D11Buffer*					WallVertexBuffer3		= nullptr;
ID3D11Buffer*					WallIndexBuffer3		= nullptr;
ID3D11Buffer*					WallConstantBuffer3		= nullptr;
ID3D11ShaderResourceView*		WallTexture3			= nullptr;
ID3D11Buffer*					WallVertexBuffer4		= nullptr;
ID3D11Buffer*					WallIndexBuffer4		= nullptr;
ID3D11Buffer*					WallConstantBuffer4		= nullptr;
ID3D11ShaderResourceView*		WallTexture4			= nullptr;
//----------------------------------------------------------------------------------------


// Instancing
ID3D11Buffer*					InstanceBuffer			= nullptr;

// Needed For SwapChain
D3D_FEATURE_LEVEL				m_FeatureLevel;

// Needed to Make the Screen
D3D11_VIEWPORT					m_ViewPort[4];

// Needed For Everything
XMMATRIX	WorldMatrix;
XMMATRIX	ViewMatrix;
XMMATRIX	ViewMatrixSub;
XMMATRIX	ViewMatrix2;
XMMATRIX	ViewMatrix2Sub;
XMMATRIX	ViewMatrix3;
XMMATRIX	ViewMatrix3Sub;
XMMATRIX	ViewMatrix4;
XMMATRIX	ViewMatrix4Sub;
XMMATRIX	ProjectionMatrix;
XMMATRIX	ProjectionMatrix2;
XMMATRIX	ProjectionMatrix3;
XMMATRIX	ProjectionMatrix4;

// View Martix Vectors
XMVECTOR Eye		= XMVectorSet(0.0f, 1.5f, -5.0f, 0.0f);
XMVECTOR Focus		= XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
XMVECTOR Up			= XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
XMVECTOR ResetEye	= Eye;

// Moving Light
int DirectionCount	= 0;
int SpotCount		= 0;

// Moving Camera (Zoom/Near/Far)
float Zoom		= 2;
float NearPlane	= 0.01;
float FarPlane	= 300;

// Change Size of Cube
int cubeverts = 0;

// Swap Scenes 
int SwapSceneInt = 0;

// Swap Cameras 
int SwapCameraInt = 0;

// For Testing
XMMATRIX						CameraView;
XMMATRIX						CameraProjection;
ID3D11Texture2D*				CameraTexture2D			= nullptr;
ID3D11RenderTargetView*			CameraRender			= nullptr;
ID3D11ShaderResourceView*		CameraResource			= nullptr;

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
void DrawGSGeometry();
void DrawInstancedCube();
void SetFloorAndGeometry();
void DrawIndexedGeometry(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, ID3D11Buffer* constantBuffer, ID3D11InputLayout* input, ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader, UINT count);
void SetSkyBox(ID3D11Device* &device, const wchar_t* fileName, ID3D11ShaderResourceView* &texture, ID3D11Buffer* &vertexBuffer, ID3D11Buffer* &indexBuffer, ID3D11Buffer* &constantBuffer);
void SetModel(const char * filename, ObjLoader & model, ID3D11Buffer* &vertBuffer, ID3D11Buffer* &indexBuffer, ID3D11Buffer* &constantBuffer, ID3D11Device* &device);
void DrawModel(ObjLoader &model, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, ID3D11Buffer* constantBuffer, ID3D11ShaderResourceView* texture, ID3D11InputLayout* input, ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader);
void UpdateConstant(XMMATRIX &geometryMatrix, XMMATRIX &viewMatrix, XMMATRIX &projectionMatrix, ID3D11Buffer* &constantBuffer, ID3D11DeviceContext* &deviceContext);

void CameraMovement(XMMATRIX &viewMatrix, XMMATRIX &viewMatrixSub, XMMATRIX &projectionMatrix);
void LightMovment();
void SceneManagment();

void SetWall();
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
	texturedesc.Usage				= D3D11_USAGE_DEFAULT;
	texturedesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	m_pDevice->CreateTexture2D(&texturedesc, NULL, &m_pTexture2D);

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format				= texturedesc.Format;
	descDSV.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice	= NULL;
	m_pDevice->CreateDepthStencilView(m_pTexture2D, &descDSV, &m_pDepthStencil);


	// Initializing the Viewport
	m_ViewPort[0].Width	= static_cast<float>(width);
	m_ViewPort[0].Height	= static_cast<float>(height);
	m_ViewPort[0].MinDepth = 0.0f;
	m_ViewPort[0].MaxDepth = 1.0f;
	m_ViewPort[0].TopLeftX = 0;
	m_ViewPort[0].TopLeftY = 0;


////// For Scene 2 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG, nullptr,
		NULL, D3D11_SDK_VERSION, &swapdesc, &SwapTemp, &DeviceTemp, &m_FeatureLevel, &DeviceContextTemp);

	// TODO: PART 1 STEP 4
	SwapTemp->GetBuffer(NULL, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&BackBufferTemp));
	DeviceTemp->CreateRenderTargetView(BackBufferTemp, NULL, &RenderTemp);

	DeviceTemp->CreateTexture2D(&texturedesc, NULL, &Texture2DTemp);

	DeviceTemp->CreateDepthStencilView(Texture2DTemp, &descDSV, &DepthStencilTemp);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////CameraTexture
	D3D11_TEXTURE2D_DESC texturedesc2;
	ZeroMemory(&texturedesc2, sizeof(texturedesc2));
	texturedesc2.Width = width;
	texturedesc2.Height = height;
	texturedesc2.MipLevels = 1;
	texturedesc2.ArraySize = 1;
	texturedesc2.SampleDesc.Count = 1;
	texturedesc2.Usage = D3D11_USAGE_DEFAULT;
	texturedesc2.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texturedesc2.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	m_pDevice->CreateTexture2D(&texturedesc2, NULL, &CameraTexture2D);

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = texturedesc2.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	m_pDevice->CreateRenderTargetView(CameraTexture2D, &renderTargetViewDesc, &CameraRender);

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = texturedesc2.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	m_pDevice->CreateShaderResourceView(CameraTexture2D, &shaderResourceViewDesc, &CameraResource);

	SetWall();

	// Setting Indexed Geometry For Scene 1
	SetCube();
	SetFloorAndGeometry();
	SetSkyBox(m_pDevice, L"files/SkyboxOcean.dds", SkyBoxTexture, SkyBoxVertexBuffer, SkyBoxIndexBuffer, SkyBoxConstantBuffer);
	// Setting Onject
	SetModel("Files/Crystal.obj", Pryamid, PryamidVertexBuffer, PryamidIndexBuffer, PryamidConstantBuffer, m_pDevice);


	// Setting Indexed Geometry for Scene 2
	SetSkyBox(DeviceTemp, L"files/HW_Blue.dds", SkyBoxTexture2, SkyBoxVertexBuffer2, SkyBoxIndexBuffer2, SkyBoxConstantBuffer2);
	CreateDDSTextureFromFile(DeviceTemp, L"files/Box_Circuit.dds", NULL, &WallTexture1);
	CreateDDSTextureFromFile(DeviceTemp, L"files/Box_Ice.dds", NULL, &WallTexture2);
	CreateDDSTextureFromFile(DeviceTemp, L"files/Box_Purple2.dds", NULL, &WallTexture3);
	CreateDDSTextureFromFile(DeviceTemp, L"files/Box_Red2Dark.dds", NULL, &WallTexture4);



	
	// Loading Indexed Geometry Textures
	CreateDDSTextureFromFile(m_pDevice, L"files/Box_Circuit.dds", NULL, &m_pTexture);
	CreateDDSTextureFromFile(m_pDevice, L"files/bownCartoonGround_seamless.dds", NULL, &FloorTexture);
	CreateDDSTextureFromFile(m_pDevice, L"files/greendragon.dds", NULL, &GeometryTexture);

	// Loading Object Textures 
	CreateDDSTextureFromFile(m_pDevice, L"files/icium.dds", NULL, &PryamidTexture);


	/* Setting Lighting */
	ZeroMemory(&Lights, sizeof(Lighting) * 3);

	// Spot Light
	Lights[0].Direction = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
	Lights[0].Color		= XMFLOAT4(0.0f, 255.0f, 0.0f, 1.0f);
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
	DeviceTemp->CreateSamplerState(&sampDesc, &SamplerStateTemp);



	// Decleraing Shaders for Scene 1
	m_pDevice->CreateVertexShader(Trivial_VS, sizeof(Trivial_VS), NULL, &m_pVertexShader);
	m_pDevice->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), NULL, &m_pPixelShader);
	m_pDevice->CreateGeometryShader(Trivial_GS, sizeof(Trivial_GS), NULL, &m_pGeometryShader);
	m_pDevice->CreateVertexShader(GS_VS, sizeof(GS_VS), NULL, &m_pGeometryVertexShader);
	m_pDevice->CreatePixelShader(SkyBox_PS, sizeof(SkyBox_PS), NULL, &m_pSkyBoxPixelShader);
	m_pDevice->CreateVertexShader(SkyBox_VS, sizeof(SkyBox_VS), NULL, &m_pSkyBoxVertexShader);

	// Decleraing Shaders for Scene 2
	DeviceTemp->CreateVertexShader(Trivial_VS, sizeof(Trivial_VS), NULL, &VertexShaderTemp);
	DeviceTemp->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), NULL, &PixelShaderTemp);
	DeviceTemp->CreatePixelShader(SkyBox_PS, sizeof(SkyBox_PS), NULL, &SkyBoxPixelShaderTemp);
	DeviceTemp->CreateVertexShader(SkyBox_VS, sizeof(SkyBox_VS), NULL, &SkyBoxVertexShaderTemp);



	// Defining the Input Layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", NULL, DXGI_FORMAT_R32G32B32A32_FLOAT, NULL, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, NULL },
		{ "COLOR", NULL, DXGI_FORMAT_R32G32B32A32_FLOAT, NULL, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, NULL },
		{ "UV", NULL, DXGI_FORMAT_R32G32B32_FLOAT, NULL, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, NULL },
		{ "NORMAL", NULL, DXGI_FORMAT_R32G32B32A32_FLOAT, NULL, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, NULL },
	};
	// Number of Elements in the Layout
	UINT numberOfElements = ARRAYSIZE(layout);

	// Creating the Input Layout
	m_pDevice->CreateInputLayout(layout, numberOfElements, Trivial_VS, sizeof(Trivial_VS), &m_pInput);
	DeviceTemp->CreateInputLayout(layout, numberOfElements, Trivial_VS, sizeof(Trivial_VS), &InputTemp);

	// Initializing the world matrix
	WorldMatrix		= XMMatrixIdentity();

	CubeMatrix		= XMMatrixIdentity();
	
	FloorMatrix		= XMMatrixIdentity();
	FloorMatrix		= XMMatrixTranslation(0, -0.5f, 0);

	PryamidMatrix	= XMMatrixIdentity();
	PryamidMatrix	= XMMatrixTranslation(5, -0.5f, 15.0f);

	GeometryMatrix = XMMatrixIdentity();

	SkyBoxMatrix = XMMatrixIdentity();
	WallMatrix		= XMMatrixIdentity();

	CameraView = XMMatrixLookAtLH(Eye, Focus, Up);
	CameraProjection = XMMatrixPerspectiveFovLH(XM_PI / Zoom, BACKBUFFER_WIDTH / static_cast<float>(BACKBUFFER_HEIGHT), NearPlane, FarPlane);

	// Initializing the view matrix
	ViewMatrix = XMMatrixLookAtLH(Eye, Focus, Up);
	ViewMatrixSub = XMMatrixLookAtLH(Eye, Focus, Up);
	ViewMatrix2 = XMMatrixLookAtLH(XMVectorSet(0.0f, 1.5f, 5.0f, 0.0f), Focus, Up);
	ViewMatrix3 = XMMatrixLookAtLH(XMVectorSet(-5.0f, 1.5f, 0.0f, 0.0f), Focus, Up);
	ViewMatrix4 = XMMatrixLookAtLH(XMVectorSet(5.0f, 1.5f, 0.0f, 0.0f), Focus, Up);
	ViewMatrix2Sub = XMMatrixLookAtLH(XMVectorSet(0.0f, 1.5f, 5.0f, 0.0f), Focus, Up);
	ViewMatrix3Sub = XMMatrixLookAtLH(XMVectorSet(-5.0f, 1.5f, 0.0f, 0.0f), Focus, Up);
	ViewMatrix4Sub = XMMatrixLookAtLH(XMVectorSet(5.0f, 1.5f, 0.0f, 0.0f), Focus, Up);

	// Initializing the projection matrix
	ProjectionMatrix = XMMatrixPerspectiveFovLH(XM_PI/Zoom, BACKBUFFER_WIDTH / static_cast<float>(BACKBUFFER_HEIGHT), NearPlane, FarPlane);
	ProjectionMatrix2 = XMMatrixPerspectiveFovLH(XM_PI / Zoom, BACKBUFFER_WIDTH / static_cast<float>(BACKBUFFER_HEIGHT), NearPlane, FarPlane);
	ProjectionMatrix3 = XMMatrixPerspectiveFovLH(XM_PI / Zoom, BACKBUFFER_WIDTH / static_cast<float>(BACKBUFFER_HEIGHT), NearPlane, FarPlane);
	ProjectionMatrix4 = XMMatrixPerspectiveFovLH(XM_PI / Zoom, BACKBUFFER_WIDTH / static_cast<float>(BACKBUFFER_HEIGHT), NearPlane, FarPlane);

	return S_OK;
}


//----------------------------------------------------------------------------------------------------------
// Constant Frame Rendering
//----------------------------------------------------------------------------------------------------------
bool Run() {

	// Time Per Frame
	float t = 0.0f;
	static ULONGLONG timeStart = 0;
	ULONGLONG timeCur = GetTickCount64();
	if (timeStart == 0)
		timeStart = timeCur;
	t = (timeCur - timeStart) / 1000.0f;

	//Test Stuff

	// Swap Scenes
	SceneManagment();

	if (SwapSceneInt == 0) {
		#pragma region Scene1
		#pragma region CubeRelated
		// Inlarge Cube
		if (GetAsyncKeyState('C') & 0x1) { cubeverts++; SetCube(); }
		// Reset Cube
		if (GetAsyncKeyState('V') & 0x1) { cubeverts = 0; SetCube(); }
		#pragma endregion

		// ViewMatrix/ViewPort Movement/Rotation, Zoom and Adjustable Near/Far-Plane
		CameraMovement(ViewMatrix, ViewMatrixSub, ProjectionMatrix);

		// Light Direction/Position Movement
		LightMovment();

		// Setting Target View
		m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencil);

		// Setting Viewport
		m_pDeviceContext->RSSetViewports(1, &m_ViewPort[0]);
		// Clearing Back Buffer
		m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, Colors::DarkCyan);

		// Clearing Back Buffer
		m_pDeviceContext->ClearRenderTargetView(CameraRender, Colors::Blue);

		// Setting Sampler State
		m_pDeviceContext->PSSetSamplers(NULL, 1, &m_pSamplerState);

		// Setting Light Buffer
		m_pDeviceContext->PSSetConstantBuffers(NULL, 1, &LightConstantBuffer);

		// Rotating Cube
		//CubeMatrix = XMMatrixRotationY(t);
		if (cubeverts < 15) {
			CubeMatrix = XMMatrixMultiply(XMMatrixRotationY(t), XMMatrixTranslation(0, 0 + cubeverts, 0));
		}

		// Clearing Depth Buffer
		m_pDeviceContext->ClearDepthStencilView(m_pDepthStencil, D3D11_CLEAR_DEPTH, 1.0f, NULL);

		// Update variables
		UpdateConstant(CubeMatrix, ViewMatrix, ProjectionMatrix, m_pConstantBuffer, m_pDeviceContext);
		UpdateConstant(PryamidMatrix, ViewMatrix, ProjectionMatrix, PryamidConstantBuffer, m_pDeviceContext);
		UpdateConstant(FloorMatrix, ViewMatrix, ProjectionMatrix, FloorConstantBuffer, m_pDeviceContext);
		UpdateConstant(SkyBoxMatrix, ViewMatrix, ProjectionMatrix, SkyBoxConstantBuffer, m_pDeviceContext);
		
		Lighting constantLight[3];
		// Spot Light
		constantLight[0].Color = Lights[0].Color;
		constantLight[0].Direction = Lights[0].Direction;
		constantLight[0].Position = Lights[0].Position;
		constantLight[0].Radius = Lights[0].Radius;
		// Point Light
		constantLight[1].Color = Lights[1].Color;
		constantLight[1].Position = Lights[1].Position;
		constantLight[1].Radius = Lights[1].Radius;
		// Directional Light
		constantLight[2].Color = Lights[2].Color;
		constantLight[2].Direction = Lights[2].Direction;

		// Updating the Light Buffer
		m_pDeviceContext->UpdateSubresource(LightConstantBuffer, NULL, NULL, &constantLight, NULL, NULL);

		// Drawing Objects
		DrawIndexedGeometry(m_pDeviceContext, SkyBoxTexture, SkyBoxVertexBuffer, SkyBoxIndexBuffer, SkyBoxConstantBuffer, m_pInput, m_pSkyBoxVertexShader, m_pSkyBoxPixelShader, 36);
		DrawIndexedGeometry(m_pDeviceContext, FloorTexture, FloorVertexBuffer, FloorIndexBuffer, FloorConstantBuffer, m_pInput, m_pVertexShader, m_pPixelShader, 6);
		DrawIndexedGeometry(m_pDeviceContext, m_pTexture, m_pVertexBuffer, m_pIndexBuffer, m_pConstantBuffer, m_pInput, m_pVertexShader, m_pPixelShader, 36);
		DrawModel(Pryamid, PryamidVertexBuffer, PryamidIndexBuffer, PryamidConstantBuffer, PryamidTexture, m_pInput, m_pVertexShader, m_pPixelShader);


		m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);
		ID3D11ShaderResourceView* nip = nullptr;
		m_pDeviceContext->PSSetShaderResources(NULL, 1, &nip);
		m_pDeviceContext->OMSetRenderTargets(1, &CameraRender, NULL);

		UpdateConstant(GeometryMatrix, ViewMatrix, ProjectionMatrix, GeometryConstantBuffer, m_pDeviceContext);
		UpdateConstant(CubeMatrix, ViewMatrixSub, ProjectionMatrix, m_pConstantBuffer, m_pDeviceContext);
		UpdateConstant(PryamidMatrix, ViewMatrixSub, ProjectionMatrix, PryamidConstantBuffer, m_pDeviceContext);
		UpdateConstant(FloorMatrix, ViewMatrixSub, ProjectionMatrix, FloorConstantBuffer, m_pDeviceContext);
		UpdateConstant(SkyBoxMatrix, ViewMatrixSub, ProjectionMatrix, SkyBoxConstantBuffer, m_pDeviceContext);

		DrawIndexedGeometry(m_pDeviceContext, SkyBoxTexture, SkyBoxVertexBuffer, SkyBoxIndexBuffer, SkyBoxConstantBuffer, m_pInput, m_pSkyBoxVertexShader, m_pSkyBoxPixelShader, 36);
		DrawIndexedGeometry(m_pDeviceContext, FloorTexture, FloorVertexBuffer, FloorIndexBuffer, FloorConstantBuffer, m_pInput, m_pVertexShader, m_pPixelShader, 6);
		DrawIndexedGeometry(m_pDeviceContext, m_pTexture, m_pVertexBuffer, m_pIndexBuffer, m_pConstantBuffer, m_pInput, m_pVertexShader, m_pPixelShader, 36);


		// Setting Target View
		m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencil);
		
		DrawGSGeometry();

		/* Presenting our back buffer to our front buffer */
		m_pSwapChain->Present(0, 0);
		#pragma endregion
	}
	else if (SwapSceneInt == 1) {
		#pragma region Scene2
		#pragma region AllCamerasMovement
		if (GetAsyncKeyState('X') & 0x1) { SwapCameraInt++; }
		if (SwapCameraInt == 0) {
			// ViewMatrix/ViewPort Movement/Rotation, Zoom and Adjustable Near/Far-Plane
			CameraMovement(ViewMatrix, ViewMatrixSub, ProjectionMatrix);
		}
		else if (SwapCameraInt == 1) {
			// ViewMatrix/ViewPort Movement/Rotation, Zoom and Adjustable Near/Far-Plane
			CameraMovement(ViewMatrix2, ViewMatrix2Sub, ProjectionMatrix2);
		}
		else if (SwapCameraInt == 2) {
			// ViewMatrix/ViewPort Movement/Rotation, Zoom and Adjustable Near/Far-Plane
			CameraMovement(ViewMatrix3, ViewMatrix3Sub, ProjectionMatrix3);
		}
		else if (SwapCameraInt == 3) {
			// ViewMatrix/ViewPort Movement/Rotation, Zoom and Adjustable Near/Far-Plane
			CameraMovement(ViewMatrix4, ViewMatrix4Sub, ProjectionMatrix4);
		}
		else { SwapCameraInt = 0;}
		#pragma endregion

		// Setting Target View
		DeviceContextTemp->OMSetRenderTargets(1, &RenderTemp, DepthStencilTemp);

//------- Top Left --------------------------------------------------------------//
		#pragma region
		// Setting Viewport
		DeviceContextTemp->RSSetViewports(1, &m_ViewPort[0]);

		// Clearing Back Buffer
		DeviceContextTemp->ClearRenderTargetView(RenderTemp, Colors::DarkBlue);

		// Setting Sampler State
		DeviceContextTemp->PSSetSamplers(NULL, 1, &SamplerStateTemp);

		// Clearing Depth Buffer
		DeviceContextTemp->ClearDepthStencilView(DepthStencilTemp, D3D11_CLEAR_DEPTH, 1.0f, NULL);

		// Update variables
		UpdateConstant(SkyBoxMatrix, ViewMatrix, ProjectionMatrix, SkyBoxConstantBuffer2, DeviceContextTemp);
		UpdateConstant(WallMatrix, ViewMatrix, ProjectionMatrix, WallConstantBuffer1, DeviceContextTemp);
		UpdateConstant(WallMatrix, ViewMatrix, ProjectionMatrix, WallConstantBuffer2, DeviceContextTemp);
		UpdateConstant(WallMatrix, ViewMatrix, ProjectionMatrix, WallConstantBuffer3, DeviceContextTemp);
		UpdateConstant(WallMatrix, ViewMatrix, ProjectionMatrix, WallConstantBuffer4, DeviceContextTemp);

		// Drawing Objects
		DrawIndexedGeometry(DeviceContextTemp, SkyBoxTexture2, SkyBoxVertexBuffer2, SkyBoxIndexBuffer2, SkyBoxConstantBuffer2, InputTemp, SkyBoxVertexShaderTemp, SkyBoxPixelShaderTemp, 36);
		DrawIndexedGeometry(DeviceContextTemp, WallTexture1, WallVertexBuffer1, WallIndexBuffer1, WallConstantBuffer1, InputTemp, VertexShaderTemp, PixelShaderTemp, 6);
		DrawIndexedGeometry(DeviceContextTemp, WallTexture2, WallVertexBuffer2, WallIndexBuffer2, WallConstantBuffer2, InputTemp, VertexShaderTemp, PixelShaderTemp, 6);
		DrawIndexedGeometry(DeviceContextTemp, WallTexture3, WallVertexBuffer3, WallIndexBuffer3, WallConstantBuffer3, InputTemp, VertexShaderTemp, PixelShaderTemp, 6);
		DrawIndexedGeometry(DeviceContextTemp, WallTexture4, WallVertexBuffer4, WallIndexBuffer4, WallConstantBuffer4, InputTemp, VertexShaderTemp, PixelShaderTemp, 6);

#pragma endregion

//------ Top Right ---------------------------------------------------------------//
		#pragma region
		// Setting Viewport
		DeviceContextTemp->RSSetViewports(1, &m_ViewPort[1]);

		// Update variables
		UpdateConstant(SkyBoxMatrix, ViewMatrix2, ProjectionMatrix2, SkyBoxConstantBuffer2, DeviceContextTemp);
		UpdateConstant(WallMatrix, ViewMatrix2, ProjectionMatrix2, WallConstantBuffer1, DeviceContextTemp);
		UpdateConstant(WallMatrix, ViewMatrix2, ProjectionMatrix2, WallConstantBuffer2, DeviceContextTemp);
		UpdateConstant(WallMatrix, ViewMatrix2, ProjectionMatrix2, WallConstantBuffer3, DeviceContextTemp);
		UpdateConstant(WallMatrix, ViewMatrix2, ProjectionMatrix2, WallConstantBuffer4, DeviceContextTemp);

		// Drawing Objects
		DrawIndexedGeometry(DeviceContextTemp, SkyBoxTexture2, SkyBoxVertexBuffer2, SkyBoxIndexBuffer2, SkyBoxConstantBuffer2, InputTemp, SkyBoxVertexShaderTemp, SkyBoxPixelShaderTemp, 36);
		DrawIndexedGeometry(DeviceContextTemp, WallTexture1, WallVertexBuffer1, WallIndexBuffer1, WallConstantBuffer1, InputTemp, VertexShaderTemp, PixelShaderTemp, 6);
		DrawIndexedGeometry(DeviceContextTemp, WallTexture2, WallVertexBuffer2, WallIndexBuffer2, WallConstantBuffer2, InputTemp, VertexShaderTemp, PixelShaderTemp, 6);
		DrawIndexedGeometry(DeviceContextTemp, WallTexture3, WallVertexBuffer3, WallIndexBuffer3, WallConstantBuffer3, InputTemp, VertexShaderTemp, PixelShaderTemp, 6);
		DrawIndexedGeometry(DeviceContextTemp, WallTexture4, WallVertexBuffer4, WallIndexBuffer4, WallConstantBuffer4, InputTemp, VertexShaderTemp, PixelShaderTemp, 6);
#pragma endregion

//------ Bottom Left ---------------------------------------------------------------//
		#pragma region
		// Setting Viewport
		DeviceContextTemp->RSSetViewports(1, &m_ViewPort[2]);

		// Update variables
		UpdateConstant(SkyBoxMatrix, ViewMatrix3, ProjectionMatrix3, SkyBoxConstantBuffer2, DeviceContextTemp);
		UpdateConstant(WallMatrix, ViewMatrix3, ProjectionMatrix3, WallConstantBuffer1, DeviceContextTemp);
		UpdateConstant(WallMatrix, ViewMatrix3, ProjectionMatrix3, WallConstantBuffer2, DeviceContextTemp);
		UpdateConstant(WallMatrix, ViewMatrix3, ProjectionMatrix3, WallConstantBuffer3, DeviceContextTemp);
		UpdateConstant(WallMatrix, ViewMatrix3, ProjectionMatrix3, WallConstantBuffer4, DeviceContextTemp);

		// Drawing Objects
		DrawIndexedGeometry(DeviceContextTemp, SkyBoxTexture2, SkyBoxVertexBuffer2, SkyBoxIndexBuffer2, SkyBoxConstantBuffer2, InputTemp, SkyBoxVertexShaderTemp, SkyBoxPixelShaderTemp, 36);
		DrawIndexedGeometry(DeviceContextTemp, WallTexture1, WallVertexBuffer1, WallIndexBuffer1, WallConstantBuffer1, InputTemp, VertexShaderTemp, PixelShaderTemp, 6);
		DrawIndexedGeometry(DeviceContextTemp, WallTexture2, WallVertexBuffer2, WallIndexBuffer2, WallConstantBuffer2, InputTemp, VertexShaderTemp, PixelShaderTemp, 6);
		DrawIndexedGeometry(DeviceContextTemp, WallTexture3, WallVertexBuffer3, WallIndexBuffer3, WallConstantBuffer3, InputTemp, VertexShaderTemp, PixelShaderTemp, 6);
		DrawIndexedGeometry(DeviceContextTemp, WallTexture4, WallVertexBuffer4, WallIndexBuffer4, WallConstantBuffer4, InputTemp, VertexShaderTemp, PixelShaderTemp, 6);

#pragma endregion

//------ Bottom Right ---------------------------------------------------------------//
		#pragma region
		// Setting Viewport
		DeviceContextTemp->RSSetViewports(1, &m_ViewPort[3]);

		// Update variables
		UpdateConstant(SkyBoxMatrix, ViewMatrix4, ProjectionMatrix4, SkyBoxConstantBuffer2, DeviceContextTemp);
		UpdateConstant(WallMatrix, ViewMatrix4, ProjectionMatrix4, WallConstantBuffer1, DeviceContextTemp);
		UpdateConstant(WallMatrix, ViewMatrix4, ProjectionMatrix4, WallConstantBuffer2, DeviceContextTemp);
		UpdateConstant(WallMatrix, ViewMatrix4, ProjectionMatrix4, WallConstantBuffer3, DeviceContextTemp);
		UpdateConstant(WallMatrix, ViewMatrix4, ProjectionMatrix4, WallConstantBuffer4, DeviceContextTemp);

		// Drawing Objects
		DrawIndexedGeometry(DeviceContextTemp, SkyBoxTexture2, SkyBoxVertexBuffer2, SkyBoxIndexBuffer2, SkyBoxConstantBuffer2, InputTemp, SkyBoxVertexShaderTemp, SkyBoxPixelShaderTemp, 36);
		DrawIndexedGeometry(DeviceContextTemp, WallTexture1, WallVertexBuffer1, WallIndexBuffer1, WallConstantBuffer1, InputTemp, VertexShaderTemp, PixelShaderTemp, 6);
		DrawIndexedGeometry(DeviceContextTemp, WallTexture2, WallVertexBuffer2, WallIndexBuffer2, WallConstantBuffer2, InputTemp, VertexShaderTemp, PixelShaderTemp, 6);
		DrawIndexedGeometry(DeviceContextTemp, WallTexture3, WallVertexBuffer3, WallIndexBuffer3, WallConstantBuffer3, InputTemp, VertexShaderTemp, PixelShaderTemp, 6);
		DrawIndexedGeometry(DeviceContextTemp, WallTexture4, WallVertexBuffer4, WallIndexBuffer4, WallConstantBuffer4, InputTemp, VertexShaderTemp, PixelShaderTemp, 6);

#pragma endregion

		/* Presenting our back buffer to our front buffer */
		SwapTemp->Present(0, 0);
		#pragma endregion
	}
	
	return true;
}


//----------------------------------------------------------------------------------------------------------
// Clean/Release the Onjects we have Created in Memory
//----------------------------------------------------------------------------------------------------------
void Shutdown() {
	// Scene 1
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
	if (m_pTexture) { m_pTexture->Release(); }

	if (PryamidVertexBuffer) { PryamidVertexBuffer->Release(); }
	if (PryamidIndexBuffer) { PryamidIndexBuffer->Release(); }
	if (PryamidConstantBuffer) { PryamidConstantBuffer->Release(); }
	if (PryamidTexture) { PryamidTexture->Release(); }

	if (FloorVertexBuffer) { FloorVertexBuffer->Release(); }
	if (FloorIndexBuffer) { FloorIndexBuffer->Release(); }
	if (FloorConstantBuffer) { FloorConstantBuffer->Release(); }
	if (FloorTexture) { FloorTexture->Release(); }

	if (LightConstantBuffer) { LightConstantBuffer->Release(); }
	
	if (GeometryVertexBuffer) { GeometryVertexBuffer->Release(); }
	if (GeometryIndexBuffer) { GeometryIndexBuffer->Release(); }
	if (GeometryConstantBuffer) { GeometryConstantBuffer->Release(); }
	if (GeometryTexture) { GeometryTexture->Release(); }

	if (m_pSkyBoxVertexShader) { m_pSkyBoxVertexShader->Release(); }
	if (m_pSkyBoxPixelShader) { m_pSkyBoxPixelShader->Release(); }
	if (SkyBoxVertexBuffer) { SkyBoxVertexBuffer->Release(); }
	if (SkyBoxIndexBuffer) { SkyBoxIndexBuffer->Release(); }
	if (SkyBoxConstantBuffer) { SkyBoxConstantBuffer->Release(); }
	if (SkyBoxTexture) { SkyBoxTexture->Release(); }

	if (m_pVertexShader) { m_pVertexShader->Release(); }
	if (m_pPixelShader) { m_pPixelShader->Release(); }
	if (m_pGeometryVertexShader) { m_pGeometryVertexShader->Release(); }
	if (m_pGeometryShader) { m_pGeometryShader->Release(); }

	if (CameraTexture2D) { CameraTexture2D->Release(); }
	if (CameraRender) { CameraRender->Release(); }



	// Scene 2
	if (RenderTemp) { RenderTemp->Release(); }
	if (SwapTemp) { SwapTemp->Release(); }
	if (DeviceTemp) { DeviceTemp->Release(); }
	if (DeviceContextTemp) { DeviceContextTemp->Release(); }
	if (BackBufferTemp) { BackBufferTemp->Release(); }
	if (InputTemp) { InputTemp->Release(); }
	if (DepthStencilTemp) { DepthStencilTemp->Release(); }
	if (SamplerStateTemp) { SamplerStateTemp->Release(); }
	if (Texture2DTemp) { Texture2DTemp->Release(); }
	if (VertexShaderTemp) { VertexShaderTemp->Release(); }
	if (PixelShaderTemp) { PixelShaderTemp->Release(); }

	if (SkyBoxVertexShaderTemp) { SkyBoxVertexShaderTemp->Release(); }
	if (SkyBoxPixelShaderTemp) { SkyBoxPixelShaderTemp->Release(); }
	if (SkyBoxVertexBuffer2) { SkyBoxVertexBuffer2->Release(); }
	if (SkyBoxIndexBuffer2) { SkyBoxIndexBuffer2->Release(); }
	if (SkyBoxConstantBuffer2) { SkyBoxConstantBuffer2->Release(); }
	if (SkyBoxTexture2) { SkyBoxTexture2->Release(); }

	if (WallVertexBuffer1) { WallVertexBuffer1->Release(); }
	if (WallIndexBuffer1) { WallIndexBuffer1->Release(); }
	if (WallConstantBuffer1) { WallConstantBuffer1->Release(); }
	if (WallTexture1) { WallTexture1->Release(); }
	if (WallVertexBuffer2) { WallVertexBuffer2->Release(); }
	if (WallIndexBuffer2) { WallIndexBuffer2->Release(); }
	if (WallConstantBuffer2) { WallConstantBuffer2->Release(); }
	if (WallTexture2) { WallTexture2->Release(); }
	if (WallVertexBuffer3) { WallVertexBuffer3->Release(); }
	if (WallIndexBuffer3) { WallIndexBuffer3->Release(); }
	if (WallConstantBuffer3) { WallConstantBuffer3->Release(); }
	if (WallTexture3) { WallTexture3->Release(); }
	if (WallVertexBuffer4) { WallVertexBuffer4->Release(); }
	if (WallIndexBuffer4) { WallIndexBuffer4->Release(); }
	if (WallConstantBuffer4) { WallConstantBuffer4->Release(); }
	if (WallTexture4) { WallTexture4->Release(); }


	if (CameraResource) { CameraResource->Release(); }
}


//----------------------------------------------------------------------------------------------------------
// Functions: Creating/Initializing Stuff Needed by DirectX
//----------------------------------------------------------------------------------------------------------
void SetCube() {
	// Creating Cube Vertex

	if (cubeverts < 4) {
	#pragma region Normal
		SIMPLE_VERTEX Vertex[] = {
			#pragma region CubeVerts
			#if TextureCube
			{ XMFLOAT4(-1.0f - cubeverts, 1.0f + cubeverts, -1.0f - cubeverts, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),	XMFLOAT3(1.0f, 0.0f, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
			{ XMFLOAT4(1.0f + cubeverts, 1.0f + cubeverts, -1.0f - cubeverts, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),	XMFLOAT3(0.0f, 0.0f, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
			{ XMFLOAT4(1.0f + cubeverts, 1.0f + cubeverts, 1.0f + cubeverts, 1.0f),		XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),	XMFLOAT3(0.0f, 1.0f, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
			{ XMFLOAT4(-1.0f - cubeverts, 1.0f + cubeverts, 1.0f + cubeverts, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },

			{ XMFLOAT4(-1.0f - cubeverts, -1.0f - cubeverts, -1.0f - cubeverts, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 1.0f),	XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f) },
			{ XMFLOAT4(1.0f + cubeverts, -1.0f - cubeverts, -1.0f - cubeverts, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 1.0f),	XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f) },
			{ XMFLOAT4(1.0f + cubeverts, -1.0f - cubeverts, 1.0f + cubeverts, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f) },
			{ XMFLOAT4(-1.0f - cubeverts, -1.0f - cubeverts, 1.0f + cubeverts, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 1.0f),	XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f) },

			{ XMFLOAT4(-1.0f - cubeverts, -1.0f - cubeverts, 1.0f + cubeverts, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 1.0f),	XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f) },
			{ XMFLOAT4(-1.0f - cubeverts, -1.0f - cubeverts, -1.0f - cubeverts, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f) },
			{ XMFLOAT4(-1.0f - cubeverts, 1.0f + cubeverts, -1.0f - cubeverts, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 1.0f),	XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f) },
			{ XMFLOAT4(-1.0f - cubeverts, 1.0f + cubeverts, 1.0f + cubeverts, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 1.0f),	XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f) },

			{ XMFLOAT4(1.0f + cubeverts, -1.0f - cubeverts, 1.0f + cubeverts, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) },
			{ XMFLOAT4(1.0f + cubeverts, -1.0f - cubeverts, -1.0f - cubeverts, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) },
			{ XMFLOAT4(1.0f + cubeverts, 1.0f + cubeverts, -1.0f - cubeverts, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) },
			{ XMFLOAT4(1.0f + cubeverts, 1.0f + cubeverts, 1.0f + cubeverts, 1.0f),		XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) },
			////////////////
			{ XMFLOAT4(-1.0f - cubeverts, -1.0f - cubeverts, -1.0f - cubeverts, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },
			{ XMFLOAT4(1.0f + cubeverts, -1.0f - cubeverts, -1.0f - cubeverts, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },
			{ XMFLOAT4(1.0f + cubeverts, 1.0f + cubeverts, -1.0f - cubeverts, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },
			{ XMFLOAT4(-1.0f - cubeverts, 1.0f + cubeverts, -1.0f - cubeverts, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },
			////////////////
			{ XMFLOAT4(-1.0f - cubeverts, -1.0f - cubeverts, 1.0f + cubeverts, 1.0f),	XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f) },
			{ XMFLOAT4(1.0f + cubeverts, -1.0f - cubeverts, 1.0f + cubeverts, 1.0f),	XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f) },
			{ XMFLOAT4(1.0f + cubeverts, 1.0f + cubeverts, 1.0f + cubeverts, 1.0f),		XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f) },
			{ XMFLOAT4(-1.0f - cubeverts, 1.0f + cubeverts, 1.0f + cubeverts, 1.0f),	XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f) },
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
	buffdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffdesc.ByteWidth = sizeof(DWORD32) * 36;
	data.pSysMem = Indexes;
	m_pDevice->CreateBuffer(&buffdesc, &data, &m_pIndexBuffer);

	// Initializing/Creating Constant Buffer
	buffdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffdesc.ByteWidth = sizeof(ConstantMatrix);
	m_pDevice->CreateBuffer(&buffdesc, nullptr, &m_pConstantBuffer);
	#pragma endregion
	}
	else if (cubeverts >= 4) {
	#pragma region Inverted
		SIMPLE_VERTEX Vertex[] = {
			#pragma region CubeVerts
			{ XMFLOAT4(-1.0f - cubeverts, -1.0f - cubeverts, -1.0f - cubeverts, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
			{ XMFLOAT4(1.0f + cubeverts, -1.0f - cubeverts, -1.0f - cubeverts, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
			{ XMFLOAT4(1.0f + cubeverts, -1.0f - cubeverts, 1.0f + cubeverts, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
			{ XMFLOAT4(-1.0f - cubeverts, -1.0f - cubeverts, 1.0f + cubeverts, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },

			{ XMFLOAT4(-1.0f - cubeverts, 1.0f + cubeverts, -1.0f - cubeverts, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 1.0f),	XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f) },
			{ XMFLOAT4(1.0f + cubeverts, 1.0f + cubeverts, -1.0f - cubeverts, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 1.0f),	XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f) },
			{ XMFLOAT4(1.0f + cubeverts, 1.0f + cubeverts, 1.0f + cubeverts, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 1.0f),	XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f) },
			{ XMFLOAT4(-1.0f - cubeverts, 1.0f + cubeverts, 1.0f + cubeverts, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f) },

			{ XMFLOAT4(1.0f + cubeverts, -1.0f - cubeverts, 1.0f + cubeverts, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f) },
			{ XMFLOAT4(1.0f + cubeverts, -1.0f - cubeverts, -1.0f - cubeverts, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 1.0f),	XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f) },
			{ XMFLOAT4(1.0f + cubeverts, 1.0f + cubeverts, -1.0f - cubeverts, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 1.0f),	XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f) },
			{ XMFLOAT4(1.0f + cubeverts, 1.0f + cubeverts, 1.0f + cubeverts, 1.0f),		XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 1.0f),	XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f) },

			{ XMFLOAT4(-1.0f - cubeverts, -1.0f - cubeverts, 1.0f + cubeverts, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) },
			{ XMFLOAT4(-1.0f - cubeverts, -1.0f - cubeverts, -1.0f - cubeverts, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) },
			{ XMFLOAT4(-1.0f - cubeverts, 1.0f + cubeverts, -1.0f - cubeverts, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) },
			{ XMFLOAT4(-1.0f - cubeverts, 1.0f + cubeverts, 1.0f + cubeverts, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) },
			////////////////
			{ XMFLOAT4(-1.0f - cubeverts, -1.0f - cubeverts, 1.0f + cubeverts, 1.0f),	XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },
			{ XMFLOAT4(1.0f + cubeverts, -1.0f - cubeverts, 1.0f + cubeverts, 1.0f),	XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },
			{ XMFLOAT4(1.0f + cubeverts, 1.0f + cubeverts, 1.0f + cubeverts, 1.0f),		XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },
			{ XMFLOAT4(-1.0f - cubeverts, 1.0f + cubeverts, 1.0f + cubeverts, 1.0f),	XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },
			////////////////
			{ XMFLOAT4(-1.0f - cubeverts, -1.0f - cubeverts, -1.0f - cubeverts, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f) },
			{ XMFLOAT4(1.0f + cubeverts, -1.0f - cubeverts, -1.0f - cubeverts, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f) },
			{ XMFLOAT4(1.0f + cubeverts, 1.0f + cubeverts, -1.0f - cubeverts, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f) },
			{ XMFLOAT4(-1.0f - cubeverts, 1.0f + cubeverts, -1.0f - cubeverts, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f) },
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
	m_pDevice->CreateBuffer(&buffdesc, &data, &m_pIndexBuffer);

	// Initializing/Creating Constant Buffer
	buffdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffdesc.ByteWidth = sizeof(ConstantMatrix);
	m_pDevice->CreateBuffer(&buffdesc, nullptr, &m_pConstantBuffer);
	#pragma endregion
	}

}

void SetFloorAndGeometry()
{
	// Creating Floor Vertex
	SIMPLE_VERTEX Vertex[] = {
		{ XMFLOAT4(-30.0f, -0.55f, -30.0f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 5.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(30.0f, -0.55f, -30.0f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 5.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(30.0f, -0.55f,  30.0f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 5.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(-30.0f, -0.55f,  30.0f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 5.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
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
	buffdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffdesc.ByteWidth = sizeof(DWORD32) * 6;
	data.pSysMem = Indexes;
	m_pDevice->CreateBuffer(&buffdesc, &data, &FloorIndexBuffer);
	m_pDevice->CreateBuffer(&buffdesc, &data, &GeometryIndexBuffer);

	// Initializing/Creating Constant Buffer
	buffdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffdesc.ByteWidth = sizeof(ConstantMatrix);
	m_pDevice->CreateBuffer(&buffdesc, nullptr, &FloorConstantBuffer);
	m_pDevice->CreateBuffer(&buffdesc, nullptr, &GeometryConstantBuffer);

}

void SetSkyBox(ID3D11Device* &device, const wchar_t* fileName, ID3D11ShaderResourceView* &texture, ID3D11Buffer* &vertexBuffer, ID3D11Buffer* &indexBuffer, ID3D11Buffer* &constantBuffer)
{
	SIMPLE_VERTEX Vertex[] = {
		#pragma region SkyVerts
		{ XMFLOAT4(-100.0f, -100.0f, -100.0f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(100.0f, -100.0f, -100.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 0.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(100.0f, -100.0f, 100.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 0.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(-100.0f, -100.0f, 100.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 0.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },

		{ XMFLOAT4(-100.0f, 100.0f, -100.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 0.0f),	XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(100.0f, 100.0f, -100.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(100.0f, 100.0f, 100.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 0.0f),	XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(-100.0f, 100.0f, 100.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 0.0f),	XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f) },

		{ XMFLOAT4(100.0f, -100.0f, 100.0f, 1.0f),		XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 0.0f),	XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(100.0f, -100.0f, -100.0f, 1.0f),		XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 0.0f),	XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(100.0f, 100.0f, -100.0f, 1.0f),		XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(100.0f, 100.0f, 100.0f, 1.0f),		XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 0.0f),	XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f) },

		{ XMFLOAT4(-100.0f, -100.0f, 100.0f, 1.0f),		XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 0.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(-100.0f, -100.0f, -100.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 0.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(-100.0f, 100.0f, -100.0f, 1.0f),		XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 0.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(-100.0f, 100.0f, 100.0f, 1.0f),		XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) },
////////////////
		{ XMFLOAT4(-100.0f, -100.0f, 100.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 0.0f),	XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },
		{ XMFLOAT4(100.0f, -100.0f, 100.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 0.0f),	XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },
		{ XMFLOAT4(100.0f, 100.0f, 100.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },
		{ XMFLOAT4(-100.0f, 100.0f, 100.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 0.0f),	XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },
////////////////
		{ XMFLOAT4(-100.0f, -100.0f, -100.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 0.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f) },
		{ XMFLOAT4(100.0f, -100.0f, -100.0f, 1.0f),		XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 0.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f) },
		{ XMFLOAT4(100.0f, 100.0f, -100.0f, 1.0f),		XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 0.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f) },
		{ XMFLOAT4(-100.0f, 100.0f, -100.0f, 1.0f),		XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f) },
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
	device->CreateBuffer(&buffdesc, &data, &vertexBuffer);

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
	device->CreateBuffer(&buffdesc, &data, &indexBuffer);

	// Initializing/Creating Constant Buffer
	buffdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffdesc.ByteWidth = sizeof(ConstantMatrix);
	device->CreateBuffer(&buffdesc, nullptr, &constantBuffer);

	// Loading SkyBox Texture
	CreateDDSTextureFromFile(device, fileName, NULL, &texture);
}

void SetModel(const char* fileName, ObjLoader &model, ID3D11Buffer* &vertBuffer, ID3D11Buffer* &indexBuffer, ID3D11Buffer* &constantBuffer, ID3D11Device* &device)
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
		Vertex[i].uvs.z = 1;
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
	device->CreateBuffer(&objVertbuffdesc, &objsubResource, &vertBuffer);

	// Initializing/Creating Index Buffer
	objVertbuffdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	objVertbuffdesc.ByteWidth = sizeof(DWORD32) * objIcount;
	objsubResource.pSysMem = Indexes;
	device->CreateBuffer(&objVertbuffdesc, &objsubResource, &indexBuffer);

	// Initializing/Creating Constant Buffer
	objVertbuffdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	objVertbuffdesc.ByteWidth = sizeof(ConstantMatrix);
	device->CreateBuffer(&objVertbuffdesc, NULL, &constantBuffer);

	delete[] Vertex;
	delete[] Indexes;
}

void SetWall() {
	#pragma region Wall1
	SIMPLE_VERTEX Vertex1[] = {
		{ XMFLOAT4(0.0f, -10.0f, 10.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 1.0f),	XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(0.0f, -10.0f, -10.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(0.0f, 10.0f, -10.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 1.0f),	XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(0.0f, 10.0f, 10.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 1.0f),	XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f) },
	};

	// Initializing Buffer
	D3D11_BUFFER_DESC		buffdesc;
	ZeroMemory(&buffdesc, sizeof(D3D11_BUFFER_DESC));
	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffdesc.ByteWidth = sizeof(SIMPLE_VERTEX) * 4;

	// Initializing SubSource
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = Vertex1;

	// Creating Vertex Buffer
	DeviceTemp->CreateBuffer(&buffdesc, &data, &WallVertexBuffer1);

	// Creating Index
	DWORD32 Indexes1[] = {
		3,1,0,
		2,1,3,
	};

	// Creating Index buffer
	buffdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffdesc.ByteWidth = sizeof(DWORD32) * 6;
	data.pSysMem = Indexes1;
	DeviceTemp->CreateBuffer(&buffdesc, &data, &WallIndexBuffer1);

	// Initializing/Creating Constant Buffer
	buffdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffdesc.ByteWidth = sizeof(ConstantMatrix);
	DeviceTemp->CreateBuffer(&buffdesc, nullptr, &WallConstantBuffer1);
	#pragma endregion

	#pragma region Wall2
	SIMPLE_VERTEX Vertex2[] = {
		{ XMFLOAT4(0.0f, -10.0f, 10.0f, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(0.0f, -10.0f, -10.0f, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(0.0f, 10.0f, -10.0f, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(0.0f, 10.0f, 10.0f, 1.0f),		XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) },
	};

	// Initializing Buffer
	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffdesc.ByteWidth = sizeof(SIMPLE_VERTEX) * 4;

	// Initializing SubSource
	data.pSysMem = Vertex2;

	// Creating Vertex Buffer
	DeviceTemp->CreateBuffer(&buffdesc, &data, &WallVertexBuffer2);

	// Creating Index
	DWORD32 Indexes[] = {
		2,0,1,
		3,0,2,
	};

	// Creating Index buffer
	buffdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffdesc.ByteWidth = sizeof(DWORD32) * 6;
	data.pSysMem = Indexes;
	DeviceTemp->CreateBuffer(&buffdesc, &data, &WallIndexBuffer2);

	// Initializing/Creating Constant Buffer
	buffdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffdesc.ByteWidth = sizeof(ConstantMatrix);
	DeviceTemp->CreateBuffer(&buffdesc, nullptr, &WallConstantBuffer2);
	#pragma endregion

	#pragma region Wall3
	SIMPLE_VERTEX Vertex3[] = {
		{ XMFLOAT4(-10.0f, -10.0f, 0.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },
		{ XMFLOAT4(10.0f, -10.0f, 0.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },
		{ XMFLOAT4(10.0f, 10.0f, 0.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },
		{ XMFLOAT4(-10.0f, 10.0f, 0.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },
	};

	// Initializing Buffer
	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffdesc.ByteWidth = sizeof(SIMPLE_VERTEX) * 4;

	// Initializing SubSource
	data.pSysMem = Vertex3;

	// Creating Vertex Buffer
	DeviceTemp->CreateBuffer(&buffdesc, &data, &WallVertexBuffer3);

	// Creating Index
	DWORD32 Indexes3[] = {
		3,1,0,
		2,1,3,
	};

	// Creating Index buffer
	buffdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffdesc.ByteWidth = sizeof(DWORD32) * 6;
	data.pSysMem = Indexes3;
	DeviceTemp->CreateBuffer(&buffdesc, &data, &WallIndexBuffer3);

	// Initializing/Creating Constant Buffer
	buffdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffdesc.ByteWidth = sizeof(ConstantMatrix);
	DeviceTemp->CreateBuffer(&buffdesc, nullptr, &WallConstantBuffer3);
	#pragma endregion

	#pragma region Wall4
	SIMPLE_VERTEX Vertex4[] = {
		{ XMFLOAT4(-10.0f, -10.0f, 0.0f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f) },
		{ XMFLOAT4(10.0f, -10.0f, 0.0f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f) },
		{ XMFLOAT4(10.0f, 10.0f, 0.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f) },
		{ XMFLOAT4(-10.0f, 10.0f, 0.0f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f) },
	};

	// Initializing Buffer
	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffdesc.ByteWidth = sizeof(SIMPLE_VERTEX) * 4;

	// Initializing SubSource
	data.pSysMem = Vertex4;

	// Creating Vertex Buffer
	DeviceTemp->CreateBuffer(&buffdesc, &data, &WallVertexBuffer4);

	// Creating Index
	DWORD32 Indexes4[] = {
		2,0,1,
		3,0,2
	};

	// Creating Index buffer
	buffdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffdesc.ByteWidth = sizeof(DWORD32) * 6;
	data.pSysMem = Indexes4;
	DeviceTemp->CreateBuffer(&buffdesc, &data, &WallIndexBuffer4);

	// Initializing/Creating Constant Buffer
	buffdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffdesc.ByteWidth = sizeof(ConstantMatrix);
	DeviceTemp->CreateBuffer(&buffdesc, nullptr, &WallConstantBuffer4);
	#pragma endregion
}

//----------------------------------------------------------------------------------------------------------
// Functions: Rendering Frame by frame
//----------------------------------------------------------------------------------------------------------
void DrawIndexedGeometry(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, ID3D11Buffer* constantBuffer, ID3D11InputLayout* input, ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader, UINT count) {
	/* Renders the Triangles for the SkyBox */
	unsigned int	strides = sizeof(SIMPLE_VERTEX);
	unsigned int	offsets = 0;
	// Setting VertexBuffer
	deviceContext->IASetVertexBuffers(NULL, 1, &vertexBuffer, &strides, &offsets);
	// Setting Input Layout
	deviceContext->IASetInputLayout(input);
	// Setting Index Buffer
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, NULL);
	// Setting Topology
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	/* Setting Vertex Shader */
	deviceContext->VSSetShader(vertexShader, NULL, NULL);
	// Setting Constant Buffer
	deviceContext->VSSetConstantBuffers(NULL, 1, &constantBuffer);

	/* Setting Pixel Shader */
	deviceContext->PSSetShader(pixelShader, NULL, NULL);
	// Setting Texture Resource
	deviceContext->PSSetShaderResources(NULL, 1, &texture);

	// Drawing Indexed SkyBox
	deviceContext->DrawIndexed(count, 0, 0);
}

void DrawInstancedCube() {
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
	m_pDeviceContext->PSSetShaderResources(NULL, 1, &m_pTexture);

	// Drawing Instanced Cube
	m_pDeviceContext->DrawIndexedInstanced(36, 3, 0, 0, 0);
}

void DrawGSGeometry() {
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
	m_pDeviceContext->PSSetShaderResources(0, 1, &CameraResource);
	
	/* Setting Geometry Shader */
	m_pDeviceContext->GSSetShader(m_pGeometryShader, NULL, NULL);
	// Setting Geometry Constant Buffer
	m_pDeviceContext->GSSetConstantBuffers(0, 1, &GeometryConstantBuffer);
	
	// Drawing Indexed Quad
	m_pDeviceContext->Draw(1, 0);

	/* Nulling the Geometry Shader */
	m_pDeviceContext->GSSetShader(NULL, NULL, NULL);
}

void DrawModel(ObjLoader & model, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, ID3D11Buffer* constantBuffer, ID3D11ShaderResourceView* texture, ID3D11InputLayout* input, ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader)
{
	unsigned int	strides = sizeof(SIMPLE_VERTEX);
	unsigned int	offsets = 0;
	// Setting VertexBuffer
	m_pDeviceContext->IASetVertexBuffers(NULL, 1, &vertexBuffer, &strides, &offsets);
	// Setting Input Layout
	m_pDeviceContext->IASetInputLayout(input);
	// Setting Index Buffer
	m_pDeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, NULL);
	// Setting Topology
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	/* Setting Vertex Shader */
	m_pDeviceContext->VSSetShader(vertexShader, nullptr, NULL);
	// Setting Constant Buffer
	m_pDeviceContext->VSSetConstantBuffers(NULL, 1, &constantBuffer);
	/* Setting Pixel Shader */
	m_pDeviceContext->PSSetShader(pixelShader, nullptr, NULL);
	// Setting Texture Resource
	m_pDeviceContext->PSSetShaderResources(NULL, 1, &texture);

	// Drawing Indexed Model
	m_pDeviceContext->DrawIndexed(model.GetIndex().size(), 0, 0);
}

void UpdateConstant(XMMATRIX &geometryMatrix, XMMATRIX &viewMatrix, XMMATRIX &projectionMatrix, ID3D11Buffer* &constantBuffer, ID3D11DeviceContext* &deviceContext) {
	ConstantMatrix constantM;
	constantM.ObjectMatrix = XMMatrixTranspose(geometryMatrix);
	constantM.View = XMMatrixTranspose(viewMatrix);
	constantM.Projection = XMMatrixTranspose(projectionMatrix);
	deviceContext->UpdateSubresource(constantBuffer, NULL, NULL, &constantM, NULL, NULL);
}


//----------------------------------------------------------------------------------------------------------
// Functions: Key Presses
//----------------------------------------------------------------------------------------------------------
void CameraMovement(XMMATRIX &viewMatrix, XMMATRIX &viewMatrixSub, XMMATRIX &projectionMatrix) {

	// Reset Camera
	if (GetAsyncKeyState('R')) { viewMatrix = viewMatrixSub; }

	// ViewPort/Camera Fly Forward
	if (GetAsyncKeyState('Q')) { viewMatrix = XMMatrixMultiply(viewMatrix, XMMatrixTranslation(0, 0, -0.01f)); }
	// ViewPort/Camera Fly Backward
	if (GetAsyncKeyState('E')) { viewMatrix = XMMatrixMultiply(viewMatrix, XMMatrixTranslation(0, 0, 0.01f)); }

	// ViewPort/Camera movement Up
	if (GetAsyncKeyState('W')) { viewMatrix = XMMatrixMultiply(viewMatrix, XMMatrixTranslation(0, -0.01f, 0)); }
	// ViewPort/Camera movement Down
	if (GetAsyncKeyState('S')) { viewMatrix = XMMatrixMultiply(viewMatrix, XMMatrixTranslation(0, 0.01f, 0)); }
	// ViewPort/Camera movement Right
	if (GetAsyncKeyState('D')) { viewMatrix = XMMatrixMultiply(viewMatrix, XMMatrixTranslation(-0.01f, 0, 0)); }
	// ViewPort/Camera movement Left
	if (GetAsyncKeyState('A')) { viewMatrix = XMMatrixMultiply(viewMatrix, XMMatrixTranslation(0.01f, 0, 0)); }

	// ViewPort/Camera rotate Up
	if (GetAsyncKeyState('I')) { viewMatrix = XMMatrixMultiply(viewMatrix, XMMatrixRotationX(0.001)); }
	// ViewPort/Camera rotate Down
	if (GetAsyncKeyState('K')) { viewMatrix = XMMatrixMultiply(viewMatrix, XMMatrixRotationX(-0.001)); }
	// ViewPort/Camera rotate Left
	if (GetAsyncKeyState('J')) { viewMatrix = XMMatrixMultiply(viewMatrix, XMMatrixRotationY(0.001)); }
	// ViewPort/Camera rotate Right
	if (GetAsyncKeyState('L')) { viewMatrix = XMMatrixMultiply(viewMatrix, XMMatrixRotationY(-0.001)); }

	// Adjust Near Plane
	if (GetAsyncKeyState(VK_NUMPAD1) & 0x1) { NearPlane += 0.01f; }
	if (GetAsyncKeyState(VK_NUMPAD2) & 0x1) { if (FarPlane > 0.02) { NearPlane -= 0.01f; } }

	// Adjust Fare Plane
	if (GetAsyncKeyState(VK_NUMPAD4) & 0x1) { if (FarPlane > 1) { FarPlane -= 1.0f; } }
	if (GetAsyncKeyState(VK_NUMPAD5) & 0x1) { FarPlane += 1.0f; }

	// ViewPort/Camera Zoom In
	if (GetAsyncKeyState(VK_NUMPAD7) & 0x1) { Zoom += 0.05; }
	// ViewPort/Camera Zoom Out
	if (GetAsyncKeyState(VK_NUMPAD8) & 0x1) { if (Zoom > 1.1) { Zoom -= 0.05; } }

	// Initializing the projection matrix
	projectionMatrix = XMMatrixPerspectiveFovLH(XM_PI / Zoom, BACKBUFFER_WIDTH / static_cast<float>(BACKBUFFER_HEIGHT), NearPlane, FarPlane);
}

void LightMovment() {
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
			Lights[2].Direction = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
			DirectionCount++;
		}
		else if (DirectionCount == 5) {
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
}

void SceneManagment() {

	// Getting The Current Width and Height of the Window
	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT width = rc.right;
	UINT height = rc.bottom;

	if (GetAsyncKeyState('Z') & 0x1) {
		SwapSceneInt++;
		if (SwapSceneInt == 1) {
			SwapCameraInt = 0;
			ViewMatrix = XMMatrixMultiply(XMMatrixLookAtLH(Eye, Focus, Up), XMMatrixTranslation(0, 0, 0));
			ViewMatrixSub = XMMatrixMultiply(XMMatrixLookAtLH(Eye, Focus, Up), XMMatrixTranslation(0, 0, 0));
			ViewMatrix2 = XMMatrixMultiply(XMMatrixLookAtLH(XMVectorSet(0.0f, 1.5f, 5.0f, 0.0f), Focus, Up), XMMatrixTranslation(0, 0, 0));
			ViewMatrix2Sub = XMMatrixMultiply(XMMatrixLookAtLH(XMVectorSet(0.0f, 1.5f, 5.0f, 0.0f), Focus, Up), XMMatrixTranslation(0, 0, 0));
			ViewMatrix3 = XMMatrixMultiply(XMMatrixLookAtLH(XMVectorSet(-5.0f, 1.5f, 0.0f, 0.0f), Focus, Up), XMMatrixTranslation(0, 0, 0));
			ViewMatrix3Sub = XMMatrixMultiply(XMMatrixLookAtLH(XMVectorSet(-5.0f, 1.5f, 0.0f, 0.0f), Focus, Up), XMMatrixTranslation(0, 0, 0));
			ViewMatrix4 = XMMatrixMultiply(XMMatrixLookAtLH(XMVectorSet(5.0f, 1.5f, 0.0f, 0.0f), Focus, Up), XMMatrixTranslation(0, 0, 0));
			ViewMatrix4Sub = XMMatrixMultiply(XMMatrixLookAtLH(XMVectorSet(5.0f, 1.5f, 0.0f, 0.0f), Focus, Up), XMMatrixTranslation(0, 0, 0));

			// Initializing the Viewport
			m_ViewPort[0].Width = static_cast<float>(width * 0.5);
			m_ViewPort[0].Height = static_cast<float>(height * 0.5);
			m_ViewPort[0].MinDepth = 0.0f;
			m_ViewPort[0].MaxDepth = 1.0f;
			m_ViewPort[0].TopLeftX = 0;
			m_ViewPort[0].TopLeftY = 0;

			m_ViewPort[1].Width = static_cast<float>(width * 0.5f);
			m_ViewPort[1].Height = static_cast<float>(height * 0.5f);
			m_ViewPort[1].MinDepth = 0.0f;
			m_ViewPort[1].MaxDepth = 1.0f;
			m_ViewPort[1].TopLeftX = static_cast<float>(width * 0.5f);
			m_ViewPort[1].TopLeftY = 0;

			m_ViewPort[2].Width = static_cast<float>(width * 0.5f);
			m_ViewPort[2].Height = static_cast<float>(height * 0.5f);
			m_ViewPort[2].MinDepth = 0.0f;
			m_ViewPort[2].MaxDepth = 1.0f;
			m_ViewPort[2].TopLeftX = 0;
			m_ViewPort[2].TopLeftY = static_cast<float>(height * 0.5f);

			m_ViewPort[3].Width = static_cast<float>(width * 0.5f);
			m_ViewPort[3].Height = static_cast<float>(height * 0.5f);
			m_ViewPort[3].MinDepth = 0.0f;
			m_ViewPort[3].MaxDepth = 1.0f;
			m_ViewPort[3].TopLeftX = static_cast<float>(width * 0.5f);
			m_ViewPort[3].TopLeftY = static_cast<float>(height * 0.5f);

		}
		if (SwapSceneInt == 2) {
			ViewMatrix = XMMatrixLookAtLH(Eye, Focus, Up);
			ViewMatrixSub = XMMatrixLookAtLH(Eye, Focus, Up);

			// Initializing the Viewport
			m_ViewPort[0].Width = static_cast<float>(width);
			m_ViewPort[0].Height = static_cast<float>(height);
			m_ViewPort[0].MinDepth = 0.0f;
			m_ViewPort[0].MaxDepth = 1.0f;
			m_ViewPort[0].TopLeftX = 0;
			m_ViewPort[0].TopLeftY = 0;

			SwapSceneInt = 0;
		}
	}

}