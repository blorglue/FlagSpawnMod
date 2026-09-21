/*
	[GENERAL MODDING INFO]

	Repair Kit Types
	    1: Weapon Cleaning Kit
	    2: Sewing Kit
	    3: Leather Sewing Kit
	    4: Whetstone
	    5: Duct Tape
	    6: Tire Repair Kit
	    7: Electronic Repair Kit
	    8: Epoxy Putty
		9: DOESN'T EXIT
		10: Blowtorch

		USAGE:
		repairableWithKits[] += {5};
		repairCosts[] += {20};

	Item Rotation Flags
	    ROTATE_Front		=	1
	    ROTATE_Top			=	2 
	    ROTATE_Left			=	4 
	    ROTATE_Right		=	8 
	    ROTATE_LeftRight	=	12 
	    ROTATE_Back			=	16
	    ROTATE_FrontBack	=	17 
	    ROTATE_Bottom		=	32 
	    ROTATE_BottomTop	=	34 
	    ROTATE_All			=	63 
	    ROTATE_Ignore		=	64

		USAGE: 
		rotationFlags=16;
*/

class CfgPatches
{
	class MyFirstMod
	{
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Scripts",
			"DZ_Gear_Drinks",
			"DZ_Gear_Medical"
		};
	};
};

class CfgMods
{
	class MyFirstMod
	{
		author = "";
		type = "mod";
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = 
				{ 
					"MyFirstMod/Scripts/3_Game",
					"MyFirstMod/Scripts/Common"
				};
			};
			class worldScriptModule
			{
				value = "";
				files[] = 
				{ 
					"MyFirstMod/Scripts/4_World",
					"MyFirstMod/Scripts/Common"
				};
			};
			class missionScriptModule
			{
				value = "";
				files[] = 
				{ 
					"MyFirstMod/Scripts/5_Mission",
					"MyFirstMod/Scripts/Common"
				};
			};
		};
	};
};


// Weapon magazines
class CfgMagazines 
{
};

// Ballistic weapons
class CfgWeapons
{
};

// Ammo config
class CfgAmmo
{
	class DefaultAmmo;
};

// Ammo types
class CfgAmmoTypes
{
};

