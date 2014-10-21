#include <math.h>
#include <stdio.h>
#include "coolMatrixStuff.h"

#undef DEBUG

// Kopiert die quadratische n x n Matrix a und speichert sie in b
// b = a
void Copy(double a[][NMAX], double b[][NMAX], int n)
{
  int i, j;
  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      b[i][j] = a[i][j];
}

//  Gauss'sches Eliminationsverfahren mit Zeilenpivotisierung
//    Argumente:
//    double a[N][N+1] erweiterte Koeffizientenmatrix   Read/Write
//    int n            Anzahl der Gleichungen           Read
//    double x[N]      Loesungen                        Write
//  Resultat:
//  int Fehlercode  0 fuer Fehler, 1 fuer Erfolg 

int Gaussalg (double a[][NMAX], int n, double x[]) 
{
  int   i, j;                    // Zeile, Spalte
  int   s;                       // Elimininationsschritt
  int   pzeile;                  // Pivotzeile
  int   fehler = 0;              // Fehlerflag
  double f;                      // Multiplikationsfaktor
  const double Epsilon = 0.01;   // Genauigkeit
  double Maximum;                // Zeilenpivotisierung
  
  s = 0;
  do {             // die einzelnen Eliminationsschritte
    printf("Schritt %2i von %2i\n", s+1, n-1);
    Maximum = fabs(a[s][s]);   // groesstes Element
    pzeile = s ;               // suchen
    for (i = s+1; i < n; i++)
      if (fabs(a[i][s]) > Maximum) {
        Maximum = fabs(a[i][s]) ;
        pzeile = i;
      }
    fehler = (Maximum < Epsilon);
    if (fehler) break;           // nicht loesbar 

    if (pzeile != s)  // falls erforderlich, Zeilen tauschen
    { double h;
      for (j = s ; j <= n; j++) {
        h = a[s][j];
        a[s][j] = a[pzeile][j];
        a[pzeile][j]= h;
      }
    }

    // Elimination --> Nullen in Spalte s ab Zeile s+1
    for (i = s + 1; i < n; i++ ) {
      f = -(a[i][s]/a[s][s]);       // Multiplikationsfaktor
      a[i][s] = 0.0;
      for (j = s+1; j <= n ; j++)   // die einzelnen Spalten
        a[i][j] += f*a[s][j];       // Addition der Zeilen i, s
    }
    #if DEBUG
       //MatOut (stdout, a, n, n+1);
    #endif
    s++;
  } while ( s < n-1 ) ;

  if (fehler) 
  {
    printf ( "gauss: Gleichungssystem nicht loesbar\n");
    return 0; 
  }
  else 
  {
    // Berechnen der Loesungen aus der entstandenen Dreiecksmatrix
    // letzte Zeile
    x[n-1] =  a[n-1][n] / a[n-1][n-1];       
    // restliche Zeilen
    for (i = n-2 ; i >= 0; i-- ) 
    {
      for (j = n-1 ; j > i ; j-- ) 
      {
          a[i][n] -= x[j]*a[i][j];    // rechte Seite berechnen
      } 
      x[i] = a[i][n] / a[i][i];       // Loesung
    }
    return 1;  
  }

}

//  Inverse einer Matrix
//  Berechung nach dem Gauss-Jordan Verfahren
//  Lit. Helmut Selder, Einf¸hrung in die numerische Mathematik f¸r Ingenieure, HANSER
//
//  Das Verfahren lˆst die n Gleichungssysteme (f¸r je eine Spalte der Einheitsvektoren) 
//  in einem gemeinsamen Eliminationsverfahren.
//  Statt nur einem Vektor f¸r die rechte Seite, erweitert man die Matrix um alle n Einheitsvektoren
//  a11 a12 a13  | 1  0  0
//  a21 a22 a23  | 0  1  0
//  a31 a32 a33  | 0  0  1
//  und eliminiert in der Matrix a alle Elemente ausserhalb der Diagonalen
//  zus‰tzlich sorgt man durch eine Division f‹r lauter 1 in der Diagonalen
//  Das schaut dann so aus:
//   x1  x2  x2
//   1   0   0   | b11 b12 b13
//   0   1   0   | b21 b22 b23
//   0   0   1   | b31 b32 b33
//  Dadurch f‰llt die Berechnung der Lˆsungen aus der Dreiecksmatrix (xn, xn-1, ... x1)
//  weg, weil die Lˆsungen sofort abgelesen werden kˆnnen.
//  x1 = b11  bzw. b12 oder b13
//  x2 = b21  bzw. b22 oder b23
//  x3 = b31  bzw. b23 oder b33
//  Das bedeutet, die Einheitsmatrix ist bei der Elimination in die Matrix B ¸bergegangen,
//  und das ist die Inverse Matrix.


