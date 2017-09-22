// Graphics2Project.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Graphics2Project.h"
#include "ObjLoader.h"

#define MAX_LOADSTRING 100
#define BACKBUFFER_WIDTH	1089
#define BACKBUFFER_HEIGHT	1000
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
ID3D11Buffer*					LightConstantBuffer3	= nullptr;
ID3D11Buffer*					LightConstantBuffer4	= nullptr;

// Need For Skybox 1
XMMATRIX						SkyBoxMatrix;
ID3D11Buffer*					SkyBoxVertexBuffer		= nullptr;
ID3D11Buffer*					SkyBoxIndexBuffer		= nullptr;
ID3D11Buffer*					SkyBoxConstantBuffer	= nullptr;
ID3D11ShaderResourceView*		SkyBoxTexture			= nullptr;


// Blendinig
ID3D11BlendState * g_pBlendStateNoBlend = nullptr;
float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
UINT sampleMask = 0xffffffff;

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

//-Scene-3--------------------------------------------------------------------------------
ID3D11RenderTargetView*			Render3					= nullptr;
IDXGISwapChain*					Swap3					= nullptr;
ID3D11Device*					Device3					= nullptr;
ID3D11DeviceContext*			DeviceContext3			= nullptr;
ID3D11Resource*					BackBuffer3				= nullptr;
ID3D11InputLayout*				Input3					= nullptr;
ID3D11VertexShader* 			VertexShader3			= nullptr;
ID3D11PixelShader*				PixelShader3			= nullptr;
ID3D11VertexShader* 			SkyBoxVertexShader3		= nullptr;
ID3D11PixelShader*				SkyBoxPixelShader3		= nullptr;
ID3D11DepthStencilView*			DepthStencil3			= nullptr;
ID3D11SamplerState*				SamplerState3			= nullptr;
ID3D11Texture2D*				Texture2D3				= nullptr;

// Need For Skybox 3
XMMATRIX						SkyBoxMatrix3;
ID3D11Buffer*					SkyBoxVertexBuffer3		= nullptr;
ID3D11Buffer*					SkyBoxIndexBuffer3		= nullptr;
ID3D11Buffer*					SkyBoxConstantBuffer3	= nullptr;
ID3D11ShaderResourceView*		SkyBoxTexture3			= nullptr;

// Need for Loading Satellite
ObjLoader						Satellite;
XMMATRIX						SatelliteMatrix;
ID3D11Buffer*					SatelliteVertexBuffer	= nullptr;
ID3D11Buffer*					SatelliteIndexBuffer	= nullptr;
ID3D11Buffer*					SatelliteConstantBuffer	= nullptr;
ID3D11ShaderResourceView*		SatelliteTexture		= nullptr;

// Need for Loading Moon
ObjLoader						Moon;
XMMATRIX						MoonMatrix;
ID3D11Buffer*					MoonVertexBuffer		= nullptr;
ID3D11Buffer*					MoonIndexBuffer			= nullptr;
ID3D11Buffer*					MoonConstantBuffer		= nullptr;
ID3D11ShaderResourceView*		MoonTexture				= nullptr;

// Need for Loading Earth
ObjLoader						Earth;
XMMATRIX						EarthMatrix;
ID3D11Buffer*					EarthVertexBuffer		= nullptr;
ID3D11Buffer*					EarthIndexBuffer		= nullptr;
ID3D11Buffer*					EarthConstantBuffer		= nullptr;
ID3D11ShaderResourceView*		EarthTexture			= nullptr;

// Need for Loading Sun
ObjLoader						Sun;
XMMATRIX						SunMatrix;
ID3D11Buffer*					SunVertexBuffer			= nullptr;
ID3D11Buffer*					SunIndexBuffer			= nullptr;
ID3D11Buffer*					SunConstantBuffer		= nullptr;
ID3D11ShaderResourceView*		SunTexture				= nullptr;

// Need for Loading Ship
ObjLoader						SpaceShip;
XMMATRIX						SpaceShipMatrix;
ID3D11Buffer*					SpaceShipVertexBuffer	= nullptr;
ID3D11Buffer*					SpaceShipIndexBuffer	= nullptr;
ID3D11Buffer*					SpaceShipConstantBuffer	= nullptr;
ID3D11ShaderResourceView*		SpaceShipTexture		= nullptr;

// Need for Loading Ice Planet
ObjLoader						Planet2;
XMMATRIX						Planet2Matrix;
ID3D11Buffer*					Planet2VertexBuffer		= nullptr;
ID3D11Buffer*					Planet2IndexBuffer		= nullptr;
ID3D11Buffer*					Planet2ConstantBuffer	= nullptr;
ID3D11ShaderResourceView*		Planet2Texture			= nullptr;

//----------------------------------------------------------------------------------------


//-Scene-4--------------------------------------------------------------------------------
// Need For Skybox 4
XMMATRIX						SkyBoxMatrix4;
ID3D11Buffer*					SkyBoxVertexBuffer4		= nullptr;
ID3D11Buffer*					SkyBoxIndexBuffer4		= nullptr;
ID3D11Buffer*					SkyBoxConstantBuffer4	= nullptr;
ID3D11ShaderResourceView*		SkyBoxTexture4			= nullptr;

// Need for Loading Sea Floor
XMMATRIX						SeaFloorMatrix;
ID3D11Buffer*					SeaFloorVertexBuffer	= nullptr;
ID3D11Buffer*					SeaFloorIndexBuffer		= nullptr;
ID3D11Buffer*					SeaFloorConstantBuffer	= nullptr;
ID3D11ShaderResourceView*		SeaFloorTexture			= nullptr;
ID3D11ShaderResourceView*		CityFloorTexture		= nullptr;

// Need for Loading Ship
ObjLoader						Ship1;
XMMATRIX						Ship1Matrix;
ID3D11Buffer*					Ship1VertexBuffer		= nullptr;
ID3D11Buffer*					Ship1IndexBuffer		= nullptr;
ID3D11Buffer*					Ship1ConstantBuffer		= nullptr;
ID3D11ShaderResourceView*		Ship1Texture			= nullptr;

// Need for Loading Pirate Look
ObjLoader						Ship2;
XMMATRIX						Ship2Matrix;
ID3D11Buffer*					Ship2VertexBuffer		= nullptr;
ID3D11Buffer*					Ship2IndexBuffer		= nullptr;
ID3D11Buffer*					Ship2ConstantBuffer		= nullptr;
ID3D11ShaderResourceView*		Ship2Texture			= nullptr;

// Need for Loading Pirate Atk
ObjLoader						Ship3;
XMMATRIX						Ship3Matrix;
ID3D11Buffer*					Ship3VertexBuffer		= nullptr;
ID3D11Buffer*					Ship3IndexBuffer		= nullptr;
ID3D11Buffer*					Ship3ConstantBuffer		= nullptr;
ID3D11ShaderResourceView*		Ship3Texture			= nullptr;

// Need for Loading Pirate Run
ObjLoader						Ship4;
XMMATRIX						Ship4Matrix;
ID3D11Buffer*					Ship4VertexBuffer		= nullptr;
ID3D11Buffer*					Ship4IndexBuffer		= nullptr;
ID3D11Buffer*					Ship4ConstantBuffer		= nullptr;
ID3D11ShaderResourceView*		Ship4Texture			= nullptr;

//----------------------------------------------------------------------------------------

//-Scene-4--------------------------------------------------------------------------------
// Need For Skybox 4
XMMATRIX						SkyBoxMatrix5;
ID3D11Buffer*					SkyBoxVertexBuffer5		= nullptr;
ID3D11Buffer*					SkyBoxIndexBuffer5		= nullptr;
ID3D11Buffer*					SkyBoxConstantBuffer5	= nullptr;
ID3D11ShaderResourceView*		SkyBoxTexture5			= nullptr;

// Need for Loading Car
ObjLoader						Car;
XMMATRIX						CarMatrix;
ID3D11Buffer*					CarVertexBuffer			= nullptr;
ID3D11Buffer*					CarIndexBuffer			= nullptr;
ID3D11Buffer*					CarConstantBuffer		= nullptr;
ID3D11ShaderResourceView*		CarTexture				= nullptr;

// Need for Loading LightPoll
ObjLoader						LightPoll;
XMMATRIX						LightPollMatrix;
ID3D11Buffer*					LightPollVertexBuffer	= nullptr;
ID3D11Buffer*					LightPollIndexBuffer	= nullptr;
ID3D11Buffer*					LightPollConstantBuffer	= nullptr;
ID3D11ShaderResourceView*		LightPollTexture		= nullptr;

// Need for Loading Helicopter
ObjLoader						Heli;
XMMATRIX						HeliMatrix;
ID3D11Buffer*					HeliVertexBuffer		= nullptr;
ID3D11Buffer*					HeliIndexBuffer			= nullptr;
ID3D11Buffer*					HeliConstantBuffer		= nullptr;
ID3D11ShaderResourceView*		HeliTexture				= nullptr;

// Need for Loading Sun
ObjLoader						Building1;
XMMATRIX						Building1Matrix;
ID3D11Buffer*					Building1VertexBuffer	= nullptr;
ID3D11Buffer*					Building1IndexBuffer	= nullptr;
ID3D11Buffer*					Building1ConstantBuffer	= nullptr;
ID3D11ShaderResourceView*		BuildingTexture			= nullptr;

// Need for Loading Sun
ObjLoader						Building2;
XMMATRIX						Building2Matrix;
ID3D11Buffer*					Building2VertexBuffer	= nullptr;
ID3D11Buffer*					Building2IndexBuffer	= nullptr;
ID3D11Buffer*					Building2ConstantBuffer	= nullptr;

