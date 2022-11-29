/*
 * Recursively computes greatest common divisors.
 * CSC 225, Assignment 6
 * Given code, Spring '21
 * TODO: Complete this file.
 */
#include <stdio.h>
#include "gcd.h"
int main() {
    int num1, num2, gcdnum;
    
    printf("Enter two positive integers: ");
    scanf("%d %d", &num1, &num2);
    
    gcdnum = gcd(num1, num2);
    printf("gcd(%d, %d) = %d\n", num1, num2, gcdnum);
    
    return 0;
}
