#pragma once
#include "Entity.h"
#include <functional>
#include "UtilityFunc.h"

struct ItemType;

enum ItemIndices
{
	Coins, Diamonds, Helmets, Swords, Candies, Books, IndicesCount
};

class Item : public Entity
{
public:
	enum Type : int64_t
	{
		GreenArrow_1 = 1 << 0, YellowArrow_1 = 1 << 1, BlueArrow_1 = 1 << 2, RedArrow_1 = 1 << 3,
		GreenArrow_2 = 1 << 4, YellowArrow_2 = 1 << 5, BlueArrow_2 = 1 << 6, RedArrow_2 = 1 << 7,
		GreenArrow_3 = 1 << 8, YellowArrow_3 = 1 << 9, BlueArrow_3 = 1 << 10, RedArrow_3 = 1 << 11,
		NormalBow_1 = 1 << 12, SilverBow_1 = 1 << 13, DarkBow_1 = 1 << 14, GoldBow_1 = 1 << 15, RedBow_1 = 1 << 16,
		NormalBow_2 = 1 << 17, SilverBow_2 = 1 << 18, DarkBow_2 = 1 << 19, GoldBow_2 = 1 << 20, RedBow_2 = 1 << 21,
		NormalBow_3 = 3 << 22, SilverBow_3 = 1 << 23, DarkBow_3 = 1 << 24, GoldBow_3 = 1 << 25, RedBow_3 = 1 << 26,
		RedCandieBar = 1 << 27, BlueCandieBar = 1 << 28, GreenCandieBar = 1 << 29, PurpleCandieBar = 1 << 30, OrangeCandiBar = 1 << 31, PinkCandieBar = (1 << 31) | (1 << 0), SpecialCandieBar = (1 << 31) | (1 << 1),
		RedCandie = (1 << 31) | (1 << 2), BlueCandie = (1 << 31) | (1 << 3), PurpleCandie = (1 << 31) | (1 << 4), OrangeCandie = (1 << 31) | (1 << 5), PinkCandie = (1 << 31) | (1 << 6), SpecialCandie = (1 << 31) | (1 << 7),
		BrownCoin = (1 << 31) | (1 << 8), OrangeCoin = (1 << 31) | (1 << 9), SilverCoin = (1 << 31) | (1 << 10), GoldCoin = (1 << 31) | (1 << 11), BlueCoin = (1 << 31) | (1 << 12),
		BrownCoin_stack1 = (1 << 31) | (1 << 13), OrangeCoin_stack1 = (1 << 31) | (1 << 14), SilverCoin_stack1 = (1 << 31) | (1 << 15), GoldCoin_stack1 = (1 << 31) | (1 << 16), BlueCoin_stack1 = (1 << 31) | (1 << 17),
		BrownCoin_stack2 = (1 << 31) | (1 << 18), OrangeCoin_stack2 = (1 << 31) | (1 << 19), SilverCoin_stack2 = (1 << 31) | (1 << 20), GoldCoin_stack2 = (1 << 31) | (1 << 21), BlueCoin_stack2 = (1 << 31) | (1 << 22),
		BrownCoin_stack3 = (1 << 31) | (1 << 23), OrangeCoin_stack3 = (1 << 31) | (1 << 24), SilverCoin_stack3 = (1 << 31) | (1 << 25), GoldCoin_stack3 = (1 << 31) | (1 << 26), BlueCoin_stack3 = (1 << 31) | (1 << 27),
		BrownCoin_stack4 = (1 << 31) | (1 << 28), OrangeCoin_stack4 = (1 << 31) | (1 << 29), SilverCoin_stack4 = (1 << 31) | (1 << 30), GoldCoin_stack4 = (1 << 31) | (1 << 30) | (1 << 0), BlueCoin_stack4 = (1 << 31) | (1 << 30) | (1 << 1),
		GreenDiamond = (1 << 31) | (1 << 30) | (1 << 2), YellowDiamond = (1 << 31) | (1 << 30) | (1 << 3), BlueDiamond = (1 << 31) | (1 << 30) | (1 << 4), RedDiamond = (1 << 31) | (1 << 30) | (1 << 5),
		SilverDiamond = (1 << 31) | (1 << 30) | (1 << 6), PinkDiamond = (1 << 31) | (1 << 30) | (1 << 7), OrangeDiamond = (1 << 31) | (1 << 30) | (1 << 8), BrownDiamond = (1 << 31) | (1 << 30) | (1 << 9),
		DarkDiamond = (1 << 31) | (1 << 30) | (1 << 10), PurpleDiamond = (1 << 31) | (1 << 30) | (1 << 11),
		OrangeFish = (1 << 31) | (1 << 30) | (1 << 12), RedFish = (1 << 31) | (1 << 30) | (1 << 13), YellowFish = (1 << 31) | (1 << 30) | (1 << 14), GreenFish = (1 << 31) | (1 << 30) | (1 << 15), BlueFish = (1 << 31) | (1 << 30) | (1 << 16),
		GreenGem = (1 << 31) | (1 << 30) | (1 << 17), YellowGem = (1 << 31) | (1 << 30) | (1 << 18), BlueGem = (1 << 31) | (1 << 30) | (1 << 19), RedGem = (1 << 31) | (1 << 30) | (1 << 20), SilverGem = (1 << 31) | (1 << 30) | (1 << 21),
		PinkGem = (1 << 31) | (1 << 30) | (1 << 22), OrangeGem = (1 << 31) | (1 << 30) | (1 << 23), BrownGem = (1 << 31) | (1 << 30) | (1 << 24), DarkGem = (1 << 31) | (1 << 30) | (1 << 25), PurpleGem = (1 << 31) | (1 << 30) | (1 << 26),
		BrownHelmet_1 = (1 << 31) | (1 << 30) | (1 << 27), SilverHelmet_1 = (1 << 31) | (1 << 30) | (1 << 28), BlueHelmet_1 = (1 << 31) | (1 << 30) | (1 << 29), YellowHelmet_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 0),
		RedHelmet_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 1), BrownHelmet_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 2), SilverHelmet_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 3), BlueHelmet_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 4),
		YellowHelmet_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 5), RedHelmet_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 6),
		BrownIngot = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 7), SilverIngot = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 8), GoldIngot = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 9), BlueIngot = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 10),
		BrownKey_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 11), OrangeKey_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 12), SilverKey_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 13), GoldKey_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 14),
		BlueKey_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 15), BrownKey_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 16), OrangeKey_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 17), SilverKey_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 18),
		GoldKey_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 19), BlueKey_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 20),
		BrownNecklace_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 21), SilverNecklace_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 22), DarkNecklace_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 23), GoldNecklace_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 24),
		RedNecklace_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 25), BrownNecklace_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 26), SilverNecklace_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 27), DarkNecklace_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28),
		GoldNecklace_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 0), RedNecklace_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 1), BrownNecklace_3 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 2),
		SilverNecklace_3 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 3), DarkNecklace_3 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 4), GoldNecklace_3 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 5),
		RedNecklace_3 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 6),
		GraySphere = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 7), BlueSphere = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 8), RedSphere = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 9),
		GreenSphere = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 10), YellowSphere = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 11),
		RedTubePotion = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 12), BlueTubePotion = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 13), GreenTubePotion = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 14),
		DarkTubePotion = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 15), PurpleTubePotion = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 16), OrangeTubePotion = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 17),
		PinkTubePotion = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 18), WhiteTubePotion = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 19),
		RedSpherePotion = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 20), BlueSpherePotion = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 21), GreenSpherePotion = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 22),
		DarkSpherePotion = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 23), PurpleSpherePotion = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 24), OrangeSpherePotion = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 25),
		PinkSpherePotion = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 26), WhiteSpherePotion = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27),
		RedCubePotion = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 0), BlueCubePotion = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 1), GreenCubePotion = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 2),
		DarkCubePotion = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 3), PurpleCubePotion = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 4), OrangeCubePotion = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 5),
		PinkCubePotion = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 6), WhiteCubePotion = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 7),
		BrownRing_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 8), SilverRing_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 9), GrayRing_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 10),
		GoldRing_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 11), RedRing_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 12),
		BrownRing_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 13), SilverRing_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 14), GrayRing_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 15),
		GoldRing_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 16), RedRing_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 17),
		BrownRing_3 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 18), SilverRing_3 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 19), GrayRing_3 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 20),
		GoldRing_3 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 21), RedRing_3 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 22),
		RedParchment = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 23), GrayParchment = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 24), GreenParchment = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 25),
		BlueParchment = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26), SilverParchment = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 0), YellowParchment = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 1),
		PurpleParchment = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 2), BrownParchment = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 3),
		GreenShield_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 4), YellowShield_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 5),
		BlueShield_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 6), GoldShield_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 7),
		DarkShield_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 8),
		GreenShield_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 9), YellowShield_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 10),
		BlueShield_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 11), GoldShield_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 12),
		DarkShield_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 13),
		GreenShield_3 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 14), YellowShield_3 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 15),
		BlueShield_3 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 16), GoldShield_3 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 17),
		DarkShield_3 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 18),
		BrownBook_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 19), WhiteBook_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 20),
		GrayBook_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 21), YellowBook_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 22),
		RedBook_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 23),
		BrownBook_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 24), WhiteBook_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 0),
		GrayBook_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 1), YellowBook_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 2),
		RedBook_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 3),
		BrownBook_3 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 4), WhiteBook_3 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 5),
		GrayBook_3 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 6), YellowBook_3 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 7),
		RedBook_3 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 8),
		BrownWand_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 9), WhiteWand_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 10),
		GrayWand_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 11), GoldWand_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 12),
		RedWand_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 13),
		BrownWand_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 14), WhiteWand_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 15),
		GrayWand_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 16), GoldWand_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 17),
		RedWand_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 18),
		BrownWand_3 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 19), WhiteWand_3 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 20),
		GrayWand_3 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 21), GoldWand_3 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 22),
		RedWand_3 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 23),
		BrownSword_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 24), WhiteSword_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 24) | (1 << 0),
		SilverSword_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 24) | (1 << 1), GoldSword_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 24) | (1 << 2),
		RedSword_1 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 24) | (1 << 3),
		BrownSword_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 24) | (1 << 4), WhiteSword_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 24) | (1 << 5),
		SilverSword_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 24) | (1 << 6), GoldSword_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 24) | (1 << 7),
		RedSword_2 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 24) | (1 << 8),
		BrownSword_3 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 24) | (1 << 9), WhiteSword_3 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 24) | (1 << 10),
		SilverSword_3 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 24) | (1 << 11), GoldSword_3 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 24) | (1 << 12),
		RedSword_3 = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 24) | (1 << 13),

		//Coins = BrownCoin | OrangeCoin | SilverCoin | GoldCoin | BlueCoin,
		//Coins_1 = BrownCoin_stack1 | OrangeCoin_stack1 | SilverCoin_stack1 | GoldCoin_stack1 | BlueCoin_stack1,

	};
