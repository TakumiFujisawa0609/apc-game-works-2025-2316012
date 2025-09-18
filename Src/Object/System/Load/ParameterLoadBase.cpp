#include "ParameterLoadBase.h"

ParameterLoadBase::ParameterLoadBase()
{
}

Json ParameterLoadBase::GetParameterFile(const std::string& key)
{
	// マップからキーに対応するJsonオブジェクトを検索
	auto it = parameterMap_.find(key);

	// キーが見つかった場合
	if (it != parameterMap_.end())
	{
		// 取得した値をそのまま返す
		return it->second;
	}

	// キーが見つからない場合は空のJsonオブジェクトを返す
	return Json();
}