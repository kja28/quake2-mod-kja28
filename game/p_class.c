// KA

#include "g_local.h"
#include "p_class.h"
#define MAX 100

void IsInvincible(edict_t* ent)
{
	if (ent->client->resp.isInvi && ent->client->quad_framenum >= level.framenum)
	{
		ent->health = 200;
	}
	else
	{
		ent->client->resp.isInvi = false;
	}
}


void IsControl(edict_t* ent)
{
	if (ent->client->resp.isCont && ent->client->invincible_framenum >= level.framenum)
	{
		edict_t* target;
		target = MindControl(ent);
		if (target)
		{
			target->nextthink = 0;
		}
	}
	else
	{
		edict_t* target;
		target = MindControl(ent);
		if (target)
		{
			if (target->nextthink == 0)
			{
				target->nextthink = 1;
			}	
		}

		ent->client->resp.isCont = false;
	}
}
edict_t* FindPlayer(edict_t* self)
{
	edict_t* ent = NULL;
	edict_t* best = NULL;

	while ((ent = findradius(ent, self->s.origin, 1024)) != NULL)
	{
		if (ent->client != NULL)
		{
			best = ent;
			return best;
		}
	}

	return best;
}
static void P_ProjectSource(gclient_t* client, vec3_t point, vec3_t distance, vec3_t forward, vec3_t right, vec3_t result)
{
	vec3_t	_distance;

	VectorCopy(distance, _distance);
	if (client->pers.hand == LEFT_HANDED)
		_distance[1] *= -1;
	else if (client->pers.hand == CENTER_HANDED)
		_distance[1] = 0;
	G_ProjectSource(point, _distance, forward, right, result);
}


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
		ent->health= 100;
		gi.centerprintf(ent, "Medic\n");
	}
	else if (Q_stricmp(cmd, "timemancer") == 0)
	{
		ent->client->resp.pClass = 2;
		ent->client->pers.max_health = 100;
		ent->health= 100;
		gi.centerprintf(ent, "Timemancer\n");
	}
	else if (Q_stricmp(cmd, "juggernaut") == 0)
	{
		ent->client->resp.pClass = 3;
		ent->client->pers.max_health = 200;
		ent->health = 200;
		gi.centerprintf(ent, "Juggernaut\n");
	}
	else if (Q_stricmp(cmd, "assassin") == 0)
	{
		ent->client->resp.pClass = 4;
		ent->client->pers.max_health = 75;
		ent->health = 75;
		gi.centerprintf(ent, "Assassin\n");
	}
	else if (Q_stricmp(cmd, "madman") == 0)
	{
		ent->client->resp.pClass = 5;
		ent->client->pers.max_health = 150;
		ent->health = 150;
		gi.centerprintf(ent, "Madman\n");
	}
	else
	{
		ent->client->resp.pClass = 0;
		gi.centerprintf(ent, "Invalid Class!  How'd you even do that?\n");
	}
	gi.sound(ent, CHAN_VOICE, gi.soundindex("player/male/jump1.wav"), 1, ATTN_NORM, 0);
}

edict_t* MindControl(edict_t* self)
{
	edict_t* ent = NULL;
	edict_t* best = NULL;

	while ((ent = findradius(ent, self->s.origin, 8192)) != NULL)
	{
		if (ent == self)
			continue;
		if (!(ent->svflags & SVF_MONSTER))
			continue;
		if (!ent->health)
			continue;
		if (ent->health < 1)
			continue;
		if (!visible(self, ent))
			continue;
		if (!best)
		{
			best = ent;
			continue;
		}
		if (ent->max_health <= best->max_health)
			continue;
		best = ent;
	}

	return best;
}

// Abilities for player classes
void Cmd_Ability_f(edict_t* ent)
{
	if (ent->client->resp.pClass == 1)
	{	
		vec3_t	offset, forward, right, start, end;
		edict_t* other = NULL;
		trace_t tr;

		VectorSet(offset, 8, 8, ent->viewheight - 8);
		AngleVectors(ent->client->v_angle, forward, right, NULL);
		P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);
		VectorScale(forward, -2, ent->client->kick_origin);
		VectorMA(start, 8192, forward, end);

		tr = gi.trace(ent->s.origin, NULL, NULL, end, ent, MASK_SHOT);
		
		if (tr.fraction < 1)
		{
			other = tr.ent;
		}

		if (other->client != NULL)
		{
			other->health += 50;
		}
		else
		{
			ent->health += 50;
		}

	}
	if (ent->client->resp.pClass == 2) //timemancer
	{
		int timeout = 300;

		ent->client->resp.isCont = true;

		if (ent->client->invincible_framenum > level.framenum)
			ent->client->invincible_framenum += timeout;
		else
			ent->client->invincible_framenum = level.framenum + timeout;

	}
	if (ent->client->resp.pClass == 3) // juggernaut
	{
		ent->client->resp.isInvi = true;
		
		int timeout = 300;
		
		if (ent->client->quad_framenum > level.framenum)
			ent->client->quad_framenum += timeout;
		else
			ent->client->quad_framenum = level.framenum + timeout;
	
	}
	if (ent->client->resp.pClass == 4) // Assassin
	{
		gitem_t* it;// KA edit
		edict_t* it_ent;

		it = FindItemByClassname("weapon_railgun");
		it_ent = G_Spawn();
		it_ent->s.origin[0] = ent->s.origin[0];
		it_ent->s.origin[1] = ent->s.origin[1];
		it_ent->s.origin[2] = ent->s.origin[2];
		it_ent->classname = it->classname;
		SpawnItem(it_ent, it);
		gi.linkentity(it_ent);
		
	}
	if (ent->client->resp.pClass == 5)// Madman
	{
		vec3_t	offset;
		vec3_t	forward, right;
		vec3_t	start;
		int		damage = 120;
		float	radius;
		vec3_t  newdir;

		radius = damage + 40;
		VectorSet(offset, 8, 8, ent->viewheight - 8);
		AngleVectors(ent->client->v_angle, forward, right, NULL);
		P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);
		VectorScale(forward, -2, ent->client->kick_origin);

		for (int i = 1; i < 15; i++)
		{
			VectorSet(newdir,forward[0] * crandom(200), forward[1] * crandom(200), forward[2] * crandom(200));

			fire_grenade(ent, start, newdir, damage, 600, 2.5, radius);
		}
	}
}


