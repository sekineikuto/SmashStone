//==================================================================================================================
//
// レンダラー[renderer.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#include "renderer.h"
#include "scene.h"
#include "debugProc.h"
#include "camera.h"
#include "light.h"
#include "fade.h"
#include "game.h"
#include "title.h"
#include "tutorial.h"
#include "result.h"
#include "sound.h"
#include "kananlibrary.h"
#include "modelCharacter.h"
#include "motion.h"
#include "charaParam.h"
#include "inputKeyboard.h"
#include "mapSelect.h"
#include "ImGui/imgui.h"			// Imguiの実装に必要
#include "ImGui/imgui_impl_dx9.h"	// Imguiの実装に必要
#include "ImGui/imgui_impl_win32.h"	// Imguiの実装に必要

//==================================================================================================================
// マクロ定義
//==================================================================================================================
#define NAME_CAPTION_IMGUI	("Debug")	// ImGuiキャプション名

//==================================================================================================================
// 静的メンバ変数の初期化
//==================================================================================================================
CFade *CRenderer::m_pFade = NULL;						// Fadeの情報
CDebugProc *CRenderer::m_pDebugProc = NULL;				// デバッグ表示情報
CCamera *CRenderer::m_pCamera = NULL;					// カメラの情報
CLight *CRenderer::m_pLight = NULL;						// ライトの情報
CGame *CRenderer::m_pGame = NULL;						// ゲーム情報
CTitle *CRenderer::m_pTitle = NULL;						// タイトル情報
CResult *CRenderer::m_pResult = NULL;					// リザルト情報
CTutorial *CRenderer::m_pTutorial = NULL;				// チュートリアル情報
CSound *CRenderer::m_pSound = NULL;						// 音情報
CMapSelect *CRenderer::m_pMapSelect = NULL;				// マップ選択画面の情報
CRenderer::MODE CRenderer::m_mode = MODE_GAME;			// 最初の画面

//==================================================================================================================
// コンストラクタ
//==================================================================================================================
CRenderer::CRenderer()
{

}

//==================================================================================================================
// デストラクタ
//==================================================================================================================
CRenderer::~CRenderer()
{

}

//==================================================================================================================
// 初期化処理
//==================================================================================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	// プレゼンテーションパラメータ

	// Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ワークをゼロクリア
	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;						// バックバッファの形式
	d3dpp.BackBufferCount = 1;									// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファ(Ｚバッファ)とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;									// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート(現在の速度に合わせる)
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル(VSyncを待って描画)
	//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// クライアント領域を直ちに更新する

	// Direct3Dデバイスの生成
	// [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行なう
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,			// ディスプレイアダプタ
		D3DDEVTYPE_HAL,											// デバイスタイプ
		hWnd,													// フォーカスするウインドウへのハンドル
		D3DCREATE_HARDWARE_VERTEXPROCESSING,					// デバイス作成制御の組み合わせ
		&d3dpp,													// デバイスのプレゼンテーションパラメータ
		&m_pD3DDevice)))										// デバイスインターフェースへのポインタ
	{
		// 上記の設定が失敗したら
		// [デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行なう
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// [デバイス作成制御]<描画>と<頂点処理>をCPUで行なう
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}
#ifdef _DEBUG
	//デバッグロゴの初期化処理
	m_pDebugProc->Init();
#endif

	// フェードのロード
	CFade::Load();

	// フェード生成処理
	m_pFade = CFade::Create(m_mode);

	// 動的確保
	m_pSound = new CSound;

	// 音初期化
	m_pSound->Init(hWnd);

	// レンダーステートパラメータの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面(左回り)をカリングする
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファ(カメラとオブジェクトの距離を記憶)を使用
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ライティングモード有効
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αディスティネーションカラーの指定

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

#ifdef _DEBUG
	// ImGuiの初期化
	InitImGui(d3dpp, hWnd);
