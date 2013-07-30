#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

#include <boost/tokenizer.hpp>

#include "axasd.h"
#include "dpp_exception.h"


/** DPPSET         lpSet = NULL;
DPPDATASET     lpDataSet = NULL;
DPPLEAVE       lpLeave = NULL;
DPPSTART       lpStart = NULL;
DPPGET         lpGet = NULL;
DPPHALT        lpHalt = NULL;
DPPERASE       lpErase = NULL;
DPPSTATUS      lpStatus = NULL;
DPPGETSDD      lpGetSdd = NULL;
DPPVOLTTODAC   lpVolt2Dac = NULL;
DPPDACTOVOLT   lpDac2Volt = NULL; **/

typedef int (APIENTRY *IMCAGETSETTING)(ACQSETTING FAR *Setting, int mcano);
typedef VOID (APIENTRY *IMCASTORESETTING)(ACQSETTING FAR *Setting, int mcano);
typedef int (APIENTRY *IMCANEWSTATUS)(int mcano);
typedef int (APIENTRY *IMCAGETSTATUS)(ACQSTATUS FAR *Status, int mcano);
typedef VOID (APIENTRY *IMCARUNCMD)(int mcano, LPSTR Cmd);
typedef int (APIENTRY *IMCAGETCNT)(double far *cntp, int mcano);
typedef int (APIENTRY *IMCAGETROI)(unsigned long FAR *roip, int mcano);
typedef int (APIENTRY *IMCAGETDAT)(unsigned long HUGE *datp, int mcano);
typedef int (APIENTRY *IMCAGETSTR)(char far *strp, int mcano);
typedef UINT (APIENTRY *IMCASERVEXEC)(HWND ClientWnd);

typedef VOID (APIENTRY *IMCASTART)(int nSystem);
typedef VOID (APIENTRY *IMCAHALT)(int nSystem);
typedef VOID (APIENTRY *IMCACONTINUE)(int nSystem);
typedef VOID (APIENTRY *IMCAERASE)(int nSystem);
typedef int (APIENTRY *IMCAGETDATA)(ACQDATA FAR *Data, int mcano);

IMCAGETSETTING		fpGetSettings = NULL;
IMCASTORESETTING	fpStoreSettings = NULL;
IMCANEWSTATUS		fpNewStatus = NULL;
IMCAGETSTATUS		fpGetStatus = NULL;
IMCARUNCMD			fpRunCommand = NULL;
IMCAGETCNT			fpLVGetCnt = NULL;
IMCAGETROI			fpLVGetRoi = NULL;
IMCAGETDAT			fpLVGetDat = NULL;
IMCAGETSTR			fpLVGetStr = NULL;
IMCASERVEXEC		fpServExec = NULL;

IMCASTART			fpStart = NULL;
IMCAHALT			fpHalt = NULL;
IMCACONTINUE		fpContinue = NULL;
IMCAERASE			fpErase = NULL;
IMCAGETDATA			fpGetData = NULL;


const int DEV_ID = 0;
const char* MODULE_DISABLED = "axasd module disabled!";
//DATAPTR Data0;
//DATAPTR Data1;

axasd &axasd::getInstance()
{
	static axasd _instance;
	return _instance;
}

bool axasd::isActive()
{
	return (settings.active != 0);
}

int axasd::start(bool restart)
{
	std::cout << "starting acquisition..." << std::endl;
	if (restart)
		(*fpStart)(0);
	else
		(*fpContinue)(0);
	return 0;
}

int axasd::stop()
{
	(*fpHalt)(0);
	std::cout << "acquisition stopped" << std::endl;
	return 0;
}

int axasd::erase()
{
	(*fpErase)(0);
	std::cout << "data erased" << std::endl;
	return 0;
}

std::string axasd::getData()
{
	std::stringstream datasstr;
	datasstr << ";";
	long Errset = (*fpGetSettings)(&settings, DEV_ID);
	if (Errset == FALSE)
		throw dpp_exception("Unable to read detector settings!");
	unsigned long *datp = new unsigned long[settings.range];
	Errset = fpLVGetDat(datp, DEV_ID);
	if (Errset == FALSE)
		throw dpp_exception("Unable to read detector data!");
	for (int i = 0; i < settings.range; i++)
		datasstr << datp[i] << ";";
	delete datp;
	return datasstr.str();
}

