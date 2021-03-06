//=============================================================================
//
// キャラクターパラメーター処理  [ charaParam.cpp ]
// Author : KANAN NAGANAWA
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "charaParam.h"
#include "kananlibrary.h"
#include "player.h"
#include "game.h"
#include "mouse.h"
#include "ImGui/imgui.h"				// Imguiの実装に必要
#include "ImGui/imgui_impl_dx9.h"		// Imguiの実装に必要
#include "ImGui/imgui_impl_win32.h"		// Imguiの実装に必要

//=============================================================================
// マクロ定義
//=============================================================================	
#define SPEED_IMGUI_DRAGFLOAT	(0.1f)	// DragFloatのスピード

//=============================================================================
// 静的メンバ変数の初期化
//=============================================================================
CCharaParam::PLAYER_PARAM	CCharaParam::m_playerParam[PARAM_MAX] = {};
bool						CCharaParam::m_bShowWindow = false;

char						CCharaParam::m_aFileName[PARAM_MAX][64] =
{
	"data/PARAMETER/parameter_fokker.txt",
	"data/PARAMETER/parameter_niyasu.txt",
	"data/PARAMETER/parameter_sanyasu.txt",
	"data/PARAMETER/parameter_yonyasu.txt"
};

//=============================================================================
// コンストラクタ
//=============================================================================
CCharaParam::CCharaParam() {}
//=============================================================================
// デストラクタ
//=============================================================================
CCharaParam::~CCharaParam() {}

//=============================================================================
// キャラクターごとのパラメーターロード
//=============================================================================
HRESULT CCharaParam::Load(void)
{
	// 変数宣言
	FILE *pFile = nullptr;
	char cReadText[MAX_TEXT] = "";
	char cHeadText[MAX_TEXT] = "";
	char cDieText[MAX_TEXT] = "";

	CKananLibrary::StartBlockComment("パラメーターファイルの読み込み開始");

	for (int type = 0; type < PARAM_MAX; type++)
	{
		// 攻撃読み込みのカウンタ
		int nCntAttack = 0;

		// キャラ名を表示
		printf("キャラクター %d を読み込み (%s)\n", type, m_aFileName[type]);
		// ファイルを開く
		pFile = fopen(m_aFileName[type], "r");

		// nullcheck
		if (!pFile)
		{
			// 失敗
			printf("ファイルを開けませんでした\n");
			continue;
		}

		// スクリプトがくるまで繰り返す
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			// 一行ずつ読み込み
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s", &cHeadText);
		}

		// エンドスクリプトが来るまで繰り返す
		while (strcmp(cHeadText, "END_SCRIPT") != 0)
		{
			// 一行ずつ読み込み
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s", &cHeadText);

			// 改行
			if (strcmp(cHeadText, "\n") == 0)
				continue;
			// 最大HP
			else if (strcmp(cHeadText, "MAX_LIFE") == 0)
				sscanf(cReadText, "%s %s %f", &cDieText, &cDieText, &m_playerParam[type].fMaxLife);
			// 走る速度
			else if (strcmp(cHeadText, "RUN_SPEED") == 0)
				sscanf(cReadText, "%s %s %f", &cDieText, &cDieText, &m_playerParam[type].moveParam.fRunSpeed);
			// ジャンプ力
			else if (strcmp(cHeadText, "JUMP_POWER") == 0)
				sscanf(cReadText, "%s %s %f", &cDieText, &cDieText, &m_playerParam[type].moveParam.fJumpPower);
			// 攻撃力
			if (strcmp(cHeadText, "ATTACK_POWER") == 0)
				sscanf(cReadText, "%s %s %f",&cDieText, &cDieText, &m_playerParam[type].fAttackPower);
			// 防御力
			if (strcmp(cHeadText, "DEFENSE_POWER") == 0)
				sscanf(cReadText, "%s %s %f", &cDieText, &cDieText, &m_playerParam[type].fDefensePower);
			// モーション設定
			else if (strcmp(cHeadText, "SET_MOTIONPARAM") == 0)
			{
				// エンドスクリプトが来るまで繰り返す
				while (strcmp(cHeadText, "END_MOTIONPARAM") != 0)
				{
					// 一行ずつ読み込み
					fgets(cReadText, sizeof(cReadText), pFile);
					sscanf(cReadText, "%s", &cHeadText);
					// 攻撃のキャンセルフレーム
					if (strcmp(cHeadText, "CANCEL_FRAME") == 0)
						sscanf(cReadText, "%s %s %d %s %d",
							&cDieText,
							&cDieText,
							&m_playerParam[type].motionParam[nCntAttack].CancelFrame.start,
							&cDieText,
							&m_playerParam[type].motionParam[nCntAttack].CancelFrame.end);
					// 吹き飛び方
					if (strcmp(cHeadText, "BLOWAWAY_TYPE") == 0)
						sscanf(cReadText, "%s %s %d",
							&cDieText,
							&cDieText,
							&m_playerParam[type].motionParam[nCntAttack].blowType);
				}
				// 攻撃の加算
				nCntAttack++;
			}
		}
		// ファイルを閉じる
		fclose(pFile);
	}

	// 成功
	CKananLibrary::EndBlockComment("パラメーターファイルの読み込み終了");
	return S_OK;
}

