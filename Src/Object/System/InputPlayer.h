#pragma once
#include <functional>
#include <unordered_map>

class InputManager;

class InputPlayer
{
public:

	enum class CONFIG
	{
		RIGHT,
		LEFT,
		FORWARD,
		BACK,
		JUMP,
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	InputPlayer();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~InputPlayer() = default;

	/// <summary>
	/// キーの入力状態を返す
	/// </summary>
	/// <param name="config">種類を返す</param>
	/// <returns>true入力中、false非入力</returns>
	bool CheckKey(CONFIG config);

private:

	//入力管理クラス
	InputManager& input_;

	//キー入力チェック関数マップ
	std::unordered_map<CONFIG, std::function<bool()>> keyCheckMap_;

	//各種入力処理の登録
	void RegisterCheckKey(const CONFIG config, std::function<bool()> func);

	//キー入力チェック関数
	bool CheckKeyRight() const;
	bool CheckKeyLeft() const;
	bool CheckKeyForward() const;
	bool CheckKeyBack() const;
	bool CheckKeyJump() const;
};

