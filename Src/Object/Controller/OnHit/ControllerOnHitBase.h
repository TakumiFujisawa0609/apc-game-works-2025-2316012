#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include "../ControllerBase.h"

class ControllerOnHitBase : public ControllerBase
{
public:
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ControllerOnHitBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ControllerOnHitBase() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() override;

protected:

	//タグ別衝突後の処理の登録
	void RegisterOnHit(const std::string& tag, std::function<void()> onHit);

	//衝突後の処理を管理するマップ
	std::unordered_map<std::string, std::function<void()>> onHitMap_;
};

