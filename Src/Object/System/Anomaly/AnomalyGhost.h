#pragma once
#include "AnomalyBase.h"

class AnomalyGhost : public AnomalyBase
{
public:

	/// <summary>
	/// ‰Šú‰»ˆ—
	/// </summary>
	void Init() override;

	/// <summary>
	/// ”­¶ˆ—
	/// </summary>
	/// <param name="param">ƒpƒ‰ƒ[ƒ^</param>
	void Occurrence(Json& param) override;

private:
};