#endif

	// ロード
	CCharaParam::Load();
	CModelCharacter::Load();
	CMotion::Load();

	switch (m_mode)
	{
		// タイトルのとき
	case MODE_TITLE:
		// タイトルの生成
		m_pTitle = CTitle::Create();
		// BGM再生
		//m_pSound->PlaySound(CSound::SOUND_LABEL_BGMTITLE);
		break;

		// チュートリアルのとき
	case MODE_TUTORIAL:
		// チュートリアルの生成
		m_pTutorial = CTutorial::Create();
		// BGM再生
		//m_pSound->PlaySound(CSound::SOUND_LABEL_BGMTUTORIAL);
		break;

		// マップ選択画面のとき
	case MODE_MAPSELECT:
		// マップ選択画面の生成
		m_pMapSelect = CMapSelect::Create();
		// BGM再生
		//m_pSound->PlaySound(CSound::SOUND_LABEL_BGMTUTORIAL);
		break;

		// ゲームのとき
	case MODE_GAME:
		// ゲームの生成
		m_pGame = CGame::Create();
		// BGM再生
		//m_pSound->PlaySound(CSound::SOUND_LABEL_BGMGAME);
		break;

		// ランキングのとき
	case MODE_RESULT:
		// ランキングの生成
		m_pResult = CResult::Create();
		break;
	}

	// 値を返す
	return S_OK;
}

//==================================================================================================================
// 終了処理
//==================================================================================================================
void CRenderer::Uninit(void)
{
#ifdef _DEBUG
	// デバッグロゴの終了処理
	m_pDebugProc->Uninit();
#endif

	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();						// 開放
		m_pD3DDevice = NULL;							// NULLにする
	}
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();								// 開放
		m_pD3D = NULL;									// NULLにする
	}

	// フェードがあるとき
	if (m_pFade != NULL)
	{
		// 終了処理
		m_pFade->Uninit();

		// メモリ削除
		delete m_pFade;

		// ポインタNULL
		m_pFade = nullptr;
	}

	// 音があるとき
	if (m_pSound != NULL)
	{
		// 音の終了処理
		m_pSound->Uninit();

		// メモリ削除
		delete m_pSound;

		// ポインタNULL
		m_pSound = nullptr;
	}

	CModelCharacter::Unload();
	CMotion::UnLoad();

	switch (m_mode)
	{
		// タイトルのとき
	case MODE_TITLE:
		// タイトルの終了処理
		m_pTitle->Uninit();
		delete m_pTitle;		// メモリの削除
		m_pTitle = nullptr;		// ポインタNULL
		break;

		// チュートリアルのとき
	case MODE_TUTORIAL:
		// チュートリアルの終了処理
		m_pTutorial->Uninit();
		delete m_pTutorial;		// メモリの削除
		m_pTutorial = nullptr;	// ポインタNULL
		break;

		// マップ選択画面のとき
	case MODE_MAPSELECT:
		// マップ選択画面の終了処理
		m_pMapSelect->Uninit();
		delete m_pMapSelect;	// メモリの削除
		m_pMapSelect = nullptr;	// ポインタNULL
		break;

		// ゲームのとき
	case MODE_GAME:
		// ゲームの終了処理
		m_pGame->Uninit();
		delete m_pGame;			// メモリの削除
		m_pGame = nullptr;		// ポインタNULL
		break;

		// リザルト
	case MODE_RESULT:
		// 終了処理
		m_pResult->Uninit();
		delete m_pResult;		// メモリの削除
		m_pResult = nullptr;	// ポインタNULL
		break;
	}

#ifdef _DEBUG
	// Imguiの終了
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif
}

//==================================================================================================================
// 更新処理
//==================================================================================================================
void CRenderer::Update(void)
{
#ifdef _DEBUG
	// Imguiの新しい枠を作る
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	// キャプション名
	ImGui::Begin(NAME_CAPTION_IMGUI);
#endif

	// ゲームの状態取得
	CGame::GAMESTATE gameState = CGame::GetGameState();

	// ゲーム状態がポーズじゃないとき
	if (gameState != CGame::GAMESTATE_PAUSE && 
		gameState != CGame::GAMESTATE_KO)
	{
		// Sceneで管理するすべての更新処理
		CScene::UpdateAll();
	}

	// フェードの更新処理
	m_pFade->Update();

	switch (m_mode)
	{
		// タイトルのとき
	case MODE_TITLE:
		// 更新処理
		m_pTitle->Update();
		break;

		// チュートリアルのとき
	case MODE_TUTORIAL:
		// 更新処理
		m_pTutorial->Update();
		break;

		// マップ選択画面のとき
	case MODE_MAPSELECT:
		// 更新処理
		m_pMapSelect->Update();
		break;

		// ゲームのとき
	case MODE_GAME:
		// 更新処理
		m_pGame->Update();
		break;

		// リザルト
	case MODE_RESULT:
		// 更新処理
		m_pResult->Update();
		break;
	}

#ifdef _DEBUG
	// ImGuiの更新
	UpdateImGui();
#endif
}

