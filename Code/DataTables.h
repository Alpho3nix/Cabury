#pragma once
#include "ResourceIdentifiers.h"
#include "Animation.h"

namespace EnemyAnim {
	enum Type
	{
		Idle = 0, Walk, beingHit, Attack, Death, AnimCount,
		IdleRight = 0, IdleLeft, WalkRight, WalkLeft, beingHitRight, beingHitLeft, AttackRight, AttackLeft, DeathRight, DeathLeft, AnimCount2
	};
}

namespace NPCAnim {
	enum Type
	{
		Idle, BeginInteractTransition, BeginInteractIdle, AnimCount
	};
}

namespace BossAnim
{
	enum Type
	{
		Idle = 0, Walk, beingHit, Attack_1, Attack_2, Attack_3, Death, JumpReady, JumpLoop, Fall, AnimCount,
		IdleRight = 0, IdleLeft, WalkRight, WalkLeft, beingHitRight, beingHitLeft, Attack_1Right, Attack_1Left, Attack_2Right, Attack_2Left, Attack_3Right, Attack_3Left, DeathRight, DeathLeft,
		Jump_readyRight, Jump_readyLeft, Jump_loopRight, Jump_loopLeft, FallRight, FallLeft, AnimCount2
	};

}

enum ReadDir
{
	Right, Left, Count
};

struct EnemyAnimationData
{
	EnemyAnimationData(sf::Time* durations, sf::IntRect* texRects, size_t* numFrames, sf::Texture* texture, size_t frameNumber);
	EnemyAnimationData();

	sf::Time fDurations[EnemyAnim::AnimCount];
	sf::IntRect fTextureRects[EnemyAnim::AnimCount2];
	size_t fNumFrames[EnemyAnim::AnimCount];
	size_t frameNumberToAttack;
	sf::Texture* fTexture;
	int fReadDir[Count];
};

struct BossAnimationData
{
	BossAnimationData(sf::Time* durations, sf::IntRect* texRects, size_t* numFrames, sf::Texture* texture);
	BossAnimationData();

	sf::Time fDurations[BossAnim::AnimCount];
	sf::IntRect fTextureRects[BossAnim::AnimCount2];
	size_t fNumFrames[BossAnim::AnimCount];
	sf::Texture* fTexture;
	int fReadDir[Count];
};

struct NPCAnimData
{
	NPCAnimData(sf::Time* durations, sf::IntRect* texRects, size_t* numFrames, sf::Texture* texture);
	NPCAnimData();

	sf::Time fDurations[NPCAnim::AnimCount];
	sf::IntRect fTextureRects[NPCAnim::AnimCount];
	size_t fNumFrames[NPCAnim::AnimCount];
	sf::Texture* fTexture;
	int fReadDir[Count];
};

struct PlayerData
{
	PlayerData(sf::Vector2f spawnPoint);

	sf::Vector2f fSpawnPoint;
};