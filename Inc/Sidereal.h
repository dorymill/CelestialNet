#ifndef SIDEREAL_H
#define SIDEREAL_H

/* Time Structures */

typedef struct {

    double hours;
    double minutes;
    double seconds;

} Time_t;

typedef struct {

    int year;
    int month;
    int day;

    Time_t time;

} Date_t;

/* Targeting structures */

typedef struct {

    Date_t date;
    double longitude;
    double latitude;
    double lmst;
    double julian;

} Observer_t;

typedef struct {

    char * name;
    double ra;
    double decl;

} Target_t;

typedef struct {

    double azimuth;
    double altitude;

} LocalTarget_t;

/* Internal data structure */

typedef struct {

    Target_t       target;
    Observer_t    *observer;
    LocalTarget_t *local_target;

} thread_data_t;

/* Function prototypes */

void EquatorialToHorizontal(Target_t target, Observer_t * observer, LocalTarget_t * result);

void GetLMST(Observer_t * observer);

void GetJulian(Observer_t * observer);

void updateObserver(Observer_t * observer);

void updateAzEl(Observer_t * observer, Target_t target, LocalTarget_t * localTarget);

void initTarget(Target_t *target, char * name, double rightasc, double declination);

void initObserver(Observer_t *observer, double longitude, double latitude);

void *recordLoop (void *inp_thread_data);

#endif