int Inverse (double a[][NMAX], double ainv[][NMAX], int n) 
{
  int   i, j;                    // Zeile, Spalte
  int   s;                       // Elimininationsschritt
  int   pzeile;                  // Pivotzeile
  int   fehler = 0;              // Fehlerflag
  double f;                      // Multiplikationsfaktor
  const double Epsilon = 0.01;   // Genauigkeit
  double Maximum;                // Zeilenpivotisierung
  int pivot = 1;

  // erg‰nze die Matrix a um eine Einheitsmatrix (rechts anh‰ngen)
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++)
    {
      a[i][n+j] = 0.0;
      if (i == j)
        a[i][n+j] = 1.0;
    }
  }
  #if DEBUG
     //MatOut (stdout, a, n, 2*n);
  #endif
  
  // die einzelnen Eliminationsschritte
  s = 0;
  do {
	  // Pivotisierung vermeidet unnˆtigen Abbruch bei einer Null in der Diagnonalen und
		// erhˆht die Rechengenauigkeit
    Maximum = fabs(a[s][s]);
    if (pivot)
    {
       pzeile = s ; 
       for (i = s+1; i < n; i++)
         if (fabs(a[i][s]) > Maximum) {
           Maximum = fabs(a[i][s]) ;
           pzeile = i;
         }
    }
    fehler = (Maximum < Epsilon);

    if (fehler) break;           // nicht lˆsbar 

    if (pivot)
    {
      if (pzeile != s)  // falls erforderlich, Zeilen tauschen
      { double h;
        for (j = s ; j < 2*n; j++) {
          h = a[s][j];
          a[s][j] = a[pzeile][j];
          a[pzeile][j]= h;
        }
      }
    }

    // Eliminationszeile durch Pivot-Koeffizienten f = a[s][s] dividieren
    f = a[s][s];
    for (j = s; j < 2*n; j++)
      a[s][j] = a[s][j] / f;

    // Elimination --> Nullen in Spalte s oberhalb und unterhalb der Diagonalen
    // durch Addition der mit f multiplizierten Zeile s zur jeweiligen Zeile i
    for (i = 0; i < n; i++ ) {
      if (i != s) 
      {
        f = -a[i][s];                 // Multiplikationsfaktor
        for (j = s; j < 2*n ; j++)    // die einzelnen Spalten
          a[i][j] += f*a[s][j];       // Addition der Zeilen i, s
      }
    }
    #if DEBUG
      fprintf(stdout, "Nach %1i-tem Eliminationschritt:\n", s+1);
      //MatOut (stdout, a, n, 2*n);
    #endif
    s++;
  } while ( s < n ) ;

  if (fehler) 
  {
    printf ("Inverse: Matrix ist singul‰r\n");
    return 0; 
  }
  // Die angeh‰ngte Einheitsmatrix Matrix hat sich jetzt in die inverse Matrix umgewandelt
  // Umkopieren auf die Zielmatrix
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      ainv[i][j] = a[i][n+j];
    }
  }
  return 1;  
}

// Produkt
// Multipliziert die Matrix a mit der Matrix b , Resultat in Matrix c
// n ... Anzahl der Zeilen und Spalten
void Produkt (double a[][NMAX], double b[][NMAX], double c[][NMAX], int n)
{
  int i, j, k;
  for (i = 0; i < n; i++)
  {
    for (j = 0; j < n; j++)
    {
      c[i][j] = 0.0;
      for (k = 0; k < n; k++)
        c[i][j] += a[i][k]*b[k][j];
    }
  }
}  // end Produkt
