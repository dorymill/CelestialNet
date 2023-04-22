#ifndef SIDEREAL_H
#define SIDEREAL_H

#include <pthread.h>

/* Time Structures */
/**
 * @brief Clock Time Structure
 * 
 */
typedef struct {

    double hours;
    double minutes;
    double seconds;

} Time_t;


/**
 * @brief Calender Time Structure
 * 
 */
typedef struct {

    int year;
    int month;
    int day;

    Time_t time;

} Date_t;

/* Targeting structures */

/**
 * @brief Structure containing observer based 
 *        parameters required for placing a
 *        target in the local sky, namely,
 *        the precise time of observation--Date_t,
 *        --the Local Mean Sidereal Time--as estiamted
 *        using the Astronomer's Almanac ala 2023 to 2nd Order,
 *        --, and the decimal Julian date.
 * 
 */
typedef struct {

    pthread_mutex_t observerlock;
    
    Date_t date;
    
    double longitude;
    double latitude;
    double lmst;
    double julian;

} Observer_t;

/**
 * @brief Target structure containing the 
 *        Equatorial Coordinates and name 
 *        of a target.
 * 
 */
typedef struct {

    char * name;
    double ra;
    double decl;

} Target_t;


/**
 * @brief Structure containing the local coordiantes
 *        of a given Target_t in a given Observer_t's
 *        sky, in Azimuth and Altitude.
 * 
 */
typedef struct {

    double azimuth;
    double altitude;

} LocalTarget_t;

/* Internal data structure */
/**
 * @brief Data structure to pass to run logging threads,
 *        consisting of a Target_t structure, and pointers to
 *        an Observer_t and LocalTarget_t to be continuously updated
 *        for further calculation.
 *        
 * 
 */
typedef struct {

    Target_t       target;
    Observer_t    *observer;
    LocalTarget_t *local_target;

} ThreadData_t;

/* Function prototypes */

void EquatorialToHorizontal(Target_t target, Observer_t *observer, LocalTarget_t *result);

void GetLMST(Observer_t *observer);

void GetJulian(Observer_t *observer);

void updateObserver(Observer_t *observer);

void updateAzEl(Observer_t *observer, Target_t target, LocalTarget_t *localTarget);

void initTarget(Target_t *target, char *name, double rightasc, double declination);

void initObserver(Observer_t *observer, double longitude, double latitude);

void initThreadData(ThreadData_t *threadData, Target_t target, Observer_t *observer, LocalTarget_t *localTarget);

/* TODO: A semaphore needs to be added to handle a single
         observer logging multiple targets. */
void *recordLoop (void *inp_thread_data);

#endif