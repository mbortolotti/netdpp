// --------------------------------------------------------------------------
// TSTDPP.C : DPPLIB.DLL Software driver C example
// --------------------------------------------------------------------------

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <windows.h>
#include <time.h>

#include "dpp.h"
#define NUMBOARDS 1

// #define VBDATA
// With VBDATA defined, RAM for spectra must be allocated in main application
// With VBDATA undefined, it is done in the DLL


DATAPTR Data0[NUMBOARDS] = {NULL};
DATAPTR Data1[NUMBOARDS] = {NULL};

//HANDLE         hDLL = 0;
DPPSET         lpSet=NULL;
#ifdef VBDATA
DPPVBDATASET   lpDataSet=NULL;
#else
DPPDATASET     lpDataSet=NULL;
#endif
DPPLEAVE       lpLeave=NULL;
DPPSTART       lpStart=NULL;
DPPGET         lpGet=NULL;
DPPHALT        lpHalt=NULL;
DPPERASE       lpErase=NULL;
DPPSTATUS      lpStatus=NULL;
DPPGETSDD      lpGetSdd=NULL;
DPPVOLTTODAC   lpVolt2Dac=NULL;
DPPDACTOVOLT   lpDac2Volt=NULL;

GENSETTING     Setting[NUMBOARDS]={{0}};
BOARDPROPERTY  Prop[NUMBOARDS]={{0}};
ACQSTATUS      Status1[NUMBOARDS]={{0}};
SDDVSETTING	   Vsetting={{0}};
SDDADCDATA	   Asetting={{0}};

int Board = 0;
int devices = NUMBOARDS;

void help()
{
	printf("Commands:\n");
	printf("q		Quit\n");
	printf("?		Help\n");
	printf("h		Stop Acquisition\n");
	printf("e		Erase spectra\n");
	printf("s		Start Acquisition\n");
	printf("c		Continue Acquisition\n");
	printf("t		Show Status\n");
	printf("r		Show SDD ADC Data\n");
	printf("v		Show SDD Voltage Settings\n");
	printf("w		Write spectra data into file\n");
	printf("o		Write Oscilloscope single shot into file\n");
	//	printf("BOARD=value	change board (0=1st board)\n");
	printf("RANGE=value	spectra length\n");
	printf("PRENA=hexval (0: Realtime preset, 1: Livetime preset, 2: 2: ROI preset, ..)\n");
	// bit0=1 livetime preset enabled, 0 for realtime preset
	// bit 1 ROI preset
	// bit 2 show oscilloscope
	// bit 3 single shot
	printf("RTPRESET=value	\n");
	printf("LTPRESET=value	\n");
	printf("ROIPRESET=value	\n");
	printf("ROIMIN=value \n");
	printf("ROIMAX=value \n");
	printf("GAIN=value	(0..15)\n");
	printf("SHAPING=value	(0..15)\n");
	printf("THRESHOLD=value	(0..15)\n");

	printf("\n");
}

void PrintStatus(int ndsp, ACQSTATUS *Stat)
{
	if(ndsp < devices) {
		if(Stat->started) printf("ON\n"); else printf("OFF\n");
		printf("realtime=%.2lf\n", Stat->realtime);
	}
	else {
		printf("Channel 2:\n");
	}
	printf("crst=    %x\n",  Stat->crst);
	printf("total=   %lf\n", Stat->totalsum);
	printf("roi=     %lf\n", Stat->roisum);
	printf("rate=    %.2lf\n", Stat->totalrate);
	printf("net=     %lf\n", Stat->nettosum);
	printf("livetime=%.2lf\n", Stat->livetime);
	printf("deadtime=%.3lf\n", Stat->deadtime);
}

void PrintSDDdata(SDDADCDATA *Aset)
{
	printf("ADC0     = %-8.4lf V\n", Aset->adc0);
	printf("R18      = %-8.2lf uA\n", Aset->r18);
	printf("RD/FETB  = %-8.2lf nA\n", Aset->rd);
	printf("BACK     = %-8.3lf nA\n", Aset->back);
	printf("IGR      = %-8.2lf nA\n", Aset->igr);
	printf("R1       = %-8.2lf uA\n", Aset->r1);
	printf("TEMP     = %-8.1lf K\n", Aset->temp);
	printf("ADC7     = %-8.4lf V\n", Aset->adc7);
}

