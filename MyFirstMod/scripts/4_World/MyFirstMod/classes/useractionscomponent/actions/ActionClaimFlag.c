class ActionClaimFlag : ActionInteractBase
{
	void ActionClaimFlag()
	{
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_Text = "Claim flag";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		return TerritoryFlag.Cast(target.GetObject()) != null;
	}

	override void OnExecuteServer(ActionData action_data)
	{
		PlayerIdentity identity = action_data.m_Player.GetIdentity();
		if (!identity)
			return;

		Object flagObj = action_data.m_Target.GetObject();
		if (!flagObj)
			return;

		string steamId = identity.GetPlainId();
		string guid = identity.GetId();
		vector flagPos = flagObj.GetPosition();

		MyFlagData flag = MyFlagStore.FindFlag(flagPos);
		if (flag && !MyFlagStore.CanClaim(flag, guid))
		{
			action_data.m_Player.MessageImportant("Only the player who placed this flag, or their faction, can claim it.");
			Print("[MyFirstMod] Claim denied for " + steamId + " at " + flagPos);
			return;
		}

		bool claimed = MyFlagStore.ClaimFlag(flagPos, steamId, guid);
		MyFlagStore.SendOwnedFlags(identity);

		Print("[MyFirstMod] Claim by " + steamId + " at " + flagPos + " -> " + claimed);
	}
}