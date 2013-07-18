#undef  WINDOWS31

#ifdef WINDOWS31

#define FIND_WINDOW(a,b) FindWindow(b,a)
#define HUGE huge
#define USHORT unsigned short
#define APIENTRY FAR PASCAL

#else

#define FIND_WINDOW(a,b) FindWindow(a,b)
#define HUGE
#define _fmemcpy memcpy
#define _fstrcpy strcpy

#endif

typedef struct{
  long port;			// COM port, 0 = COM1
  long baud;			// baudrate
  long range;           // 2048 or 4096
} BOARD;

typedef struct{
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
  long range;           // spectrum length
  int prena;            // bit0=1 liveltime preset enabled, 0 for realtime preset
						// bit 1 ROI preset
						// bit 2 show oscilloscope
  int d0;               // r18 (rx)  [V] = -(180/4095) * D0 (def. 2321)
  long roimin;          // lower ROI limit
  long roimax;          // upper limit: roimin <= channel < roimax
  int d1;			    // rd (fetb) [V] = -( 25/4095) * D1 (def. 1146)
  double roipreset;     // ROI preset value
  double rtpreset;      // time preset value
  int savedata;         // 1 if auto save after stop
  int fmt;              // format type: 0 == ASCII, 1 == binary, 2 == GANAAS
  int autoinc;          // 1 if auto increment filename
  int d2;               // back      [V] = -(180/4095) * D2 (def. 1754)
  int d3;               // igr       [V] = -( 25/4095) * D3 (def. 0)
  int d4;               // r1        [V] = -( 25/4095) * D4 (def. 1239)
  int d5;               // D5(spared)[V] = -(2.5/4095) * D5 (def. 0)
  int d6;               // temp      [K] = 226 + D6 / 30    (def. 577)
  int d7;               // cooling   [V] = -(2.5/4095) * D7 (def. 4095)
  int gain;             // 0..15, def. 8
  int shaping;          // 0..15, def. 9 
  int threshold;		// 0..15, def. 6
  double ltpreset;      // 
  int nregions;         // number of regions
  int caluse;           // use calibration
  double scalpreset;    // dummy
  int active;           // 0 if disabled, 1..4 if active in system 1..4
  int calpoints;        // number of calibration points
} ACQSETTING;


typedef struct{
  long range;           // spectrum length
  int prena;            // bit0=1 liveltime preset enabled, 0 for realtime preset
						// bit 1 ROI preset
						// bit 2 show oscilloscope
  int d0;               // r18 (rx)  [V] = -(180/4095) * D0 (def. 2321)
  long roimin;          // lower ROI limit
  long roimax;          // upper limit: roimin <= channel < roimax
  int d1;			    // rd (fetb) [V] = -( 25/4095) * D1 (def. 1146)
  double roipreset;     // ROI preset value
  double rtpreset;      // time preset value
  int savedata;         // 1 if auto save after stop
  int fmt;              // format type: 0 == ASCII, 1 == binary, 2 == GANAAS
  int autoinc;          // 1 if auto increment filename
  int d2;               // back      [V] = -(180/4095) * D2 (def. 1754)
  int d3;               // igr       [V] = -( 25/4095) * D3 (def. 0)
  int d4;               // r1        [V] = -( 25/4095) * D4 (def. 1239)
  int d5;               // D5(spared)[V] = -(2.5/4095) * D5 (def. 0)
  int d6;               // temp      [K] = 226 + D6 / 30    (def. 577)
  int d7;               // cooling   [V] = -(2.5/4095) * D7 (def. 4095)
  int gain;             // 0..15, def. 8
  int shaping;          // 0..15, def. 9 
  int threshold;		// 0..15, def. 6
  double ltpreset;      // 
  int nregions;         // number of regions
  int caluse;           // use calibration
  double scalpreset;    // dummy
  int active;           // 0 if disabled, 1..4 if active in system 1..4
  int calpoints;        // number of calibration points
  long xdim;
  long ndim;
  long type;
  long reserved[13];
} EXTACQSETTING;

typedef struct{
  int r18;		// RX   for VITUS, outer resistive divider voltage
				// R18[V]  = -(180/4095) * D0
  int reset;	// FETB for VITUS, Reset Diode voltage (IFET SDD) or FFET Bulk voltage
				// RD[V]   = -(25/4095)  * D1
  int back;		// Back voltage (both sensors)
				// Back[V] = -(180/4095) * D2
  int igr;		// Inner Guard Ring voltage (IFET SDD only)
				// IGR[V]  = -(25/4095)  * D3
  int r1;		// Inner resistive divider voltage, R1 for both IFET and VITUS SDD
				// R1[V]   = -(25/4095)  * D4
  int d5;		// Spared
				// D5[V]   = -(2.5/4095) * D5
  int temp;		// Set temperature
  				// Temp[K] = 226         + D6 / 30
  int d7;		// Allow cooling, should be set between 3900 and 4095. 
				// If lower, detector cooling is forbidden
  				// D7[V]   = -(2.5/4095) * D7
  int gain;		// 0 .. 15, 0 switches MCA off.
  int threshold; // 0 .. 15, 5 or 6 is normal
  int shaping;  // 0..15, {0.44, 0.7, 1, 1.25, 1.5, 1.8, 2, 2.3, 
				//         2.6, 2.85, 3.1, 3.4, 3.65, 3.9, 4.2, 4.5} us 
  int blive;	// =1 means timepreset set to livetime, otherwise to realtime
} AXASSETTING;

typedef struct{
  unsigned long HUGE *s0;          // pointer to spectrum
  unsigned long *region;       // pointer to regions
  unsigned char *comment0;     // pointer to strings
  double *cnt;                 // pointer to counters
  HANDLE hs0;
  HANDLE hrg;
  HANDLE hcm;
  HANDLE hct;
} ACQDATA;

typedef struct {
  int nDevices;          // Number of spectra = number of modules
  int nDisplays;         // Number of active displays 0...nDevices
  int nSystems;          // Number of systems 0...4
  int bRemote;           // 1 if server controlled by MCDWIN
  unsigned int sys;        // System definition word:
                           // bit0=0, bit1=0: MCD#0 in system 1
                           // bit0=1, bit1=0: MCD#0 in system 2
                           // bit0=0, bit1=1: MCD#0 in system 3
                           // bit0=1, bit1=1: MCD#0 in system 4
                           // bit2=0, bit3=0: MCD#1 in system 1 ...
                           // bit6=1, bit7=1: MCD#3 in system 4
						   // bit14=1, bit15=1: MCD#7 in system 4
} ACQDEF;