void PrintVoltData(SDDVSETTING *Vset, GENSETTING *set)
{
	printf("D5 [V]        : %.4lf\n",Vset->d5v);
	printf("R18 [V]       : %.2lf\n",Vset->r18v);
	printf("RD/FETB [V]   : %.3lf\n",Vset->rdv);
	printf("BACK [V]      : %.2lf\n",Vset->backv);
	printf("IGR [V]       : %.3lf\n",Vset->igrv);
	printf("R1 [V]        : %.3lf\n",Vset->r1v);
	printf("D7 Cooling [V]: %.4lf\n",Vset->r18v);
	printf("TEMP [K]      : %.1lf\n",Vset->tempv);
	printf("THRESHOLD     : %d\n",set->threshold);
	printf("GAIN          : %d\n",set->gain);
	printf("PEAKING [us]  : %.2lf\n", Vset->shapingus);
	if(set->prena & 0x01)
		printf("LTPRESET      : %.0lf\n", set->ltpreset);
	else
		printf("RTPRESET      : %.0lf\n", set->rtpreset);
}

void showstat(void)
{
	int i;
	for (i=0; i<NUMBOARDS; i++) {
		if (Setting[i].active) {
			if (i > 0) printf("Board %d:\n", i);
			(*lpStatus)(i, &Status1[i]);
			PrintStatus(i, &Status1[i]);
		}
	}
}

void writedat(void)
{
	int i,n;
	FILE *f;
	f = fopen("test.mcd", "wt");
	fprintf(f, "range=%ld\n", Setting[Board].range);
	fprintf(f, "prena=%lx\n", Setting[Board].prena);
	fprintf(f, "fmt=asc\n");
	fclose(f);
	f = fopen("test.asc", "wt");
	n = Setting[Board].range;
	for (i=0; i<n; i++)
		fprintf(f, "%ld\n", Data0[Board][i]);
	fclose(f);
}

void writeosc(void)
{
	int i,n;
	FILE *f;
	f = fopen("osc.mcd", "wt");
	fprintf(f, "range=1024\n");
	fprintf(f, "fmt=asc\n");
	fclose(f);
	f = fopen("osc.asc", "wt");
	n = 1024;
	for (i=0; i<n; i++)
		fprintf(f, "%ld\n", Data1[Board][i]);
	fclose(f);
}

