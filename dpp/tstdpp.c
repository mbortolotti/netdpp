// --------------------------------------------------------------------------
// TSTDPP.C : DDPP.DLL Software driver C example
// --------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <time.h>

#undef DLL
#include "ddpp.h"


HANDLE          hDLL = 0;

IMCAGETSETTING  lpSet=NULL;
IMCANEWSTATUS   lpNewStat=NULL;
IMCAGETSTATUS   lpStat=NULL;
IMCARUNCMD      lpRun=NULL;
IMCAGETCNT      lpCnt=NULL;
IMCAGETROI      lpRoi=NULL;
IMCAGETDAT      lpDat=NULL;
IMCAGETSTR      lpStr=NULL;
IMCASERVEXEC    lpServ=NULL;

ACQSETTING     Setting={0};
ACQDATA        Data={0};
ACQDEF         Def={0};
ACQSTATUS      Status={0};

short Board=0;

void help()
{
	printf("Commands:\n");
	printf("Q		Quit\n");
	printf("H		Help\n");
	printf("S       Status\n");
    printf("(... more see command language in MCDWIN help)\n");
    printf("\n");
}

void PrintStatus(ACQSTATUS *Stat)
{
  if(Stat->started) printf("ON\n"); else printf("OFF\n");
  printf("realtime= %.2lf\n", Stat->realtime);
  printf("total=   %lf\n", Stat->totalsum);
  printf("roi=     %lf\n", Stat->roisum);
  printf("totalrate= %.2lf\n", Stat->totalrate);
  printf("livetime=  %lf\n", Stat->livetime);
}

void PrintSetting(ACQSETTING *Set)
{
  printf("range= %ld\n", Set->range);
  printf("prena= %x\n", Set->prena);
  printf("d0= %d\n", Set->d0);
  printf("roimin= %ld\n", Set->roimin);
  printf("roimax= %ld\n", Set->roimax);
  printf("d1= %d\n", Set->d1);
  printf("roipreset= %lg\n", Set->roipreset);
  printf("rtpreset= %lg\n", Set->rtpreset);
  printf("savedata= %d\n", Set->savedata);
  printf("fmt= %d\n", Set->fmt);
  printf("autoinc= %d\n", Set->autoinc);
  printf("d2= %d\n", Set->d2);
  printf("d3= %d\n", Set->d3);
  printf("d4= %d\n", Set->d4);
  printf("d5= %d\n", Set->d5);
  printf("d6= %d\n", Set->d6);
  printf("d7= %d\n", Set->d7);
  printf("gain= %d\n", Set->gain);
  printf("shaping= %d\n", Set->shaping);
  printf("threshold= %d\n", Set->threshold);
  printf("ltpreset= %lg\n", Set->ltpreset);
  printf("nregions= %d\n", Set->nregions);
  printf("caluse= %d\n", Set->caluse);
  //printf("scalpreset= %lg\n", Set->scalpreset);
  printf("active= %d\n", Set->active);
  printf("calpoints= %d\n", Set->calpoints);
}

int run(char *command)
{
	int err;
	if (!stricmp(command, "H"))           help();
	else if (!stricmp(command,"Q"))       return 1;
	else if (!stricmp(command,"S")) {
      err = (*lpStat)(&Status, 0);
      PrintStatus(&Status);
	}
	else {
		(*lpRun)(0, command);
		printf("%s\n", command);
	}
	return 0;
}

//int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmd, int nShow)
void main(int argc, char *argv[])  
{
  long Errset=0, Erracq=0, Errread=0;
  char command[80];

  hDLL = LoadLibrary("DDPP.DLL");
  if(hDLL){
      lpSet=(IMCAGETSETTING)GetProcAddress(hDLL,"GetSettingData");
	  lpNewStat=(IMCANEWSTATUS)GetProcAddress(hDLL,"GetStatus");
	  lpStat=(IMCAGETSTATUS)GetProcAddress(hDLL,"GetStatusData");
	  lpRun=(IMCARUNCMD)GetProcAddress(hDLL,"RunCmd");
	  lpCnt=(IMCAGETCNT)GetProcAddress(hDLL,"LVGetCnt");
	  lpRoi=(IMCAGETROI)GetProcAddress(hDLL,"LVGetRoi");
	  lpDat=(IMCAGETDAT)GetProcAddress(hDLL,"LVGetDat");
	  lpStr=(IMCAGETSTR)GetProcAddress(hDLL,"LVGetStr");
	  lpServ=(IMCASERVEXEC)GetProcAddress(hDLL,"ServExec");
  }
  else return;

  // Initialize parameters
//  Errset = (*lpServ)(0);
  Errset = (*lpNewStat)(0);
  Errset = (*lpStat)(&Status, 0);
  PrintStatus(&Status);

  (*lpSet)(&Setting, 0);  
  PrintSetting(&Setting);
 

  printf("\nCommands:\n");
  help();

  while(TRUE)
	{
		scanf("%s", command);
		if (run(command)) break;
	}

  FreeLibrary(hDLL);

  return;
}
