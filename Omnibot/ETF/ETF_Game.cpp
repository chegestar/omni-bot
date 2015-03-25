////////////////////////////////////////////////////////////////////////////////
//
// $LastChangedBy$
// $LastChangedDate$
// $LastChangedRevision$
//
////////////////////////////////////////////////////////////////////////////////

#include "ETF_Game.h"
#include "TF_InterfaceFuncs.h"
#include "ETF_Client.h"

#include "System.h"
#include "RenderBuffer.h"

#include "PathPlannerBase.h"
#include "BotPathing.h"
#include "TF_BaseStates.h"
#include "NameManager.h"
#include "ScriptManager.h"

BOOST_STATIC_ASSERT( ETF_TEAM_MAX == TF_TEAM_MAX );

IGame *CreateGameInstance()
{
	return new ETF_Game;
}

ETF_Game::ETF_Game()
{
}

ETF_Game::~ETF_Game()
{
}

int ETF_Game::GetVersionNum() const
{
	return ETF_VERSION_LATEST;
}

Client *ETF_Game::CreateGameClient()
{
	return new ETF_Client;
}

const char *ETF_Game::GetModSubFolder() const
{
#ifdef WIN32
	return "etf\\";
#else
	return "etf";
#endif
}

const char *ETF_Game::GetDLLName() const
{
#ifdef WIN32
	return "omnibot_etf.dll";
#else
	return "omnibot_etf.so";
#endif
}

const char *ETF_Game::GetGameName() const
{
	return "ETF";
}

const char *ETF_Game::GetNavSubfolder() const
{
#ifdef WIN32
	return "etf\\nav\\";
#else
	return "etf/nav";
#endif
}

const char *ETF_Game::GetScriptSubfolder() const
{
#ifdef WIN32
	return "etf\\scripts\\";
#else
	return "etf/scripts";
#endif
}

bool ETF_Game::GetAnalyticsKeys( GameAnalyticsKeys & keys )
{
	keys.mGameKey = "68aa5fcc90a58d3de2ba80e8dc6f6a88";
	keys.mSecretKey = "283e40eb98703bedef2a5fa3e6a996b4c3233545";
	keys.mDataApiKey = "73a8f10945d4ac7bb1bde2fddb7905ddcbc7dbe1";
	keys.mVersionKey = va( "%s:v%s", GetGameName(), GetVersion() );
	return true;
}

bool ETF_Game::Init( System & system )
{
	if ( !TF_Game::Init( system ) )
		return false;

	// Set the sensory systems callback for getting aim offsets for entity types.
	AiState::SensoryMemory::SetEntityTraceOffsetCallback( ETF_Game::ETF_GetEntityClassTraceOffset );
	AiState::SensoryMemory::SetEntityAimOffsetCallback( ETF_Game::ETF_GetEntityClassAimOffset );

	// Run the games autoexec.
	int threadId;

	system.mScript->ExecuteFile( "scripts/etf_autoexec.gm", threadId );

	// Set up ETF specific data.
	using namespace AiState;
	TF_Options::GRENADE_VELOCITY = 650.f;
	TF_Options::PIPE_MAX_DEPLOYED = 6;

	TF_Options::DisguiseTeamFlags[ ETF_TEAM_BLUE ] = TF_PWR_DISGUISE_BLUE;
	TF_Options::DisguiseTeamFlags[ ETF_TEAM_RED ] = TF_PWR_DISGUISE_RED;
	TF_Options::DisguiseTeamFlags[ ETF_TEAM_GREEN ] = TF_PWR_DISGUISE_GREEN;
	TF_Options::DisguiseTeamFlags[ ETF_TEAM_YELLOW ] = TF_PWR_DISGUISE_YELLOW;

	return true;
}

void ETF_Game::GetGameVars( GameVars &_gamevars )
{
	_gamevars.mPlayerHeight = 64.f;
}

static IntEnum ETF_TeamEnum [] =
{
	IntEnum( "SPECTATOR", OB_TEAM_SPECTATOR ),
	IntEnum( "NONE", ETF_TEAM_NONE ),
	IntEnum( "RED", ETF_TEAM_RED ),
	IntEnum( "BLUE", ETF_TEAM_BLUE ),
	IntEnum( "YELLOW", ETF_TEAM_YELLOW ),
	IntEnum( "GREEN", ETF_TEAM_GREEN ),
};

void ETF_Game::GetTeamEnumeration( const IntEnum *&_ptr, int &num )
{
	num = sizeof( ETF_TeamEnum ) / sizeof( ETF_TeamEnum[ 0 ] );
	_ptr = ETF_TeamEnum;
}

