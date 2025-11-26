#pragma once
#include <memory>
#include <unordered_map>
#include "CoreGameBase.h"
#include "../../Common/Sprite.h"
#include "../../Common/Image.h"
#include "../../Common/CharacterString.h"

class PostEffectBase;
class InputManager;

class CameraScreen : public CoreGameBase
{
public:

	/// <summary>
	/// スクリーンの描画モード
	/// </summary>
	enum class SCREEN_MODE
	{
		NORMAL,		// 通常
		NIGHT_VIEW,	// 暗視
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CameraScreen();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CameraScreen() override;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// スクリーンの状態
	/// </summary>
	/// <returns>スクリーン状態を返す</returns>
	const SCREEN_MODE GetScreenMode() const { return mode_; }

	/// <summary>
	/// カメラモードの遷移
	/// </summary>
	/// <param name="mode">カメラモード</param>
	void ChangeScreenMode(const SCREEN_MODE mode);

private:

	// 入力管理クラスの参照
	InputManager& input_;

	// キャラクターの管理クラスの参照
	CharacterManager& charaMng_;

	// 状態
	SCREEN_MODE mode_;

	// カメラフレーム
	Image cameraFrame_;

	// カメラ切り替え用のアイコン
	Sprite switchIcon_;

	// 活動状態の切り替え
	void SetActiveActors();
};