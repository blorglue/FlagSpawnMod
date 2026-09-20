class MyFactionData
{
	string Name;
	string LeaderID;
	string HQGrid;
	ref array<string> OwnedGrids = new array<string>;
	ref array<string> Members = new array<string>;
	ref array<string> SubLeaders = new array<string>;
	ref array<int> Color = new array<int>;
}

class MyFactionStore
{
	// Live file written by the TerritoryWars mod in the server profile (player GUIDs, not SteamIDs)
	static const string FILE_PATH = "$profile:TerritoryWars/factions.json";

	static bool IsInFaction(MyFactionData faction, string guid)
	{
		return faction.LeaderID == guid || faction.Members.Find(guid) != -1 || faction.SubLeaders.Find(guid) != -1;
	}

	// True if both GUIDs belong to the same faction. The file is re-read on every call
	// because TerritoryWars rewrites it whenever factions change.
	static bool AreTeammates(string guidA, string guidB)
	{
		if (guidA == "" || guidB == "")
			return false;

		if (!FileExist(FILE_PATH))
		{
			Print("[MyFirstMod] factions.json not found at " + FILE_PATH);
			return false;
		}

		map<string, ref MyFactionData> factions = new map<string, ref MyFactionData>;
		JsonFileLoader<map<string, ref MyFactionData>>.JsonLoadFile(FILE_PATH, factions);

		Print("[MyFirstMod] factions.json loaded " + factions.Count() + " factions");

		foreach (string factionName, MyFactionData faction : factions)
		{
			if (IsInFaction(faction, guidA) && IsInFaction(faction, guidB))
				return true;
		}

		return false;
	}
}
