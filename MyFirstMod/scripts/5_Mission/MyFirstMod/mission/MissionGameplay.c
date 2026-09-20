// Client-side only code
modded class MissionGameplay
{
	
	ref EffectSound	m_ExampleSoundEffect;
	
	void MenuHandler(){
	
			
	}
	override void OnInit()
    {
        super.OnInit();

		// Load client-side config
		GetMyFirstModConfig();
		
    }

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice); 

		if (!GetGame())
			return;

		UpdateExample();
		MenuShowHandler.Check();
	}

	override void OnKeyPress(int key)
	{
		super.OnKeyPress(key);

		//Print("Key pressed ID: " + key);
	}

	void UpdateExample()
	{
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());

        if (!player)
			return;

		if (Zen_CheckInput("UAMyFirstModKeybind"))
        {
            // do something
			

			if (m_ExampleSoundEffect)
			{
				MyFirstModFunctions.DebugMessage("Stop sound loop");

				m_ExampleSoundEffect.SoundStop();
			}
			else 
			{
				MyFirstModFunctions.DebugMessage("Play sound");

				// LOOP EXAMPLE:
				// Soundset, object, fade in secs, fade out secs, loop on/off
				//m_ExampleSoundEffect = SEffectManager.PlaySoundOnObject("MyFirstModExample_SoundSet", GetGame().GetPlayer(), 0.0, 0.0, true);
				//m_ExampleSoundEffect.SetAutodestroy(true);

				// ONE-SHOT EXAMPLE: 
				// Soundset, position, fade in secs, fade out secs, loop on/off
				vector pos = GetGame().GetPlayer().GetPosition();
				m_ExampleSoundEffect = SEffectManager.PlaySound("Flies_SoundSet", pos, 1.0, 1.0);
				m_ExampleSoundEffect.SetAutodestroy(true);
				m_ExampleSoundEffect.SetSoundVolume(10);
			}
        }
	}

    bool Zen_CheckInput(string inputName)
    {
        if (GetUApi())
		{
            UAInput uai = GetUApi().GetInputByName(inputName);

            if (uai && uai.LocalPress())
            {
                return true;
            }
        }

        return false;
    }
}