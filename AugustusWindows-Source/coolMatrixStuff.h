#include <math.h>
#include <stdio.h>

#define DEBUG 1
#define NMAX 10

int Gaussalg (double a[][NMAX], int n, double x[]);
int Inverse (double a[][NMAX], double ainv[][NMAX], int n);
void MatIn  (FILE *f, double a[][NMAX], int *n, int *m);
void MatOut (FILE *f, double a[][NMAX], int n, int m);
void Produkt (double a[][NMAX], double b[][NMAX], double c[][NMAX], int n);
void Copy(double a[][NMAX], double b[][NMAX], int n);
