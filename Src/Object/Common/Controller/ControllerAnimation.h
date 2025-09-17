#pragma once
#include <string>
#include <map>

class SceneManager;

class ControllerAnimation
{
public :

	/// <summary>
	/// アニメーションデータ
	/// </summary>
	struct Animation
	{
		int model = -1;			// モデルID
		int attachNo = -1;		// アニメーションの種類(番号)
		int animIndex = 0;		// アニメーションのハンドルID
		float speed = 0.0f;		// アニメーションの長さ(秒)
		float totalTime = 0.0f;	// アニメーションの再生時間(秒)
		float step = 0.0f;		// アニメーションの進行度(0.0f～1.0f)
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="modelId">モデルID</param>
	ControllerAnimation(int modelId);
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~ControllerAnimation(void);

	/// <summary>
	/// アニメーションの追加
	/// </summary>
	/// <param name="type">種類(番号)</param>
	/// <param name="handle">アニメーションハンドル</param>
	/// <param name="speed">アニメーション速度</param>
	void Add(const int type, const int handle, const float speed);

	/// <summary>
	/// アニメーション再生
	/// </summary>
	/// <param name="type">アニメーション番号</param>
	/// <param name="isLoop">ループ有無</param>
	/// <param name="startStep">開始ステップ</param>
	/// <param name="endStep">終了ステップ</param>
	/// <param name="isStop">停止判定</param>
	/// <param name="isForce"></param>
	void Play(
		const int type,
		const bool isLoop = true,
		const float startStep = 0.0f,
		const float endStep = -1.0f,
		const bool isStop = false,
		const bool isForce = false);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	void Update(void);

	/// <summary>
	/// アニメーション終了後に繰り返すループステップ
	/// </summary>
	/// <param name="startStep">開始ステップ</param>
	/// <param name="endStep">終了ステップ</param>
	/// <param name="speed">アニメーション速度</param>
	void SetEndLoop(const float startStep, const float endStep, const float speed);

	/// <summary>
	/// 再生中のアニメーションの種類を返す
	/// </summary>
	/// <param name=""></param>
	/// <returns>アニメーションの種類番号</returns>
	int GetPlayType(void) const;

	/// <summary>
	/// 再生終了判定
	/// </summary>
	/// <param name=""></param>
	/// <returns>trueなら終了,falseなら再生中</returns>
	bool IsEnd(void) const;

private :

	// モデルのハンドルID
	int modelId_;

	// 種類別のアニメーションデータ
	std::map<int, Animation> animations_;

	int playType_;
	Animation playAnim_;

	// アニメーションをループするかしないか
	bool isLoop_;

	// アニメーションを止めたままにする
	bool isStop_;

	// アニメーション終了後に繰り返すループステップ
	float stepEndLoopStart_;
	float stepEndLoopEnd_;
	float endLoopSpeed_;

	// 逆再生
	float switchLoopReverse_;
};