//=============================================================================
// キャラクターごとのパラメーターセーブ
//=============================================================================
HRESULT CCharaParam::Save(const PARAM_TYPE type)
{
	// 変数宣言
	FILE *pFile;

	// テキストファイルを見やすくするコメント
	char cEqual[2] = "=";

	// 変数名用と書き込み用の変数
	char cHeadText[MAX_TEXT];
	char cWriteText[MAX_TEXT];

	char cMotionName[ATTACK_MAX][64] =
	{
		"通常攻撃 1",
		"通常攻撃 2",
		"通常攻撃 3 ",
		"通常攻撃 4 ",
		"空中パンチ",
		"空中キック",
		"スマッシュ",
	};

	// 位置・回転の取得用
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;

	// コンソールに表示
	CKananLibrary::StartBlockComment("キャラクターのパラメーターセーブ開始");

	// ファイルを開く
	pFile = fopen(m_aFileName[type], "w");

	// nullcheck
	if (!pFile)
	{
		// コンソールに表示
		CKananLibrary::EndBlockComment("ファイルを開けませんでした");
		// 失敗
		return E_FAIL;
	}

	fputs(COMMENT_BLOCK, pFile);													// #=====================================================
	fputs(COMMENT_BLOCK_LINE, pFile);												// #
	fputs("# キャラクターパラメーター\n", pFile);									// # キャラクターパラメーター
	fputs(COMMENT_AUTHOR, pFile);													// # Author : KANAN NAGANAWA
	fputs(COMMENT_BLOCK_LINE, pFile);												// #
	fputs(COMMENT_BLOCK, pFile);													// #=====================================================
	fputs("SCRIPT	# 消さないで ", pFile);											// SCRIPT
	fputs(COMMENT_NEW_LINE, pFile);													// \n
	fputs(COMMENT_NEW_LINE, pFile);													// \n

	fputs(COMMENT_BLOCK, pFile);													// #=====================================================
	fputs("# 基本ステータス\n", pFile);												// # 基本ステータス
	fputs(COMMENT_BLOCK, pFile);													// #=====================================================

	strcpy(cHeadText, "MAX_LIFE");
	sprintf(cWriteText, "%s      %s %.5f # 最大HP\n",
		&cHeadText,
		&cEqual,
		m_playerParam[type].fMaxLife);
	fputs(cWriteText, pFile);														// MAX_LIFE = fMaxLife

	strcpy(cHeadText, "RUN_SPEED");
	sprintf(cWriteText, "%s     %s %.5f # 移動速度\n",
		&cHeadText,
		&cEqual,
		m_playerParam[type].moveParam.fRunSpeed);
	fputs(cWriteText, pFile);														// RUN_SPEED = fRunSpeed

	strcpy(cHeadText, "JUMP_POWER");
	sprintf(cWriteText, "%s    %s %.5f # ジャンプ力\n",
		&cHeadText,
		&cEqual,
		m_playerParam[type].moveParam.fJumpPower);
	fputs(cWriteText, pFile);														// JUMP_POWER = fJumpPower

	strcpy(cHeadText, "ATTACK_POWER");
	sprintf(cWriteText, "%s  %s %.5f # 攻撃力\n",
		&cHeadText,
		&cEqual,
		m_playerParam[type].fAttackPower);
	fputs(cWriteText, pFile);														// ATTACK_POWER = fAttackPower

	strcpy(cHeadText, "DEFENSE_POWER");
	sprintf(cWriteText, "%s %s %.5f # 防御力\n",
		&cHeadText,
		&cEqual,
		m_playerParam[type].fDefensePower);
	fputs(cWriteText, pFile);														// DEFENSE_POWER = fDefensePower

	fputs(COMMENT_NEW_LINE, pFile);													// \n

	fputs(COMMENT_BLOCK, pFile);													// #=====================================================
	fputs("# モーションごとのパラメーター\n", pFile);								// # モーションごとのパラメーター
	fputs(COMMENT_BLOCK, pFile);													// #=====================================================

	// 攻撃数分繰り返す
	for (int nCnt = 0; nCnt < ATTACK_MAX; nCnt++)
	{
		sprintf(cWriteText, "SET_MOTIONPARAM	# %s\n",
			cMotionName[nCnt]);
		fputs(cWriteText, pFile);													//	SET_MOTIONPARAM  # モーション名

		strcpy(cHeadText, "CANCEL_FRAME");
		sprintf(cWriteText, "	%s %s %d - %d\n",
			&cHeadText,
			&cEqual,
			m_playerParam[type].motionParam[nCnt].CancelFrame.start,
			m_playerParam[type].motionParam[nCnt].CancelFrame.end);
		fputs(cWriteText, pFile);													//		CANCEL_FRAME = start - end

		strcpy(cHeadText, "BLOWAWAY_TYPE");
		sprintf(cWriteText, "	%s %s %d\n",
			&cHeadText,
			&cEqual,
			(int)m_playerParam[type].motionParam[nCnt].blowType);
		fputs(cWriteText, pFile);													//		BLOWAWAY_TYPE = blowType
		fputs("END_MOTIONPARAM\n", pFile);											//	END_MOTIONPARAM
		fputs(COMMENT_NEW_LINE, pFile);												// \n
	}
	fputs("END_SCRIPT	# 消さないで \n", pFile);									// END_SCRIPT

	CKananLibrary::EndBlockComment("キャラクターのパラメーターセーブ終了");
	// ファイルを閉じる
	fclose(pFile);

	// 成功
	return S_OK;
}

