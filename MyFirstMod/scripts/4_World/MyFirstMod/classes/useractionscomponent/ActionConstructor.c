modded class ActionConstructor
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);

        actions.Insert(ActionExample);
		actions.Insert(ActionClaimFlag); //Claiming flag prompt
    }
}