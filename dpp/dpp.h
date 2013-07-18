#define WINDOWSNT

	// Board Properties and initialisation parameters
	typedef struct{
		long range;           // 4096 or 8192
		char SerNum[64];		// "SE000008"
		char Description[64]; // "DPP V1.0"
	} BOARDPROPERTY;

	// Status
	typedef struct {
		int started;          // aquisition status: 1 if running, 0 else
		int crst;				// bit 3 = C acquisition complete, 
		// bit 2 = R spectra cleared, 
		// bit 1 = S stopped, 
		// bit 0 = T live time mode
		double realtime;      // real time in seconds
		double totalsum;      // total events
		double roisum;        // events within ROI
		double totalrate;     // acquired events per second

		double nettosum;      // ROI sum with background subtracted
		double livetime;      // live time in seconds
		double deadtime;      // deadtime in percent
		unsigned long maxval; // Maximum value in spectrum
	} ACQSTATUS;

	typedef struct{
		int range;           // spectrum length
		int prena;            // bit0=1 livetime preset enabled, 0 for realtime preset
		// bit 1 ROI preset
		// bit 2 show oscilloscope
		// bit 3 single shot
		int d0;               // r18 (rx)  [V] = -(180/4095) * D0 (def. 2321)
		int d1;			    // rd (fetb) [V] = -( 25/4095) * D1 (def. 1146)
		int d2;               // back      [V] = -(180/4095) * D2 (def. 1754)
		int d3;               // igr       [V] = -( 25/4095) * D3 (def. 0)
		int d4;               // r1        [V] = -( 25/4095) * D4 (def. 1239)
		int d5;               // D5(spared)[V] = -(2.5/4095) * D5 (def. 0)
		int d6;               // temp      [K] = 226 + D6 / 30    (def. 577)
		int d7;               // cooling   [V] = -(2.5/4095) * D7 (def. 4095)
		int gain;             // 0..15, def. 8
		int shaping;          // 0..15, def. 9 
		int threshold;		// 0..15, def. 6
		int roimin;			// lower ROI limit
		int roimax;			// upper limit: roimin <= channel < roimax
		int active;           // system number 1..4 if active, 0 if not active
		double roipreset;     // ROI preset value
		double rtpreset;      // rtime preset value
		double ltpreset;      // ltime preset value
	} GENSETTING;

	typedef struct{
		double r18v;	// RX   for VITUS, outer resistive divider voltage
		// R18[V]  = -(180/4095) * D0
		double rdv;	// FETB for VITUS, Reset Diode voltage (IFET SDD) or FFET Bulk voltage
		// RD[V]   = -(25/4095)  * D1
		double backv;	// Back voltage (both sensors)
		// Back[V] = -(180/4095) * D2
		double igrv;	// Inner Guard Ring voltage (IFET SDD only)
		// IGR[V]  = -(25/4095)  * D3
		double r1v;	// Inner resistive divider voltage, R1 for both IFET and VITUS SDD
		// R1[V]   = -(25/4095)  * D4
		double d5v;	// Spared
		// D5[V]   = -(2.5/4095) * D5
		double tempv;	// Set temperature
		// Temp[K] = 226         + D6 / 30
		double d7v;	// Allow cooling, should be set between 3900 and 4095. 
		// If lower, detector cooling is forbidden
		// D7[V]   = -(2.5/4095) * D7
		double shapingus; // 0..15, {0.44, 0.7, 1, 1.25, 1.5, 1.8, 2, 2.3, 
		//         2.6, 2.85, 3.1, 3.4, 3.65, 3.9, 4.2, 4.5} us 
	} SDDVSETTING;

	typedef struct {
		double adc0;	// ADC0 / 1638. (V)
		double r18;	// ADC5 / 16.38 (mikroA)
		double rd;	// ADC4 / 16.38 (nA)
		double back;	// ADC3 / 163.8 (nA)
		double igr;	// ADC2 / 16.38 (nA)
		double r1;	// ADC1 / 16.38 (mikroA)
		double temp;	// ADC6 / 12.   (K)
		double adc7;  // ADC7 / 1638. (V)
	} SDDADCDATA;


#define CN_REALTIME  0
#define CN_TOTALSUM  1
#define CN_ROISUM    2
#define CN_RATE		 3
#define CN_NETTOSUM  4
#define CN_LIVETIME  5
#define CN_DEADTIME  6
#define CN_STATUS	 10

#define DATAPTR   unsigned long*

