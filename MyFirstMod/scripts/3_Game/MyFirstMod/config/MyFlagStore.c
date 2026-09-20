class MyFlagData
{
	string Position;                                  // "x y z"
	ref array<string> Owners = new array<string>;     // owner SteamID64s
	string PlacedBy;                                  // GUID of the player who placed the flag
}

class MyFlagDatabase
{
	ref array<ref MyFlagData> Flags = new array<ref MyFlagData>;
}

class MyFlagStore
{
	static const string FOLDER = "$profile:MyFirstMod";
	static const string FILE_PATH = "$profile:MyFirstMod/flags.json";

	static ref MyFlagDatabase s_Db;

	static MyFlagDatabase Get()
	{
		if (!s_Db)
			Load();
		return s_Db;
	}

	static void Load()
	{
		s_Db = new MyFlagDatabase;

		if (FileExist(FILE_PATH))
			JsonFileLoader<MyFlagDatabase>.JsonLoadFile(FILE_PATH, s_Db);
	}

	static void Save()
	{
		if (!FileExist(FOLDER)){
			Print("Creating folder");
			MakeDirectory(FOLDER);

		}
			
		JsonFileLoader<MyFlagDatabase>.JsonSaveFile(FILE_PATH, Get());
	}

	static void AddFlag(vector pos, string placerGuid = "")
	{
		string posStr = pos.ToString(false);

		foreach (MyFlagData existing : Get().Flags)
		{
			if (vector.Distance(existing.Position.ToVector(), pos) <= 1.5)
				return;   // already saved
		}

		MyFlagData flag = new MyFlagData;
		flag.Position = posStr;
		flag.PlacedBy = placerGuid;
		Get().Flags.Insert(flag);
		Save();

		Print("[MyFirstMod] Saved flag at " + posStr);
	}
	
	static string GetGuid(Man player)
	{
		if (player && player.GetIdentity())
			return player.GetIdentity().GetId();

		return "";
	}

	static MyFlagData FindFlag(vector flagPos)
	{
		foreach (MyFlagData flag : Get().Flags)
		{
			if (vector.Distance(flag.Position.ToVector(), flagPos) <= 1.5)
				return flag;
		}

		return null;
	}

	// A flag can be claimed by the player who placed it, or by anyone in the same faction as them.
	// Flags saved before PlacedBy existed (empty) can be claimed by anyone, and the first claimer becomes the placer.
	static bool CanClaim(MyFlagData flag, string claimerGuid)
	{
		if (flag.PlacedBy == "")
			return true;

		if (flag.PlacedBy == claimerGuid)
			return true;

		return MyFactionStore.AreTeammates(flag.PlacedBy, claimerGuid);
	}

	static bool ClaimFlag(vector flagPos, string steamId, string claimerGuid)
	{
		MyFlagData flag = FindFlag(flagPos);
		if (!flag)
			return false;                          // no saved flag near that position

		if (flag.Owners.Find(steamId) != -1)
			return false;                          // already an owner

		if (!CanClaim(flag, claimerGuid))
			return false;                          // not allowed

		if (flag.PlacedBy == "")
			flag.PlacedBy = claimerGuid;

		flag.Owners.Insert(steamId);
		Save();
		return true;
	}
	
	static void SendOwnedFlagsToPlayers(array<string> steamIds)
	{
		if (!steamIds || steamIds.Count() == 0)
			return;

		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);

		foreach (Man man : players)
		{
			PlayerIdentity identity = man.GetIdentity();
			if (identity && steamIds.Find(identity.GetPlainId()) != -1)
				SendOwnedFlags(identity);
		}
	}

	static void RemoveFlag(vector flagPos){
		
	for (int i = Get().Flags.Count() - 1; i >= 0; i--)
	{
		if (vector.Distance(Get().Flags[i].Position.ToVector(), flagPos) <= 1.5)
		{
			array<string> formerOwners = new array<string>;
			foreach (string ownerId : Get().Flags[i].Owners)
				formerOwners.Insert(ownerId);

			Get().Flags.Remove(i);
			Save();
			Print("[MyFirstMod] Removed flag at " + flagPos);

			// tell any owners who are online, so their spawn menu drops the deleted flag
			SendOwnedFlagsToPlayers(formerOwners);
			return;
		}
	}
}
	
	static array<string> GetOwnedPositions(string steamId)
{
	array<string> result = new array<string>;

	foreach (MyFlagData flag : Get().Flags)
	{
		if (flag.Owners.Find(steamId) != -1)
			result.Insert(flag.Position);
	}

	return result;
}

//Sending flag data to client
static void SendOwnedFlags(PlayerIdentity identity)
{
	if (!identity)
		return;

	array<string> ownedPositions = GetOwnedPositions(identity.GetPlainId());
	GetRPCManager().SendRPC("RPC", "RPC_Receive_MyFlags", new Param1<ref array<string>>(ownedPositions), true, identity);
}
}
class MyFlagClientCache
{
	static ref array<string> Owned = new array<string>;
	static ref array<ref MySpawnPoint> Presets = new array<ref MySpawnPoint>;
}