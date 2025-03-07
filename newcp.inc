/*

# -*- newcp.inc -*- 

Created on November 14, 2020/23:53

""""""

A dynamic checkpoint system with various colors can make the user experience significantly more engaging and efficient. 
This system can use colors to easily represent different statuses. 
For example, 

red checkpoints can indicate issues or errors, 
while green checkpoints can signify successful progress. 
This type of system allows users to quickly and effortlessly understand the information they need and take necessary actions.

Overall, incorporating colors into a dynamic checkpoint system greatly improves accessibility and user experience.

Visual Representation: Colors play a crucial role in making the system more intuitive. Here’s how different colors can be used:

Red: Indicates errors or critical issues that need immediate attention.
Green: Shows successful completion or smooth progress.
Blue: Can be used for informational checkpoints or optional actions.


@author: Mohammad Norozani (@itsVoid)

""""""

*/

#include <a_samp.inc>
#include <fcnpc.inc>

#define CP_COLOR_RED 19945
#define CP_COLOR_GREEN 19946
#define CP_COLOR_BLUE 19947

#define MAX_NEW_CPS 20

forward OnPlayerEnterNewCP(playerid, checkpoint);

new bool:cpNEW[MAX_PLAYERS];
new cpArea[MAX_PLAYERS];
new cpObj[MAX_OBJECTS];

stock CountNewCheckPoints() 
{
	new cps = 0;

	foreach (new i : Player) 
	{
		if (IsPlayerNPC(i) && cpNEW[i]) 
		{
			cps ++;
		}
	}

	return cps;
}

stock CreateNewCheckpoint(Float:x, Float:y, Float:z, color = CP_COLOR_GREEN) 
{
	if (MAX_NEW_CPS == CountNewCheckPoints()) return print("[CreateNewCheckpoint]: Checkpoint not created, ERROR: #MAX_NEW_CPS LIMITED."); // Check #DEFINE MAX_NEW_CPS

	new str[58];
	format(str, sizeof str, "newcp%d%dz", random(1000), random(500));
	new npc = FCNPC_Create(str);
	FCNPC_Spawn(npc, 0, x, y, z - 2.5);
	FCNPC_SetHealth(npc, 100000000);

	cpNEW[npc] = true;
	cpArea[npc] = CreateDynamicSphere(x, y, z, 1.3);
	cpObj[npc] = CreateDynamicObject(19435, x, y, z - 3.5, 0, 90.0, 0);

	if (color == CP_COLOR_GREEN || color == CP_COLOR_BLUE || color == CP_COLOR_RED) 
	{
		SetPlayerAttachedObject(npc, 0, color, 10, -3.0099, -0.0339, 0.0000, 19.2999, 93.4998, -4.7999, 0.0518, 0.0538, 0.0298); // CP attached to the Spine of Mohammad
	}
	else 
	{ 
		DestroyNewCP(npc);
		printf("[CreateNewCheckpoint]: Checkpoint #%d not create becuase you entered invalid color id.", npc - 998);
	}
	TogglePlayerControllable(npc, false);
	SetPlayerColor(npc, (0xFFFFFFFF & 0xFFFFFF00));
	return npc;
}

#if defined OnPlayerEnterDynamicArea
	#if defined STREAMER_ENABLE_TAGS
		public OnPlayerEnterDynamicArea(playerid, STREAMER_TAG_AREA:areaid)
	#else
		public OnPlayerEnterDynamicArea(playerid, areaid)
	#endif
	{
		for (new i = 0; i < sizeof(cpArea); i++) 
		{
			if (areaid == cpArea[i]) 
			{
				CallRemoteFunction("OnPlayerEnterNewCP", "ii", playerid, i);
			}
		}
		#if defined cp_OnPlayerEnterDynamicArea
			return cp_OnPlayerEnterDynamicArea(playerid, areaid);
		#else
			return 1;
		#endif
	}

	#if defined _ALS_OnPlayerEnterDynamicArea
		#undef OnPlayerEnterDynamicArea
	#else
		#define _ALS_OnPlayerEnterDynamicArea
	#endif
	#define OnPlayerEnterDynamicArea cp_OnPlayerEnterDynamicArea
	#if defined cp_OnPlayerEnterDynamicArea
		#if defined STREAMER_ENABLE_TAGS
			forward cp_OnPlayerEnterDynamicArea(playerid, STREAMER_TAG_AREA:areaid);
		#else
			forward cp_OnPlayerEnterDynamicArea(playerid, areaid);
		#endif
	#endif
#endif

stock DestroyNewCP(cp) 
{
	FCNPC_Destroy(cp);
	RemovePlayerAttachedObject(cp, 0);
	cpNEW[cp] = false;
	DestroyDynamicArea(cpArea[cp]);
	DestroyDynamicObject(cpObj[cp]);
	return true;
}

stock IsValidNewCP(cp) 
{
	if (cpNEW[cp]) 
		return true;
	return false;
}

/*

how to use?

public OnPlayerEnterNewCP(playerid, checkpoint)
{
	return true;
}

*/