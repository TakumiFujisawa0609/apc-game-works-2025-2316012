#include <DxLib.h>
#include "../../../Manager/Generic/SceneManager.h"
#include "ControllerAnimation.h"

ControllerAnimation::ControllerAnimation(int modelId)
{
	modelId_ = modelId;

	playType_ = -1;
	isLoop_ = false;

	isStop_ = false;
	switchLoopReverse_ = 0.0f;
	endLoopSpeed_ = 0.0f;
	stepEndLoopStart_ = 0.0f;
	stepEndLoopEnd_ = 0.0f;

	deltaTime_ = 0.0f;
	preBlendAnimationRateMap_.clear();
	blendAnimRate_ = 0.0f;
}

ControllerAnimation::~ControllerAnimation(void)
{
	//for (const auto& anim : animations_)
	//{
	//	MV1DeleteModel(anim.second.model);
	//}
}

void ControllerAnimation::Add(const int type, const int handle, const float speed)
{
	// アニメーション情報の登録
	Animation anim;
	anim.model = handle;	//モデルハンドル
	anim.animIndex = type;	//アニメーションの種類(番号)
	anim.speed = speed;		//アニメーション速度

	//登録されていない場合
	if (animations_.count(type) == 0)
	{
		//新しいアニメーションとして追加
		animations_.emplace(type, anim);
	}
	//既に登録されている場合
	else
	{
		//既存のアニメーションのデータを更新
		animations_[type].model = anim.model;
		animations_[type].animIndex = anim.animIndex;
		animations_[type].attachNo = anim.attachNo;
		animations_[type].totalTime = anim.totalTime;
	}
}

void ControllerAnimation::Play(const int type, const bool isLoop, 
	const float startStep, const float endStep, const bool isStop, const bool isForce)
{
	//前のアニメーションと違う、または強制再生の場合
	if (playType_ != type || isForce) 
	{
		//再生中のアニメーションがある場合
		if (playType_ != -1)
		{
			// モデルからアニメーションを外す
			//playAnim_.attachNo = MV1DetachAnim(modelId_, playAnim_.attachNo);

			// まだ登録されていないアニメーションなら
			if (preBlendAnimationRateMap_.find(type) == preBlendAnimationRateMap_.end())
			{
				//ブレンドアニメーション率を0で登録
				preBlendAnimationRateMap_.emplace(type, 0.0f);
			}
			//ブレンドアニメーションを追加
			preBlendAnimationRateMap_.emplace(playType_, blendAnimRate_);

			// ブレンドアニメーション率を初期化
			blendAnimRate_ = 0.0f;
		}

		// アニメーション種別を変更
		playType_ = type;
		playAnim_ = animations_[type];

		// 初期化
		playAnim_.step = startStep;

		// モデルにアニメーションを付ける
		int animIdx = 0;
		if (MV1GetAnimNum(playAnim_.model) > 1)
		{
			// アニメーションが複数保存されていたら、番号1を指定
			animIdx = 1;
		}
		playAnim_.attachNo = MV1AttachAnim(modelId_, animIdx, playAnim_.model);

		// アニメーション総時間の取得
		if (endStep > 0.0f)
		{
			playAnim_.totalTime = endStep;
		}
		else
		{
			playAnim_.totalTime = MV1GetAttachAnimTotalTime(modelId_, playAnim_.attachNo);
		}

		// アニメーションループ
		isLoop_ = isLoop;

		// アニメーションしない
		isStop_ = isStop;

		stepEndLoopStart_ = -1.0f;
		stepEndLoopEnd_ = -1.0f;
		switchLoopReverse_ = 1.0f;
	}
}

void ControllerAnimation::Update(void)
{
	// 経過時間の取得
	deltaTime_ = SceneManager::GetInstance().GetDeltaTime();

	// メインアニメーション更新
	UpdateMainAnimation();

	// ブレンドアニメーション更新
	UpdateBlendAnimation();
}

void ControllerAnimation::SetEndLoop(float startStep, float endStep, float speed)
{
	stepEndLoopStart_ = startStep;
	stepEndLoopEnd_ = endStep;
	endLoopSpeed_ = speed;
}

