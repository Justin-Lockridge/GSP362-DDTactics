#pragma once

#include "fmod.h"
#include "fmod.hpp"
#pragma comment(lib, "fmodex_vc.lib")
#include <string>
#include <iostream>
#include <vector>

#define SAFE_RELEASE2(x) if(x){x->release(); x = 0;}

class SoundManager
{
private:
	
	std::vector<FMOD::Sound*> songs;
	std::vector<bool>song_playing;
	std::vector<FMOD::Sound*> sound_effects;

	FMOD_RESULT		m_result;
	unsigned int	m_nVersion;
	int				m_nNum_drivers;
	FMOD_SPEAKERMODE	m_speakermode;
	FMOD_CAPS			m_caps;
	char				m_cName[256];

	FMOD::System*  system;
	FMOD::Channel* m_musicChannel;

	//Menu Sound
	FMOD::Sound			*menuMusic;

	//Overworld sounds
	FMOD::Sound			*overworldMusic, *townMusic;

	/*
		ADD ADDITIONAL SOUNDS HERE
		NOTE:  ADD ONTO THE SONG_SELECTION ENUM IN GameEnums WHEN CREATING A NEW 
					SOUND FOR STREAMING.
				ADD ONTO THE SOUND_EFFECT_SELECTION ENUM IN GameEnums WHEN
					CREATING A NEW SOUND FOR SOUND EFFECTS.
	*/
	////////////////////////////////////////////////////////////////////////
	FMOD::Sound			*temp_battle;


	////////////////////////////////////////////////////////////////////////	

	SoundManager();
	bool ERRCHECK(FMOD_RESULT result);
	
	//////////////////////////////////////////////////////////////////////
	//Name:			resizeSongplayer									//
	//Parameters:	none												//
	//Return:		bool												//
	//Description:	An internal helper function used to resize the		//
	//					vector song_playing								//
	//////////////////////////////////////////////////////////////////////
	bool resizeSongplaying();

	//////////////////////////////////////////////////////////////////////
	//Name:			createStream										//
	//Parameters:	std::string fileName - the song's file name			//
	//				FMOD::Sound *sound - the sound object container		//
	//Return:		bool												//
	//Description:  An internal helper function used to create a Stream	//
	//					for songs.										//
	//////////////////////////////////////////////////////////////////////
	bool createStream(std::string fileName, FMOD::Sound *sound);

	//////////////////////////////////////////////////////////////////////
	//Name:			createSound											//
	//Parameters:	std::string fileName - the sound effect file name	//
	//				FMOD::Sound *sound - the sound object container		//
	//Return:		bool												//
	//Description:	An internal helper function used to create a Sound	//
	//					for sound effects.								//
	//////////////////////////////////////////////////////////////////////
	bool createSound(std::string fileName, FMOD::Sound *sound);

public:
	
	static SoundManager* instance();
	~SoundManager();

	bool init();
	bool update();
	
	//////////////////////////////////////////////////////////////////////
	//Name:			playStream											//
	//Parameters:	unsigned int SONG_NUM - an enum value associated	//
	//					with the song being played.						//
	//Return:		bool												//
	//Description:	Plays the song according to the parameter.			//
	//				NOTE:  SONG_SELECTION enum is located in GameEnums.h//
	//////////////////////////////////////////////////////////////////////
	bool playStream(unsigned int SONG_NUM);

	//////////////////////////////////////////////////////////////////////
	//Name:			playSound											//
	//Parameters:	unsigned int EFFECT_NUM - an enum value associated	//
	//					with the sound effect being played.				//
	//Return:		bool												//
	//Description:	Plays the sound effect according to the parameter.	//
	//				NOTE:  SOUND_EFFECT_SELECTION is located in			//
	//						GameEnums.h									//
	//////////////////////////////////////////////////////////////////////
	bool playSound(unsigned int EFFECT_NUM );

	//////////////////////////////////////////////////////////////////////
	//Name:			stopStream											//
	//Parameters:	none												//
	//Return:		bool												//
	//Description:	Stops the music channel.  Used to stop a stream		//
	//////////////////////////////////////////////////////////////////////
	bool stopStream();
	

	void shutdown();

};