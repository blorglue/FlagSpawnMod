class MenuShowHandler
{
	static ref DeathMenu m_DeathMenu;
	static bool wasDead = false;
	static void Check()

	{
		
	

		Man player = GetGame().GetPlayer();
		if (!player)
			return;

			if (!player.IsAlive())	//Returns the opposite of player is alive)
			{
				//When player dies !player.IsAlive=True. So now we set wasDead=true and return. We never reach the second block
				wasDead = true;
				return;
			}
			
		//Once you respawn. Now !player.IsAlive=False so it skips it but wasDead is still true so it comes into this block and calls the OpenDeathMenu
		if (wasDead){		
	
			Print("Player respawned - opening menu shortly");
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(OpenDeathMenu, 1500, false);	
			wasDead = false;
}
		
	}
	
static void OpenDeathMenu()
{
	m_DeathMenu = new DeathMenu;
	GetGame().GetUIManager().ShowScriptedMenu(m_DeathMenu, null);
}
}