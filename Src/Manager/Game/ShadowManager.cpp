#include <DxLib.h>
#include "../Common/Camera.h"
#include "../Common/SceneManager.h"
#include "../Common/ResourceManager.h"
#include "CharacterManager.h"
#include "StageManager.h"
#include "ShadowManager.h"

void ShadowManager::Init()
{
	// 描画可能テクスチャをシャドウマップ用にする
	SetCreateDrawValidGraphChannelNum(1);

	// 描画可能テクスチャを浮動小数点型で作成するフラグを有効にする
	SetDrawValidFloatTypeGraphCreateFlag(TRUE);

	// 描画可能テクスチャの色深度を設定する
	SetCreateGraphColorBitDepth(COLOR_BIT_DEPTH);

	// シャドウマップ用テクスチャの作成
	shadowMapTexture_ = MakeScreen(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, false);

	// 描画可能テクスチャを通常の設定に戻す
	SetDrawValidFloatTypeGraphCreateFlag(FALSE);

	// 描画可能テクスチャのチャンネル数を元に戻す
	SetCreateDrawValidGraphChannelNum(DEFAULT_CHANNEL_NUM);

	// 描画可能テクスチャの色深度を元に戻す
	SetCreateGraphChannelBitDepth(0);

	// シャドウマップ生成用シェーダーハンドルの取得
	auto& resMng = ResourceManager::GetInstance();
	shadowPs_ = resMng.GetHandle("shadowPs");
	shadowMeshVs_ = resMng.GetHandle("shadowMeshVs");
	shadowSkinnedMeshVs_ = resMng.GetHandle("shadowSkinnedMeshVs");
}

void ShadowManager::Draw()
{
	SetDrawScreen(shadowMapTexture_);

	// 影用深度記録画像を真っ白にクリア
	constexpr int MAX_COLOR = 255;
	SetBackgroundColor(MAX_COLOR, MAX_COLOR, MAX_COLOR);
	ClearDrawScreen();
	SetBackgroundColor(0, 0, 0);

	// シャドウカメラの設定
	mainCamera.CameraSettingShadow();

	// イベント時以外
	if (!isEvent_)
	{
		// 設定したカメラのビュー行列と射影行列を取得しておく
		lightViewMatrix_ = GetCameraViewMatrix();
		lightProjectionMatrix_ = GetCameraProjectionMatrix();
	}

	// オリジナルシェーダー使用の設定
	MV1SetUseOrigShader(TRUE);

	// 深度記録画像への剛体メッシュ描画用の頂点シェーダーをセット
	SetUseVertexShader(shadowMeshVs_);

	// 深度記録画像への描画用のピクセルシェーダーをセット
	SetUsePixelShader(shadowPs_);

	// ステージの描画
	stageMng_.DrawShadow();

	// シェーダーのリセット
	ResetShader();

	// オリジナルシェーダー使用の再設定
	MV1SetUseOrigShader(TRUE);

	// 深度記録画像への剛体メッシュ描画用の頂点シェーダーをセット
	SetUseVertexShader(shadowSkinnedMeshVs_);

	// キャラクターの描画
	charaMng_.DrawShadow();

	// シェーダーのリセット
	ResetShader();

	// 描画スクリーンを元に戻す
	SetDrawScreen(scnMng_.GetMainScreen());

	// 画面を初期化
	ClearDrawScreen();

	// カメラ設定を元に戻す
	mainCamera.CameraSetting();
}

void ShadowManager::ResetShader()
{
	// Shaderを元に戻す
	MV1SetUseOrigShader(FALSE);
	SetUseVertexShader(-1);
	SetUsePixelShader(-1);
	// 前回使用分のテクスチャを引き継がないように
	SetUseTextureToShader(0, -1);
}

ShadowManager::ShadowManager() :
	scnMng_(SceneManager::GetInstance()),
	charaMng_(CharacterManager::GetInstance()),
	stageMng_(StageManager::GetInstance())
{
	shadowMapTexture_ = -1;
	shadowPs_ = -1;
	shadowMeshVs_ = -1;
	shadowSkinnedMeshVs_ = -1;
	lightViewMatrix_ = MATRIX{};
	lightProjectionMatrix_ = MATRIX{};
	isEvent_ = false;
}

ShadowManager::~ShadowManager()
{
	DeleteGraph(shadowMapTexture_);
}
