#include "HomingProjectile.h"

#include "Enemy.h"
#include "EnemySpawnController.h"
#include "SpriteManager.h"
#include <cmath>

void HomingProjectile::Initialize(
	SpriteManager* spriteManager,
	EnemySpawnController* enemySpawnController,
	const Enemy* target,
	const Vector3& position,
	const Vector3& initialVelocity)
{
	enemySpawnController_ = enemySpawnController;
	target_ = target;

	const float initialSpeed = std::sqrt(
		initialVelocity.x * initialVelocity.x +
		initialVelocity.y * initialVelocity.y +
		initialVelocity.z * initialVelocity.z);
	if (initialSpeed > 0.0001f) {
		speed_ = initialSpeed;
	}

	Projectile::Initialize(
		spriteManager,
		"Resources/white1x1.png",
		ProjectileOwner::Player,
		position,
		initialVelocity,
		{ 1.0f, 0.45f, 0.05f, 1.0f });

	SetSpriteAppearance({ 32.0f, 10.0f }, 0.0f);
}

void HomingProjectile::Update(const Camera& camera)
{
	if (!IsActive()) {
		return;
	}

	if (target_ != nullptr &&
		enemySpawnController_ != nullptr &&
		enemySpawnController_->ContainsEnemy(target_)) {
		Vector3 targetPosition = target_->GetTranslate();
		// 機体の中心へ追尾する。
		const Vector3 toTarget = {
			targetPosition.x - GetPosition().x,
			targetPosition.y - GetPosition().y,
			targetPosition.z - GetPosition().z
		};
		const float targetLength = std::sqrt(
			toTarget.x * toTarget.x +
			toTarget.y * toTarget.y +
			toTarget.z * toTarget.z);

		if (targetLength > 0.0001f) {
			const Vector3 desiredDirection = {
				toTarget.x / targetLength,
				toTarget.y / targetLength,
				toTarget.z / targetLength
			};
			const float currentLength = std::sqrt(
				GetVelocity().x * GetVelocity().x +
				GetVelocity().y * GetVelocity().y +
				GetVelocity().z * GetVelocity().z);
			Vector3 currentDirection = desiredDirection;
			if (currentLength > 0.0001f) {
				currentDirection = {
					GetVelocity().x / currentLength,
					GetVelocity().y / currentLength,
					GetVelocity().z / currentLength
				};
			}

			const Vector3 blendedDirection = {
				currentDirection.x * (1.0f - homingRate_) +
					desiredDirection.x * homingRate_,
				currentDirection.y * (1.0f - homingRate_) +
					desiredDirection.y * homingRate_,
				currentDirection.z * (1.0f - homingRate_) +
					desiredDirection.z * homingRate_
			};
			const float blendedLength = std::sqrt(
				blendedDirection.x * blendedDirection.x +
				blendedDirection.y * blendedDirection.y +
				blendedDirection.z * blendedDirection.z);
			if (blendedLength > 0.0001f) {
				SetVelocity({
					blendedDirection.x / blendedLength * speed_,
					blendedDirection.y / blendedLength * speed_,
					blendedDirection.z / blendedLength * speed_
				});
			}
		}
	}
	else {
		target_ = nullptr;
	}

	Projectile::Update(camera);
	if (!IsActive() || !IsVisible()) {
		return;
	}

	Vector2 currentScreen{};
	Vector2 nextScreen{};
	float currentClipW = 0.0f;
	float nextClipW = 0.0f;
	const Vector3 nextPosition = {
		GetPosition().x + GetVelocity().x * 8.0f,
		GetPosition().y + GetVelocity().y * 8.0f,
		GetPosition().z + GetVelocity().z * 8.0f
	};
	if (WorldPositionToScreen(
		camera, GetPosition(), currentScreen, currentClipW) &&
		WorldPositionToScreen(
			camera, nextPosition, nextScreen, nextClipW)) {
		const float angle = std::atan2(
			nextScreen.y - currentScreen.y,
			nextScreen.x - currentScreen.x);
		float size = 180.0f / currentClipW;
		if (size < 10.0f) {
			size = 10.0f;
		}
		if (size > 34.0f) {
			size = 34.0f;
		}
		SetSpriteAppearance({ size * 1.8f, size * 0.65f }, angle);
	}
}