int axasd::updateSettings(std::string settingsStr)
{
	long Errset = (*fpGetSettings)(&settings, DEV_ID);
	if (Errset == FALSE)
		throw dpp_exception("Unable to read detector settings!");
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	boost::char_separator<char> sep(";");
	tokenizer tokens(settingsStr, sep);
	for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
	{
		std::string entry = (*tok_iter);
		int eq_pos;
		if ((eq_pos = entry.find_first_of("=")) != std::string::npos)
		{
			std::string key = entry.substr(0, eq_pos);
			std::string value = entry.substr(eq_pos, entry.length() - 1);
			if (key == "range")
				settings.range = std::stol(value);
			if (key == "prena")
				settings.prena = std::stoi(value);
			if (key == "gain")
				settings.gain = std::stoi(value);
			if (key == "shaping")
				settings.shaping = std::stoi(value);
			if (key == "threshold")
				settings.prena = std::stoi(value);
			if (key == "roimin")
				settings.roimin = std::stol(value);
			if (key == "roimax")
				settings.roimax = std::stol(value);
			if (key == "roipreset")
				settings.roipreset = std::stod(value);
			if (key == "rtpreset")
				settings.rtpreset = std::stod(value);
			if (key == "ltpreset")
				settings.ltpreset = std::stod(value);
		}
	}
	(*fpStoreSettings)(&settings, DEV_ID);
	return 0;
}

std::string axasd::getSettings()
{
	long Errset = (*fpGetSettings)(&settings, DEV_ID);

	std::cout << "getSettings:errset=" << Errset << std::endl;
	if (Errset == FALSE)
		throw dpp_exception("Unable to retrieve settings from DPP server!\n");/****/
	std::stringstream settingssstr;
	settingssstr << "active=" << settings.active << ";";
	settingssstr << "range=" << settings.range << ";";
	settingssstr << "prena=" << settings.prena << ";";
	settingssstr << "gain=" << settings.gain << ";";
	settingssstr << "shaping=" << settings.shaping << ";";
	settingssstr << "threshold=" << settings.threshold << ";";
	settingssstr << "roimin=" << settings.roimin << ";";
	settingssstr << "roimax=" << settings.roimax << ";";
	settingssstr << "roipreset=" << settings.roipreset << ";";
	settingssstr << "rtpreset=" << settings.rtpreset << ";";
	settingssstr << "ltpreset=" << settings.ltpreset << ";";
	return settingssstr.str(); 
}

int axasd::initDpp()
{
	/** long Errset=0;

	properties.range = 128;
	settings.range = 128;
	settings.d0 = 2958;
	settings.d1 = 983;
	settings.d2 = 1479;
	settings.d3 = 0;
	settings.d4 = 3276;
	settings.d5 = 0;
	settings.d6 = 576;
	settings.d7 = 4095;
	settings.gain = 8;
	settings.shaping = 8;
	settings.threshold = 2;
	settings.roimin = 1;
	settings.roimax = settings.range;
	settings.active = 1;
	settings.prena = 0;
	settings.roimin = 0;
	settings.roimax =  settings.range;
	settings.roipreset = 0.;
	settings.rtpreset = 200000;
	settings.ltpreset = 1000;


	Errset = (*lpSet)(DEV_ID,  &properties, &settings);
	if (Errset) {
	settings.active = 0;
	throw std::exception("Unable to connect to DPP module!");
	}

	Errset = (*lpDataSet)(DEV_ID, 1, &settings, &Data0, &Data1);
	if (Errset) {
	std::cout << "Unable to read DPP data! Error code: " << Errset << std::endl;
	settings.active = 0;
	return -1;
	}

	Errset = (*lpHalt)(1);
	Errset = (*lpErase)(DEV_ID);
	if (Errset) {
	std::cout << "Unable to reset DPP module! Error code: " << Errset << std::endl;
	settings.active = 0;
	return -1;
	} **/

	long Errset=0;

	Errset = (*fpNewStatus)(0);
	if (Errset == FALSE)
		throw dpp_exception("Unable to set new detector status!\n");

	return 0;

}

