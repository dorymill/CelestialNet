#include "Sidereal.h"
#include <stdio.h>
#include <time.h>
#include <math.h>

#define PI 3.1415926535897932384626433832

/**
 * @brief 
 * 
 * @param observer Pointer to observer structure 
 */
void GetLMST(Observer_t * observer)
{

    double gmst;

    GetJulian(observer);

    double D = observer->julian-2451545.0;
    double T = D / 36525;
    double H = observer->date.time.hours + 
               observer->date.time.minutes/60 +
               observer->date.time.seconds/3600;

    double D_o = D - H/24;

    /* In degrees*/
    /* Approx good until around 2200 ACE */
    gmst = 15*(6.697374558 + 
           0.06570982441908*(D_o) + 
           1.00273790935*(H) + 0.000026*T*T); 


    observer->lmst = gmst + observer->longitude;
    
    while (observer->lmst > 360) {
        observer->lmst -= 360;
    }

    double sid_minutes = 60*(observer->lmst/15 - (int)(observer->lmst/15));
    double sid_seconds = 60*(sid_minutes - (int)(sid_minutes));

    //printf("\nLocal Mean Sidereal Time | %d:%d:%d \n", (int) observer->lmst/15, (int) sid_minutes, (int) sid_seconds);

}

/**
 * @brief Get the Julian object
 * 
 * @param observer Pointer to observer structure 
 */
void GetJulian(Observer_t * observer)
{
    double decimalDays = (long double)((observer->date.day) + 
                         (observer->date.time.hours/24) + 
                         (observer->date.time.minutes/1440) + 
                         (observer->date.time.seconds/86400));

    int calcYear  = observer->date.year;
    int calcMonth = observer->date.month;

    /* If The date is January or February, then we are in the 13th or 14th month of the prior year */
    switch (calcMonth)
    {
        case 1:
        case 2:
            calcYear  -=  1;
            calcMonth += 12;
            break;
        default:
            break;
    }

    /* All input dates will be Gregorian by design */
    int A = (int) (calcYear / 100);
    int B = 2 - A + (int) (A / 4);

    observer->julian = (int) (365.25*(calcYear + 4716)) + (int) (30.6001*(calcMonth+1)) + decimalDays + B - 1524.5;
}

/**
 * @brief 
 * 
 * @param target 
 * @param observer Pointer to observer structure 
 * @param result 
 */
void EquatorialToHorizontal(Target_t target, Observer_t * observer, LocalTarget_t * result)
{

    /* Get current time */
    updateObserver(observer);

    /* Equatorial Coordinates */
    double ra   = target.ra;
    double decl = target.decl;
    double hrang;
    double x;
    double y;

    /* Horizontal Coordinates */
    double az;
    double alt;

    /* Observer Parameters */
    double lst;
    double lat = observer->latitude;

    /* Heavy lifting */
    GetLMST(observer);

    /* Determine hour angle */
    hrang = observer->lmst - ra;

    /* Convert relevant angles to radians */
    ra    *= PI/180;
    decl  *= PI/180;
    hrang *= PI/180;
    lat   *= PI/180;

    
    /* Determine azimuth and altitude (rad) */
    x = -sin(lat)*cos(decl)*cos(hrang) + cos(lat)*sin(decl);
    y = cos(decl)*sin(hrang);
    
    az  = -atan2(y , x);
    alt = asin(sin(lat)*sin(decl) + cos(lat)*cos(decl)*cos(hrang));

    /* Convert to degrees */
    az  *= 180/PI;
    alt *= 180/PI;

    if (az < 0 ) {
        az += 360;
    }

    result->azimuth  = az;
    result->altitude = alt; 

}

/**
 * @brief 
 * 
 * @param observer Pointer to observer structure 
 */
void updateObserver(Observer_t * observer)
{
    /* Update Observer time and date with UTC time parameters from the machine. */
    time_t utc = time(NULL);
    struct tm utcs;
    gmtime_s(&utcs, &utc);

    /* Load the user structure */
    observer->date.year  = utcs.tm_year + 1900;
    observer->date.month = utcs.tm_mon + 1;
    observer->date.day   = utcs.tm_mday;

    observer->date.time.hours   = utcs.tm_hour;
    observer->date.time.minutes = utcs.tm_min;
    observer->date.time.seconds = utcs.tm_sec;

}

/**
 * @brief 
 * 
 * @param target 
 * @param name 
 * @param rightasc 
 * @param declination 
 */
void initTarget(Target_t *target, char * name, double rightasc, double declination) 
{
    
    target->name = name;
    target->ra = rightasc;
    target->decl = declination;

}

/**
 * @brief 
 * 
 * @param observer Pointer to observer structure 
 * @param longitude 
 * @param latitude 
 */
void initObserver(Observer_t *observer, double longitude, double latitude) 
{

    observer->longitude = longitude;
    observer->latitude  = latitude;

}

/**
 * @brief 
 * 
 * @param inp_thread_data 
 * @return void* 
 */
void *recordLoop (void *inp_thread_data) /* TO-DO: Create local copy of observer for thread. */
{
    /* Extract data from input struct */
    thread_data_t *locThread_data;
    locThread_data = (thread_data_t *) inp_thread_data;

    Target_t             target = locThread_data->target;
    Observer_t        *observer = locThread_data->observer;
    LocalTarget_t *local_target = locThread_data->local_target;

    /* Open file for logging and print header. */
    FILE *fptr = NULL;
    fptr = fopen(target.name, "w+");

    if (fptr != NULL) {
        fprintf(fptr, "Azimuth, Altitude\n");
    } else {
        printf("[-] Failed to open file to log.\n");
        return NULL;
    }

    /* Begin logging for 24 hours. */
    printf("\n[+] Logging of %s begun. . .\n", target.name);

    for (int counter = 0; counter < 1440; counter++){

        /* Update data and log it to file. */
        EquatorialToHorizontal(target, observer, local_target);

        fprintf(fptr,"%f,%f\n", local_target->azimuth, local_target->altitude);

        if (counter == 720) {
            printf("\n[+] %s halfway through logging.\n", target.name);
        }

        /* Log every minute. */
        Sleep(60000);
    }

    /* Clean up shop. */
    fclose(fptr);
    printf("[+] %s logging complete.\n", target.name);
}