// Need for Loading Sun
ObjLoader						Building3;
XMMATRIX						Building3Matrix;
ID3D11Buffer*					Building3VertexBuffer	= nullptr;
ID3D11Buffer*					Building3IndexBuffer	= nullptr;
ID3D11Buffer*					Building3ConstantBuffer	= nullptr;

// Need for Loading Sun
ObjLoader						Building4;
XMMATRIX						Building4Matrix;
ID3D11Buffer*					Building4VertexBuffer	= nullptr;
ID3D11Buffer*					Building4IndexBuffer	= nullptr;
ID3D11Buffer*					Building4ConstantBuffer	= nullptr;

// Need for Loading Sun
ObjLoader						Building5;
XMMATRIX						Building5Matrix;
ID3D11Buffer*					Building5VertexBuffer	= nullptr;
ID3D11Buffer*					Building5IndexBuffer	= nullptr;
ID3D11Buffer*					Building5ConstantBuffer	= nullptr;

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
float FarPlane	= 1750;

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

XMMATRIX	ProjectionMatrixTemp;
XMMATRIX	ViewMatrixTemp;


POINT cursorPos;

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
void SetSeaFloor();
void DrawIndexedGeometry(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, ID3D11Buffer* constantBuffer, ID3D11InputLayout* input, ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader, UINT count);
void SetSkyBox(ID3D11Device* &device, const wchar_t* fileName, ID3D11ShaderResourceView* &texture, ID3D11Buffer* &vertexBuffer, ID3D11Buffer* &indexBuffer, ID3D11Buffer* &constantBuffer);
void SetModel(const char * filename, ObjLoader & model, ID3D11Buffer* &vertBuffer, ID3D11Buffer* &indexBuffer, ID3D11Buffer* &constantBuffer, ID3D11Device* &device);
void DrawModel(ObjLoader &model, ID3D11DeviceContext * devicecontext, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, ID3D11Buffer* constantBuffer, ID3D11ShaderResourceView* texture, ID3D11InputLayout* input, ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader);
void UpdateConstant(XMMATRIX &geometryMatrix, XMMATRIX &viewMatrix, XMMATRIX &projectionMatrix, ID3D11Buffer* &constantBuffer, ID3D11DeviceContext* &deviceContext);

void CameraMovement(XMMATRIX &viewMatrix, XMMATRIX &viewMatrixSub, XMMATRIX &projectionMatrix);
void LightMovment();
void SceneManagment();
void Resize();

void SetWall();
void SwapChainInt();
// Microsoft::WRL::ComPtr<var> name;
#pragma endregion


