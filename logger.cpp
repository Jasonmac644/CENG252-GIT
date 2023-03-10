/** @file logger.cpp
 *  @brief Data logger Functions
 */
#include "logger.h"
#include "sensehat.h"
#include <stdio.h>
#include <cinttypes>
#include <cstring>
#include <cstdlib>

/*
GLOBAL OBJECTS
*/

SenseHat sh;
/** @brief Getss RPi serial number*
@author Jason Macdonald*
@date 19JAN2023*
@param void*
@return int64 unsigned serial number*/
uint64_t DlGetSerial(void)
{
	static uint64_t serial = 0;
	FILE * fp;
	char buf[SYSINFOBUFSZ];
	char searchstring[] = SEARCHSTR;
	fp = fopen ("/proc/cpuinfo", "r");
	if (fp != NULL)
	{
		while (fgets(buf, sizeof(buf), fp) != NULL)
		{
			if (!strncasecmp(searchstring, buf, strlen(searchstring)))
			{
				sscanf(buf+strlen(searchstring), "%Lx", &serial);
			}
		}
		fclose(fp);
	}
     if(serial==0)
     {
         system("uname -a");
         system("ls --fu /usr/lib/codeblocks | grep -Po '\\.\\K[^ ]+'>stamp.txt");
         fp = fopen ("stamp.txt", "r");
         if (fp != NULL)
         {
             while (fgets(buf, sizeof(buf), fp) != NULL)
             {
                sscanf(buf, "%Lx", &serial);
             }
             fclose(fp);
        }
     }
	return serial;
}
/** @brief DLInitialization Function*
@author Jason Macdonald*
@date 19JAN2023*
@param void*
@return int64*/
int DlInitialization(void){
    fprintf(stdout,"Jason Macdonald's CENG252 Vehicle Data Logger\n");
    return 1;
}

/** @brief GetloggerReadings Function*
@author Jason Macdonald*
@date 19JAN2023*
@param void*
@return int64*/

reading_s DlGetLoggerReadings (){
    reading_s creads;
    time(&creads.rtime);
    #if SENSEHAT

        creads.temperature = sh.GetTemperature();
        creads.humidity = sh.GetHumidity();
        creads.pressure = sh.GetPressure();
        sh.GetAcceleration(creads.xa,creads.ya,creads.za);
        sh.GetMagnetism(creads.xm,creads.ym,creads.zm);
        sh.GetOrientation(creads.pitch,creads.roll,creads.yaw);
        usleep(IMUDELAY);
    #else
        creads.altitude = DALT;
        creads.heading = DHEADING;
        creads.humidity = DHUMID;
        creads.latitude = DLAT;
        creads.longitude = DLONG;
        creads.pitch = DPITCH;
        creads.pressure = DPRESS;
        creads.roll = DROLL;
        creads.speed = DSPEED;
        creads.temperature = DTEMP;
        creads.xa = DXA;
        creads.xm = DXM;
        creads.ya = DYA;
        creads.yaw = DYAW;
        creads.ym = DYM;
        creads.za = DZA;
        creads.zm = DZM;
        #endif // SENSEHAT

        return creads;
}
/** @brief Display readings *
@author Jason Macdonald*
@date 19JAN2023*
@param reading_s Structure*
@return void*/

void DlDisplayLoggerReadings(reading_s lreads){
    fprintf(stdout,"Unit:%LX %s\nT: %3.1fC\t\tH: %3.0f%\t\t\tP: %4.1fkpa\nXa: %1.6fg",DlGetSerial(),ctime(&lreads.rtime),lreads.temperature,lreads.humidity,lreads.pressure,lreads.xa);
    fprintf(stdout, "\t\tYa:%1.6fg\t\tZa: %2.6fg\nPitch: %2.6f\t\tRoll: %2.6f\t\tYaw: %2.6f\nXm: %1.6f\t\tYm:%1.6f\t\tZm: %1.6f\n",lreads.ya,lreads.za,lreads.pitch,lreads.roll,lreads.yaw,lreads.xm,lreads.ym,lreads.zm);
    fprintf(stdout, "Latitude: %2.6f\tLongitude:%2.6f\tAltitude: %3.6f\nSpeed: %2.6f\tHeading:%3.6f\n",lreads.latitude,lreads.longitude,lreads.altitude,lreads.speed,lreads.heading);

}
/** @brief Save Log Data *
@author Jason Macdonald*
@date 19JAN2023*
@param Reading_s structure*
@return int*/

int DlSaveLoggerData(reading_s creads){
    fprintf(stdout,"\nSaving Data Logger\n");
    return 1;

}
/** @brief Display logo function *
@author Jason Macdonald*
@date 03FEB2023*
@param void*
@return void*/

void DlDisplayLogo(void){
    uint16_t logo[8][8] = {	HB,HB,HB,HB,HB,HB,HB,HB,
			HB,HB,HW,HB,HB,HW,HB,HY,
			HB,HB,HW,HB,HB,HW,HY,HY,
			HB,HB,HW,HB,HB,HW,HY,HY,
			HB,HB,HW,HW,HW,HW,HY,HY,
			HB,HB,HW,HY,HY,HW,HY,HY,
			HB,HY,HW,HY,HY,HW,HY,HY,
			HY,HY,HY,HY,HY,HY,HY,HY,
            };
    sh.WipeScreen();
    sh.ViewPattern(logo);

}
/** @brief Dl Update function *
@author Jason Macdonald*
@date 03FEB2023*
@param void*
@return void*/
void DlUpdateLevel(float xa, float ya){
    int x,y;
    sh.WipeScreen();
    y = (int) (xa * -30.0 +4);
    x = (int) (ya * -30.0 +4);
    if(x<0){
        x = 0;
    }else if(x > 6){
        x = 6;
    }
    if(y < 0){
        y = 0;
    }else if(y > 6){
        y = 6;
    }

    sh.LightPixel(x,y,HY);
    sh.LightPixel(x+1,y,HY);
    sh.LightPixel(x,y+1,HY);
    sh.LightPixel(x+1,y+1,HY);
}
