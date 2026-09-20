modded class MissionBase
{
	void MissionBase()
	{
		#ifdef SERVER
		// SERVER RECEIVE RPCs
       GetRPCManager().AddRPC("RPC", "RPC_Receive_SpawnAtFlag", this, SingeplayerExecutionType.Server);
		#else
		// CLIENT RECEIVE RPCs
        GetRPCManager().AddRPC("RPC", "RPC_Receive_MyFirstModConfig", this, SingeplayerExecutionType.Client);
		GetRPCManager().AddRPC("RPC", "RPC_Receive_MyFlags", this, SingeplayerExecutionType.Client);
		GetRPCManager().AddRPC("RPC", "RPC_Receive_MySpawnPoints", this, SingeplayerExecutionType.Client);
		#endif
	}

	void RPC_Receive_MySpawnPoints(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		Param1<ref MySpawnPointConfig> data;
		if (!ctx.Read(data))
			return;

		MyFlagClientCache.Presets = data.param1.Points;
		Print("[MyFirstMod] Received " + MyFlagClientCache.Presets.Count() + " preset spawn points");
	}

	void RPC_Receive_MyFlags(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
{
	Param1<ref array<string>> data;
	if (!ctx.Read(data))
		return;

	MyFlagClientCache.Owned = data.param1;
	Print("[MyFirstMod] Received " + MyFlagClientCache.Owned.Count() + " owned flags");
}
	void RPC_Receive_SpawnAtFlag(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
{
	if (type != CallType.Server || !sender)
		return;

	Param1<string> data;
	if (!ctx.Read(data))
		return;

	// only teleport to a flag this player owns, or to a preset from spawnpoints.json
	string requested = data.param1;
	bool allowed = MyFlagStore.GetOwnedPositions(sender.GetPlainId()).Find(requested) != -1;

	if (!allowed)
	{
		foreach (MySpawnPoint preset : MySpawnPointStore.Get().Points)
		{
			if (preset.Position == requested)
			{
				allowed = true;
				break;
			}
		}
	}

	if (!allowed)
	{
		Print("[MyFirstMod] Rejected spawn request from " + sender.GetPlainId() + " for " + requested);
		return;
	}

	// find this sender's living player object
	PlayerBase player;
	array<Man> players = new array<Man>;
	GetGame().GetPlayers(players);
	foreach (Man man : players)
	{
		if (man.GetIdentity() && man.GetIdentity().GetPlainId() == sender.GetPlainId())
		{
			player = PlayerBase.Cast(man);
			break;
		}
	}

	if (!player || !player.IsAlive())
		return;

	vector flagPos = requested.ToVector();
	if (flagPos[0] == 0 && flagPos[2] == 0)
	{
		Print("[MyFirstMod] Rejected spawn at invalid position: " + requested);
		return;
	}

	float x = flagPos[0] + 3;
	float z = flagPos[2];
	if (GetGame().SurfaceIsSea(x, z))
	{
		Print("[MyFirstMod] Rejected spawn in water at: " + requested);
		return;
	}

	player.SetPosition(Vector(x, GetGame().SurfaceY(x, z), z));

	Print("[MyFirstMod] Teleported " + sender.GetPlainId() + " to " + requested);
}

	override UIScriptedMenu CreateScriptedMenu(int id)
    {
        UIScriptedMenu menu = super.CreateScriptedMenu(id);

        if (!menu)
        {
            switch (id)
            {
                /*
				case YourConstantsFile.YOUR_GUI_ID:
                {
                    menu = new CustomGUI();
                    break;
                }
                */
            }
        }

        return menu;
    }

	// Server -> client
    void RPC_Receive_MyFirstModConfig(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        Param1<ref MyFirstModConfig> data;
        if (!ctx.Read(data))
        {
            Error("Error sync'ing server-side data to client - RPC_Receive_MyFirstModConfig");
            return;
        }

        m_MyFirstModConfig = data.param1;

        Print("[MyFirstMod] Received config sync: " + GetMyFirstModConfig().ExampleConfig);

        for (int i = 0; i < GetMyFirstModConfig().ExampleArrayConfig.Count(); i++)
        {
            MyFirstModCustomConfig customConfig = GetMyFirstModConfig().ExampleArrayConfig.Get(i);
            Print("[MyFirstMod] Received no build zone: " + customConfig.Name + " - dist = " + customConfig.NoBuildZone + " @ " + customConfig.Location);

            for (int x = 0; x < customConfig.Whitelist.Count(); x++)
            {
                Print("[" + customConfig.Name + "] Whitelist Item name=" + customConfig.Whitelist.Get(x));
            }
        }
    }
}