#pragma region Windows Code
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

	// Getting The Current Width and Height of the Window
	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT width = rc.right;
	UINT height = rc.bottom;

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
	case WM_SIZE:
		//if (m_pSwapChain)
		//{
		//	m_pDeviceContext->OMSetRenderTargets(0, 0, 0);
		//	// Release all outstanding references to the swap chain's buffers.
		//	m_pRenderTargetView->Release();
		//	m_pBackBuffer->Release();
		//	// Preserve the existing buffer count and format.
		//	// Automatically choose the width and height to match the client rect for HWNDs.
		//	m_pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		//	// Get buffer and create a render-target-view.
		//	ID3D11Texture2D* pBuffer;
		//	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);
		//	m_pDevice->CreateRenderTargetView(pBuffer, NULL, &m_pRenderTargetView);
		//	// Perform error handling here!
		//	pBuffer->Release();
		//	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);
		//	// Set up the viewport.
		//	D3D11_VIEWPORT vp;
		//	vp.Width = width;
		//	vp.Height = height;
		//	vp.MinDepth = 0.0f;
		//	vp.MaxDepth = 1.0f;
		//	vp.TopLeftX = 0;
		//	vp.TopLeftY = 0;
		//	m_pDeviceContext->RSSetViewports(1, &vp);
		//	m_pDeviceContext->RSSetViewports(0, &vp);
		//	// Create depth stencil texture
		//	D3D11_TEXTURE2D_DESC texturedesc;
		//	ZeroMemory(&texturedesc, sizeof(texturedesc));
		//	texturedesc.Width = vp.Width;
		//	texturedesc.Height = vp.Height;
		//	texturedesc.MipLevels = 1;
		//	texturedesc.ArraySize = 1;
		//	texturedesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		//	texturedesc.SampleDesc.Count = 1;
		//	texturedesc.Usage = D3D11_USAGE_DEFAULT;
		//	texturedesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		//	m_pDevice->CreateTexture2D(&texturedesc, NULL, &m_pTexture2D);
		//	// Create the depth stencil view
		//	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		//	ZeroMemory(&descDSV, sizeof(descDSV));
		//	descDSV.Format = texturedesc.Format;
		//	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		//	descDSV.Texture2D.MipSlice = NULL;
		//	m_pDevice->CreateDepthStencilView(m_pTexture2D, &descDSV, &m_pDepthStencil);
		//}
		Resize();
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

	SwapChainInt();

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

	// For Blending
	D3D11_BLEND_DESC BlendState;
	ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
	BlendState.RenderTarget[0].BlendEnable = TRUE;
	BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	m_pDevice->CreateBlendState(&BlendState, &g_pBlendStateNoBlend);




	SetWall();

	// Setting Indexed Geometry For Scene 1
	SetCube();
	SetFloorAndGeometry();
	SetSkyBox(m_pDevice, L"files/SkyboxOcean.dds", SkyBoxTexture, SkyBoxVertexBuffer, SkyBoxIndexBuffer, SkyBoxConstantBuffer);
	
	// Setting Onject
	SetModel("Files/Crystal.obj", Pryamid, PryamidVertexBuffer, PryamidIndexBuffer, PryamidConstantBuffer, m_pDevice);

	// Loading Indexed Geometry Textures
	CreateDDSTextureFromFile(m_pDevice, L"files/Box_Circuit.dds", NULL, &m_pTexture);
	CreateDDSTextureFromFile(m_pDevice, L"files/bownCartoonGround_seamless.dds", NULL, &FloorTexture);
	CreateDDSTextureFromFile(m_pDevice, L"files/greendragon.dds", NULL, &GeometryTexture);

	// Loading Object Textures 
	CreateDDSTextureFromFile(m_pDevice, L"files/icium.dds", NULL, &PryamidTexture);


	// Setting Indexed Geometry for Scene 2
	SetSkyBox(DeviceTemp, L"files/NW_Entropic.dds", SkyBoxTexture2, SkyBoxVertexBuffer2, SkyBoxIndexBuffer2, SkyBoxConstantBuffer2);
	CreateDDSTextureFromFile(DeviceTemp, L"files/Box_Circuit.dds", NULL, &WallTexture1);
	CreateDDSTextureFromFile(DeviceTemp, L"files/Box_Ice.dds", NULL, &WallTexture2);
	CreateDDSTextureFromFile(DeviceTemp, L"files/Box_Purple2.dds", NULL, &WallTexture3);
	CreateDDSTextureFromFile(DeviceTemp, L"files/Box_Red2Dark.dds", NULL, &WallTexture4);


	// Setting Indexed Geometry for Scene 3
	SetSkyBox(Device3, L"files/Stars.dds", SkyBoxTexture3, SkyBoxVertexBuffer3, SkyBoxIndexBuffer3, SkyBoxConstantBuffer3);

	// Setting Objects that Orbit Planets
	SetModel("Files/Satellite.obj", Satellite, SatelliteVertexBuffer, SatelliteIndexBuffer, SatelliteConstantBuffer, Device3);
	CreateDDSTextureFromFile(Device3, L"files/Satellite.dds", NULL, &SatelliteTexture);
	
	SetModel("Files/Moon.obj", Moon, MoonVertexBuffer, MoonIndexBuffer, MoonConstantBuffer, Device3);
	CreateDDSTextureFromFile(Device3, L"files/Moon.dds", NULL, &MoonTexture);

	// Planets
	SetModel("Files/Earth.obj", Earth, EarthVertexBuffer, EarthIndexBuffer, EarthConstantBuffer, Device3);
	CreateDDSTextureFromFile(Device3, L"files/Earth.dds", NULL, &EarthTexture);

	SetModel("Files/Sun.obj", Sun, SunVertexBuffer, SunIndexBuffer, SunConstantBuffer, Device3);
	CreateDDSTextureFromFile(Device3, L"files/Sun.dds", NULL, &SunTexture);

	SetModel("Files/Moon.obj", Planet2, Planet2VertexBuffer, Planet2IndexBuffer, Planet2ConstantBuffer, Device3);
	CreateDDSTextureFromFile(Device3, L"files/iceplanet.dds", NULL, &Planet2Texture);

	// Setting Space Ship
	SetModel("Files/talon.obj", SpaceShip, SpaceShipVertexBuffer, SpaceShipIndexBuffer, SpaceShipConstantBuffer, Device3);
	CreateDDSTextureFromFile(Device3, L"files/talon.dds", NULL, &SpaceShipTexture);


	// Setting Indexed Geometry for Scene 4
	SetSkyBox(Device3, L"files/HW_Blue.dds", SkyBoxTexture4, SkyBoxVertexBuffer4, SkyBoxIndexBuffer4, SkyBoxConstantBuffer4);
	
	// Setting Sea Floor
	SetSeaFloor();
	CreateDDSTextureFromFile(Device3, L"files/Sea_Texture.dds", NULL, &SeaFloorTexture);

	// Setting Pirate Ships
	SetModel("Files/Ship1.obj", Ship1, Ship1VertexBuffer, Ship1IndexBuffer, Ship1ConstantBuffer, Device3);
	CreateDDSTextureFromFile(Device3, L"files/Ship1.dds", NULL, &Ship1Texture);

	SetModel("Files/Line.obj", Ship2, Ship2VertexBuffer, Ship2IndexBuffer, Ship2ConstantBuffer, Device3);
	CreateDDSTextureFromFile(Device3, L"files/Line.dds", NULL, &Ship2Texture);

	SetModel("Files/Line1.obj", Ship3, Ship3VertexBuffer, Ship3IndexBuffer, Ship3ConstantBuffer, Device3);
	CreateDDSTextureFromFile(Device3, L"files/Line.dds", NULL, &Ship3Texture);

	SetModel("Files/Line2.obj", Ship4, Ship4VertexBuffer, Ship4IndexBuffer, Ship4ConstantBuffer, Device3);
	CreateDDSTextureFromFile(Device3, L"files/Line.dds", NULL, &Ship4Texture);

	// Setting Indexed Geometry for Scene 5
	SetSkyBox(Device3, L"files/City/City.dds", SkyBoxTexture5, SkyBoxVertexBuffer5, SkyBoxIndexBuffer5, SkyBoxConstantBuffer5);
	CreateDDSTextureFromFile(Device3, L"files/City/CityFloor.dds", NULL, &CityFloorTexture);

	SetModel("Files/City/Car.obj", Car, CarVertexBuffer, CarIndexBuffer, CarConstantBuffer, Device3);
	CreateDDSTextureFromFile(Device3, L"files/City/Car.dds", NULL, &CarTexture);

	SetModel("Files/City/LightPoll.obj", LightPoll, LightPollVertexBuffer, LightPollIndexBuffer, LightPollConstantBuffer, Device3);
	CreateDDSTextureFromFile(Device3, L"files/City/LightPoll.dds", NULL, &LightPollTexture);

	SetModel("Files/City/Helicopter.obj", Heli, HeliVertexBuffer, HeliIndexBuffer, HeliConstantBuffer, Device3);
	CreateDDSTextureFromFile(Device3, L"files/City/Helicopter.dds", NULL, &HeliTexture);

	SetModel("Files/City/Building.obj", Building1, Building1VertexBuffer, Building1IndexBuffer, Building1ConstantBuffer, Device3);
	SetModel("Files/City/Building1.obj", Building2, Building2VertexBuffer, Building2IndexBuffer, Building2ConstantBuffer, Device3);
	SetModel("Files/City/Building2.obj", Building3, Building3VertexBuffer, Building3IndexBuffer, Building3ConstantBuffer, Device3);
	SetModel("Files/City/Building3.obj", Building4, Building4VertexBuffer, Building4IndexBuffer, Building4ConstantBuffer, Device3);
	SetModel("Files/City/Building4.obj", Building5, Building5VertexBuffer, Building5IndexBuffer, Building5ConstantBuffer, Device3);
	CreateDDSTextureFromFile(Device3, L"files/City/Building.dds", NULL, &BuildingTexture);



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
	Lights[2].Direction = XMFLOAT4(1.0f, -1.0f, -1.0f, 0.0f);
	Lights[2].Color		= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// Setting up the Light Buffer
	D3D11_BUFFER_DESC lightbuffdesc;
	ZeroMemory(&lightbuffdesc, sizeof(D3D11_BUFFER_DESC));
	lightbuffdesc.Usage		= D3D11_USAGE_DEFAULT;
	lightbuffdesc.BindFlags	= D3D11_BIND_CONSTANT_BUFFER;
	lightbuffdesc.ByteWidth	= sizeof(Lighting) * 3;
	m_pDevice->CreateBuffer(&lightbuffdesc, nullptr, &LightConstantBuffer);
	Device3->CreateBuffer(&lightbuffdesc, nullptr, &LightConstantBuffer3);
	Device3->CreateBuffer(&lightbuffdesc, nullptr, &LightConstantBuffer4);

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

	// Decleraing Shaders for Scene 3
	Device3->CreateVertexShader(Trivial_VS, sizeof(Trivial_VS), NULL, &VertexShader3);
	Device3->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), NULL, &PixelShader3);
	Device3->CreatePixelShader(SkyBox_PS, sizeof(SkyBox_PS), NULL, &SkyBoxPixelShader3);
	Device3->CreateVertexShader(SkyBox_VS, sizeof(SkyBox_VS), NULL, &SkyBoxVertexShader3);

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
	Device3->CreateInputLayout(layout, numberOfElements, Trivial_VS, sizeof(Trivial_VS), &Input3);

	// Initializing the world matrix
	WorldMatrix		= XMMatrixIdentity();
	// Scene 1
	CubeMatrix		= XMMatrixIdentity();
	
	FloorMatrix		= XMMatrixIdentity();
	FloorMatrix		= XMMatrixTranslation(0, -0.5f, 0);

	PryamidMatrix	= XMMatrixIdentity();
	PryamidMatrix	= XMMatrixTranslation(5, -0.5f, 15.0f);

	GeometryMatrix	= XMMatrixIdentity();
	SkyBoxMatrix	= XMMatrixIdentity();

	// Scene 2
	SkyBoxMatrix2	= XMMatrixIdentity();
	WallMatrix		= XMMatrixIdentity();

	// Scene 3
	EarthMatrix		= XMMatrixIdentity();
	EarthMatrix		= XMMatrixScaling(0.01f,0.01f,0.01f);

	SatelliteMatrix = XMMatrixIdentity();
	MoonMatrix		= XMMatrixIdentity();
	SpaceShipMatrix = XMMatrixIdentity();
	Planet2Matrix	= XMMatrixIdentity();

	SunMatrix		= XMMatrixIdentity();
	SunMatrix		= XMMatrixScaling(3.0f,3.0f,3.0f);

	SkyBoxMatrix3	= XMMatrixIdentity();

	// Scene 4
	SeaFloorMatrix	= XMMatrixIdentity();
	SeaFloorMatrix	= XMMatrixTranslation(0.0f, -0.5f, 0.0f);
	Ship1Matrix		= XMMatrixIdentity();

	Ship2Matrix		= XMMatrixIdentity();
	Ship2Matrix		= XMMatrixTranslation(25.0f, 10.3f, 40.0f) * XMMatrixRotationY(XM_PI / 3);

	Ship3Matrix		= XMMatrixIdentity();
	Ship3Matrix		= XMMatrixTranslation(0.0f, 1.82f, 0.0f) * XMMatrixRotationY(XM_PI);

	Ship4Matrix		= XMMatrixIdentity();
	Ship4Matrix		= XMMatrixTranslation(0.0f, 1.82f, 5.0f) * XMMatrixRotationY(XM_PI);

	// Scene 5
	CarMatrix		= XMMatrixIdentity();
	CarMatrix		= XMMatrixTranslation(-10.0f, -1.0f, 25.0f) * XMMatrixRotationY(XM_PI / 2);

	LightPollMatrix = XMMatrixIdentity();
	LightPollMatrix = XMMatrixTranslation(10.0f, -1.0f, -13.0f) * XMMatrixRotationY(XM_PI / -3);

	HeliMatrix		= XMMatrixIdentity();

	Building1Matrix = XMMatrixIdentity();
	Building1Matrix = XMMatrixTranslation(5.0f, -1.0f, 100.0f) * XMMatrixRotationY(XM_PI / 2);
	Building2Matrix = XMMatrixIdentity();
	Building2Matrix = XMMatrixTranslation(5.0f, -1.0f, 57.0f) * XMMatrixRotationY(XM_PI / 2);
	Building3Matrix = XMMatrixIdentity();
	Building3Matrix = XMMatrixTranslation(5.0f, -1.0f, 28.3f) * XMMatrixRotationY(XM_PI / 2);
	Building4Matrix = XMMatrixIdentity();
	Building4Matrix = XMMatrixTranslation(5.0f, -1.0f, -20.0f) * XMMatrixRotationY(XM_PI / 2);
	Building5Matrix = XMMatrixIdentity();
	Building5Matrix = XMMatrixTranslation(5.0f, -1.0f, -32.0f) * XMMatrixRotationY(XM_PI / 2);


	CameraView = XMMatrixLookAtLH(Eye, Focus, Up);
	CameraProjection = XMMatrixPerspectiveFovLH(XM_PI / Zoom, width / static_cast<float>(height), NearPlane, FarPlane);

	// Initializing the view matrix
	ViewMatrix = XMMatrixLookAtLH(Eye, Focus, Up);
	ViewMatrixSub = XMMatrixLookAtLH(Eye, Focus, Up);
	ViewMatrix2 = XMMatrixLookAtLH(XMVectorSet(0.0f, 1.5f, 5.0f, 0.0f), Focus, Up);
	ViewMatrix3 = XMMatrixLookAtLH(XMVectorSet(-5.0f, 1.5f, 0.0f, 0.0f), Focus, Up);
	ViewMatrix4 = XMMatrixLookAtLH(XMVectorSet(5.0f, 1.5f, 0.0f, 0.0f), Focus, Up);
	ViewMatrix2Sub = XMMatrixLookAtLH(XMVectorSet(0.0f, 1.5f, 5.0f, 0.0f), Focus, Up);
	ViewMatrix3Sub = XMMatrixLookAtLH(XMVectorSet(-5.0f, 1.5f, 0.0f, 0.0f), Focus, Up);
	ViewMatrix4Sub = XMMatrixLookAtLH(XMVectorSet(5.0f, 1.5f, 0.0f, 0.0f), Focus, Up);

	// Tests with Render to Texture
	ProjectionMatrixTemp = XMMatrixPerspectiveFovLH(XM_PI / Zoom, width / static_cast<float>(height), NearPlane, FarPlane);
	ViewMatrixTemp = XMMatrixLookAtLH(XMVectorSet(0.0f, 1.5, -5.0f, 0.0f), Focus, Up);

	// Initializing the projection matrix
	ProjectionMatrix = XMMatrixPerspectiveFovLH(XM_PI / Zoom, width / static_cast<float>(height), NearPlane, FarPlane);
	ProjectionMatrix2 = XMMatrixPerspectiveFovLH(XM_PI / Zoom, width / static_cast<float>(height), NearPlane, FarPlane);
	ProjectionMatrix3 = XMMatrixPerspectiveFovLH(XM_PI / Zoom, width / static_cast<float>(height), NearPlane, FarPlane);
	ProjectionMatrix4 = XMMatrixPerspectiveFovLH(XM_PI / Zoom, width / static_cast<float>(height), NearPlane, FarPlane);

	//ShowCursor(FALSE);
	return S_OK;
}

