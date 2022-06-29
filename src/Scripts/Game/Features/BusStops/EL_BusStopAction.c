private bool m_CanBePerformed = true;


class EL_BusStopAction : ScriptedUserAction
{
	[Attribute(defvalue:"20", UIWidgets.EditBox, desc: "Travel Time Delay")]
	private int m_DelayTimeMilliseconds;
	
	[Attribute("", UIWidgets.SearchComboBox, desc: "Available Bus Stop Desitinations", enums: ParamEnumArray.FromEnum(AvailableBusStops))]
	private ref array<AvailableBusStops> m_BusStops;

	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{

		
        // Destination pos and dir
	    pUserEntity.SetOrigin("2550 10 2900" );

        
	}

	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
 	{
		return (m_CanBePerformed);
	}	
	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		IEntity myHouseEntity = GetGame().FindEntity("myNewHouse");
        array<IEntity> s_aQueryPropsList = new array<IEntity>();
        
        s_aQueryPropsList = GetAllChildrensWithComponent(myHouseEntity,DoorComponent);
		
	    array<IEntity> GetAllChildrensWithComponent(IEntity entity, typename myTypename)
	    {
	        array<IEntity> propsList = new array<IEntity>();
	        IEntity child = entity.GetChildren();
	        while (child)
	        {            
	            IEntity prop = child;
	            child = child.GetSibling();            
	            
	            // Checking if has the component, if so we add it to the list.
	            Managed comp = prop.FindComponent(myTypename);
	            if(comp)
	                propsList.Insert(prop);            
	            
	            // Checking if has more childs if so we check them for components to.
	            IEntity childFromChild = prop.GetChildren();            
	            if(childFromChild)
	            {
	                array<IEntity> propsChildList = new array<IEntity>();
	                propsChildList = GetAllChildrensWithComponent(prop,myTypename);
	                propsList.InsertAll(propsChildList);
	            }                        
	        }
	        return propsList;
	    }
		
	}
	
}

