// Server-side only code
modded class MissionServer
{
	override void OnInit()
	{
		super.OnInit();

		Print("[MyFirstMod] MissionServer::OnInit");

		MySpawnPointStore.Get();
	}

	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);

		MyFlagStore.SendOwnedFlags(identity);
		MySpawnPointStore.SendToClient(identity);
	}
}