void SwapChainInt() {

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
	swapdesc.SampleDesc.Count = 8;
	swapdesc.SampleDesc.Quality = NULL;
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
	texturedesc.SampleDesc.Count = 8;
	texturedesc.Usage = D3D11_USAGE_DEFAULT;
	texturedesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	m_pDevice->CreateTexture2D(&texturedesc, NULL, &m_pTexture2D);

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = texturedesc.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	descDSV.Texture2D.MipSlice = NULL;
	m_pDevice->CreateDepthStencilView(m_pTexture2D, &descDSV, &m_pDepthStencil);


	// Initializing the Viewport
	m_ViewPort[0].Width = static_cast<float>(width);
	m_ViewPort[0].Height = static_cast<float>(height);
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
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	////// For Scene 3 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG, nullptr,
		NULL, D3D11_SDK_VERSION, &swapdesc, &Swap3, &Device3, &m_FeatureLevel, &DeviceContext3);

	// TODO: PART 1 STEP 4
	Swap3->GetBuffer(NULL, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&BackBuffer3));
	Device3->CreateRenderTargetView(BackBuffer3, NULL, &Render3);

	Device3->CreateTexture2D(&texturedesc, NULL, &Texture2D3);

	Device3->CreateDepthStencilView(Texture2D3, &descDSV, &DepthStencil3);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
	DeviceTemp->CreateSamplerState(&sampDesc, &SamplerStateTemp);
	Device3->CreateSamplerState(&sampDesc, &SamplerState3);

}

