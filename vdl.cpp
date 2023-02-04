/** @file vdl.cpp Serial: 1a2b3c4d*
@author Jason Macdonald*
@date 20OCT2020*
@brief Vehicle Data Logger main function*/

#include <cstdio>
#include <unistd.h>
#include "vdl.h"
#include "logger.h"

/** @brief Vehicle Data Logger main function*
@author Jason Macdonald*
@date 25MAR2019*
@param void*
@return int program status*/
int main(void){
    //function variables
    int tc = 0;

    printf("Data Logger Initiliazation\n\n");
    reading_s current = {0};
    DlInitialization();
    DlDisplayLogo();
    sleep(3);

    while(1){
        current = DlGetLoggerReadings();
        DlUpdateLevel(current.xa, current.ya);
        if(tc == LOGCOUNT){
            DlDisplayLoggerReadings(current);
            DlSaveLoggerData(current);
            tc = 0;
        }else{
            usleep(SLEEPTIME);
            tc++;
        }
    }
}