std::string axasd::getStatus()
{

	/**if(Status.started) printf("ON\n"); else printf("OFF\n");
	printf("realtime= %.2lf\n", Status.realtime);
	printf("total=   %lf\n", Status.totalsum);
	printf("roi=     %lf\n", Status.roisum);
	printf("totalrate= %.2lf\n", Status.totalrate);
	printf("livetime=  %lf\n", Status.livetime);

	(*lpStatus)(DEV_ID, &acqStatus);

	printf("crst=    %x\n",  acqStatus.crst);
	printf("total=   %lf\n", acqStatus.totalsum);
	printf("roi=     %lf\n", acqStatus.roisum);
	printf("rate=    %.2lf\n", acqStatus.totalrate);
	printf("net=     %lf\n", acqStatus.nettosum);
	printf("livetime=%.2lf\n", acqStatus.livetime);
	printf("deadtime=%.3lf\n", acqStatus.deadtime);**/


	long Errset = (*fpGetStatus)(&acqStatus, DEV_ID);
	if (Errset == FALSE)
		throw dpp_exception("Unable to retrieve detector status!\n");

	std::stringstream statussstr;
	if(acqStatus.started) 
		statussstr << "status=ON;";
	else 
		statussstr << "status=OFF;";
	statussstr << "realtime=" << acqStatus.realtime << ";";
	statussstr << "crst=" << acqStatus.crst << ";";
	statussstr << "totalsum=" << acqStatus.totalsum << ";";
	statussstr << "roisum=" << acqStatus.roisum << ";";
	statussstr << "totalrate=" << acqStatus.totalrate << ";";
	statussstr << "nettosum=" << acqStatus.nettosum << ";";
	statussstr << "livetime=" << acqStatus.livetime << ";";
	statussstr << "deadtime=" << acqStatus.deadtime << ";";
	return statussstr.str();
}

axasd::axasd()
{
	loadDppDll();
	initDpp();
	std::cout << "Connection with DPP server established!" << std::endl;
}

axasd::~axasd()
{
	unloadDppDll();
}

void axasd::loadDppDll() {

	/**hDppDll = LoadLibrary(L"DPPLIB.DLL");
	if(hDppDll) 
	{
	lpSet = (DPPSET) GetProcAddress(hDppDll,"SetDpp");
	lpDataSet = (DPPDATASET) GetProcAddress(hDppDll,"SetDataDpp");
	lpLeave = (DPPLEAVE) GetProcAddress(hDppDll,"LeaveDpp");
	lpGet = (DPPGET) GetProcAddress(hDppDll,"GetDpp");
	lpStart = (DPPSTART) GetProcAddress(hDppDll,"StartDpp");
	lpHalt = (DPPHALT) GetProcAddress(hDppDll,"HaltDpp");
	lpErase = (DPPERASE) GetProcAddress(hDppDll,"EraseDpp");
	lpStatus = (DPPSTATUS) GetProcAddress(hDppDll,"StatusDpp");
	lpGetSdd = (DPPGETSDD) GetProcAddress(hDppDll, "GetSDDAdcData");
	lpVolt2Dac = (DPPVOLTTODAC) GetProcAddress(hDppDll, "VoltToDac");
	lpDac2Volt = (DPPDACTOVOLT) GetProcAddress(hDppDll, "DacToVolt");
	}
	else
	throw std::exception("DPPLIB.DLL not found!\n");**/

	std::cout << "Connecting to DPP dll...";
	hDLL = LoadLibrary(L"DDPP.DLL");
	if (hDLL){
		fpGetSettings = (IMCAGETSETTING) GetProcAddress(hDLL,"GetSettingData");
		fpStoreSettings = (IMCASTORESETTING) GetProcAddress(hDLL,"StoreSettingData");
		fpNewStatus = (IMCANEWSTATUS) GetProcAddress(hDLL,"GetStatus");
		fpGetStatus = (IMCAGETSTATUS) GetProcAddress(hDLL,"GetStatusData");
		fpRunCommand = (IMCARUNCMD) GetProcAddress(hDLL,"RunCmd");
		fpLVGetCnt = (IMCAGETCNT) GetProcAddress(hDLL,"LVGetCnt");
		fpLVGetRoi = (IMCAGETROI) GetProcAddress(hDLL,"LVGetRoi");
		fpLVGetDat = (IMCAGETDAT) GetProcAddress(hDLL,"LVGetDat");
		fpLVGetStr = (IMCAGETSTR) GetProcAddress(hDLL,"LVGetStr");
		fpServExec = (IMCASERVEXEC) GetProcAddress(hDLL,"ServExec");

		fpStart = (IMCASTART) GetProcAddress(hDLL,"Start");
		fpHalt = (IMCAHALT) GetProcAddress(hDLL,"Halt");
		fpContinue = (IMCACONTINUE) GetProcAddress(hDLL,"Continue");
		fpErase = (IMCAERASE) GetProcAddress(hDLL,"Erase");
		fpGetData = (IMCAGETDATA) GetProcAddress(hDLL, "GetData");
	}
	else 
		throw std::exception("DPPLIB.DLL not found!\n");
	std::cout << "done!" << std::endl;
}

void axasd::unloadDppDll()
{
	// (*lpLeave)();
	FreeLibrary(hDLL);
}
