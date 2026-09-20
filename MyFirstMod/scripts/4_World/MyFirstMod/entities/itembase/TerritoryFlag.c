modded class TerritoryFlag
{
	override void SetActions()
	{
		super.SetActions();

		AddAction(ActionClaimFlag);
	}
	override void EEDelete(EntityAI parent)
{
	super.EEDelete(parent);

	if (GetGame().IsServer())
		MyFlagStore.RemoveFlag(GetPosition());
}
}