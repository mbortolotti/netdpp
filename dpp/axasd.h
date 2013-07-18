#ifndef AXASD_H
#define AXASD_H

//#include "dpp.h"
#include "ddpp.h"

#include <iostream>



class axasd {

public:

	static axasd &getInstance();

	bool isActive();
	int start(bool restart);
	int stop();
	int erase();
	std::string getData();
	int updateSettings(std::string);
	std::string getSettings();
	int initDpp();
	std::string getStatus();

private:

	//HMODULE hDppDll;

	//GENSETTING     settings;
	//BOARDPROPERTY  properties;
	// ACQSTATUS      acqStatus;

	HMODULE hDLL;

	ACQSETTING     settings;
	ACQDATA        Data;
	ACQDEF         Def;
	ACQSTATUS      acqStatus;

	short Board;

	axasd();
	~axasd();
	
	void loadDppDll();
	void unloadDppDll();

};

#endif



