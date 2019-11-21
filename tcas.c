
/*  -*- Last-Edit:  Fri Jan 29 11:13:27 1993 by Tarak S. Goradia; -*- */
/* $Log: tcas.c,v $
 * Revision 1.2  1993/03/12  19:29:50  foster
 * Correct logic bug which didn't allow output of 2 - hf
 * */
#include "FaultSeeds.h"
#include <stdio.h>

#ifndef FAULT_V13
#define OLEV       600		/* in feets/minute */
#else
#define OLEV       600+100		/* in feets/minute */
#endif

#ifndef FAULT_V14
#define MAXALTDIFF 600		/* max altitude difference in feet */
#else
#define MAXALTDIFF 600+50		/* max altitude difference in feet */
#endif

#ifndef FAULT_V15
#define MINSEP     300          /* min separation in feet */
#else
#define MINSEP     300+350          /* min separation in feet */
#endif

#define NOZCROSS   100		/* in feet */
				/* variables */

typedef int bool;

int Cur_Vertical_Sep;
bool High_Confidence;
bool Two_of_Three_Reports_Valid;

int Own_Tracked_Alt;
int Own_Tracked_Alt_Rate;
int Other_Tracked_Alt;

int Alt_Layer_Value;		/* 0, 1, 2, 3 */
#ifndef FAULT_V38
int Positive_RA_Alt_Thresh[4];
#else
int Positive_RA_Alt_Thresh[3];
#endif

int Up_Separation;
int Down_Separation;

				/* state variables */
int Other_RAC;			/* NO_INTENT, DO_NOT_CLIMB, DO_NOT_DESCEND */
#define NO_INTENT 0
#define DO_NOT_CLIMB 1
#define DO_NOT_DESCEND 2

int Other_Capability;		/* TCAS_TA, OTHER */
#define TCAS_TA 1
#define OTHER 2

int Climb_Inhibit;		/* true/false */

#define UNRESOLVED 0
#define UPWARD_RA 1
#ifndef FAULT_V36
#define DOWNWARD_RA 2
#else
#define DOWNWARD_RA 1
#endif

void initialize()
{
#ifndef FAULT_V33
  #ifndef FAULT_V16
    Positive_RA_Alt_Thresh[0] = 400;
  #else
    Positive_RA_Alt_Thresh[0] = 400+1;
  #endif

  #ifndef FAULT_V7
    #ifndef FAULT_V17
    Positive_RA_Alt_Thresh[1] = 500;
    #else
    Positive_RA_Alt_Thresh[1] = 500+1;
    #endif
  #else
    Positive_RA_Alt_Thresh[1] = 550; /* constant mutation */
  #endif

  #ifndef FAULT_V18
    Positive_RA_Alt_Thresh[2] = 640;
  #else
    Positive_RA_Alt_Thresh[2] = 640+50;
  #endif

  #ifndef FAULT_V8
    #ifndef FAULT_V19
    Positive_RA_Alt_Thresh[3] = 740;
    #else
    Positive_RA_Alt_Thresh[3] = 740+20;
    #endif
  #else
    Positive_RA_Alt_Thresh[3] = 700; /* constant mutation */
  #endif
#else
    Positive_RA_Alt_Thresh[1] = 400;
    Positive_RA_Alt_Thresh[2] = 500;
    Positive_RA_Alt_Thresh[3] = 640;
    Positive_RA_Alt_Thresh[4] = 740;
#endif
}

int ALIM ()
{
#ifndef FAULT_V37
 return Positive_RA_Alt_Thresh[Alt_Layer_Value];
#else
 return Positive_RA_Alt_Thresh[0];
#endif
}

int Inhibit_Biased_Climb ()
{
#ifndef FAULT_V2
  #ifndef FAULT_V28
    #ifndef FAULT_V29
      #ifndef FAULT_V30
        #ifndef FAULT_V35
    return (Climb_Inhibit ? Up_Separation + NOZCROSS : Up_Separation);
        #else
    return (Climb_Inhibit ? Up_Separation : Up_Separation + NOZCROSS);
        #endif
      #else
    return (Up_Separation + NOZCROSS);
      #endif
    #else
    return (Up_Separation);
    #endif
  #else
    return ((Climb_Inhibit == 0) ? Up_Separation + NOZCROSS : Up_Separation);
  #endif
#else
    return (Climb_Inhibit ? Up_Separation + MINSEP /* operand mutation NOZCROSS */ : Up_Separation);
#endif
}

