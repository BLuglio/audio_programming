//
//  eq.c
//  audio_programming
//
//  Created by Biagio Luglio on 08/12/2020.
//  Copyright © 2020 Biagio Luglio. All rights reserved.
//

//#include "eq.h"
//#include <gsl/gsl_math.h>

/*
Parametric EQ with matching gain at Nyquist frequency

 Usage: [b, a, G1] = peq(G0, G, GB, w0, Dw)
 
G0 = reference gain at DC
G = boost/cut gain
GB = bandwidth gain

w0 = center frequency in rads/sample
Dw = bandwidth in rads/sample

b = [b0, b1, b2] = numerator coefficients
a = [1, a1, a2] = denominator coefficients
G1 = Nyquist-frequency gain
 */

//void peq(double G0, double G, double GB, double w0, double wB){
//    const double two_pi = 2*acos(-1.);
//    double two_pi_2 = gsl_pow_2(two_pi);
//    double F = fabs(gsl_pow_2(G) - gsl_pow_2(GB));
//    double G00 = fabs(gsl_pow_2(G) - gsl_pow_2(G0));
//    double F00 = fabs(gsl_pow_2(GB) - gsl_pow_2(G0));
//    
//    double num = (gsl_pow_2(G0) * gsl_pow_2(gsl_pow_2(w0) - two_pi_2) + gsl_pow_2(G)*F00*two_pi_2*gsl_pow_2(wB))/F;
//    double den = ( gsl_pow_2(gsl_pow_2(w0) - two_pi_2) + F00*two_pi_2 * gsl_pow_2(wB))/F;
//    
//    double G1 = sqrt(num/den); // Nyquist-frequency gain
//    double G01 = fabs(gsl_pow_2(G) - G0*G1);
//    double G11 = fabs(gsl_pow_2(G) - gsl_pow_2(G1));
//    double F01 = fabs(gsl_pow_2(GB) - G0*G1);
//    double F11 = fabs(gsl_pow_2(GB) - gsl_pow_2(G1));
//    
//    double W2 = sqrt(G11/G00) * gsl_pow_2(tan(w0/2));
//    double DW = (1 + sqrt(F00 / F11) * W2) * tan(wB/2);
//    double C = F11 * gsl_pow_2(DW) - 2 * W2 * (F01 - sqrt(F00 * F11));
//    double D = 2 * W2 * (G01 - sqrt(G00 * G11));
//    double A = sqrt((C + D) / F);
//    double B = sqrt((gsl_pow_2(G) * C + gsl_pow_2(GB) * D) / F);
    
    /*
     - come si ritorna un array in C?
     - fare funzione che trasforma una frequenza in w
     - alla fine bisogna ritornare G1 e i 2 array a = [1, a1, a2] e b = [b1, b2, b3]
     
    b = [(G1 + G0*W2 + B), -2*(G1 - G0*W2), (G1-B+ G0*W2)] / (1 + W2 + A);
    a = [1, [-2*(1 - W2), (1 + W2 - A)] / (1 + W2 + A)];
    */
//}
