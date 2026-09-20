class MyFlagData
{
	string Position;                                  // "x y z"
	ref array<string> Owners = new array<string>;     // player IDs, filled in the next step
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

	static void AddFlag(vector pos)
	{
		string posStr = pos.ToString(false);

		foreach (MyFlagData existing : Get().Flags)
		{
			if (vector.Distance(existing.Position.ToVector(), pos) <= 1.5)
				return;   // already saved
		}

		MyFlagData flag = new MyFlagData;
		flag.Position = posStr;
		Get().Flags.Insert(flag);
		Save();

		Print("[MyFirstMod] Saved flag at " + posStr);
	}
	
	static bool ClaimFlag(vector flagPos, string playerId){
	
	foreach (MyFlagData flag : Get().Flags)
	{
		if (vector.Distance(flag.Position.ToVector(), flagPos) > 1.5)
			continue;                              // not this flag, keep looking

		if (flag.Owners.Find(playerId) != -1)
			return false;                          // already an owner

		flag.Owners.Insert(playerId);
		Save();
		return true;
	}

	return false;   
	
	
	}
	
	static void RemoveFlag(vector flagPos){
		
	for (int i = Get().Flags.Count() - 1; i >= 0; i--)
	{
		if (vector.Distance(Get().Flags[i].Position.ToVector(), flagPos) <= 1.5)
		{
			Get().Flags.Remove(i);
			Save();
			Print("[MyFirstMod] Removed flag at " + flagPos);
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