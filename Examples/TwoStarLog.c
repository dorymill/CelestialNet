/**
 * @file    TwoStarLog.c
 * @author  Asmod
 * @brief   Example usage of code implentation in C to log the position
 *          of astronomical targets in an observer's local sky.
 * @version 0.1
 * @date    2023-04-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */


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

    pthread_t loggerThread1;
    pthread_t loggerThread2;

    ThreadData_t l1Data;
    ThreadData_t l2Data;

    /* Data structure initialization routines. */
    initObserver(&observer, -90, 40);

    initTarget(&betelgeuese, "Betelgeuese", 75.91944444,   7.4072222);
    initTarget(&rigel,       "Rigel",       75.242222222, -8.201666667);

    initThreadData(&l2Data, rigel,       &observer, &loc_rigel);
    initThreadData(&l2Data, betelgeuese, &observer, &loc_betelgeuese);

    /* Launch the threads of fate! */
    pthread_create(&loggerThread1, NULL, &recordLoop, (void *) &l1Data);
    pthread_create(&loggerThread2, NULL, &recordLoop, (void *) &l2Data);

    pthread_join(loggerThread1, NULL);
    pthread_join(loggerThread2, NULL);

    printf("[+] Logging threads complete.\n");

}