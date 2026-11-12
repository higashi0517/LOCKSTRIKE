#pragma once

#include "Camera.h"
#include "Enemy.h"
#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <vector>

class Object3DManager;
class Collider;
struct SkinCluster;

// 指定フレームに敵を発生させ、更新・描画・破棄をまとめて管理する
class EnemySpawnController
{
public:
	struct SpawnEvent
	{
		int frame = 0;
		Vector3 position = { 0.0f, 0.0f, 0.0f };
		Vector3 entranceStart{};
		int entranceDuration = 0;
	};

	void Initialize(
		Object3DManager* object3DManager,
		const std::string& modelFileName);
	void AddSpawnEvent(int frame, const Vector3& position);
	void AddSpawnEvent(int frame, const Vector3& position, const Vector3& entranceStart, int entranceDuration);
	void Update(const Vector3& playerPosition);
	void Draw(const SkinCluster& skinCluster);
	void Reset();

	const Enemy* GetNearestEnemy(const Vector3& position) const;
	const Enemy* GetLockOnTarget(
		const Camera& camera,
		const Vector2& reticlePosition,
		float lockOnRadius) const;
	bool ContainsEnemy(const Enemy* target) const;
	bool HitEnemy(const Collider& attackCollider);
	bool IsCollision(const Collider& collider) const;

	const Enemy& GetEnemyAt(std::size_t index) const { return *enemies_.at(index); }
	void EditEnemyTransform(
		const Enemy& target,
		const std::optional<Vector3>& position,
		const std::optional<Vector3>& rotation);
	std::size_t GetEnemyCount() const { return enemies_.size(); }
	int GetElapsedFrames() const { return elapsedFrames_; }
	void SetDespawnDistance(float distance) { despawnDistance_ = distance; }

private:
	void Spawn(const SpawnEvent& event);

	Object3DManager* object3DManager_ = nullptr;
	std::string modelFileName_;
	std::vector<SpawnEvent> spawnEvents_;
	std::vector<std::unique_ptr<Enemy>> enemies_;
	std::size_t nextSpawnIndex_ = 0;
	int elapsedFrames_ = 0;
	float despawnDistance_ = 60.0f;
};
