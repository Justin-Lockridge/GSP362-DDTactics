#include "SoundManager.h"


SoundManager::SoundManager()
{
	system = 0;
	m_musicChannel = 0;
	menuMusic = 0;
	overworldMusic = 0;
	townMusic = 0;
	songs.clear();
	song_playing.clear();
	sound_effects.clear();
}


SoundManager* SoundManager::instance()
{
	static SoundManager sound;
	return &sound;
}

SoundManager::~SoundManager()
{}

bool SoundManager::init()
{
	m_result = FMOD::System_Create(&system);
	ERRCHECK(m_result);

	m_musicChannel->setVolume(0.3f);
	//Set FMOD version
	m_result = system->getVersion(&m_nVersion);
	ERRCHECK(m_result);

	if(m_nVersion < FMOD_VERSION)
	{
		printf("Error! You are using an old version of FMOD %08x.  This program requires %98x\n",
			m_nVersion, FMOD_VERSION);
		return false;
	}
	
	//Set drivers
	m_result = system->getNumDrivers(&m_nNum_drivers);
	ERRCHECK(m_result);

	if(m_nNum_drivers == 0)
	{
		m_result = system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		ERRCHECK(m_result);
	}
	else
	{
		//Set speaker mode
		m_result = system->getDriverCaps(0, &m_caps, NULL, &m_speakermode);
		ERRCHECK(m_result);

		m_result = system->setSpeakerMode(m_speakermode);
		ERRCHECK(m_result);

		//Set Driver Info
		if(m_caps & FMOD_CAPS_HARDWARE_EMULATED)
		{
			m_result = system->setDSPBufferSize(1024,10);
			ERRCHECK(m_result);
		}

		m_result = system->getDriverInfo(0, m_cName, 256, 0);
		ERRCHECK(m_result);

		if(strstr(m_cName, "SigmaTel"))
		{
			m_result = system->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0,
				FMOD_DSP_RESAMPLER_LINEAR);
			ERRCHECK(m_result);
		}
	}
	//Check sound support
	m_result = system->init(10, FMOD_INIT_NORMAL, 0);

	if(m_result == FMOD_ERR_OUTPUT_CREATEBUFFER)
	{
		m_result = system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		ERRCHECK(m_result);

		m_result = system->init(10, FMOD_INIT_NORMAL, 0);

	}
	ERRCHECK(m_result);

	
	createStream("Prelude.mp3", menuMusic);
	createStream("RO Juno.mp3", overworldMusic);
	createStream("AbSolitude.mp3", townMusic);



	/*
		ADD ADDITIONAL STREAMS/SOUNDEFFECTS HERE
	*/
	///////////////////////////////////////////////////////////////////////

	createStream("FFXI Battle in the Dungeon.mp3", temp_battle);

	createStream("KingdomBaron.mp3", introMusic);
	createStream("The Fighting Mongooses.mp3",Mongoose);
	///////////////////////////////////////////////////////////////////////

	//resize vector of bools
	resizeSongplaying();

	return true;
}

bool SoundManager::resizeSongplaying()
{
	song_playing.resize(songs.size());
	for(unsigned int i = 0; i < song_playing.size(); i++)
	{
		song_playing[i] = false;
	}
	return true;
}

bool SoundManager::update()
{
	m_result = system->update();
	ERRCHECK(m_result);
	return true;
}

bool SoundManager::createStream(std::string fileName, FMOD::Sound *sound)
{
	m_result = system->createStream(fileName.c_str(), FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &sound);
	ERRCHECK(m_result);
	songs.push_back(sound);

	return true;
}

bool SoundManager::createSound(std::string fileName, FMOD::Sound *sound)
{
	m_result = system->createSound(fileName.c_str(), FMOD_DEFAULT, 0, &sound);
	ERRCHECK(m_result);
	sound_effects.push_back(sound);
	return true;
}

bool SoundManager::playStream(unsigned int SONG_NUM)
{
	if(!song_playing[SONG_NUM])
	{
		m_result = system->playSound(FMOD_CHANNEL_FREE, songs[SONG_NUM], false, &m_musicChannel);
		ERRCHECK(m_result);
		song_playing[SONG_NUM] = true;
	}
	return true;
}

bool SoundManager::playSound(unsigned int EFFECT_NUM)
{
	m_result = system->playSound(FMOD_CHANNEL_FREE, sound_effects[EFFECT_NUM], false, &m_musicChannel);
	ERRCHECK(m_result);
	return true;
}

bool SoundManager::stopStream()
{
	m_result = m_musicChannel->stop();
	ERRCHECK(m_result);

	for(unsigned int i = 0; i < song_playing.size(); i++)
		song_playing[i] = false;
	return true;
}

bool SoundManager::ERRCHECK(FMOD_RESULT result)
{
	if(result != FMOD_OK)
		return false;
	return true;
}

void SoundManager::shutdown()
{
	//clear pointers before releasing objects
	songs.clear();
	sound_effects.clear();

	/*
		Release Sound objects
	*/
	SAFE_RELEASE2(introMusic);
	SAFE_RELEASE2(temp_battle);
	SAFE_RELEASE2(townMusic);
	SAFE_RELEASE2(overworldMusic);
	SAFE_RELEASE2(menuMusic);
	SAFE_RELEASE2(system);
	
}