//==================================================================================================================
// 描画処理
//==================================================================================================================
void CRenderer::Draw(void)
{
	// バックバッファ&Zバッファのクリア
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	// ステンシルバッファを0にクリア
	m_pD3DDevice->Clear(0, 0, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(50, 50, 50, 0), 1.0f, 0);

	// Direc3Dによる描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// Sceneで管理するすべての描画処理
		CScene::DrawAll();

		switch (m_mode)
		{
			// タイトルのとき
		case MODE_TITLE:
			// 描画処理
			m_pTitle->Draw();
			break;

			// チュートリアルのとき
		case MODE_TUTORIAL:
			// 描画処理
			m_pTutorial->Draw();
			break;

			// マップ選択画面のとき
		case MODE_MAPSELECT:
			// 描画処理
			m_pMapSelect->Draw();
			break;

			// ゲームのとき
		case MODE_GAME:
			// 描画処理
			m_pGame->Draw();
			break;

			// リザルト
		case MODE_RESULT:
			// 描画処理
			m_pResult->Draw();
			break;
		}

		// フェードがあるとき
		if (m_pFade != NULL)
		{
			// フェードの描画処理
			m_pFade->Draw();
		}

#ifdef _DEBUG
		// FPSカウンタ取得
		m_nCountFPS = GetFPS();

		// FPS表示
		m_pDebugProc->Print("FPS:%d\n", m_nCountFPS);

		// デバッグロゴの描画
		m_pDebugProc->Draw();

		// ImGuiの描画
		DrawImGui();
#endif

		// 開始されたシーンの終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	HRESULT result =  m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

#ifdef _DEBUG
	// デバイスのリセット
	if (result == D3DERR_DEVICELOST && 
		m_pD3DDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		ResetDevice();
#endif
}

//==================================================================================================================
// モードの設定
//==================================================================================================================
void CRenderer::SetMode(MODE mode)
{
	// なにもないなら処理しない
	if (mode == CRenderer::MODE_NONE)
		return;

	// 指定した音を止める
	switch (mode)
	{
		// タイトルのとき
	case MODE_TITLE:
		// 音を止める
		//m_pSound->StopSound(CSound::SOUND_LABEL_BGMRESULT);
		//m_pSound->StopSound(CSound::SOUND_LABEL_BGMGAME);
		break;

		// チュートリアルのとき
	case MODE_TUTORIAL:
		// 音を止める
		//m_pSound->StopSound(CSound::SOUND_LABEL_BGMTITLE);
		break;

		// マップ選択画面のとき
	case MODE_MAPSELECT:
		// 音を止める
		//m_pSound->StopSound(CSound::SOUND_LABEL_BGMTITLE);
		break;

		// ゲームとき
	case MODE_GAME:
		// 音を止める
		//m_pSound->StopSound(CSound::SOUND_LABEL_BGMTUTORIAL);
		break;

		// ランキングのとき
	case MODE_RESULT:
		// 音を止める
		//m_pSound->StopSound(CSound::SOUND_LABEL_BGMGAME);
		//m_pSound->StopSound(CSound::SOUND_LABEL_BGMTITLE);

		break;
	}

	switch (m_mode)
	{
		// タイトルのとき
	case MODE_TITLE:
		// 終了処理
		m_pTitle->Uninit();
		// 破棄
		delete m_pTitle;
		// NULLにする
		m_pTitle = nullptr;
		break;

		// チュートリアルのとき
	case MODE_TUTORIAL:
		// 終了処理
		m_pTutorial->Uninit();
		// 破棄
		delete m_pTutorial;
		// NULLにする
		m_pTutorial = nullptr;
		break;

		// マップ選択画面のとき
	case MODE_MAPSELECT:
		// 終了処理
		m_pMapSelect->Uninit();
		// 破棄
		delete m_pMapSelect;
		// NULLにする
		m_pMapSelect = nullptr;
		break;

		// ゲームのとき
	case MODE_GAME:
		// 終了処理
		m_pGame->Uninit();
		// 破棄
		delete m_pGame;
		// NULLにする
		m_pGame = nullptr;
		break;

		// リザルト
	case MODE_RESULT:
		// 終了処理
		m_pResult->Uninit();
		// 破棄
		delete m_pResult;
		// NULLにする
		m_pResult = nullptr;
		break;
	}

	// モード変数を代入
	m_mode = mode;

	switch (mode)
	{
		// タイトルのとき
	case MODE_TITLE:
		// 生成処理
		m_pTitle = CTitle::Create();
		//m_pSound->PlaySound(CSound::SOUND_LABEL_BGMTITLE);
		break;

		// チュートリアルのとき
	case MODE_TUTORIAL:
		// 生成処理
		m_pTutorial = CTutorial::Create();
		//m_pSound->PlaySound(CSound::SOUND_LABEL_BGMTUTORIAL);
		break;

		// マップ選択画面のとき
	case MODE_MAPSELECT:
		// 生成処理
		m_pMapSelect = CMapSelect::Create();
		//m_pSound->PlaySound(CSound::SOUND_LABEL_BGMTUTORIAL);
		break;

		// ゲームのとき
	case MODE_GAME:
		// 生成処理
		m_pGame = CGame::Create();
		//m_pSound->PlaySound(CSound::SOUND_LABEL_BGMGAME);
		break;

		// リザルト
	case MODE_RESULT:
		// 生成処理
		m_pResult = CResult::Create();
		//m_pSound->PlaySound(CSound::SOUND_LABEL_BGMRESULT);
		break;
	}
}

//==================================================================================================================
// モードの取得
//==================================================================================================================
CRenderer::MODE CRenderer::GetMode(void)
{
	return m_mode;
}

//==================================================================================================================
// ゲーム情報取得
//==================================================================================================================
CGame * CRenderer::GetGame(void)
{
	return m_pGame;
}

//==================================================================================================================
// 音情報取得
//==================================================================================================================
CSound * CRenderer::GetSound(void)
{
	return m_pSound;
}

//==================================================================================================================
// デバイス取得
//==================================================================================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	// 値を返す
	return m_pD3DDevice;
}
 