int ControllerAnimation::GetPlayType(void) const
{
	return playType_;
}

bool ControllerAnimation::IsEnd(void) const
{

	bool ret = false;

	if (isLoop_)
	{
		// ループ設定されているなら、
		// 無条件で終了しないを返す
		return ret;
	}

	if (playAnim_.step >= playAnim_.totalTime)
	{
		// 再生時間を過ぎたらtrue
		return true;
	}

	return ret;

}

void ControllerAnimation::DebugDraw(void) const
{
	int marginY = 20;
	int index = 0;
	for (auto it = preBlendAnimationRateMap_.begin(); it != preBlendAnimationRateMap_.end(); )
	{
		DrawFormatString(0, marginY, 0xff0000,  L"animType:%d", it->first);
		DrawFormatString(200, marginY, 0xff0000, L"blendRate:%2f", it->second);
		marginY += 20;
		it++;
	}
}

void ControllerAnimation::UpdateMainAnimation(void)
{
	if (!isStop_)
	{
		// 再生
		playAnim_.step += (deltaTime_ * playAnim_.speed * switchLoopReverse_);

		// アニメーション終了判定
		bool isEnd = false;
		if (switchLoopReverse_ > 0.0f)
		{
			// 通常再生の場合
			if (playAnim_.step > playAnim_.totalTime)
			{
				isEnd = true;
			}
		}
		else
		{
			// 逆再生の場合
			if (playAnim_.step < playAnim_.totalTime)
			{
				isEnd = true;
			}
		}

		if (isEnd)
		{
			// アニメーションが終了したら
			if (isLoop_)
			{
				// ループ再生
				if (stepEndLoopStart_ > 0.0f)
				{
					// アニメーション終了後の指定フレーム再生
					switchLoopReverse_ *= -1.0f;
					if (switchLoopReverse_ > 0.0f)
					{
						playAnim_.step = stepEndLoopStart_;
						playAnim_.totalTime = stepEndLoopEnd_;
					}
					else
					{
						playAnim_.step = stepEndLoopEnd_;
						playAnim_.totalTime = stepEndLoopStart_;
					}
					playAnim_.speed = endLoopSpeed_;
				}
				else
				{
					// 通常のループ再生
					playAnim_.step = 0.0f;
				}
			}
			else
			{
				// ループしない
				playAnim_.step = playAnim_.totalTime;
			}
		}
	}

	// アニメーション設定
	MV1SetAttachAnimTime(modelId_, playAnim_.attachNo, playAnim_.step);
}

void ControllerAnimation::UpdateBlendAnimation(void)
{
	// ブレンドアニメーションが登録されているなら
	if (static_cast<int>(preBlendAnimationRateMap_.size()) <= 1)
	{
		return;
	}

	// ブレンドアニメーション率を増加
	blendAnimRate_ += deltaTime_ * BLEND_SPEED;
	if (blendAnimRate_ >= BLEND_ANIM_TIME)
	{
		blendAnimRate_ = BLEND_ANIM_TIME;
	}

	//ブレンド進行率を計算
	float blendRate = blendAnimRate_ / 1.0f;

	// 登録されているブレンドアニメーション率を更新
	for (auto it = preBlendAnimationRateMap_.begin(); it != preBlendAnimationRateMap_.end(); )
	{
		//変更後のアニメーションの場合
		if (it->first == playType_)
		{
			//ブレンドアニメーション率を増加
			it->second += (1.0 - it->second) * blendRate;

			//アニメーションのアタッチ
			MV1SetAttachAnimBlendRate(modelId_, it->first, it->second);
		}
		//変更前のアニメーションの場合
		else
		{
			//ブレンドアニメーション率を減少
			it->second -= it->second * blendRate;

			// ブレンドアニメーション率が0以下になったら
			if (it->second <= 0.0001f)
			{
				//アニメーションのデタッチ
				MV1DetachAnim(modelId_, it->first);

				// ブレンドアニメーション率が0以下になったら、リストから削除
				it = preBlendAnimationRateMap_.erase(it);
				continue;
			}

			//アニメーションのアタッチ
			MV1SetAttachAnimBlendRate(modelId_, it->first, it->second);
		}
		++it;
	}
}