#ifdef DLL
	long WINAPI SetDpp(int board, BOARDPROPERTY *prop, GENSETTING *setting);
	// Initialize, set parameters and board properties
	// Pass NULL for any settings pointer you don't want to set now.
	// Must be called at first to get a handle to the driver

	long WINAPI SetDataDpp(int board, int allocmode, GENSETTING *set,
		DATAPTR *data0, DATAPTR *data1);
	// if allocmode==1 memory will be allocated, 
	// for allocmode==0 allocate memory in your application 
	// (length Setting->range*sizeof(DWORD) or for sequential mode 
	// cycles * range ))
	// and pass pointer to spectra pointer  

	long WINAPI SetVBDataDpp(int board, GENSETTING *set,
		DATAPTR data0, DATAPTR data1);
	// Version for Visual Basic or other languages which 
	// don't have pointers to pointers
	// allocmode = 0, allocate memory in your application: 
	// Dim Data0(8192) As Integer, Dim Data1(1024) As Integer
	// Declare Function SetVBDataDpp Lib "DPPLIB.DLL" Alias "#4" 
	// (ByVal Board As Integer, ByRef Setting As Gensetting, 
	// ByRef Dat0 As Integer, ByRef Dat1 As Integer)
	// Calling GetStatus will also read the spectra

	long WINAPI OpenDatDpp(int board, 
		HANDLE *handle0, HANDLE *handle1,
		DATAPTR *data0, DATAPTR *data1);
	// Open a view to the data for allocmode=1
	// Can be used for secondary application accessing the DLL
	// Save handle for closing

	long WINAPI CloseDatDpp(int board, 
		HANDLE *handle0, HANDLE *handle1,
		DATAPTR *data0, DATAPTR *data1);
	// Close a view to the data for allocmode=1

	long WINAPI LeaveDpp(void); // Close everything

	long WINAPI GetDpp(int board, BOARDPROPERTY *prop, GENSETTING *setting);
	// Get settings, returns the active memory size. 
	// Pass NULL for any settings pointer you are not interested now.					 

	long WINAPI StartDpp(int isys, int bcontinue);
	// Start acquisition
	// isys is the system number Setting.active (def. == 1)
	// Set bcontinue=1 for continue acquisition 

	long WINAPI HaltDpp(int isys);
	// Stop acquisition

	long WINAPI EraseDpp(int board);
	// Clear spectra

	long WINAPI StatusDpp (int board, ACQSTATUS *status);
	// gets acquisition status

	long WINAPI WriteDpp (int board);
	// Load spectra in data0 into DPP, including
	// corresponding Status.realtime and Status.livetime

	int WINAPI GetAdc(int board, int adcnum);
	// Read SDD single ADC	

	int WINAPI GetSDDAdcData(int board, SDDADCDATA *adcdata);
	// Read SDD all ADCs

	int WINAPI VoltToDac(GENSETTING *setting, SDDVSETTING *vsetting);
	// transforms voltages to dac values

	int WINAPI DacToVolt(SDDVSETTING *vsetting, GENSETTING *setting);
	// transforms dac values to voltages

	int APIENTRY BytearrayToShortarray(short *Shortarray, char *Bytearray, int length);
	// auxiliary function for VB.NET to convert strings

#else

	typedef long (WINAPI *DPPSET) (int board, BOARDPROPERTY *prop,
		GENSETTING *setting);

	typedef long (WINAPI *DPPDATASET) (int board, int allocmode, GENSETTING *set,
		DATAPTR *data0, DATAPTR *data1);

	typedef long (WINAPI *DPPVBDATASET) (int board, GENSETTING *set,
		DATAPTR data0, DATAPTR data1);

	typedef long (WINAPI *DPPOPENDAT) (int board, 
		HANDLE *handle0, HANDLE *handle1,
		DATAPTR *data0, DATAPTR *data1);

	typedef long (WINAPI *DPPCLOSEDAT) (int board, 
		HANDLE *handle0, HANDLE *handle1,
		DATAPTR *data0, DATAPTR *data1);

	typedef long (WINAPI *DPPLEAVE) (void); 

	typedef long (WINAPI *DPPGET) (int board, BOARDPROPERTY *prop,
		GENSETTING *setting);

	typedef long (WINAPI *DPPSTART) (int isys, int bcontinue);

	typedef long (WINAPI *DPPHALT) (int isys);

	typedef long (WINAPI *DPPERASE) (int board);

	typedef long (WINAPI *DPPSTATUS) (int board, ACQSTATUS *status);

	typedef long (WINAPI *DPPGETSDD) (int board, SDDADCDATA *adcdata);
	// Read SDD all ADCs

	typedef long (WINAPI *DPPVOLTTODAC) (GENSETTING *setting, SDDVSETTING *vsetting);
	// transforms voltages to dac values

	typedef long (WINAPI *DPPDACTOVOLT) (SDDVSETTING *vsetting, GENSETTING *setting);
	// transforms dac values to voltages

	typedef long (WINAPI *DPPWRITE) (int board);
	// Load spectra in data0 into DPP, including
	// corresponding Status.realtime and Status.livetime

#endif
