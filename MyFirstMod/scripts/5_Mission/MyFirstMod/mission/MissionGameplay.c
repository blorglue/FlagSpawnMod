// Client-side only code
modded class MissionGameplay
{

	void MenuHandler(){
	
			
	}
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice); 

		if (!GetGame())
			return;

		MenuShowHandler.Check();
	}

	override void OnKeyPress(int key)
	{
		super.OnKeyPress(key);

		//Print("Key pressed ID: " + key);
	}
}