private bool m_CanBePerformed = true;


class EL_BusStopAction : ScriptedUserAction
{
	[Attribute(defvalue:"20", UIWidgets.EditBox, desc: "Travel Time Delay")]
	private int m_DelayTimeMilliseconds;
	
	// [Attribute("", UIWidgets.SearchComboBox, desc: "Available Bus Stop Desitinations", enums: ParamEnumArray.FromEnum(AvailableBusStops))]
	// private ref array<AvailableBusStops> m_BusStops;

	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		// Player Entity
        IEntity teleportedPlayer = SCR_PlayerController.GetLocalControlledEntity();

        // Teleport
        teleportedPlayer.SetOrigin("4573.21 18.59 10816.93");
      
	}

	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
 	{
		return (m_CanBePerformed);
	}
	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
	IEntity ThisWorld = GetGame().GetWorldEntity();
		
	Print(ThisWorld);
	}
}