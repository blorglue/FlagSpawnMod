// Server-side only code
modded class MissionServer
{
	override void OnInit()
	{
		super.OnInit();

		Print("[MyFirstMod] MissionServer::OnInit");

		// Load server-side config 
		GetMyFirstModConfig();
		MySpawnPointStore.Get();
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DebugPrint, 5000, true);
	}

	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity) 
	{
		super.InvokeOnConnect(player, identity);

	if (MyFirstModConfig.SYNC_TO_CLIENTS)
	{
		GetRPCManager().SendRPC("RPC", "RPC_Receive_MyFirstModConfig", new Param1<ref MyFirstModConfig>(GetMyFirstModConfig()), true, identity);
		MyFlagStore.SendOwnedFlags(identity);
			MySpawnPointStore.SendToClient(identity);
	}
	}
	
		void DebugPrint(){
	
		Print("[Server] Game tick:" + GetGame().GetTime());
	
	}
}