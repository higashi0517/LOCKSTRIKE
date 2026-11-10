#pragma once

#include "Camera.h"
#include "Sprite.h"
#include <memory>
#include <array>

class GameUi;
class SpriteManager;
class WinApp;

// カメラ前方の3D座標を画面へ投影して表示する照準
class Reticle3D
{
public:
	void Initialize(SpriteManager* spriteManager, WinApp* winApp);
	void Update(const Camera& camera);
	void Draw(GameUi& ui);
	void SetLocked(bool isLocked) { isLocked_ = isLocked; }
	void UpdateLockMarker(const Camera& camera, const Vector3* targetPosition);
	void DrawDebug(
		const Matrix4x4& viewProjectionMatrix,
		const Vector3& playerPosition,
		float screenWidth,
		float screenHeight) const;

	const Vector3& GetWorldPosition() const { return worldPosition_; }
	const Vector2& GetScreenPosition() const { return screenPosition_; }
	bool IsVisible() const { return isVisible_; }
	bool IsLocked() const { return isLocked_; }

private:
	bool WorldToScreen(
		const Camera& camera,
		const Vector3& worldPosition,
		Vector2& screenPosition) const;
	std::array<std::unique_ptr<Sprite>, 8> lockCorners_;
	std::array<std::unique_ptr<Sprite>, 8> lockCornerOutlines_;
	std::array<std::unique_ptr<Sprite>, 7> lockLabel_;
	std::unique_ptr<Sprite> lockLabelBackground_;
	bool isLockMarkerVisible_ = false;
	float lockPulse_ = 0.0f;

	WinApp* winApp_ = nullptr;
	Vector3 worldPosition_ = { 0.0f, 0.0f, 0.0f };
	Vector2 screenPosition_ = { 640.0f, 360.0f };
	float distance_ = 20.0f;
	bool isVisible_ = false;
	bool isLocked_ = false;
};