#ifdef _DEBUG
//==================================================================================================================
// デバッグの描画
//==================================================================================================================
void CRenderer::DrawImGui(void)
{
	if (CManager::GetShowImGui())
	{
		// ImGuiウィンドウは常に通常描画
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		// Imguiの描画
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		// 元の描画方法に戻す
		CKananLibrary::GetWire() ?
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME) :	// ワイヤーフレーム
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);		// 通常
	}
}

//==================================================================================================================
// デバイスのクリーンアップ
//==================================================================================================================
void CRenderer::CleanupDeviceD3D()
{
	if (m_pD3DDevice) 
		{ m_pD3DDevice->Release(); m_pD3DDevice = NULL; }
	if (m_pD3D) 
		{ m_pD3D->Release(); m_pD3D = NULL; }
}

//==================================================================================================================
// デバイスのリセット
//==================================================================================================================
void CRenderer::ResetDevice()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	HRESULT hr = m_pD3DDevice->Reset(&m_d3dpp);

	if (hr == D3DERR_INVALIDCALL)
		IM_ASSERT(0);

	ImGui_ImplDX9_CreateDeviceObjects();
}

//==================================================================================================================
// ImGuiの初期化
//==================================================================================================================
void CRenderer::InitImGui(D3DPRESENT_PARAMETERS d3dpp, HWND hWnd)
{
	// プレゼンテーションパラメータを保存
	m_d3dpp = d3dpp;
	
	// 要素の初期化
	m_nCntWire = 0;
	m_bWire = false;
	
	// ImGuiの初期化
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	
	// Imguiのスタイル (見た目) の決定
	ImGui::StyleColorsDark();
	
	// プラットフォームに合わせたセットアップを行う
	ImGui_ImplWin32_Init(hWnd);
	// ImgGuiのレンダラーをバインド
	ImGui_ImplDX9_Init(m_pD3DDevice);
}

//==================================================================================================================
// ImGuiの更新
//==================================================================================================================
void CRenderer::UpdateImGui(void)
{
	// ImGui表示確認
	CManager::GetShowImGui() ?
		CDebugProc::Print("ImGui表示中	 [ F2で非表示 ]\n") :
		CDebugProc::Print("ImGui非表示中 [ F2で表示 ]\n");

	// F2キーで表示切替
	if (CInputKeyboard::GetKeyboardTrigger(DIK_F2))
	{
		CManager::GetShowImGui() ?
			CManager::SetShowImGui(false) :
			CManager::SetShowImGui(true);
	}

	// デバッグの基本情報の更新
	CKananLibrary::ShowDebugInfo();

	// ゲームモード中
	if (m_mode == MODE_GAME)
		// キャラパラメーターの情報更新
		CCharaParam::UpdateImGui();

	ImGui::End();

	// 枠を作り終わったら必ず書く
	ImGui::EndFrame();
	// ステンシルテストを終える
	m_pD3DDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
}
#endif
