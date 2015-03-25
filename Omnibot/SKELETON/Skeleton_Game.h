////////////////////////////////////////////////////////////////////////////////
//
// $LastChangedBy$
// $LastChangedDate$
// $LastChangedRevision$
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __SKELETON_GAME_H__
#define __SKELETON_GAME_H__

class gmMachine;
class gmTableObject;

#include "IGame.h"
#include "Skeleton_Config.h"

// class: Skeleton_Game
//		Basic Game subclass
class Skeleton_Game : public IGame
{
public:
	bool Init( System & system );

	virtual Client *CreateGameClient();

	int GetVersionNum() const;
	const char *GetDLLName() const;
	const char *GetGameName() const;
	const char *GetModSubFolder() const;
	const char *GetNavSubfolder() const;
	const char *GetScriptSubfolder() const;
	const char *GetGameDatabaseAbbrev() const;
	
	void GetTeamEnumeration( const IntEnum *&_ptr, int &num );
	void GetWeaponEnumeration( const IntEnum *&_ptr, int &num );

	Skeleton_Game();
	~Skeleton_Game();
protected:

	void GetGameVars( GameVars &_gamevars );

	// Script support.
	void InitScriptClasses( gmMachine *_machine, gmTableObject *_table );
	void InitScriptEvents( gmMachine *_machine, gmTableObject *_table );

	// Commands
	void InitCommands();

	static const float Skeleton_GetEntityClassTraceOffset( const TargetInfo &_target );
	static const float Skeleton_GetEntityClassAimOffset( const TargetInfo &_target );
};

#endif