#ifdef _DEBUG
//=============================================================================
// ImGuiの更新
//=============================================================================
void CCharaParam::UpdateImGui(void)
{
	// ウィンドウを表示するか
	ImGui::Checkbox("show charaParam window", &m_bShowWindow);

	// nullcheck
	if (!m_bShowWindow)
		// 表示しない
		return;

	// ImGuiの更新開始
	ImGui::Begin("charaParam window", &m_bShowWindow);

	PARAM_TYPE charaType[MAX_PLAYER];
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		charaType[nCnt] = (PARAM_TYPE)(CManager::GetRenderer()->GetGame()->GetPlayer(nCnt)->GetCharaType() / 2);
	}

	char cName[PARAM_MAX][16] = 
	{
		"1 yasu",
		"2 yasu",
		"3 yasu",
		"4 yasu"
	};

	// タブ用の線を表示
	if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
	{
		for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
		{
			// プレイヤー1と同じキャラ
			if (nCnt != 0 && charaType[nCnt] == charaType[0])
				// 処理しない
				continue;

			// キャラパラメーターの詳細
			ShowCharaParam(cName[charaType[nCnt]], charaType[nCnt]);
		}
		// タブの終了に必ず書く
		ImGui::EndTabBar();
	}

	// 閉じるボタン
	if (ImGui::Button("close this window"))
		m_bShowWindow = false;

	// ImGuiの更新終了
	ImGui::End();
}

//=============================================================================
// キャラパラメーターの詳細
//=============================================================================
void CCharaParam::ShowCharaParam(const char cName[16], PARAM_TYPE type)
{
	// プレイヤー1の情報
	if (ImGui::BeginTabItem(cName))
	{
		// モーション名の表記を設定
		char cMotionName[ATTACK_MAX][64] =
		{
			"NormalAttack_0",
			"NormalAttack_1",
			"NormalAttack_2",
			"NormalAttack_3",
			"AirPunch",
			"AirKick",
			"Smash",
		};

		// 基本情報
		ImGui::DragFloat("MaxLife", &m_playerParam[type].fMaxLife, SPEED_IMGUI_DRAGFLOAT);
		ImGui::DragFloat("RunSpeed", &m_playerParam[type].moveParam.fRunSpeed, SPEED_IMGUI_DRAGFLOAT);
		ImGui::DragFloat("JumpPower", &m_playerParam[type].moveParam.fJumpPower, SPEED_IMGUI_DRAGFLOAT);
		ImGui::DragFloat("AttackPower", &m_playerParam[type].fAttackPower, SPEED_IMGUI_DRAGFLOAT);
		ImGui::DragFloat("DefensePower", &m_playerParam[type].fDefensePower, SPEED_IMGUI_DRAGFLOAT);

		for (int nCnt = 0; nCnt < ATTACK_MAX; nCnt++)
		{
			// モーション名
			ImGui::Text(cMotionName[nCnt]);
			sprintf(cMotionName[nCnt], "%d", nCnt);
			// 項目名
			ImGui::Text("CancelFrame");
			ImGui::SameLine();
			ImGui::InputInt2(cMotionName[nCnt], &m_playerParam[type].motionParam[nCnt].CancelFrame.start);
			ImGui::Dummy(ImVec2(0.0f, 2.0f));
		}

		if (ImGui::Button("Save"))
		{
			// セーブ
			if (Save(type) == S_OK)
			{
				// マウスの取得
				CMouse *pMouse = CManager::GetMouse();
				// ウィンドウハンドルの取得
				HWND hWnd = pMouse->GetWnd();
				// 確認のメッセージ
				MessageBox(hWnd, "パラメーターファイルをセーブしました", "確認", MB_OK);
			}
		}

		// タブの終了に必ず書く
		ImGui::EndTabItem();
	}
}

#endif