int run(char *command)
{
	int i,ret;
	if (!stricmp(command, "?"))           help();
	else if (!stricmp(command,"q"))       return 1;
	else if (!stricmp(command,"h")) {
		(*lpHalt)(1);
		showstat();
	}
	else if (!stricmp(command,"e")) {
		for (i=0; i<NUMBOARDS; i++) {
			if (Setting[i].active) (*lpErase)((short)i);
		}
		showstat();
	}
	else if (!stricmp(command,"s")) {
		for (i=0; i<NUMBOARDS; i++) {
			if (Setting[i].active) (*lpErase)((short)i);
		}
		(*lpStart)(1, 0);
		showstat();
	}
	else if (!stricmp(command,"c")) {
		(*lpStart)(1, 1);
		showstat();
	}
	else if (!stricmp(command,"t")) {
		showstat();
	}
	else if (!stricmp(command,"w")) {
		writedat();
		if (Setting[Board].prena & 0x04) writeosc();
	}
	else if (!stricmp(command,"o")) {
		Setting[Board].prena |= 0x08;
		(*lpSet)(Board, NULL, &Setting[Board]);
		(*lpStatus)(Board, &Status1[Board]);
		writeosc();
	}
	else if (!stricmp(command,"r")) {
		(*lpGetSdd)(Board, &Asetting);
		PrintSDDdata(&Asetting);
	}
	else if (!stricmp(command,"v")) {
		(*lpDac2Volt)(&Vsetting, &Setting[Board]);
		PrintVoltData(&Vsetting, &Setting[Board]);
	}
	/*
	else if (!strncmp(command, "BOARD=", 6)) {  // Change actual Board
	sscanf(command+6, "%ld", &val);
	if ((val < 0) || (val >= NUMBOARDS)) return 0;
	Board = val;
	}
	*/
	else if (!strnicmp(command, "RANGE=", 6)) {
		int val;
		sscanf(command+6, "%ld", &val);
		if (val == Setting[Board].range) return 0;
#ifdef VBDATA
		free(Data0[Board]);
		Setting[Board].range = val;
		Data0[Board] = (unsigned int *)malloc((size_t)val * sizeof(unsigned int));
		ret = (*lpDataSet)(Board, &Setting[Board], Data0[Board], Data1[Board]);
#else
		Setting[Board].range = val;
		ret = (*lpDataSet)(Board, 1, &Setting[Board], &Data0[Board], &Data1[Board]);
#endif
		if (ret) return ret;
	}
	else if (!strnicmp(command, "PRENA=", 6)) {
		sscanf(command+6, "%lx", &Setting[Board].prena);
#ifdef VBDATA
		ret = (*lpDataSet)(Board, &Setting[Board], Data0[Board], Data1[Board]);
#else
		ret = (*lpDataSet)(Board, 1, &Setting[Board], &Data0[Board], &Data1[Board]);
#endif
	}
	else if (!strnicmp(command, "RTPRESET=", 9)) {
		sscanf(command+9, "%lf", &Setting[Board].rtpreset);
		(*lpSet)(Board, NULL, &Setting[Board]);
	}
	else if (!strnicmp(command, "LTPRESET=", 9)) {
		sscanf(command+9, "%lf", &Setting[Board].ltpreset);
		(*lpSet)(Board, NULL, &Setting[Board]);
	}
	else if (!strnicmp(command, "ROIPRESET=", 10)) {
		sscanf(command+10, "%lf", &Setting[Board].roipreset);
		(*lpSet)(Board, NULL, &Setting[Board]);
	}
	else if (!strnicmp(command, "ROIMIN=", 7)) {
		sscanf(command+7, "%ld", &Setting[Board].roimin);
		(*lpSet)(Board, NULL, &Setting[Board]);
	}
	else if (!strnicmp(command, "ROIMAX=", 7)) {
		sscanf(command+7, "%ld", &Setting[Board].roimax);
		(*lpSet)(Board, NULL, &Setting[Board]);
	}
	else if (!strnicmp(command, "GAIN=", 5)) {
		sscanf(command+5, "%ld", &Setting[Board].gain);
		(*lpSet)(Board, NULL, &Setting[Board]);
	}
	else if (!strnicmp(command, "SHAPING=", 8)) {
		sscanf(command+8, "%lx", &Setting[Board].shaping);
		(*lpSet)(Board, NULL, &Setting[Board]);
	}
	else if (!strnicmp(command, "THRESHOLD=", 10)) {
		sscanf(command+10, "%ld", &Setting[Board].threshold);
		(*lpSet)(Board, NULL, &Setting[Board]);
	}

	return 0;
}

int readstr(char *buff, int buflen)
{
	int i=0,ic;

	while ((ic=_getche()) != 13) {
		if (ic == EOF) {
			buff[i]='\0';
			return 1;
		}
		if (ic == 8) {
			if (i>0) i--;
		}
		else {
			buff[i]=(char)ic;
			i++;
		}
		if (i==buflen-1) break;
	}
	buff[i]='\0';
	printf("\n");
	return 0;
}