//----------------------------------------------------------------------------------------------------------
// Constant Frame Rendering
//----------------------------------------------------------------------------------------------------------
bool Run() {

	GetCursorPos(&cursorPos);
	float x = 0;
	x = cursorPos.x;
	float y = 0;
	y = cursorPos.y;

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

		//m_pDeviceContext->OMSetBlendState(g_pBlendStateNoBlend, blendFactor, sampleMask);
		
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
		CubeMatrix = XMMatrixMultiply(XMMatrixRotationY(t), XMMatrixTranslation(0, 0 + cubeverts, 0));


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
		DrawModel(Pryamid, m_pDeviceContext, PryamidVertexBuffer, PryamidIndexBuffer, PryamidConstantBuffer, PryamidTexture, m_pInput, m_pVertexShader, m_pPixelShader);

		//-- Render To Texture --------------------------------------------------------------------------------------------------------
		m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);
		ID3D11ShaderResourceView* nip = nullptr;
		m_pDeviceContext->PSSetShaderResources(NULL, 1, &nip);
		m_pDeviceContext->OMSetRenderTargets(1, &CameraRender, NULL);

		UpdateConstant(GeometryMatrix, ViewMatrix, ProjectionMatrix, GeometryConstantBuffer, m_pDeviceContext);
		UpdateConstant(CubeMatrix, ViewMatrixTemp, ProjectionMatrixTemp, m_pConstantBuffer, m_pDeviceContext);
		UpdateConstant(PryamidMatrix, ViewMatrixTemp, ProjectionMatrixTemp, PryamidConstantBuffer, m_pDeviceContext);
		UpdateConstant(FloorMatrix, ViewMatrixTemp, ProjectionMatrixTemp, FloorConstantBuffer, m_pDeviceContext);
		UpdateConstant(SkyBoxMatrix, ViewMatrixTemp, ProjectionMatrixTemp, SkyBoxConstantBuffer, m_pDeviceContext);

		DrawIndexedGeometry(m_pDeviceContext, SkyBoxTexture, SkyBoxVertexBuffer, SkyBoxIndexBuffer, SkyBoxConstantBuffer, m_pInput, m_pSkyBoxVertexShader, m_pSkyBoxPixelShader, 36);
		DrawIndexedGeometry(m_pDeviceContext, FloorTexture, FloorVertexBuffer, FloorIndexBuffer, FloorConstantBuffer, m_pInput, m_pVertexShader, m_pPixelShader, 6);
		DrawModel(Pryamid, m_pDeviceContext, PryamidVertexBuffer, PryamidIndexBuffer, PryamidConstantBuffer, PryamidTexture, m_pInput, m_pVertexShader, m_pPixelShader);
		DrawIndexedGeometry(m_pDeviceContext, m_pTexture, m_pVertexBuffer, m_pIndexBuffer, m_pConstantBuffer, m_pInput, m_pVertexShader, m_pPixelShader, 36);

		// Setting Target View
		m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencil);
		//----------------------------------------------------------------------------------------------------------------------------

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
	else if (SwapSceneInt == 2) {
		#pragma region Scene3
		// ViewMatrix/ViewPort Movement/Rotation, Zoom and Adjustable Near/Far-Plane
		CameraMovement(ViewMatrix, ViewMatrixSub, ProjectionMatrix);

		// Setting Target View
		DeviceContext3->OMSetRenderTargets(1, &Render3, DepthStencil3);

		// Setting Viewport
		DeviceContext3->RSSetViewports(1, &m_ViewPort[0]);

		// Clearing Back Buffer
		DeviceContext3->ClearRenderTargetView(Render3, Colors::Black);

		// Setting Sampler State
		DeviceContext3->PSSetSamplers(NULL, 1, &SamplerState3);

		// Setting Light Buffer
		DeviceContext3->PSSetConstantBuffers(NULL, 1, &LightConstantBuffer3);

		// Clearing Depth Buffer
		DeviceContext3->ClearDepthStencilView(DepthStencil3, D3D11_CLEAR_DEPTH, 1.0f, NULL);

		// Space Ship 
		SpaceShipMatrix = XMMatrixMultiply(XMMatrixTranslation(-3.5f, -3.5f, 0), SunMatrix);
		SpaceShipMatrix = XMMatrixMultiply(SpaceShipMatrix, XMMatrixScaling(10.0f, 10.0f, 10.0f));
		SpaceShipMatrix = XMMatrixMultiply(SpaceShipMatrix, XMMatrixRotationY(XM_PI));
		SpaceShipMatrix = XMMatrixMultiply(SpaceShipMatrix, XMMatrixRotationY(t));

		// Ice Planet
		Planet2Matrix = XMMatrixMultiply(XMMatrixTranslation(-150, 0, 0), SunMatrix);
		Planet2Matrix = XMMatrixMultiply(Planet2Matrix, XMMatrixScaling(0.25f, 0.25f, 0.25f));
		Planet2Matrix = XMMatrixMultiply(Planet2Matrix, XMMatrixRotationY(-t * 0.25f));
		Planet2Matrix = XMMatrixRotationY(-t * 0.25f) * Planet2Matrix;

		// Satellite 
		SatelliteMatrix = XMMatrixMultiply(XMMatrixTranslation(5000, 0, 5000), Planet2Matrix);
		SatelliteMatrix = XMMatrixMultiply(SatelliteMatrix, XMMatrixScaling(5, 5, 5));
		SatelliteMatrix = XMMatrixMultiply(SatelliteMatrix, XMMatrixRotationZ(15));
		SatelliteMatrix.r[3] = Planet2Matrix.r[3];
		SatelliteMatrix = XMMatrixMultiply(XMMatrixTranslation(2, 0, 2), SatelliteMatrix);
		SatelliteMatrix = XMMatrixRotationY(t) * SatelliteMatrix;
		SatelliteMatrix = XMMatrixRotationY(t * 2) * SatelliteMatrix;

		// Earth
		EarthMatrix = XMMatrixMultiply(XMMatrixTranslation(-3000, 0, -3000), SunMatrix);
		EarthMatrix = XMMatrixMultiply(EarthMatrix, XMMatrixScaling(0.005f, 0.005f, 0.005f));
		EarthMatrix = XMMatrixMultiply(EarthMatrix, XMMatrixRotationY(-t * 0.5f));
		EarthMatrix = XMMatrixRotationY(-t * 0.5f) * EarthMatrix;

		// Moon
		MoonMatrix = XMMatrixMultiply(XMMatrixTranslation(5000, 0, 5000), EarthMatrix);
		MoonMatrix = XMMatrixMultiply(MoonMatrix, XMMatrixScaling(15, 15, 15));
		MoonMatrix = XMMatrixMultiply(MoonMatrix, XMMatrixRotationZ(5));
		MoonMatrix.r[3] = EarthMatrix.r[3];
		MoonMatrix = XMMatrixMultiply(XMMatrixTranslation(-40, 0, -40), MoonMatrix);
		MoonMatrix = XMMatrixMultiply(XMMatrixRotationY(t), MoonMatrix);
		MoonMatrix = XMMatrixRotationY(t) * MoonMatrix;



		// Update variables
		UpdateConstant(SkyBoxMatrix3, ViewMatrix, ProjectionMatrix, SkyBoxConstantBuffer3, DeviceContext3);
		UpdateConstant(SatelliteMatrix, ViewMatrix, ProjectionMatrix, SatelliteConstantBuffer, DeviceContext3);
		UpdateConstant(EarthMatrix, ViewMatrix, ProjectionMatrix, EarthConstantBuffer, DeviceContext3);
		UpdateConstant(SunMatrix, ViewMatrix, ProjectionMatrix, SunConstantBuffer, DeviceContext3);
		UpdateConstant(MoonMatrix, ViewMatrix, ProjectionMatrix, MoonConstantBuffer, DeviceContext3);
		UpdateConstant(SpaceShipMatrix, ViewMatrix, ProjectionMatrix, SpaceShipConstantBuffer, DeviceContext3);
		UpdateConstant(Planet2Matrix, ViewMatrix, ProjectionMatrix, Planet2ConstantBuffer, DeviceContext3);

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
		DeviceContext3->UpdateSubresource(LightConstantBuffer3, NULL, NULL, &constantLight, NULL, NULL);

		// Drawing Objects
		DrawIndexedGeometry(DeviceContext3, SkyBoxTexture3, SkyBoxVertexBuffer3, SkyBoxIndexBuffer3, SkyBoxConstantBuffer3, Input3, SkyBoxVertexShader3, SkyBoxPixelShader3, 36);
		DrawModel(Sun, DeviceContext3, SunVertexBuffer, SunIndexBuffer, SunConstantBuffer, SunTexture, Input3, VertexShader3, PixelShader3);
		DrawModel(Earth, DeviceContext3, EarthVertexBuffer, EarthIndexBuffer, EarthConstantBuffer, EarthTexture, Input3, VertexShader3, PixelShader3);
		DrawModel(Satellite, DeviceContext3, SatelliteVertexBuffer, SatelliteIndexBuffer, SatelliteConstantBuffer, SatelliteTexture, Input3, VertexShader3, PixelShader3);
		DrawModel(Moon, DeviceContext3, MoonVertexBuffer, MoonIndexBuffer, MoonConstantBuffer, MoonTexture, Input3, VertexShader3, PixelShader3);
		DrawModel(SpaceShip, DeviceContext3, SpaceShipVertexBuffer, SpaceShipIndexBuffer, SpaceShipConstantBuffer, SpaceShipTexture, Input3, VertexShader3, PixelShader3);
		DrawModel(Planet2, DeviceContext3, Planet2VertexBuffer, Planet2IndexBuffer, Planet2ConstantBuffer, Planet2Texture, Input3, VertexShader3, PixelShader3);

		/* Presenting our back buffer to our front buffer */
		Swap3->Present(0, 0);
		#pragma endregion
	}
	else if (SwapSceneInt == 3) {
		#pragma region Scene4
		// ViewMatrix/ViewPort Movement/Rotation, Zoom and Adjustable Near/Far-Plane
		CameraMovement(ViewMatrix, ViewMatrixSub, ProjectionMatrix);

		// Setting Target View
		DeviceContext3->OMSetRenderTargets(1, &Render3, DepthStencil3);

		// Setting Viewport
		DeviceContext3->RSSetViewports(1, &m_ViewPort[0]);

		// Clearing Back Buffer
		DeviceContext3->ClearRenderTargetView(Render3, Colors::Black);

		// Setting Sampler State
		DeviceContext3->PSSetSamplers(NULL, 1, &SamplerState3);

		// Setting Light Buffer
		DeviceContext3->PSSetConstantBuffers(NULL, 1, &LightConstantBuffer3);

		// Clearing Depth Buffer
		DeviceContext3->ClearDepthStencilView(DepthStencil3, D3D11_CLEAR_DEPTH, 1.0f, NULL);

		// Update variables
		UpdateConstant(SkyBoxMatrix3, ViewMatrix, ProjectionMatrix, SkyBoxConstantBuffer4, DeviceContext3);
		UpdateConstant(SeaFloorMatrix, ViewMatrix, ProjectionMatrix, SeaFloorConstantBuffer, DeviceContext3);
		UpdateConstant(Ship1Matrix, ViewMatrix, ProjectionMatrix, Ship1ConstantBuffer, DeviceContext3);
		UpdateConstant(Ship2Matrix, ViewMatrix, ProjectionMatrix, Ship2ConstantBuffer, DeviceContext3);
		UpdateConstant(Ship3Matrix, ViewMatrix, ProjectionMatrix, Ship3ConstantBuffer, DeviceContext3);
		UpdateConstant(Ship4Matrix, ViewMatrix, ProjectionMatrix, Ship4ConstantBuffer, DeviceContext3);

		// Lighting
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
		DeviceContext3->UpdateSubresource(LightConstantBuffer3, NULL, NULL, &constantLight, NULL, NULL);


		// Drawing Objects
		DrawIndexedGeometry(DeviceContext3, SkyBoxTexture4, SkyBoxVertexBuffer4, SkyBoxIndexBuffer4, SkyBoxConstantBuffer4, Input3, SkyBoxVertexShader3, SkyBoxPixelShader3, 36);
		DrawIndexedGeometry(DeviceContext3, SeaFloorTexture, SeaFloorVertexBuffer, SeaFloorIndexBuffer, SeaFloorConstantBuffer, Input3, VertexShader3, PixelShader3, 6);
		DrawModel(Ship1, DeviceContext3, Ship1VertexBuffer, Ship1IndexBuffer, Ship1ConstantBuffer, Ship1Texture, Input3, VertexShader3, PixelShader3);
		DrawModel(Ship2, DeviceContext3, Ship2VertexBuffer, Ship2IndexBuffer, Ship2ConstantBuffer, Ship2Texture, Input3, VertexShader3, PixelShader3);
		DrawModel(Ship3, DeviceContext3, Ship3VertexBuffer, Ship3IndexBuffer, Ship3ConstantBuffer, Ship3Texture, Input3, VertexShader3, PixelShader3);
		DrawModel(Ship4, DeviceContext3, Ship4VertexBuffer, Ship4IndexBuffer, Ship4ConstantBuffer, Ship4Texture, Input3, VertexShader3, PixelShader3);


		/* Presenting our back buffer to our front buffer */
		Swap3->Present(0, 0);
		#pragma endregion
	}
	else if (SwapSceneInt == 4) {
		#pragma region Scene5
		// ViewMatrix/ViewPort Movement/Rotation, Zoom and Adjustable Near/Far-Plane
		CameraMovement(ViewMatrix, ViewMatrixSub, ProjectionMatrix);

		// Setting Target View
		DeviceContext3->OMSetRenderTargets(1, &Render3, DepthStencil3);

		// Setting Viewport
		DeviceContext3->RSSetViewports(1, &m_ViewPort[0]);

		// Clearing Back Buffer
		DeviceContext3->ClearRenderTargetView(Render3, Colors::Black);

		// Setting Sampler State
		DeviceContext3->PSSetSamplers(NULL, 1, &SamplerState3);

		// Setting Light Buffer
		DeviceContext3->PSSetConstantBuffers(NULL, 1, &LightConstantBuffer3);

		// Clearing Depth Buffer
		DeviceContext3->ClearDepthStencilView(DepthStencil3, D3D11_CLEAR_DEPTH, 1.0f, NULL);

		// Updating Helicopter Rotation
		HeliMatrix = XMMatrixTranslation(0.0f, 40.0f, 0.0f);
		HeliMatrix = HeliMatrix * XMMatrixRotationY(t);



		// Update variables
		UpdateConstant(SkyBoxMatrix3, ViewMatrix, ProjectionMatrix, SkyBoxConstantBuffer5, DeviceContext3);
		UpdateConstant(SeaFloorMatrix, ViewMatrix, ProjectionMatrix, SeaFloorConstantBuffer, DeviceContext3);

		UpdateConstant(CarMatrix, ViewMatrix, ProjectionMatrix, CarConstantBuffer, DeviceContext3);
		UpdateConstant(LightPollMatrix, ViewMatrix, ProjectionMatrix, LightPollConstantBuffer, DeviceContext3);
		UpdateConstant(HeliMatrix, ViewMatrix, ProjectionMatrix, HeliConstantBuffer, DeviceContext3);
		UpdateConstant(Building1Matrix, ViewMatrix, ProjectionMatrix, Building1ConstantBuffer, DeviceContext3);
		UpdateConstant(Building2Matrix, ViewMatrix, ProjectionMatrix, Building2ConstantBuffer, DeviceContext3);
		UpdateConstant(Building3Matrix, ViewMatrix, ProjectionMatrix, Building3ConstantBuffer, DeviceContext3);
		UpdateConstant(Building4Matrix, ViewMatrix, ProjectionMatrix, Building4ConstantBuffer, DeviceContext3);
		UpdateConstant(Building5Matrix, ViewMatrix, ProjectionMatrix, Building5ConstantBuffer, DeviceContext3);


		// Lighting
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
		DeviceContext3->UpdateSubresource(LightConstantBuffer3, NULL, NULL, &constantLight, NULL, NULL);


		// Drawing Objects
		DrawIndexedGeometry(DeviceContext3, SkyBoxTexture5, SkyBoxVertexBuffer5, SkyBoxIndexBuffer5, SkyBoxConstantBuffer5, Input3, SkyBoxVertexShader3, SkyBoxPixelShader3, 36);
		DrawIndexedGeometry(DeviceContext3, CityFloorTexture, SeaFloorVertexBuffer, SeaFloorIndexBuffer, SeaFloorConstantBuffer, Input3, VertexShader3, PixelShader3, 6);

		DrawModel(Car, DeviceContext3, CarVertexBuffer, CarIndexBuffer, CarConstantBuffer, CarTexture, Input3, VertexShader3, PixelShader3);
		DrawModel(LightPoll, DeviceContext3, LightPollVertexBuffer, LightPollIndexBuffer, LightPollConstantBuffer, LightPollTexture, Input3, VertexShader3, PixelShader3);
		DrawModel(Heli, DeviceContext3, HeliVertexBuffer, HeliIndexBuffer, HeliConstantBuffer, HeliTexture, Input3, VertexShader3, PixelShader3);
		DrawModel(Building1, DeviceContext3, Building1VertexBuffer, Building1IndexBuffer, Building1ConstantBuffer, BuildingTexture, Input3, VertexShader3, PixelShader3);
		DrawModel(Building2, DeviceContext3, Building2VertexBuffer, Building2IndexBuffer, Building2ConstantBuffer, BuildingTexture, Input3, VertexShader3, PixelShader3);
		DrawModel(Building3, DeviceContext3, Building3VertexBuffer, Building3IndexBuffer, Building3ConstantBuffer, BuildingTexture, Input3, VertexShader3, PixelShader3);
		DrawModel(Building4, DeviceContext3, Building4VertexBuffer, Building4IndexBuffer, Building4ConstantBuffer, BuildingTexture, Input3, VertexShader3, PixelShader3);
		DrawModel(Building5, DeviceContext3, Building5VertexBuffer, Building5IndexBuffer, Building5ConstantBuffer, BuildingTexture, Input3, VertexShader3, PixelShader3);



		/* Presenting our back buffer to our front buffer */
		Swap3->Present(0, 0);
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

	// Scene 3
	if (Render3) { Render3->Release(); }
	if (Swap3) { Swap3->Release(); }
	if (Device3) { Device3->Release(); }
	if (DeviceContext3) { DeviceContext3->Release(); }
	if (BackBuffer3) { BackBuffer3->Release(); }
	if (Input3) { Input3->Release(); }
	if (DepthStencil3) { DepthStencil3->Release(); }
	if (SamplerState3) { SamplerState3->Release(); }
	if (Texture2D3) { Texture2D3->Release(); }
	if (VertexShader3) { VertexShader3->Release(); }
	if (PixelShader3) { PixelShader3->Release(); }

	if (SkyBoxVertexShader3) { SkyBoxVertexShader3->Release(); }
	if (SkyBoxPixelShader3) { SkyBoxPixelShader3->Release(); }
	if (SkyBoxVertexBuffer3) { SkyBoxVertexBuffer3->Release(); }
	if (SkyBoxIndexBuffer3) { SkyBoxIndexBuffer3->Release(); }
	if (SkyBoxConstantBuffer3) { SkyBoxConstantBuffer3->Release(); }
	if (SkyBoxTexture3) { SkyBoxTexture3->Release(); }

	if (LightConstantBuffer3) { LightConstantBuffer3->Release(); }

	if (SatelliteVertexBuffer) { SatelliteVertexBuffer->Release(); }
	if (SatelliteIndexBuffer) { SatelliteIndexBuffer->Release(); }
	if (SatelliteConstantBuffer) { SatelliteConstantBuffer->Release(); }
	if (SatelliteTexture) { SatelliteTexture->Release(); }
	
	if (EarthVertexBuffer) { EarthVertexBuffer->Release(); }
	if (EarthIndexBuffer) { EarthIndexBuffer->Release(); }
	if (EarthConstantBuffer) { EarthConstantBuffer->Release(); }
	if (EarthTexture) { EarthTexture->Release(); }
	
	if (SunVertexBuffer) { SunVertexBuffer->Release(); }
	if (SunIndexBuffer) { SunIndexBuffer->Release(); }
	if (SunConstantBuffer) { SunConstantBuffer->Release(); }
	if (SunTexture) { SunTexture->Release(); }
	
	if (MoonVertexBuffer) { MoonVertexBuffer->Release(); }
	if (MoonIndexBuffer) { MoonIndexBuffer->Release(); }
	if (MoonConstantBuffer) { MoonConstantBuffer->Release(); }
	if (MoonTexture) { MoonTexture->Release(); }

	if (SpaceShipVertexBuffer) { SpaceShipVertexBuffer->Release(); }
	if (SpaceShipIndexBuffer) { SpaceShipIndexBuffer->Release(); }
	if (SpaceShipConstantBuffer) { SpaceShipConstantBuffer->Release(); }
	if (SpaceShipTexture) { SpaceShipTexture->Release(); }

	if (Planet2VertexBuffer) { Planet2VertexBuffer->Release(); }
	if (Planet2IndexBuffer) { Planet2IndexBuffer->Release(); }
	if (Planet2ConstantBuffer) { Planet2ConstantBuffer->Release(); }
	if (Planet2Texture) { Planet2Texture->Release(); }


	// Scene 4
	if (LightConstantBuffer4) { LightConstantBuffer4->Release(); }

	if (SkyBoxVertexBuffer4) { SkyBoxVertexBuffer4->Release(); }
	if (SkyBoxIndexBuffer4) { SkyBoxIndexBuffer4->Release(); }
	if (SkyBoxConstantBuffer4) { SkyBoxConstantBuffer4->Release(); }
	if (SkyBoxTexture4) { SkyBoxTexture4->Release(); }

	if (SeaFloorVertexBuffer) { SeaFloorVertexBuffer->Release(); }
	if (SeaFloorIndexBuffer) { SeaFloorIndexBuffer->Release(); }
	if (SeaFloorConstantBuffer) { SeaFloorConstantBuffer->Release(); }
	if (SeaFloorTexture) { SeaFloorTexture->Release(); }

	if (Ship1VertexBuffer) { Ship1VertexBuffer->Release(); }
	if (Ship1IndexBuffer) { Ship1IndexBuffer->Release(); }
	if (Ship1ConstantBuffer) { Ship1ConstantBuffer->Release(); }
	if (Ship1Texture) { Ship1Texture->Release(); }
	
	if (Ship2VertexBuffer) { Ship2VertexBuffer->Release(); }
	if (Ship2IndexBuffer) { Ship2IndexBuffer->Release(); }
	if (Ship2ConstantBuffer) { Ship2ConstantBuffer->Release(); }
	if (Ship2Texture) { Ship2Texture->Release(); }
	
	if (Ship3VertexBuffer) { Ship3VertexBuffer->Release(); }
	if (Ship3IndexBuffer) { Ship3IndexBuffer->Release(); }
	if (Ship3ConstantBuffer) { Ship3ConstantBuffer->Release(); }
	if (Ship3Texture) { Ship3Texture->Release(); }
	
	if (Ship4VertexBuffer) { Ship4VertexBuffer->Release(); }
	if (Ship4IndexBuffer) { Ship4IndexBuffer->Release(); }
	if (Ship4ConstantBuffer) { Ship4ConstantBuffer->Release(); }
	if (Ship4Texture) { Ship4Texture->Release(); }

	// Scene 5
	if (SkyBoxVertexBuffer5) { SkyBoxVertexBuffer5->Release(); }
	if (SkyBoxIndexBuffer5) { SkyBoxIndexBuffer5->Release(); }
	if (SkyBoxConstantBuffer5) { SkyBoxConstantBuffer5->Release(); }
	if (SkyBoxTexture5) { SkyBoxTexture5->Release(); }

	if (CityFloorTexture) { CityFloorTexture->Release(); }

	if (CarVertexBuffer) { CarVertexBuffer->Release(); }
	if (CarIndexBuffer) { CarIndexBuffer->Release(); }
	if (CarConstantBuffer) { CarConstantBuffer->Release(); }
	if (CarTexture) { CarTexture->Release(); }
	
	if (LightPollVertexBuffer) { LightPollVertexBuffer->Release(); }
	if (LightPollIndexBuffer) { LightPollIndexBuffer->Release(); }
	if (LightPollConstantBuffer) { LightPollConstantBuffer->Release(); }
	if (LightPollTexture) { LightPollTexture->Release(); }
	
	if (HeliVertexBuffer) { HeliVertexBuffer->Release(); }
	if (HeliIndexBuffer) { HeliIndexBuffer->Release(); }
	if (HeliConstantBuffer) { HeliConstantBuffer->Release(); }
	if (HeliTexture) { HeliTexture->Release(); }
	
	if (Building1VertexBuffer) { Building1VertexBuffer->Release(); }
	if (Building1IndexBuffer) { Building1IndexBuffer->Release(); }
	if (Building1ConstantBuffer) { Building1ConstantBuffer->Release(); }
	if (BuildingTexture) { BuildingTexture->Release(); }
	
	if (Building2VertexBuffer) { Building2VertexBuffer->Release(); }
	if (Building2IndexBuffer) { Building2IndexBuffer->Release(); }
	if (Building2ConstantBuffer) { Building2ConstantBuffer->Release(); }
	
	if (Building3VertexBuffer) { Building3VertexBuffer->Release(); }
	if (Building3IndexBuffer) { Building3IndexBuffer->Release(); }
	if (Building3ConstantBuffer) { Building3ConstantBuffer->Release(); }
	
	if (Building4VertexBuffer) { Building4VertexBuffer->Release(); }
	if (Building4IndexBuffer) { Building4IndexBuffer->Release(); }
	if (Building4ConstantBuffer) { Building4ConstantBuffer->Release(); }
	
	if (Building5VertexBuffer) { Building5VertexBuffer->Release(); }
	if (Building5IndexBuffer) { Building5IndexBuffer->Release(); }
	if (Building5ConstantBuffer) { Building5ConstantBuffer->Release(); }


	if (CameraResource) { CameraResource->Release(); }
}


