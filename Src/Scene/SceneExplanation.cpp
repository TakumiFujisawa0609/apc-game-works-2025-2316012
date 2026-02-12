#include "SceneExplanation.h"

SceneExplanation::SceneExplanation()
{
	// ó‘Ô‘JˆÚ‚Ìˆ—‚ğ“o˜^
	stateChangeMap_.emplace(STATE::SELECT, std::bind(&SceneExplanation::ChangeStateSelect, this));
	stateChangeMap_.emplace(STATE::OPERATION_EXPLANATION, std::bind(&SceneExplanation::ChangeStateOperationExplanation, this));
	stateChangeMap_.emplace(STATE::PLAY_EXPLANATION, std::bind(&SceneExplanation::ChangeStatePlayExplanation, this));
	
	// ‰Šúó‘Ô‚Ö‘JˆÚ
	ChangeState(STATE::SELECT);
}

SceneExplanation::~SceneExplanation()
{
}

void SceneExplanation::NormalUpdate()
{
	update_();
}

void SceneExplanation::NormalDraw()
{
	draw_();
}

void SceneExplanation::UpdateSelect()
{
}

void SceneExplanation::UpdateOperationExplanation()
{
}

void SceneExplanation::UpdatePlayExplanation()
{
}

void SceneExplanation::DrawSelect()
{
}

void SceneExplanation::DrawOperationExplanation()
{
}

void SceneExplanation::DrawPlayExplanation()
{
}

void SceneExplanation::ChangeState(const STATE state)
{
	state_ = state;

	stateChangeMap_[state_]();
}

void SceneExplanation::ChangeStateSelect()
{
	update_ = std::bind(&SceneExplanation::UpdateSelect, this);
	draw_ = std::bind(&SceneExplanation::DrawSelect, this);
}

void SceneExplanation::ChangeStateOperationExplanation()
{
	update_ = std::bind(&SceneExplanation::UpdateOperationExplanation, this);
	draw_ = std::bind(&SceneExplanation::DrawOperationExplanation, this);
}

void SceneExplanation::ChangeStatePlayExplanation()
{
	update_ = std::bind(&SceneExplanation::UpdatePlayExplanation, this);
	draw_ = std::bind(&SceneExplanation::DrawPlayExplanation, this);
}