void ETF_Game::InitScriptEntityFlags( gmMachine *_machine, gmTableObject *_table )
{
	//Override TF_Game, because we don't want caltrops, sabotage or radiotagged but we do want blind flag
	IGame::InitScriptEntityFlags( _machine, _table );

	_table->Set( _machine, "NEED_HEALTH", gmVariable( TF_ENT_FLAG_SAVEME ) );
	_table->Set( _machine, "NEED_ARMOR", gmVariable( TF_ENT_FLAG_ARMORME ) );
	_table->Set( _machine, "BURNING", gmVariable( TF_ENT_FLAG_BURNING ) );
	_table->Set( _machine, "TRANQUED", gmVariable( TF_ENT_FLAG_TRANQED ) );
	_table->Set( _machine, "INFECTED", gmVariable( TF_ENT_FLAG_INFECTED ) );
	_table->Set( _machine, "GASSED", gmVariable( TF_ENT_FLAG_GASSED ) );
	_table->Set( _machine, "SNIPE_AIMING", gmVariable( ENT_FLAG_IRONSIGHT ) );
	_table->Set( _machine, "AC_FIRING", gmVariable( TF_ENT_FLAG_ASSAULTFIRING ) );
	_table->Set( _machine, "LEGSHOT", gmVariable( TF_ENT_FLAG_LEGSHOT ) );
	_table->Set( _machine, "BLIND", gmVariable( ETF_ENT_FLAG_BLIND ) );
	_table->Set( _machine, "BUILDING_SG", gmVariable( TF_ENT_FLAG_BUILDING_SG ) );
	_table->Set( _machine, "BUILDING_DISP", gmVariable( TF_ENT_FLAG_BUILDING_DISP ) );
	_table->Set( _machine, "BUILDING_DETP", gmVariable( TF_ENT_FLAG_BUILDING_DETP ) );
	_table->Set( _machine, "BUILDINPROGRESS", gmVariable( TF_ENT_FLAG_BUILDINPROGRESS ) );
	_table->Set( _machine, "DISGUISED", gmVariable( ETF_ENT_FLAG_DISGUISED ) );
}

void ETF_Game::InitScriptPowerups( gmMachine *_machine, gmTableObject *_table )
{
	TF_Game::InitScriptPowerups( _machine, _table );

	_table->Set( _machine, "QUAD", gmVariable( ETF_PWR_QUAD ) );
	_table->Set( _machine, "SUIT", gmVariable( ETF_PWR_SUIT ) );
	_table->Set( _machine, "HASTE", gmVariable( ETF_PWR_HASTE ) );
	_table->Set( _machine, "INVIS", gmVariable( ETF_PWR_INVIS ) );
	_table->Set( _machine, "REGEN", gmVariable( ETF_PWR_REGEN ) );
	_table->Set( _machine, "FLIGHT", gmVariable( ETF_PWR_FLIGHT ) );
	_table->Set( _machine, "INVULN", gmVariable( ETF_PWR_INVULN ) );
	_table->Set( _machine, "AQUALUNG", gmVariable( ETF_PWR_AQUALUNG ) );
	//_table->Set(_machine, "CEASEFIRE",	gmVariable(ETF_PWR_CEASEFIRE));
}

const float ETF_Game::ETF_GetEntityClassTraceOffset( const TargetInfo &_target )
{
	if ( _target.mEntInfo.mGroup == ENT_GRP_PLAYER )
	{
		if ( _target.mEntInfo.mClassId > TF_CLASS_NONE && _target.mEntInfo.mClassId < FilterSensory::ANYPLAYERCLASS )
		{
			if ( _target.mEntInfo.mFlags.CheckFlag( ENT_FLAG_PRONED ) )
				return 34.0f;
			if ( _target.mEntInfo.mFlags.CheckFlag( ENT_FLAG_CROUCHED ) )
				return 24.0f;
			return 48.0f;
		}
	}
	return TF_Game::TF_GetEntityClassTraceOffset( _target );
}

const float ETF_Game::ETF_GetEntityClassAimOffset( const TargetInfo &_target )
{
	if ( _target.mEntInfo.mGroup == ENT_GRP_PLAYER )
	{
		if ( _target.mEntInfo.mClassId > TF_CLASS_NONE && _target.mEntInfo.mClassId < FilterSensory::ANYPLAYERCLASS )
		{
			if ( _target.mEntInfo.mFlags.CheckFlag( ENT_FLAG_PRONED ) )
				return 34.0f;
			if ( _target.mEntInfo.mFlags.CheckFlag( ENT_FLAG_CROUCHED ) )
				return 24.0f;
			return 48.0f;
		}
	}
	return TF_Game::TF_GetEntityClassAimOffset( _target );
}
