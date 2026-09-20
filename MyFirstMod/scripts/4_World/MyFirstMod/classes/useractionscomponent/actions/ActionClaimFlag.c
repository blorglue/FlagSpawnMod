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
		bool claimed = MyFlagStore.ClaimFlag(flagObj.GetPosition(), steamId);
		MyFlagStore.SendOwnedFlags(identity); 

		Print("[MyFirstMod] Claim by " + steamId + " at " + flagObj.GetPosition() + " -> " + claimed);
	}
}