//----------------------------------------------------------------------------------------------------------
// Functions: Creating/Initializing Stuff Needed by DirectX
//----------------------------------------------------------------------------------------------------------
void SetCube() {
	// Creating Cube Vertex

	if (cubeverts < 3) {
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
	else if (cubeverts >= 3) {
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
		{ XMFLOAT4(-500.0f, -500.0f, -500.0f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(500.0f, -500.0f, -500.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 0.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(500.0f, -500.0f, 500.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 0.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(-500.0f, -500.0f, 500.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 0.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },

		{ XMFLOAT4(-500.0f, 500.0f, -500.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 0.0f),	XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(500.0f, 500.0f, -500.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(500.0f, 500.0f, 500.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 0.0f),	XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(-500.0f, 500.0f, 500.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 0.0f),	XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f) },

		{ XMFLOAT4(500.0f, -500.0f, 500.0f, 1.0f),		XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 0.0f),	XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(500.0f, -500.0f, -500.0f, 1.0f),		XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 0.0f),	XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(500.0f, 500.0f, -500.0f, 1.0f),		XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(500.0f, 500.0f, 500.0f, 1.0f),		XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 0.0f),	XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f) },

		{ XMFLOAT4(-500.0f, -500.0f, 500.0f, 1.0f),		XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 0.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(-500.0f, -500.0f, -500.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 0.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(-500.0f, 500.0f, -500.0f, 1.0f),		XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 0.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(-500.0f, 500.0f, 500.0f, 1.0f),		XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) },
////////////////
		{ XMFLOAT4(-500.0f, -500.0f, 500.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 0.0f),	XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },
		{ XMFLOAT4(500.0f, -500.0f, 500.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 0.0f),	XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },
		{ XMFLOAT4(500.0f, 500.0f, 500.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },
		{ XMFLOAT4(-500.0f, 500.0f, 500.0f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 0.0f),	XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f) },
////////////////
		{ XMFLOAT4(-500.0f, -500.0f, -500.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 0.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f) },
		{ XMFLOAT4(500.0f, -500.0f, -500.0f, 1.0f),		XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 0.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f) },
		{ XMFLOAT4(500.0f, 500.0f, -500.0f, 1.0f),		XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 0.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f) },
		{ XMFLOAT4(-500.0f, 500.0f, -500.0f, 1.0f),		XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f) },
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

