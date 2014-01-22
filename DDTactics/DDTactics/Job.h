// base job used for container
#pragma once
#include <iostream>
#include <string>
#include <fstream>


struct job_modifiers
{
	unsigned static int job_level;
	unsigned static int job_xp;

	float mod_health;
	float mod_mana;
	float mod_attack;
	float mod_magic;
	float mod_defense;
	float mod_resistance;
	float mod_speed;
	float mod_evasion;
	float mod_hit;
};


enum JOB{
	WARRIOR,
	GREYMAGE,
	ARCHER
};


class CJob
{

private:



	int job;
	job_modifiers modify;
	

public:
	CJob(void);
	~CJob(void);
	//////////////////////////////////////////////////////////////////////////////
	//Name:			init														//
	//Parameters:	int job - the job name							//
	//				float a_health	- job health modifier						//
	//				float a_mana	- job mana modifier							//
	//				float a_attack	- job attack modifier						//
	//				float a_magic	- job magic modifier						//
	//				float a_defense	- job defense modifier						//
	//				float a_resist	- job resist modifier						//
	//				float a_speed	- job speed modifier						//
	//				float a_evasion - job evasion modifier						//
	//				float a_hit		- job hit modifier							//
	//				
	//Return:		void														//
	//Description:	initializes job class variables								//
	//////////////////////////////////////////////////////////////////////////////
	void init(int job, 
		float a_health,
		float a_mana,
		float a_attack,
		float a_magic,
		float a_defense,
		float a_resist,
		float a_speed,
		float a_evasion,
		float a_hit
		);

	//////////////////////////////////////////////////////////////////////////////
	//Name:		load_from_file													//
	//Parameters: string file_name - name of the text file holding the job info	//
	//Return:	void															//
	//Description:	initializes job class variables based on text file			//
	//////////////////////////////////////////////////////////////////////////////
	void load_from_file(std::string file_name);

	//Return functions
	int get_job;
	job_modifiers get_job_modify();
	

};



