package com.Extern

import kotlin.math.*

class KotlinTest {
    fun berechneErgebnis(n: Double, alpha: Double) {
        var alpha = alpha
        alpha = Math.toRadians(alpha) // convert degrees to radians
        val deltaAlphaRad = Math.toRadians(0.4) // convert Δα from degrees to radians
        val lambda = 587.56e-3 // example λ in appropriate units
        val deltaLambda = 10.9e-3 // Δλ in same units as λ

        // first term: |n λ cos(α) / sin^2(α)| * Δα
        val term1 = abs(-n * lambda * (cos(alpha) / sin(alpha).pow(2.0))) * deltaAlphaRad

        // second term: |n / sin(α)| * Δλ
        val term2 = abs(n / sin(alpha)) * deltaLambda

        val ergebnis = term1 + term2
        println(ergebnis)
    }

    fun averageDistance(total: DoubleArray) {
        var previous = total[0]
        var distances = 0.0
        for (i in 1..<total.size) {
            val current = total[i]
            distances += (current - previous)
            previous = current
        }
        System.out.printf("Average distance is: %f\n", distances / (total.size - 1))
    }

    fun averageDistances(total: DoubleArray): DoubleArray {
        var previous = total[0]
        val distances = DoubleArray(total.size - 1)
        for (i in 1..<total.size) {
            val current = total[i]
            distances[i - 1] = current - previous
            previous = current
        }
        System.out.printf("Average distance is: %s\n", distances.contentToString())
        return distances
    }

    fun standartAbweichung(elements: DoubleArray) {
        var average = 0.0
        for (d in elements) {
            average += d
        }
        average = average / elements.size

        var sum = 0.0
        for (element in elements) {
            sum += (element - average).pow(2.0)
        }
        val result = sqrt(1.0 / (elements.size - 1) * sum)
        System.out.printf("Result is: %f\n", result)
    }

    fun schallSchluckGrad(D: Double) {
        val z = 10.0.pow(D / 20)
        val n = (z + 1).pow(2.0)
        System.out.printf("Schallschluckgrad ist: %f\n", 4 * z / n)
    }

    fun fehlerrechnung_DeltaAlpha(D: Double, deltaD: Double) {
        val z = 10.0.pow(D / 20) - 10.0.pow(D / 10)
        val n = 5 * (1 + 10.0.pow(D / 20)).pow(3.0)
        val result = abs(ln(10.0) * (z / n) * deltaD)
        System.out.printf("DeltaAlpha ist: %f\n", result)
    }
}