void SetSeaFloor()
{
	// Creating Sea Floor Vertex
	SIMPLE_VERTEX Vertex[] = {
		{ XMFLOAT4(-250.0f, -0.55f, -250.0f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 25.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(250.0f, -0.55f, -250.0f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 25.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(250.0f, -0.55f,  250.0f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(1.0f, 0.0f, 25.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
		{ XMFLOAT4(-250.0f, -0.55f,  250.0f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 25.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) },
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
	Device3->CreateBuffer(&buffdesc, &data, &SeaFloorVertexBuffer);

	// Creating Sea Floor Index
	DWORD32 Indexes[] = {
		3,1,0,
		2,1,3,
	};

	// Initializing/Creating Index Buffer
	buffdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffdesc.ByteWidth = sizeof(DWORD32) * 6;
	data.pSysMem = Indexes;
	Device3->CreateBuffer(&buffdesc, &data, &SeaFloorIndexBuffer);

	// Initializing/Creating Constant Buffer
	buffdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffdesc.ByteWidth = sizeof(ConstantMatrix);
	Device3->CreateBuffer(&buffdesc, nullptr, &SeaFloorConstantBuffer);
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

void DrawModel(ObjLoader & model, ID3D11DeviceContext * devicecontext, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, ID3D11Buffer* constantBuffer, ID3D11ShaderResourceView* texture, ID3D11InputLayout* input, ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader)
{
	unsigned int	strides = sizeof(SIMPLE_VERTEX);
	unsigned int	offsets = 0;
	// Setting VertexBuffer
	devicecontext->IASetVertexBuffers(NULL, 1, &vertexBuffer, &strides, &offsets);
	// Setting Input Layout
	devicecontext->IASetInputLayout(input);
	// Setting Index Buffer
	devicecontext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, NULL);
	// Setting Topology
	devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	/* Setting Vertex Shader */
	devicecontext->VSSetShader(vertexShader, nullptr, NULL);
	// Setting Constant Buffer
	devicecontext->VSSetConstantBuffers(NULL, 1, &constantBuffer);
	/* Setting Pixel Shader */
	devicecontext->PSSetShader(pixelShader, nullptr, NULL);
	// Setting Texture Resource
	devicecontext->PSSetShaderResources(NULL, 1, &texture);

	// Drawing Indexed Model
	devicecontext->DrawIndexed(model.GetIndex().size(), 0, 0);
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
	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT width = rc.right;
	UINT height = rc.bottom;

	// Reset Camera
	if (GetAsyncKeyState('R')) { viewMatrix = viewMatrixSub; }

	// ViewPort/Camera Fly Forward
	if (GetAsyncKeyState('Q')) { viewMatrix = XMMatrixMultiply(viewMatrix, XMMatrixTranslation(0, 0, -0.05f)); }
	// ViewPort/Camera Fly Backward
	if (GetAsyncKeyState('E')) { viewMatrix = XMMatrixMultiply(viewMatrix, XMMatrixTranslation(0, 0, 0.05f)); }

	// ViewPort/Camera movement Up
	if (GetAsyncKeyState('W')) { viewMatrix = XMMatrixMultiply(viewMatrix, XMMatrixTranslation(0, -0.05f, 0)); }
	// ViewPort/Camera movement Down
	if (GetAsyncKeyState('S')) { viewMatrix = XMMatrixMultiply(viewMatrix, XMMatrixTranslation(0, 0.05f, 0)); }
	// ViewPort/Camera movement Right
	if (GetAsyncKeyState('D')) { viewMatrix = XMMatrixMultiply(viewMatrix, XMMatrixTranslation(-0.05f, 0, 0)); }
	// ViewPort/Camera movement Left
	if (GetAsyncKeyState('A')) { viewMatrix = XMMatrixMultiply(viewMatrix, XMMatrixTranslation(0.05f, 0, 0)); }

	// ViewPort/Camera rotate Up
	if (GetAsyncKeyState('I')) { viewMatrix = XMMatrixMultiply(viewMatrix, XMMatrixRotationX(0.0035)); }
	// ViewPort/Camera rotate Down
	if (GetAsyncKeyState('K')) { viewMatrix = XMMatrixMultiply(viewMatrix, XMMatrixRotationX(-0.0035)); }
	// ViewPort/Camera rotate Left
	if (GetAsyncKeyState('J')) { viewMatrix = XMMatrixMultiply(viewMatrix, XMMatrixRotationY(0.0035)); }
	// ViewPort/Camera rotate Right
	if (GetAsyncKeyState('L')) { viewMatrix = XMMatrixMultiply(viewMatrix, XMMatrixRotationY(-0.0035)); }

	// Adjust Near Plane
	if (GetAsyncKeyState(VK_NUMPAD1) & 0x1) { NearPlane += 0.1f; }
	if (GetAsyncKeyState(VK_NUMPAD2) & 0x1) { if (NearPlane > 0.1) { NearPlane -= 0.1f; } }

	// Adjust Fare Plane
	if (GetAsyncKeyState(VK_NUMPAD4) & 0x1) { if (FarPlane > 1) { FarPlane -= 1.0f; } }
	if (GetAsyncKeyState(VK_NUMPAD5) & 0x1) { FarPlane += 1.0f; }

	// ViewPort/Camera Zoom In
	if (GetAsyncKeyState(VK_NUMPAD7) & 0x1) { Zoom += 0.05; }
	// ViewPort/Camera Zoom Out
	if (GetAsyncKeyState(VK_NUMPAD8) & 0x1) { if (Zoom > 2.1) { Zoom -= 0.05; } }

	// Initializing the projection matrix
	projectionMatrix = XMMatrixPerspectiveFovLH(XM_PI / Zoom, width / static_cast<float>(height), NearPlane, FarPlane);
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
		// Split-4 Screen
		if (SwapSceneInt == 1) {
			#pragma region 1
			// Resetting Camera
			SwapCameraInt = 0;

			// Setting View Matrixes Used
			ViewMatrix = XMMatrixMultiply(XMMatrixLookAtLH(XMVectorSet(0.0f, 1.5f, -25.0f, 0.0f), Focus, Up), XMMatrixTranslation(0, 0, 0));
			ViewMatrixSub = XMMatrixMultiply(XMMatrixLookAtLH(XMVectorSet(0.0f, 1.5f, -25.0f, 0.0f), Focus, Up), XMMatrixTranslation(0, 0, 0));
			ViewMatrix2 = XMMatrixMultiply(XMMatrixLookAtLH(XMVectorSet(0.0f, 1.5f, 25.0f, 0.0f), Focus, Up), XMMatrixTranslation(0, 0, 0));
			ViewMatrix2Sub = XMMatrixMultiply(XMMatrixLookAtLH(XMVectorSet(0.0f, 1.5f, 25.0f, 0.0f), Focus, Up), XMMatrixTranslation(0, 0, 0));
			ViewMatrix3 = XMMatrixMultiply(XMMatrixLookAtLH(XMVectorSet(-25.0f, 1.5f, 0.0f, 0.0f), Focus, Up), XMMatrixTranslation(0, 0, 0));
			ViewMatrix3Sub = XMMatrixMultiply(XMMatrixLookAtLH(XMVectorSet(-25.0f, 1.5f, 0.0f, 0.0f), Focus, Up), XMMatrixTranslation(0, 0, 0));
			ViewMatrix4 = XMMatrixMultiply(XMMatrixLookAtLH(XMVectorSet(25.0f, 1.5f, 0.0f, 0.0f), Focus, Up), XMMatrixTranslation(0, 0, 0));
			ViewMatrix4Sub = XMMatrixMultiply(XMMatrixLookAtLH(XMVectorSet(25.0f, 1.5f, 0.0f, 0.0f), Focus, Up), XMMatrixTranslation(0, 0, 0));

			// Initializing the Top Left Viewport
			m_ViewPort[0].Width = static_cast<float>(width * 0.5);
			m_ViewPort[0].Height = static_cast<float>(height * 0.5);
			m_ViewPort[0].MinDepth = 0.0f;
			m_ViewPort[0].MaxDepth = 1.0f;
			m_ViewPort[0].TopLeftX = 0;
			m_ViewPort[0].TopLeftY = 0;

			// Initializing the Top Right Viewport
			m_ViewPort[1].Width = static_cast<float>(width * 0.5f);
			m_ViewPort[1].Height = static_cast<float>(height * 0.5f);
			m_ViewPort[1].MinDepth = 0.0f;
			m_ViewPort[1].MaxDepth = 1.0f;
			m_ViewPort[1].TopLeftX = static_cast<float>(width * 0.5f);
			m_ViewPort[1].TopLeftY = 0;

			// Initializing the Bottom Left Viewport
			m_ViewPort[2].Width = static_cast<float>(width * 0.5f);
			m_ViewPort[2].Height = static_cast<float>(height * 0.5f);
			m_ViewPort[2].MinDepth = 0.0f;
			m_ViewPort[2].MaxDepth = 1.0f;
			m_ViewPort[2].TopLeftX = 0;
			m_ViewPort[2].TopLeftY = static_cast<float>(height * 0.5f);

			// Initializing the Bottom Right Viewport
			m_ViewPort[3].Width = static_cast<float>(width * 0.5f);
			m_ViewPort[3].Height = static_cast<float>(height * 0.5f);
			m_ViewPort[3].MinDepth = 0.0f;
			m_ViewPort[3].MaxDepth = 1.0f;
			m_ViewPort[3].TopLeftX = static_cast<float>(width * 0.5f);
			m_ViewPort[3].TopLeftY = static_cast<float>(height * 0.5f);
			#pragma endregion
		}
		// Space Theme
		if (SwapSceneInt == 2) {
			#pragma region 2
			// Resetting Camera
			SwapCameraInt = 0;

			// Setting View Matrixes
			ViewMatrix = XMMatrixLookAtLH(XMVectorSet(0.0f, 25.0f, -100.0f, 0.0f), Focus, Up);
			ViewMatrixSub = XMMatrixLookAtLH(XMVectorSet(0.0f, 25.0f, -100.0f, 0.0f), Focus, Up);

			// Initializing the Viewport
			m_ViewPort[0].Width = static_cast<float>(width);
			m_ViewPort[0].Height = static_cast<float>(height);
			m_ViewPort[0].MinDepth = 0.0f;
			m_ViewPort[0].MaxDepth = 1.0f;
			m_ViewPort[0].TopLeftX = 0;
			m_ViewPort[0].TopLeftY = 0;

			// Setting Point Light
			Lights[1].Position = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
			Lights[1].Color = XMFLOAT4(255.0f, 255.0f, 255.0f, 1.0f);
			Lights[1].Radius = XMFLOAT4(500.0f, 500.0f, 500.0f, 1.0f);

			// Setting Directional Lighting
			Lights[2].Direction = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
			Lights[2].Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			Resize();
			#pragma endregion
		}
		// Pirate Theme
		if (SwapSceneInt == 3) {
			#pragma region 3
			// Resetting Camera
			SwapCameraInt = 0;

			// Setting View Matrixies
			ViewMatrix = XMMatrixLookAtLH(XMVectorSet(-5.0f, 25.0f, -50.0f, 0.0f), Focus, Up);
			ViewMatrixSub = XMMatrixLookAtLH(XMVectorSet(-5.0f, 25.0f, -50.0f, 0.0f), Focus, Up);

			// Initializing the Viewport
			m_ViewPort[0].Width = static_cast<float>(width);
			m_ViewPort[0].Height = static_cast<float>(height);
			m_ViewPort[0].MinDepth = 0.0f;
			m_ViewPort[0].MaxDepth = 1.0f;
			m_ViewPort[0].TopLeftX = 0;
			m_ViewPort[0].TopLeftY = 0;
			Resize();

			// Resetting Spot Light
			Lights[0].Direction = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
			Lights[0].Color = XMFLOAT4(0.0f, 255.0f, 0.0f, 1.0f);
			Lights[0].Position = XMFLOAT4(0.0f, 7.0f, 0.0f, 0.0f);
			Lights[0].Radius = XMFLOAT4(0.923f, 0.707f, 10.0f, 0.0f);

			// Resetting Point Light
			Lights[1].Position = XMFLOAT4(6.0f, 1.0f, 0.0f, 0.0f);
			Lights[1].Color = XMFLOAT4(255.0f, 0.0f, 255.0f, 1.0f);
			Lights[1].Radius = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

			// Resetting Directional Lighting
			Lights[2].Direction = XMFLOAT4(1.0f, -1.0f, -1.0f, 0.0f);
			Lights[2].Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			#pragma endregion
		}
		// City Theme
		if (SwapSceneInt == 4) {
			#pragma region 4

			// Resetting Scene Managment 
			SwapCameraInt = 0;

			// Setting View Matrixies
			ViewMatrix = XMMatrixLookAtLH(XMVectorSet(0.0f, 25.0f, 50.0f, 0.0f), Focus, Up);
			ViewMatrixSub = XMMatrixLookAtLH(XMVectorSet(0.0f, 25.0f, 50.0f, 0.0f), Focus, Up);

			// Initializing the Viewport
			m_ViewPort[0].Width = static_cast<float>(width);
			m_ViewPort[0].Height = static_cast<float>(height);
			m_ViewPort[0].MinDepth = 0.0f;
			m_ViewPort[0].MaxDepth = 1.0f;
			m_ViewPort[0].TopLeftX = 0;
			m_ViewPort[0].TopLeftY = 0;

			
			// Resetting Spot Light
			Lights[0].Direction = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
			Lights[0].Color = XMFLOAT4(0.0f, 255.0f, 255.0f, 1.0f);
			Lights[0].Position = XMFLOAT4(20.0f, 10.0f, 9.0f, 0.0f);
			Lights[0].Radius = XMFLOAT4(0.923f, 0.707f, 10.0f, 0.0f);

			// Resetting Point Light
			Lights[1].Position = XMFLOAT4(25.0f, 1.0f, 9.5f, 0.0f);
			Lights[1].Color = XMFLOAT4(255.0f, 155.0f, 155.0f, 1.0f);
			Lights[1].Radius = XMFLOAT4(7.0f, 0.0f, 0.0f, 1.0f);

			// Resetting Directional Lighting
			Lights[2].Direction = XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f);
			Lights[2].Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			Resize();
			#pragma endregion
		}
		// Back To First Scene
		if (SwapSceneInt == 5) {
			#pragma region 5
			// Setting View Matrixies
			ViewMatrix = XMMatrixLookAtLH(Eye, Focus, Up);
			ViewMatrixSub = XMMatrixLookAtLH(Eye, Focus, Up);

			// Initializing the Viewport
			m_ViewPort[0].Width = static_cast<float>(width);
			m_ViewPort[0].Height = static_cast<float>(height);
			m_ViewPort[0].MinDepth = 0.0f;
			m_ViewPort[0].MaxDepth = 1.0f;
			m_ViewPort[0].TopLeftX = 0;
			m_ViewPort[0].TopLeftY = 0;

			
			// Resetting Spot Light
			Lights[0].Direction = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
			Lights[0].Color = XMFLOAT4(0.0f, 255.0f, 0.0f, 1.0f);
			Lights[0].Position = XMFLOAT4(0.0f, 7.0f, 0.0f, 0.0f);
			Lights[0].Radius = XMFLOAT4(0.923f, 0.707f, 10.0f, 0.0f);

			// Resetting Point Light
			Lights[1].Position = XMFLOAT4(6.0f, 1.0f, 0.0f, 0.0f);
			Lights[1].Color = XMFLOAT4(255.0f, 0.0f, 255.0f, 1.0f);
			Lights[1].Radius = XMFLOAT4(4.0f, 0.0f, 0.0f, 1.0f);

			// Resetting Directional Lighting
			Lights[2].Direction = XMFLOAT4(1.0f, -1.0f, -1.0f, 0.0f);
			Lights[2].Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			// Resetting Camera
			SwapSceneInt = 0;

			// Resetting Scene Managment 
			SwapCameraInt = 0;

			Resize();
			#pragma endregion
		}

	}

}

void Resize() {

	// Getting The Current Width and Height of the Window
	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT width = rc.right;
	UINT height = rc.bottom;

	// Tests with Render to Texture
	ProjectionMatrixTemp = XMMatrixPerspectiveFovLH(XM_PI / Zoom, width / static_cast<float>(height), NearPlane, FarPlane);

	// Initializing the projection matrix
	ProjectionMatrix = XMMatrixPerspectiveFovLH(XM_PI / Zoom, width / static_cast<float>(height), NearPlane, FarPlane);
	ProjectionMatrix2 = XMMatrixPerspectiveFovLH(XM_PI / Zoom, width / static_cast<float>(height), NearPlane, FarPlane);
	ProjectionMatrix3 = XMMatrixPerspectiveFovLH(XM_PI / Zoom, width / static_cast<float>(height), NearPlane, FarPlane);
	ProjectionMatrix4 = XMMatrixPerspectiveFovLH(XM_PI / Zoom, width / static_cast<float>(height), NearPlane, FarPlane);

}

