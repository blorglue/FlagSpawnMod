class MySpawnPoint
{
	string Name;
	string Position;   // "x y z"

	void MySpawnPoint(string name = "", string position = "")
	{
		Name = name;
		Position = position;
	}
}

class MySpawnPointConfig
{
	ref array<ref MySpawnPoint> Points = new array<ref MySpawnPoint>;
}

class MySpawnPointStore
{
	static const string FOLDER = "$profile:MyFirstMod";
	static const string FILE_PATH = "$profile:MyFirstMod/spawnpoints.json";

	static ref MySpawnPointConfig s_Config;

	static MySpawnPointConfig Get()
	{
		if (!s_Config)
			Load();
		return s_Config;
	}

	static void Load()
	{
		s_Config = new MySpawnPointConfig;

		if (FileExist(FILE_PATH))
		{
			JsonFileLoader<MySpawnPointConfig>.JsonLoadFile(FILE_PATH, s_Config);
			return;
		}

		// first run: write an example file for you to edit
		s_Config.Points.Insert(new MySpawnPoint("Chernogorsk", "6387.43 0 2699.47"));
		s_Config.Points.Insert(new MySpawnPoint("Elektrozavodsk", "10322.40 0 2171.15"));
		Save();
	}

	static void Save()
	{
		if (!FileExist(FOLDER))
			MakeDirectory(FOLDER);

		JsonFileLoader<MySpawnPointConfig>.JsonSaveFile(FILE_PATH, s_Config);
	}

	static void SendToClient(PlayerIdentity identity)
	{
		if (!identity)
			return;

		GetRPCManager().SendRPC("RPC", "RPC_Receive_MySpawnPoints", new Param1<ref MySpawnPointConfig>(Get()), true, identity);
	}
}