

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef USER_DEFINES_HEADER
#define	USER_DEFINES_HEADER

#include <xc.h> // include processor files - each processor file is guarded.  

#define TOTAL_LAPS                  (uint8_t) 11   //Total number of laps in the race
#define OPTIMAL_LAP_TIME            (uint16_t) 191 //optimal lap time in seconds
#define DISTANCE_STEP               (float)0 //how many meters ahead is the LUT value we want to give
#define LUT_DISTANCE_RESOLUTION     (uint8_t)2 //in meters | may change depending on measuring method

#define DISPLAY_BRIGHTNESS          15 //0 to 15, 15 is the brightest
#define DISPLAY_WIDTH               256
#define DISPLAY_HEIGHT              64
#define SPD_GRAPH_START             2 //the lowest speed (in km/h) that is displayed on the graph
#define SPD_GRAPH_OFFSET            DISPLAY_HEIGHT-1+(SPD_GRAPH_START*2)
//#define WHEEL_DIAMETER              (double)0.5586 // in meters
//#define M_PI                        3.14159265358979323846
#define SPEED_MULT_FACTOR           (float)0.10376 // (M_PI*WHEEL_DIAMETER*3.6)/60
#define LUT_SIZE                    (uint16_t)(sizeof(lut)/sizeof(lut[0]))



#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

