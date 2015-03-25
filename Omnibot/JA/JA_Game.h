////////////////////////////////////////////////////////////////////////////////
//
// $LastChangedBy$
// $LastChangedDate$
// $LastChangedRevision$
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __JA_GAME_H__
#define __JA_GAME_H__

class gmMachine;
class gmTableObject;

#include "IGame.h"

// class: JA_Game
//		Game Type for Jedi-Academy.
class JA_Game : public IGame
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
	bool ReadyForDebugWindow() const;

	void AddBot( Msg_Addbot &_addbot, bool _createnow = true );

	void ClientJoined( const Event_SystemClientConnected *_msg );

	void GetTeamEnumeration( const IntEnum *&_ptr, int &num );
	void GetWeaponEnumeration( const IntEnum *&_ptr, int &num );

	JA_Game();
	virtual ~JA_Game();
protected:

	void GetGameVars( GameVars &_gamevars );

	// Script support.
	void InitScriptClasses( gmMachine *_machine, gmTableObject *_table );
	void InitScriptEvents( gmMachine *_machine, gmTableObject *_table );
	void InitScriptEntityFlags( gmMachine *_machine, gmTableObject *_table );
	void InitScriptPowerups( gmMachine *_machine, gmTableObject *_table );
	void InitScriptContentFlags( gmMachine *_machine, gmTableObject *_table );
	void InitScriptBotButtons( gmMachine *_machine, gmTableObject *_table );
	void InitVoiceMacros( gmMachine *_machine, gmTableObject *_table );

	// Commands
	void InitCommands();

	static const float JA_GetEntityClassTraceOffset( const TargetInfo &_target );
	static const float JA_GetEntityClassAimOffset( const TargetInfo &_target );
};

#endif
