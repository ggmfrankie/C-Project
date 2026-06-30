package com.Extern;

import java.util.ArrayList;
import java.util.Arrays;

import static java.lang.Math.*;

public class Rechner {

    public static void berechneErgebnis(double n, double alpha){
        alpha = toRadians(alpha); // convert degrees to radians
        double deltaAlphaRad = toRadians(0.4); // convert Δα from degrees to radians
        double lambda = 587.56e-3; // example λ in appropriate units
        double deltaLambda = 10.9e-3; // Δλ in same units as λ

        // first term: |n λ cos(α) / sin^2(α)| * Δα
        double term1 = abs(-n * lambda * (cos(alpha)/pow(sin(alpha), 2))) * deltaAlphaRad;

        // second term: |n / sin(α)| * Δλ
        double term2 = abs(n / sin(alpha)) * deltaLambda;

        double ergebnis = term1 + term2;
        System.out.println(ergebnis);
    }

    public static void averageDistance(double[] total){
        double previous = total[0];
        double distances = 0.0;
        for(int i = 1; i < total.length; i++){
            double current = total[i];
            distances += (current-previous);
            previous = current;
        }
        System.out.printf("Average distance is: %f\n", distances/(total.length-1));
    }
    public static double[] averageDistances(double[] total){
        double previous = total[0];
        double[] distances = new double[total.length-1];
        for(int i = 1; i < total.length; i++){
            double current = total[i];
            distances[i-1] = current-previous;
            previous = current;
        }
        System.out.printf("Average distance is: %s\n", Arrays.toString(distances));
        return distances;
    }

    public static void standartAbweichung(double[] elements){
        double average = 0;
        for(double d : elements){
            average += d;
        }
        average = average/elements.length;

        double sum = 0;
        for (double element : elements) {
            sum += pow((element - average), 2);
        }
        double result = sqrt(1.0/(elements.length - 1) * sum);
        System.out.printf("Result is: %f\n", result);
    }

    public static void schallSchluckGrad(double D){
        double z = pow(10, D/20);
        double n = pow((z + 1), 2);
        System.out.printf("Schallschluckgrad ist: %f\n", 4*z/n);
    }

    public static void fehlerrechnung_DeltaAlpha(double D, double deltaD){
        double z = pow(10, D/20) - pow(10, D/10);
        double n = 5 *pow((1 + pow(10, D/20)), 3);
        double result = abs(log(10) * (z/n) * deltaD);
        System.out.printf("DeltaAlpha ist: %f\n", result);
    }

    public static void average(double[] werte){
        double total = 0.0;
        for(double d : werte){
            total += d;
        }
        System.out.printf("Average is: %f", total/werte.length);
    }

    public static void refactorString(String s){
        System.out.println(s.replace(",", ".").replace("\n", ","));
    }

}
