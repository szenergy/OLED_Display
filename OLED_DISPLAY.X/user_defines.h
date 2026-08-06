

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef USER_DEFINES_HEADER
#define	USER_DEFINES_HEADER

#include <xc.h> // include processor files - each processor file is guarded.  

#define TOTAL_LAPS                  (uint8_t) 7     //Total number of laps in the race
#define OPTIMAL_LAP_TIME            (float)   191.1 //optimal lap time in seconds
#define DISTANCE_STEP               (float)   0     //how many meters ahead is the LUT value we want to give

#define DISPLAY_BRIGHTNESS          15 //0 to 15, 15 is the brightest
#define DIM_BRIGHTNESS              15

#define DISPLAY_WIDTH               256
#define DISPLAY_HEIGHT              64
#define SPD_GRAPH_START             2 //the lowest speed (in km/h) that is displayed on the graph
#define SPD_GRAPH_OFFSET            (DISPLAY_HEIGHT-20)-1+(SPD_GRAPH_START*2)


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

