#include "../../Render/PixelMaterial.h"
#include "../../Render/PixelRenderer.h"
#include "../../Manager/Common/ResourceManager.h"
#include "../../Manager/Common/SceneManager.h"
#include "SceneBackGround.h"

SceneBackGround::SceneBackGround()
{
	step_ = 0.0f;
}

SceneBackGround::~SceneBackGround()
{
}

void SceneBackGround::Init()
{
	// 背景
	material_ = std::make_unique<PixelMaterial>(resMng_.GetHandle("backGroundShader"), BUFFER_SIZE);
	renderer_ = std::make_unique<PixelRenderer>(*material_);

	// マテリアル
	material_->AddTextureBuf(resMng_.GetHandle("sceneBack"));
	material_->AddConstBuf(FLOAT4{ 0.0f, DISTORTION_STRENGTH, MOVE_SPEED,0.0f });

	// レンダラー
	renderer_->SetPos(POS);
	renderer_->SetSize(SIZE);
	renderer_->MakeSquereVertex();
}

void SceneBackGround::Draw()
{
	step_ += scnMng_.GetDeltaTime();
	material_->SetConstBuf(0, FLOAT4{step_,DISTORTION_STRENGTH, MOVE_SPEED,0.0f });
	renderer_->Draw();
}