bool Non_Crossing_Biased_Climb()
{
    int upward_preferred;
    int upward_crossing_situation;
    bool result;
#ifndef FAULT_V20
  #ifndef FAULT_V21
    #ifndef FAULT_V22
    upward_preferred = Inhibit_Biased_Climb() > Down_Separation;
    #else
    upward_preferred = Up_Separation > Down_Separation;
    #endif
  #else
    upward_preferred = (Up_Separation + NOZCROSS) > Down_Separation;
  #endif
#else
    upward_preferred = Inhibit_Biased_Climb() >= Down_Separation;
#endif
    if (upward_preferred)
    {
#ifndef FAULT_V1
  #ifndef FAULT_V31
    #ifndef FAULT_V40
	result = !(Own_Below_Threat()) || ((Own_Below_Threat()) && (!(Down_Separation >= ALIM())));
    #else
	result = ((Own_Below_Threat()) && (!(Down_Separation >= ALIM())));
    #endif
  #else
	result = !(Own_Below_Threat()) || ((Own_Below_Threat()) && (!(Down_Separation >= ALIM())));
	result = result && (Own_Tracked_Alt <= Other_Tracked_Alt);
  #endif
#else
	result = !(Own_Below_Threat()) || ((Own_Below_Threat()) && (!(Down_Separation > ALIM())));
#endif
    }
    else
    {
#ifndef FAULT_V31	
  #ifndef FAULT_V4
    #ifndef FAULT_V41
	result = Own_Above_Threat() && (Cur_Vertical_Sep >= MINSEP) && (Up_Separation >= ALIM());
    #else
	result = (Cur_Vertical_Sep >= MINSEP) && (Up_Separation >= ALIM());
    #endif
  #else
	result = Own_Above_Threat() && (Cur_Vertical_Sep >= MINSEP) || (Up_Separation >= ALIM());
  #endif
#else
	result = Own_Above_Threat() && (Cur_Vertical_Sep >= MINSEP) && (Up_Separation >= ALIM());
	result = result && (Own_Tracked_Alt < Other_Tracked_Alt);
#endif
    }
    return result;
}

bool Non_Crossing_Biased_Descend()
{
    int upward_preferred;
    int upward_crossing_situation;
    bool result;

#ifndef FAULT_V9
  #ifndef FAULT_V23
    #ifndef FAULT_V24
    upward_preferred = Inhibit_Biased_Climb() > Down_Separation;
    #else
    upward_preferred = Up_Separation > Down_Separation;
    #endif
  #else
    upward_preferred = (Up_Separation + NOZCROSS) > Down_Separation;
  #endif
#else
    upward_preferred = Inhibit_Biased_Climb() >= Down_Separation; /* operator mutation */
#endif
    if (upward_preferred)
    {
#ifndef FAULT_V32
	result = Own_Below_Threat() && (Cur_Vertical_Sep >= MINSEP) && (Down_Separation >= ALIM());
#else
	result = Own_Below_Threat() && (Cur_Vertical_Sep >= MINSEP) && (Down_Separation >= ALIM());
	result = result && (Other_Tracked_Alt < Own_Tracked_Alt);
#endif
    }
    else
    {
#ifndef FAULT_V25
  #ifndef FAULT_V32
    #ifndef FAULT_V39
	result = !(Own_Above_Threat()) || ((Own_Above_Threat()) && (Up_Separation >= ALIM()));
    #else
	result = !(Own_Above_Threat()) || ((Own_Above_Threat()) && (Up_Separation > ALIM()));
    #endif
  #else
	result = !(Own_Above_Threat()) || ((Own_Above_Threat()) && (Up_Separation >= ALIM()));
	result = result && (Other_Tracked_Alt <= Own_Tracked_Alt);
  #endif
#else
	result = !(Own_Above_Threat()) || ((Own_Above_Threat()) && (Up_Separation > ALIM()));
#endif
    }
    return result;
}

bool Own_Below_Threat()
{
#ifndef FAULT_V6
  #ifndef FAULT_V10
    #ifndef FAULT_V11
    return (Own_Tracked_Alt < Other_Tracked_Alt);
    #else
    return (Own_Tracked_Alt <= Other_Tracked_Alt);
    #endif
  #else
    return (Own_Tracked_Alt <= Other_Tracked_Alt);
  #endif
#else
    return (Own_Tracked_Alt <= Other_Tracked_Alt); /* operator mutation */
#endif
}

bool Own_Above_Threat()
{
#ifndef FAULT_V10
  #ifndef FAULT_V11
    return (Other_Tracked_Alt < Own_Tracked_Alt);
  #else
    return (Other_Tracked_Alt <= Own_Tracked_Alt);
  #endif
#else
    return (Other_Tracked_Alt <= Own_Tracked_Alt);
#endif
}

