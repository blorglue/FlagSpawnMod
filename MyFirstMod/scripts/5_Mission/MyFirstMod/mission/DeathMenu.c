class DeathMenu extends UIScriptedMenu
{
	//Defines the buttons/UI objects
	protected MapWidget m_Map;
	protected ButtonWidget m_ButtonSpawn;
	protected TextListboxWidget m_SpawnList;
	protected int m_LastRow = -1;

	// One "x y z" position per list row, in the same order as the rows
	protected ref array<string> m_RowPositions = new array<string>;

	override Widget Init()
	{
		//tells the game where the UI design is
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("MyFirstMod/layouts/DeathMenu.layout");

		m_Map = MapWidget.Cast(layoutRoot.FindAnyWidget("spawn_map_widget"));
		m_ButtonSpawn = ButtonWidget.Cast(layoutRoot.FindAnyWidget("button_spawn"));
		m_SpawnList = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("spawn_list"));

		m_SpawnList.ClearItems();
		m_RowPositions.Clear();

		// Preset spawn points from spawnpoints.json come first
		for (int p = 0; p < MyFlagClientCache.Presets.Count(); p++)
		{
			MySpawnPoint preset = MyFlagClientCache.Presets.Get(p);
			if (preset.Position == "")
				continue;

			m_SpawnList.AddItem(preset.Name, null, 0);
			m_RowPositions.Insert(preset.Position);
		}

		// Then this player's own flags
		for (int i = 0; i < MyFlagClientCache.Owned.Count(); i++)
		{
			m_SpawnList.AddItem("Base " + (i + 1), null, 0);
			m_RowPositions.Insert(MyFlagClientCache.Owned.Get(i));
		}

		if (m_RowPositions.Count() > 0)
			m_SpawnList.SelectRow(0);

		m_Map.SetScale(0.1);   // as FlagSpawn does

		return layoutRoot;
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);

		if (w == m_ButtonSpawn)
		{
			int row = m_SpawnList.GetSelectedRow();
			if (row < 0 || row >= m_RowPositions.Count())
				return true;                     // nothing selected: stay open

			GetRPCManager().SendRPC("RPC", "RPC_Receive_SpawnAtFlag", new Param1<string>(m_RowPositions.Get(row)), true);
			Close();
			return true;
		}

		return false;
	}

	override void Update(float timeslice)
	{
		super.Update(timeslice);

		int row = m_SpawnList.GetSelectedRow();
		if (row == -1 || row == m_LastRow || row >= m_RowPositions.Count())
			return;

		m_LastRow = row;
		vector pos = m_RowPositions.Get(row).ToVector();

		string label;
		m_SpawnList.GetItemText(row, 0, label);

		m_Map.ClearUserMarks();
		m_Map.SetMapPos(pos);
		m_Map.AddUserMark(pos, label, ARGB(255, 255, 255, 255), "\\DZ\\gear\\navigation\\data\\map_border_cross_ca.paa");
	}

	override void OnShow()
	{
		super.OnShow();
		PPEffects.SetBlurMenu(1);
		GetGame().GetInput().ChangeGameFocus(1);
		GetGame().GetUIManager().ShowUICursor(true);
		SetFocus(layoutRoot);
	}

	override void OnHide()
	{
		super.OnHide();
		PPEffects.SetBlurMenu(0);
		GetGame().GetUIManager().ShowUICursor(false);
		GetGame().GetInput().ResetGameFocus();
	}
}
