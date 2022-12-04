// KA

#include "g_local.h"
#include "p_class.h"


// Check if the player has a class yet. If they don't tell them how to pick one
void CheckClass(edict_t *ent)
{
	if (ent->client->resp.pClass < 1)
	{
		gi.centerprintf(ent, "Please choose a class\n\nC - Medic\nV - Timemancer\nB - Juggernaught\nN - Assassin\nM - Madman\n"); 	
	}
}

// Used to change the player's class if they don't have one
void Cmd_Class_f(edict_t* ent, char* cmd)
{
	
	gi.cprintf(ent, PRINT_HIGH, "Got: %s\n", cmd);
	if (Q_stricmp(cmd, "medic") == 0)
	{
		ent->client->resp.pClass = 1;
		ent->client->pers.max_health = 100;
		ent->client->pers.health = 100;
		gi.centerprintf(ent, "Medic\n");
	}
	else if (Q_stricmp(cmd, "timemancer") == 0)
	{
		ent->client->resp.pClass = 2;
		ent->client->pers.max_health = 100;
		ent->client->pers.health = 100;
		gi.centerprintf(ent, "Timemancer\n");
	}
	else if (Q_stricmp(cmd, "juggernaut") == 0)
	{
		ent->client->resp.pClass = 3;
		ent->client->pers.max_health = 200;
		ent->client->pers.health = 200;
		gi.centerprintf(ent, "Juggernaut\n");
	}
	else if (Q_stricmp(cmd, "assassin") == 0)
	{
		ent->client->resp.pClass = 4;
		ent->client->pers.max_health = 75;
		ent->client->pers.health = 75;
		gi.centerprintf(ent, "Assassin\n");
	}
	else if (Q_stricmp(cmd, "madman") == 0)
	{
		ent->client->resp.pClass = 5;
		ent->client->pers.max_health = 150;
		ent->client->pers.health = 150;
		gi.centerprintf(ent, "Madman\n");
	}
	else
	{
		ent->client->resp.pClass = 0;
		gi.centerprintf(ent, "Invalid Class!  How'd you even do that?\n");
	}
	gi.sound(ent, CHAN_VOICE, gi.soundindex("player/male/jump1.wav"), 1, ATTN_NORM, 0);
}

// Abilities for player classes
void Cmd_Ability_f(edict_t* ent)
{
	if (ent->client->resp.pClass == 1)
	{	
		if (ent->health < ent->max_health)
		{
			ent->health += 50;
		}		
	}
	if (ent->client->resp.pClass == 2)
	{

	}
	if (ent->client->resp.pClass == 3)
	{

	}
	if (ent->client->resp.pClass == 4)
	{

	}
	if (ent->client->resp.pClass == 5)
	{

	}
}