#ifdef __cplusplus
extern "C"	
{
#endif

#include "struct.h"
#define MAXCNT              64
#define MAXDEV 9
#define MAXDSP 9

#define ID_SAVE             103
#define ID_CONTINUE         106
#define ID_START            109
#define ID_BREAK            137
#define ID_NEWSETTING       139
#define ID_GETSTATUS        141
#define ID_SAVEFILE         151
#define ID_ERASE            154
#define ID_LOADFILE         155
#define ID_NEWDATA          160
#define ID_HARDWDLG         161
#define ID_SAVEFILE2                194
#define ID_LOADFILE2                203
#define ID_SAVEFILE3                217
#define ID_LOADFILE3                219
#define ID_SAVEFILE4                223
#define ID_LOADFILE4                225
#define ID_LOADFILE5                226
#define ID_LOADFILE6                227
#define ID_LOADFILE7                228
#define ID_LOADFILE8                229
#define ID_SAVEFILE5                230
#define ID_SAVEFILE6                231
#define ID_SAVEFILE7                232
#define ID_SAVEFILE8                233
#define ID_SUMFILE                      234
#define ID_SUMFILE2                     235
#define ID_SUMFILE3                     236
#define ID_SUMFILE4                     237
#define ID_SUMFILE5                     238
#define ID_SUMFILE6                     239
#define ID_SUMFILE7                     240
#define ID_SUMFILE8                     241
#define ID_SUBTRACT                     289
#define ID_SMOOTH                       290
#define ID_SUBTRACT2                    296
#define ID_SMOOTH2                      297
#define ID_SUBTRACT3                    298
#define ID_SMOOTH3                      299
#define ID_SUBTRACT4                    300
#define ID_SMOOTH4                      301
#define ID_SUBTRACT5                    302
#define ID_SMOOTH5                      303
#define ID_SUBTRACT6                    304
#define ID_SMOOTH6                      305
#define ID_SUBTRACT7                    306
#define ID_SMOOTH7                      307
#define ID_SUBTRACT8                    308
#define ID_SMOOTH8                      309
#define ID_COMBDLG          401
#define ID_DATADLG          402
#define ID_ERASE2          1108
#define ID_ERASE3          1109
#define ID_ERASE4          1110
#define ID_ERASEFILE2      1111
#define ID_ERASEFILE3      1112
#define ID_ERASEFILE4      1113
#define ID_START2          1114
#define ID_BREAK2          1115
#define ID_CONTINUE2       1116
#define ID_START3          1117
#define ID_BREAK3          1118
#define ID_CONTINUE3       1119
#define ID_START4          1120
#define ID_BREAK4          1121
#define ID_CONTINUE4       1122
#define ID_RUNCMD                       1123
#define ID_RUNCMD2                      1124
#define ID_RUNCMD3                      1125
#define ID_RUNCMD4                      1126
#define ID_RUNCMD5                      1127
#define ID_RUNCMD6                      1128
#define ID_RUNCMD7                      1129
#define ID_RUNCMD8                      1130
#define ID_ERASEFILE5                   1131
#define ID_ERASEFILE6                   1132
#define ID_ERASEFILE7                   1133
#define ID_ERASEFILE8                   1134
#define ID_CARDSET                      1147
#define ID_AXASSET						1148
#define ID_AXASGETEEP					1149
#define ID_GETAXASADC					1150
	

/*** FUNCTION PROTOTYPES (do not change) ***/

#ifdef WINDOWS31
int  FAR PASCAL LibMain(HANDLE, WORD, WORD, LPSTR);
int FAR PASCAL WEP (int bSystemExit);
#else
BOOL APIENTRY DllMain(HANDLE hInst, DWORD ul_reason_being_called, LPVOID lpReserved);
#endif
VOID APIENTRY StoreBoardData(BOARD FAR *Mca, int mcano);
                                           // Stores Board Properties into the DLL
int APIENTRY GetBoardData(BOARD FAR *Mca, int mcano);
                                           // Get Board Properties stored in the DLL
VOID APIENTRY UnregisterSlave(int mcano);  // Inform DLL that Slave is closed
VOID APIENTRY DestroySlaves(void);		   // Close all Slaves
VOID APIENTRY DestroySlave(int mcano);	   // Close one slave
VOID APIENTRY HaltSlave(int mcano);        // Stop slave
VOID APIENTRY ClearSlave(int mcano);	   // Clear Slave
VOID APIENTRY ContinueSlave(int mcano);	   // Continue Slave
VOID APIENTRY StartSlave(int mcano);	   // Start Slave
VOID APIENTRY GetSlaveStatus(int mcano);   // Get Slave Status
VOID APIENTRY StoreSettingData(ACQSETTING FAR *Setting, int mcano);
                                           // Stores Settings into the DLL
int APIENTRY GetSettingData(ACQSETTING FAR *Setting, int mcano);
                                           // Get Settings stored in the DLL
                                           // Store System Definition into DLL
VOID FAR PASCAL StoreExtSettingData(EXTACQSETTING FAR *Setting, int mcano);
int APIENTRY GetExtSettingData(EXTACQSETTING FAR *Setting, int mcano);
VOID APIENTRY StoreStatusData(ACQSTATUS FAR *Status, int mcano);
                                           // Store the Status into the DLL
int APIENTRY GetStatusData(ACQSTATUS FAR *Status, int mcano);
                                           // Get the Status
VOID APIENTRY Start(int nSystem);          // Start
VOID APIENTRY Halt(int nSystem);           // Halt

VOID APIENTRY Continue(int nSystem);       // Continue
VOID APIENTRY NewSetting(int mcano);     // Indicate new Settings to Server
UINT APIENTRY ServExec(HWND ClientWnd);  // Execute the Server MCA.EXE
VOID APIENTRY StoreData(ACQDATA FAR *Data, int mcano);
                                           // Stores Data pointers into the DLL
int APIENTRY GetData(ACQDATA FAR *Data, int mcano);
                                           // Get Data pointers
long APIENTRY GetSpec(long i, int mcano);
                                           // Get a spectrum value
VOID APIENTRY SaveSetting();               // Save Settings
int APIENTRY GetStatus(int mcano);         // Request actual Status from Server
VOID APIENTRY Erase(int nSystem);          // Erase spectrum
VOID APIENTRY SaveData(int mcano);         // Saves data
VOID APIENTRY GetBlock(long FAR *hist, int start, int end, int step,
  int mcano);                              // Get a block of spectrum data
VOID APIENTRY StoreDefData(ACQDEF FAR *Def);
int APIENTRY GetDefData(ACQDEF FAR *Def);
                                           // Get System Definition
VOID APIENTRY LoadData(int mcano);       // Loads data
VOID APIENTRY AddData(int mcano);        // Adds data
VOID APIENTRY SubData(int mcano);        // Subtracts data
VOID APIENTRY Smooth(int mcano);         // Smooth data
VOID APIENTRY NewData(void);             // Indicate new ROI or string Data
VOID APIENTRY HardwareDlg(int item);     // Calls the Settings dialog box
VOID APIENTRY UnregisterClient(void);    // Clears remote mode from MCDWIN
VOID APIENTRY DestroyClient(void);       // Close MCDWIN
UINT APIENTRY ClientExec(HWND ServerWnd);
                                           // Execute the Client MCDWIN.EXE
int APIENTRY LVGetDat(unsigned long HUGE *datp, int mcano);
                                           // Copies the spectrum to an array
VOID APIENTRY RunCmd(int mcano, LPSTR Cmd);
                                           // Executes command
int APIENTRY LVGetRoi(unsigned long FAR *roip, int mcano);
                                     // Copies the ROI boundaries to an array
int APIENTRY LVGetCnt(double far *cntp, int mcano);
                                           // Copies Cnt numbers to an array
int APIENTRY LVGetStr(char far *strp, int mcano);
                                           // Copies strings to an array
int APIENTRY CardSet(int mcano);
										   // Update hardware settings
int APIENTRY AxasSet(int mcano, AXASSETTING FAR *ASetting);
										   // Store New Axas Settings
int APIENTRY GetAxasSetting(int mcano, AXASSETTING FAR *ASetting);
										   // Get Axas Settings
int APIENTRY StoreAxasSetting(int mcano, AXASSETTING FAR *ASetting);
										   // Slave stores New Axas Settings
int APIENTRY AxasGetEEP(int mcano, AXASSETTING FAR *ASetting);
										   // Read Axas EEP
int APIENTRY AxasGetADC(int mcano, int adcnum);
										   // Read Axas ADC
#ifdef __cplusplus
}
#endif

