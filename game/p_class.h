// KA
// Ceclatration of functions used in quake 2 borderquake mod



void IsInvincible(edict_t* ent);
void CheckClass(edict_t * ent);
void Cmd_Class_f(edict_t* ent, char* cmd);
void Cmd_Ability_f(edict_t* ent);
edict_t* MindControl(edict_t* self);
void IsControl(edict_t* ent);
edict_t* FindPlayer(edict_t* self);