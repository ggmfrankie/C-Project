//
// Created by ertls on 19.11.2025.
//

#include "Wetterberechnungen.h"

#include <stdio.h>

int wetterRechnung()
{
    double precipitationPerMonth[] = {36.9, 29.0, 56.3, 46.2, 93.6, 45.8, 99.1, 65.2, 20.8, 48.0, 33.0, 57.8};
    double maxPercipitation = 0.0;
    double minPercipitation = 1000.0;
    double percipitationSum = 0.0;

    double *pprecipitationPerMonth = precipitationPerMonth;
    ;

    const int NUMBER_OF_MONTHS = sizeof(precipitationPerMonth) / sizeof(double);

    for (int i = 0; i < NUMBER_OF_MONTHS; i++)
    {
        double currentPercipitation = *(pprecipitationPerMonth + i);
        percipitationSum = percipitationSum + currentPercipitation;
        if (currentPercipitation > maxPercipitation)
        {
            maxPercipitation = currentPercipitation;
        }
        else if (currentPercipitation < minPercipitation)
        {
            minPercipitation = currentPercipitation;
        }
    }
    double meanPercipitation = percipitationSum / NUMBER_OF_MONTHS;

    printf("Sum: %5.1f\n", percipitationSum);
    printf("Mean: %5.1f\n", meanPercipitation);
    printf("Max: %5.1f\n", maxPercipitation);
    printf("Min: %5.1f\n", minPercipitation);
    return 0;
}
