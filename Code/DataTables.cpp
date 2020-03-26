#include "DataTables.h"

EnemyAnimationData::EnemyAnimationData(sf::Time * durations, sf::IntRect * texRects, size_t * numFrames, sf::Texture * texture, size_t frameNumber)
	:fTexture(texture), frameNumberToAttack(frameNumber)
{
	//be careful not to put durations, etc with not the same size as AnimCount
	for (size_t i = 0; i < EnemyAnim::AnimCount; ++i)
	{
		fDurations[i] = durations[i];
		fNumFrames[i] = numFrames[i];
		fTextureRects[i] = texRects[i];
	}

	for (size_t j = EnemyAnim::AnimCount; j < EnemyAnim::AnimCount2; ++j)
	{
		fTextureRects[j] = texRects[j];
	}

	fReadDir[Right] = 1;
	fReadDir[Left] = -1;
}

EnemyAnimationData::EnemyAnimationData()
	:fTexture(nullptr), frameNumberToAttack(0)
{
	for (size_t i = 0; i < EnemyAnim::AnimCount; ++i)
	{
		fDurations[i] = sf::seconds(0.f);
		fNumFrames[i] = 0;
		fTextureRects[i] = sf::IntRect();
	}

	for (size_t j = EnemyAnim::AnimCount; j < EnemyAnim::AnimCount2; ++j)
	{
		fTextureRects[j] = sf::IntRect();
	}

	fReadDir[Right] = 0;
	fReadDir[Left] = 0;
}

PlayerData::PlayerData(sf::Vector2f spawnPoint)
	:fSpawnPoint(spawnPoint)
{
}

NPCAnimData::NPCAnimData(sf::Time * durations, sf::IntRect * texRects, size_t * numFrames, sf::Texture * texture)
	:fTexture(texture)
{
	for (size_t i = 0; i < NPCAnim::AnimCount; ++i)
	{
		fDurations[i] = durations[i];
		fTextureRects[i] = texRects[i];
		fNumFrames[i] = numFrames[i];
	}
}

NPCAnimData::NPCAnimData()
	:fTexture(nullptr)
{
	for (size_t i = 0; i < NPCAnim::AnimCount; ++i)
	{
		fDurations[i] = sf::seconds(0.f);
		fTextureRects[i] = sf::IntRect();
		fNumFrames[i] = 0;
	}
}

BossAnimationData::BossAnimationData(sf::Time * durations, sf::IntRect * texRects, size_t * numFrames, sf::Texture * texture)
	:fTexture(texture)
{
	//be careful not to put durations, etc with not the same size as AnimCount
	for (size_t i = 0; i < BossAnim::AnimCount; ++i)
	{
		fDurations[i] = durations[i];
		fNumFrames[i] = numFrames[i];
		fTextureRects[i] = texRects[i];
	}

	for (size_t j = BossAnim::AnimCount; j < BossAnim::AnimCount2; ++j)
	{
		fTextureRects[j] = texRects[j];
	}

	fReadDir[Right] = 1;
	fReadDir[Left] = -1;
}

BossAnimationData::BossAnimationData()
	:fTexture(nullptr)
{
	//be careful not to put durations, etc with not the same size as AnimCount
	for (size_t i = 0; i < BossAnim::AnimCount; ++i)
	{
		fDurations[i] = sf::seconds(0.f);
		fNumFrames[i] = 0;
		fTextureRects[i] = sf::IntRect();
	}

	for (size_t j = BossAnim::AnimCount; j < BossAnim::AnimCount2; ++j)
	{
		fTextureRects[j] = sf::IntRect();
	}

	fReadDir[Right] = 0;
	fReadDir[Left] = 0;
}