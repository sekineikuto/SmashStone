//==================================================================================================================
//
// 一安 ドゥ マルタ 三世[charaICHIYAS.h]
// Author : Seiya Takahashi
//
//==================================================================================================================
#ifndef _CHARAICHIYAS_H_
#define _CHARAICHIYAS_H_

//==================================================================================================================
// インクルードファイル
//==================================================================================================================
#include "main.h"
#include "sceneX.h"
#include "fade.h"
#include "character.h"

//==================================================================================================================
// マクロ定義
//==================================================================================================================
#define CHARAICHIYAS_MODEL "data/MODEL/testmodel.x"			// 読み込むモデルのソース先
#define CHARAICHIYAS_MAX_MODEL (6)							// player.cpp内のモデルのパーツ数

//==================================================================================================================
// 前方宣言
//==================================================================================================================
class CCamera;
class CMotionModel;
class CInputKeyboard;
class CInputGamepad;

//==================================================================================================================
//
// プレイヤークラスの定義
//
//==================================================================================================================
class CCharaICHIYAS : public CCharacter
{
public:
	//=============================================================================
	// 構造体定義
	//=============================================================================
	// キー要素
	typedef struct
	{
		D3DXVECTOR3 pos;				// キー要素の位置
		D3DXVECTOR3 rot;				// キー要素の回転
	}KEY;

	// キー情報
	typedef struct
	{
		KEY key[CHARAICHIYAS_MAX_MODEL];		// パーツ数
		int nFram;								// フレーム
	}KEY_INFO;

	// プレイヤーのモーション
	typedef struct
	{
		KEY_INFO key_info[10];				// キーが何セットあるか
		D3DXVECTOR3 CollisionOfset;			// オフセット
		int nLoop;							// ループ
		int nNumKey;						// キー情報の数
		int Collision_nParts;				// パーツ番号
		int CollisionStartFram;				// 開始フレーム
		int CollisionEndFram;				// 終了フレーム
		int Collision_Damage;				// ダメージ数
		float CollisionRadius;				// 円の半径
	}MOTION_PLAYER;

	// モーションのタイプ
	typedef enum
	{
		PLAYER_MOTION_NORMAL = 0,			// ニュートラル
		PLAYER_MOTION_WALK,					// 歩き
		PLAYER_MOTION_ACTION,				// アクション
		PLAYER_MOTION_JUMP,					// ジャンプ
		PLAYER_MOTION_LANDING,				// 着地
		PLAYER_MOTION_DAMAGE,				// ダメージ
		PLAYER_MOTION_MAX					// モーションの最大数
	}PLAYER_MOTION_STATE;

	CCharaICHIYAS(PRIORITY type);					// コンストラクタ
	~CCharaICHIYAS();								// デストラクタ
	void Init(void);								// 初期化処理
	void Uninit(void);								// 終了処理
	void Update(void);								// 更新処理
	void Draw(void);								// 描画処理

	static CCharaICHIYAS *Create(void);				// 生成処理

protected:

private:
	void LoadMotion(void);							// モーション読み込み
	void Moation(void);								// モーションさせる

	CMotionModel *m_MotionModel[CHARAICHIYAS_MAX_MODEL];	// モデル情報
	MOTION_PLAYER m_PlayerMotion[PLAYER_MOTION_MAX];// モーション情報

	static CCamera *m_pCamera;				// カメラの情報ポインタ

	D3DXVECTOR3 posBET[CHARAICHIYAS_MAX_MODEL];	// 各パーツの位置
	D3DXVECTOR3 rotBET[CHARAICHIYAS_MAX_MODEL];	// 各パーツの回転

	int m_MotionType;						// モーションの種類
	int m_MotionOld;						// 前のモーション
	int m_Fram;								// フレーム
	int m_nCntKeySet;						// キーセットのカウント
	int m_nCntState;						// ステータスのカウント

	float m_RotModel;						// モデルの回転情報
};
#endif