int alt_sep_test()
{
    bool enabled, tcas_equipped, intent_not_known;
    bool need_upward_RA, need_downward_RA;
    int alt_sep;
#ifndef FAULT_V5
  #ifndef FAULT_V12
    #ifndef FAULT_V15
      #ifndef FAULT_V26
        #ifndef FAULT_V27
    enabled = High_Confidence && (Own_Tracked_Alt_Rate <= OLEV) && (Cur_Vertical_Sep > MAXALTDIFF);
        #else
    enabled = High_Confidence && (Own_Tracked_Alt_Rate <= OLEV) ;
        #endif
      #else
    enabled = High_Confidence && (Cur_Vertical_Sep > MAXALTDIFF);
      #endif
    #else
    enabled = High_Confidence && (Own_Tracked_Alt_Rate <= OLEV); 
    #endif
  #else
    enabled = High_Confidence || (Own_Tracked_Alt_Rate <= OLEV) && (Cur_Vertical_Sep > MAXALTDIFF);
  #endif
#else
    enabled = High_Confidence && (Own_Tracked_Alt_Rate <= OLEV);
#endif
    tcas_equipped = Other_Capability == TCAS_TA;
#ifndef FAULT_V3
    intent_not_known = Two_of_Three_Reports_Valid && Other_RAC == NO_INTENT;
#else
    intent_not_known = Two_of_Three_Reports_Valid || Other_RAC == NO_INTENT; /* logic change */
#endif    
    alt_sep = UNRESOLVED;
#ifndef FAULT_V34    
    if (enabled && ((tcas_equipped && intent_not_known) || !tcas_equipped))
#else
    if (enabled && tcas_equipped && intent_not_known || !tcas_equipped)
#endif
    {
#ifndef FAULT_V40
  #ifndef FAULT_V31
	need_upward_RA = Non_Crossing_Biased_Climb() && Own_Below_Threat();
  #else
	need_upward_RA = Non_Crossing_Biased_Climb();
  #endif
#else
	need_upward_RA = Non_Crossing_Biased_Climb();
#endif

#ifndef FAULT_V32
	need_downward_RA = Non_Crossing_Biased_Descend() && Own_Above_Threat();
#else
	need_downward_RA = Non_Crossing_Biased_Descend();
#endif

#ifndef FAULT_V11
	if (need_upward_RA && need_downward_RA)
        /* unreachable: requires Own_Below_Threat and Own_Above_Threat
           to both be true - that requires Own_Tracked_Alt < Other_Tracked_Alt
           and Other_Tracked_Alt < Own_Tracked_Alt, which isn't possible */
	    alt_sep = UNRESOLVED;
	else if (need_upward_RA)
	    alt_sep = UPWARD_RA;
#else
     /* NOTE JMW removed exclusive-or condition from upward and downward
             computations. */

	if (need_upward_RA)
        /* unreachable: requires Own_Below_Threat and Own_Above_Threat
           to both be true - that requires Own_Tracked_Alt < Other_Tracked_Alt
           and Other_Tracked_Alt < Own_Tracked_Alt, which isn't possible */
	    alt_sep = UPWARD_RA;
#endif
	else if (need_downward_RA)
	    alt_sep = DOWNWARD_RA;
	else
	    alt_sep = UNRESOLVED;
    }
    
    return alt_sep;
}

main(argc, argv)
int argc;
char *argv[];
{
    if(argc < 13)
    {
	fprintf(stdout, "Error: Command line arguments are\n");
	fprintf(stdout, "Cur_Vertical_Sep, High_Confidence, Two_of_Three_Reports_Valid\n");
	fprintf(stdout, "Own_Tracked_Alt, Own_Tracked_Alt_Rate, Other_Tracked_Alt\n");
	fprintf(stdout, "Alt_Layer_Value, Up_Separation, Down_Separation\n");
	fprintf(stdout, "Other_RAC, Other_Capability, Climb_Inhibit\n");
	exit(1);
    }
    initialize();
    Cur_Vertical_Sep = atoi(argv[1]);
    High_Confidence = atoi(argv[2]);
    Two_of_Three_Reports_Valid = atoi(argv[3]);
    Own_Tracked_Alt = atoi(argv[4]);
    Own_Tracked_Alt_Rate = atoi(argv[5]);
    Other_Tracked_Alt = atoi(argv[6]);
    Alt_Layer_Value = atoi(argv[7]);
    Up_Separation = atoi(argv[8]);
    Down_Separation = atoi(argv[9]);
    Other_RAC = atoi(argv[10]);
    Other_Capability = atoi(argv[11]);
    Climb_Inhibit = atoi(argv[12]);

    fprintf(stdout, "%d\n", alt_sep_test());
    exit(0);
}
