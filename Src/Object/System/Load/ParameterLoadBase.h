#pragma once
#include <memory>
#include <unordered_map>
#include <string>

class ParameterBase;

class ParameterLoadBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ParameterLoadBase();
	 
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ParameterLoadBase() {};
	 
	/// <summary>
	/// 読み込み処理
	/// </summary>
	virtual void Load() = 0;

	// 参照ではなくポインタを返すように修正
	ParameterBase* GetParameter(const std::string& name);

protected:

	//パラメーターマップ
	std::unordered_map<std::string, std::unique_ptr<ParameterBase>> parametersMap_;
};

