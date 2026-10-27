#include "GameOverScene.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include <string>
#include <algorithm>

void GameOverScene::Initialize(WinApp* winApp, GraphicsDevice* graphicsDevice)
{
	effectFrames_ = 0;
	input_ = std::make_unique<Input>();
	input_->Initialize(winApp);
	// シーンを切り替えた瞬間の押しっぱなしを、新しい決定入力にしない。
	input_->Update();
	spriteManager_ = std::make_unique<SpriteManager>();
	spriteManager_->Initialize(graphicsDevice);
	gameUi_ = std::make_unique<GameUi>();
	gameUi_->Initialize(spriteManager_.get());
	const char* path = "Resources/UI/game-over.png";
	TextureManager::GetInstance()->LoadTexture(path);
	const auto& metadata = TextureManager::GetInstance()->GetMetaData(path);
	screen_ = std::make_unique<Sprite>();
	screen_->Initialize(spriteManager_.get(), path);
	screen_->SetTextureSize({float(metadata.width), float(metadata.height)});
	screen_->SetSize({1280.0f, 720.0f});
	screen_->SetPosition({0.0f, 0.0f});
	screen_->Update();
}

void GameOverScene::Update()
{
	input_->Update();
	// 演出中に押されたキーは持ち越さず、表示後の新しい入力だけ受け付ける。
	if (effectFrames_ < kRevealFrames) {
		++effectFrames_;
		return;
	}
	if (input_->TriggerKey(DIK_RETURN)) {
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}
	else if (input_->TriggerKey(DIK_ESCAPE)) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
}

void GameOverScene::Draw()
{
	spriteManager_->SetCommonRenderState();
	const float t = std::clamp(float(effectFrames_) / float(kRevealFrames), 0.0f, 1.0f);
	const float eased = 1.0f - (1.0f - t) * (1.0f - t) * (1.0f - t);
	const float zoom = 1.0f + 0.06f * (1.0f - eased);
	screen_->SetSize({1280.0f * zoom, 720.0f * zoom});
	screen_->SetPosition({640.0f * (1.0f - zoom), 360.0f * (1.0f - zoom)});
	screen_->Update();
	screen_->Draw();
	gameUi_->Begin();
	gameUi_->Rect(0, 0, 1280, 720, {0.0f, 0.0f, 0.0f, 1.0f - eased});
	const Vector4 white{0.92f, 0.96f, 1.0f, eased};
	gameUi_->CenteredText("TARGETS " + std::to_string(result_.GetDefeated()) +
		" / 5    HP " + std::to_string(result_.GetHealth()) + " / 3    TIME " +
		std::to_string(result_.GetRemainingSeconds()) + "s", 652, 1.3f, white);
	gameUi_->CenteredText(result_.GetHealth() == 0 ? "HP ZERO" : "TIME UP", 682, 1.0f, white);
}

void GameOverScene::Finalize()
{
	gameUi_.reset();
	screen_.reset();
	spriteManager_.reset();
	input_.reset();
}
