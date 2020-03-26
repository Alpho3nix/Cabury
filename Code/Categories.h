#pragma once

enum Category
{
	None = 0,
	Scene = 1 << 0,
	PlayerChar = 1 << 1,
	Enemies = 1 << 2,
	NPC = 1 << 3,
	item = 1 << 4,
	Bosses = 1 << 5,
	platform = 1 << 6,
	projectile = 1 << 7,
	breakableBox = 1 << 8,
	ladder = 1 << 9,
	cabury = 1 << 10,
	hitBoxes = 1 << 11,
	cage = 1 << 12,
	damagable = Enemies | Bosses | breakableBox | cabury,
	entities = Enemies | Bosses | item | NPC | PlayerChar | cabury,
};