void main(int argc, char *argv[])  
{
	int i;

	long Errset=0, Erracq=0, Errread=0;
	char command[80];

	HMODULE hDLL1 = LoadLibrary(L"DPPLIB.DLL");
	if(hDLL1){
		printf("DPPLIB.DLL was found!\n");
		lpSet=(DPPSET)GetProcAddress(hDLL1,"SetDpp");
#ifdef VBDATA
		lpDataSet=(DPPVBDATASET)GetProcAddress(hDLL,"SetVBDataDpp");
#else
		lpDataSet=(DPPDATASET)GetProcAddress(hDLL1,"SetDataDpp");
#endif
		lpLeave=(DPPLEAVE)GetProcAddress(hDLL1,"LeaveDpp");
		lpGet=(DPPGET)GetProcAddress(hDLL1,"GetDpp");
		lpStart=(DPPSTART)GetProcAddress(hDLL1,"StartDpp");
		lpHalt=(DPPHALT)GetProcAddress(hDLL1,"HaltDpp");
		lpErase=(DPPERASE)GetProcAddress(hDLL1,"EraseDpp");
		lpStatus=(DPPSTATUS)GetProcAddress(hDLL1,"StatusDpp");
		lpGetSdd=(DPPGETSDD)GetProcAddress(hDLL1, "GetSDDAdcData");
		lpVolt2Dac=(DPPVOLTTODAC)GetProcAddress(hDLL1, "VoltToDac");
		lpDac2Volt=(DPPDACTOVOLT)GetProcAddress(hDLL1, "DacToVolt");
	}
	else {
		printf("DPPLIB.DLL not found!\n");
		printf("Error code:%d\n", GetLastError());
		return;
	}/****/

	// Initialize parameters
	//Prop[0].updaterate = 500;
	Prop[0].range = 8192;
	Setting[0].range = 8192;
	Setting[0].d0 = 2958;
	Setting[0].d1 = 983;
	Setting[0].d2 = 1479;
	Setting[0].d3 = 0;
	Setting[0].d4 = 3276;
	Setting[0].d5 = 0;
	Setting[0].d6 = 576;
	Setting[0].d7 = 4095;
	Setting[0].gain = 7;
	Setting[0].shaping = 15;
	Setting[0].threshold = 2;
	Setting[0].roimin = 1;
	Setting[0].roimax = Setting[0].range;
	Setting[0].active = 1;
	Setting[0].prena = 0;
	Setting[0].roimin = 0;
	Setting[0].roimax =  Setting[0].range;
	Setting[0].roipreset = 0.;
	Setting[0].rtpreset = 200000;
	Setting[0].ltpreset = 1000;

	for (i=1; i<NUMBOARDS; i++) {
		memcpy(&Setting[i], &Setting[0], sizeof(GENSETTING));
		memcpy(&Prop[i], &Prop[0], sizeof(BOARDPROPERTY));
	}
	for (i=0; i< NUMBOARDS; i++) {
		Errset = (*lpSet)(i,  &Prop[i], &Setting[i]);
		if (Errset) {
			//printf("SetDpp returned %ld\n", Errset);  goto out;
			Setting[i].active = 0;
		}
		//Errset = (*lpSet)((short)i, NULL, &Setting[i]);
		else {
			printf ("DPP serial no. : %s\n", Prop[i].SerNum);
			printf ("DPP description: %s\n", Prop[i].Description);
		}
	}
	for (i=0; i< NUMBOARDS; i++) {
		if (!Setting[i].active) continue;
#ifdef VBDATA
		Data0[i] = (unsigned int *)malloc(8192 * sizeof(unsigned int));
		Data1[i] = (unsigned int *)malloc(1024 * sizeof(unsigned int));
		Errset=(*lpDataSet)(i, &Setting[i], Data0[i], Data1[i]);
		if (Errset) {
			printf("SetVBDataDpp returned %ld\r\n", Errset);
			return;
		}
#else
		Errset=(*lpDataSet)(i, 1, &Setting[i], &Data0[i], &Data1[i]);
		if (Errset) {
			printf("SetDataDpp returned %ld\r\n", Errset);
			return;
		}
#endif
		Errset = (*lpGet)(i, &Prop[i], &Setting[i]);
		(*lpStatus)(i, &Status1[i]);

		if (!Status1[i].started) {
			//   ----- Erase memory --------------
			Errset = (*lpErase)(i);
			if (Errset) {
				printf("EraseDpp returned %ld\n", Errset);
			}
		}
	}

	// ----- Get Status ----------------
	showstat();

	//help();
	printf("? for help\n");

	while(TRUE)
	{
		/* if (!_kbhit()) {
		for (i=0; i< NUMBOARDS; i++) {
		if (Status1[i].started && Setting[i].sequential) {
		if ((*lpCheckSeq)((short)i, &adr)) printf("%ld\n",adr);
		}
		}
		}
		else  */
		{
			i = _getche();
			switch(i) {
			case 'q':  // Quit
			case '?':  // Help
			case 's':  // Start
			case 'h':  // Halt
			case 't':  // Status
			case 'c':  // Continue
			case 'e':  // Erase
				command[0] = i;
				command[1] = '\0';
				break;
			default:
				command[0] = i;
				readstr(command+1,70);
				//printf("%s\r\n", command);
				break;
			}
			if (run(command)) break;
		}
		/*  scanf("%s", command);
		if (run(command)) break; */
	}

	//out:
	(*lpLeave)();

	//FreeLibrary(hDLL);

	return;
}
