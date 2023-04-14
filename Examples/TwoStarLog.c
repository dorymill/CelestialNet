
#include "Sidereal.h"
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define PI 3.1415926535897932384626433832

void main (void) {

    /* Primary data structures to initialize. */
    Observer_t    observer;

    Target_t      betelgeuese;
    LocalTarget_t loc_betelgeuese;

    Target_t      rigel;
    LocalTarget_t loc_rigel;

    /* Data structure initialization routines. */
    initObserver(&observer, -90, 40);

    initTarget(&betelgeuese, "Betelgeuese", 75.91944444,   7.4072222);
    initTarget(&rigel,       "Rigel",       75.242222222, -8.201666667);

    
    /* Instantiate thread and create input data structure. */
    pthread_t loggerThread1;
    pthread_t loggerThread2;

    thread_data_t t1Data;
    thread_data_t t2Data;

    t2Data.target       = rigel;
    t2Data.observer     = &observer;
    t1Data.local_target = &loc_rigel;

    t1Data.target       = betelgeuese;
    t1Data.observer     = &observer;
    t1Data.local_target = &loc_betelgeuese;

    pthread_create(&loggerThread1, NULL, &recordLoop, (void *) &t1Data);
    pthread_create(&loggerThread2, NULL, &recordLoop, (void *) &t2Data);

    pthread_join(loggerThread1, NULL);
    pthread_join(loggerThread2, NULL);

    printf("[+] Logging threads complete.\n");

}