private:

	double ratio = 1.f;

	bool drawItem = true;
	bool canBePickedUp = false;
	Type type;
	sf::FloatRect worldBounds;

	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

	Utility::OutSide isOutOfTheWorld(const sf::FloatRect& pos) const;
	bool fBounce = false;
public:

	Item(int hp, const sf::Texture& tex, ItemType itemType);
	~Item();

	unsigned int getCategory() const override;
	void updateNode(sf::Time dt, CommandQueue& commands) override;

	void canPickedUp(bool flag);
	

	const sf::Sprite& getItemSprite() const;
	Type getType() const;
	const SceneNode* getParent() const;

	std::function<bool(class Player& player)> fEffectOnPlayer;
	bool fCanNotMove = false;
};

struct ItemType
{
	ItemType(Item::Type t, sf::IntRect rect, std::function<bool(Player& player)> effectOnPlayer, sf::FloatRect worldBounds, float upVel)
		:type(t), texRect(rect), effect(effectOnPlayer), worldBounds(worldBounds), upwardsVel(upVel) { }
	ItemType(Item::Type t, sf::IntRect rect, sf::FloatRect worldBounds, float upVel)
		:type(t), texRect(rect), effect([](Player& player) { return false; }), worldBounds(worldBounds), upwardsVel(upVel)
	{
		
	}
	ItemType()
		:type(Item::Type::GoldCoin), texRect(), effect([](Player& player) { return false; }), worldBounds(), upwardsVel(0.f)
	{
		
	}
	Item::Type type;
	sf::IntRect texRect;
	std::function<bool(Player& player)> effect;
	sf::FloatRect worldBounds;
	float upwardsVel;
};