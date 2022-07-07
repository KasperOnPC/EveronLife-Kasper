[BaseContainerProps()]
class EL_BaseEntry
{
	// The lights affected by the entry
	protected ref array<EL_LightComponent> m_AffectedLights = {};
	
	//------------------------------------------------------------------------------------------------
	/**
	\brief Called when a light is registered, can be used to store only the affected lights by the entry
	\param light - the light being regisred
	**/
	void OnRegister(EL_LightComponent light);
	
	//------------------------------------------------------------------------------------------------
	/**
	\brief Called when the entry is executed. Can be used to affect the lights or the flow of the animation
	\param animation - the animation that is executing the entry
	**/
	void OnExecute(EL_LightAnimation animation);
}

[BaseContainerProps(), SCR_BaseContainerCustomTitleField("m_DelayMS", "Wait %1ms")]
class EL_WaitEntry : EL_BaseEntry
{
	[Attribute(desc: "Time in milliseconds that the entry will take", params: "0")]
	protected int m_DelayMS;
	
	/**
	\brief Sets a delay on the animation
	\param animation - the affected animation
	**/
	override void OnExecute(EL_LightAnimation animation)
	{
		animation.SetDelay(m_DelayMS / 1000.0);
	}
}

[BaseContainerProps(), SCR_BaseContainerCustomTitleField("m_Steps", "Loop %1 steps")]
class EL_LoopEntry : EL_BaseEntry
{
	[Attribute(desc: "Number of steps to loop. If 0, the animation will loop from the beginning")]
	protected int m_Steps;
	
	[Attribute(desc: "Number of times the loop should be executed. Number 0 means infinite")]
	protected int m_LoopCount;
	
	// Keeps track of remaining loops in the current iteration
	protected int m_LoopsRemaining = m_LoopCount;
	
	//------------------------------------------------------------------------------------------------
	override void OnExecute(EL_LightAnimation animation)
	{
		// if steps is not 0
		if(m_Steps)
		{
			// loops if there are remaining loops or the user set infinite loops
			if(m_LoopsRemaining > 0 || !m_LoopCount)
			{
				// if loops are not infinite, subtract 1 from remaining loops
				if(m_LoopCount) m_LoopsRemaining--;
				// goes back the user defined steps + this loop entry
				animation.GoBack(m_Steps + 1);
			}
			// Resets loops remaining when it already looped the user defined amount 
			// This prevents the loop from occuring only the first time in a nested loop
			else
			{
				m_LoopsRemaining = m_LoopCount;
			}
		}
		// resets the animation if steps is 0
		else
		{	
			animation.Reset();
		}
	
	}
}

// Light entry types define the name of the entry and will be displayed to the user when selected
enum EL_LightEntryType
{
	TURN_LIGHTS_ON,
	TURN_LIGHTS_OFF,
	TOGGLE_LIGHTS
}

[BaseContainerProps(), EL_BaseContainerCustomTitleEnumReadable(EL_LightEntryType, "m_Type")]
class EL_LightEntry : EL_WaitEntry
{
	
	// Types displayed to the user on a drop down menu
	protected ref static const ParamEnumArray enums = 
	{
		ParamEnum("Turn On", EL_LightEntryType.TURN_LIGHTS_ON.ToString()),
		ParamEnum("Turn Off", EL_LightEntryType.TURN_LIGHTS_OFF.ToString()),
		ParamEnum("Toggle", EL_LightEntryType.TOGGLE_LIGHTS.ToString())
	};
	
	[Attribute("Turn On", uiwidget: UIWidgets.ComboBox, enums: enums)]
	protected EL_LightEntryType m_Type;

	[Attribute(desc: "Names separated by space. Lights that contains the name will be affected by the entry. If left blank all lights are affected.")]
	protected string m_Name;
	
	//------------------------------------------------------------------------------------------------
	/**
	\brief Called when a light registers itself
	\param light - the light being registered
	**/
	override void OnRegister(EL_LightComponent light)
	{
		if(Affects(light)) m_AffectedLights.Insert(light);
	}
	
	//------------------------------------------------------------------------------------------------
	/**
	\brief Acts on each light and sets the specified delay
	\param animation - the affected animation
	**/
	override void OnExecute(EL_LightAnimation animation)
	{
		foreach(EL_LightComponent light : m_AffectedLights)
		{
			Act(light);
		}
		animation.SetDelay(m_DelayMS / 1000.0);
	}
	
	//------------------------------------------------------------------------------------------------
	/**
	\brief If the light is affected by the entry or not
	\param light - light that is being checked if is affected or not 
	\return true if the light is affected by the entry, false if not
	**/
	bool Affects(EL_LightComponent light)
	{
		if(!m_Name) return true;
		array<string> names = {};
		m_Name.Split(" ", names, true);
		foreach(string name : names)
		{
			if(light.GetName().Contains(name))
				return true;
		}
		
		return false;
	}
	
	//------------------------------------------------------------------------------------------------
	/**
	\brief How the entry affects each affected light
	\param light - the affected light 
	**/
	void Act(EL_LightComponent light)
	{
		if(m_Type == EL_LightEntryType.TURN_LIGHTS_ON) light.TurnOn();
		else if(m_Type == EL_LightEntryType.TURN_LIGHTS_OFF) light.TurnOff();
		else light.Toggle();
	}
}

// See EL_LightEntryType
enum EL_AnimationEntryType
{
	TURN_ANIMATION_ON,
	TURN_ANIMATION_OFF,
	TOGGLE_ANIMATION
}


[BaseContainerProps(), EL_BaseContainerCustomTitleEnumReadable(EL_AnimationEntryType, "m_Type")]
class EL_AnimationEntry : EL_LightEntry
{
	// see EL_LightEntry.Act()
	override void Act(EL_LightComponent light)
	{
		if(m_Type == EL_AnimationEntryType.TURN_ANIMATION_ON) light.Play();
		else if(m_Type == EL_AnimationEntryType.TURN_ANIMATION_OFF) light.Stop();
		else light.ToggleAnim();
	}
}
