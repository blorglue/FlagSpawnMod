class MyRespawnStore
{
	static const float DELAY_PER_REPEAT_DEATH = 60;   // seconds added per repeat death
	static const float RESET_AFTER = 1800;            // seconds without dying before the count resets

	static ref map<string, int> s_Deaths = new map<string, int>;
	static ref map<string, float> s_LastDeath = new map<string, float>;
	static ref map<string, float> s_ReadyAt = new map<string, float>;

	static void OnDeath(string steamId)
	{
		float now = GetGame().GetTickTime();

		int count = 0;
		if (s_LastDeath.Contains(steamId) && now - s_LastDeath.Get(steamId) < RESET_AFTER)
			count = s_Deaths.Get(steamId);

		count++;
		s_Deaths.Set(steamId, count);
		s_LastDeath.Set(steamId, now);
		s_ReadyAt.Set(steamId, now + (count - 1) * DELAY_PER_REPEAT_DEATH);  // 1st death 0 s, 2nd 60 s, 3rd 120 s
	}

	static float SecondsLeft(string steamId)
	{
		if (!s_ReadyAt.Contains(steamId))
			return 0;
		return Math.Max(0, s_ReadyAt.Get(steamId) - GetGame().GetTickTime());
	}

	static void SendToClient(PlayerIdentity identity)
	{
		if (!identity)
			return;
		GetRPCManager().SendRPC("RPC", "RPC_Receive_RespawnCooldown", new Param1<float>(SecondsLeft(identity.GetPlainId())), true, identity);
	}
}

class MyRespawnClientCache
{
	static float ReadyAt;   // client clock time when the cooldown ends
}