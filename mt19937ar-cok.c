/* 
   A C-program for MT19937, with initialization improved 2002/2/10.
   Coded by Takuji Nishimura and Makoto Matsumoto.
   This is a faster version by taking Shawn Cokus's optimization,
   Matthe Bellew's simplification, Isaku Wada's real version.

   Before using, initialize the state by using init_genrand(seed) 
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.                          

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote 
        products derived from this software without specific prior written 
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> // Ajouté pour l'initialisation du générateur aléatoire

#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UMASK 0x80000000UL /* most significant w-r bits */
#define LMASK 0x7fffffffUL /* least significant r bits */
#define MIXBITS(u,v) ( ((u) & UMASK) | ((v) & LMASK) )
#define TWIST(u,v) ((MIXBITS(u,v) >> 1) ^ ((v)&1UL ? MATRIX_A : 0UL))

static unsigned long state[N]; /* the array for the state vector  */
static int left = 1;
static int initf = 0;
static unsigned long *next;

/* Prototypes des fonctions supplémentaires */
bool Verify_Point(double x_r, double y_r);
double Approximation_Of_Pi(int nombre_of_iterations);

/* initialise le générateur avec une graine */
void init_genrand(unsigned long s) {
    int j;
    state[0] = s & 0xffffffffUL;
    for (j = 1; j < N; j++) {
        state[j] = (1812433253UL * (state[j-1] ^ (state[j-1] >> 30)) + j); 
        state[j] &= 0xffffffffUL;  /* pour les machines >32 bits */
    }
    left = 1; 
    initf = 1;
}

/* initialise par un tableau avec une longueur donnée */
void init_by_array(unsigned long init_key[], int key_length) {
    int i, j, k;
    init_genrand(19650218UL);
    i = 1; 
    j = 0;
    k = (N > key_length ? N : key_length);
    for (; k; k--) {
        state[i] = (state[i] ^ ((state[i-1] ^ (state[i-1] >> 30)) * 1664525UL))
          + init_key[j] + j; /* non linéaire */
        state[i] &= 0xffffffffUL; /* pour WORDSIZE > 32 bits */
        i++; 
        j++;
        if (i >= N) { state[0] = state[N-1]; i = 1; }
        if (j >= key_length) j = 0;
    }
    for (k = N-1; k; k--) {
        state[i] = (state[i] ^ ((state[i-1] ^ (state[i-1] >> 30)) * 1566083941UL))
          - i; /* non linéaire */
        state[i] &= 0xffffffffUL; /* pour WORDSIZE > 32 bits */
        i++;
        if (i >= N) { state[0] = state[N-1]; i = 1; }
    }
    state[0] = 0x80000000UL; /* MSB is 1; assure une initialisation non nulle */ 
    left = 1; 
    initf = 1;
}

static void next_state(void) {
    unsigned long *p = state;
    int j;

    /* si init_genrand() n'a pas été appelé, */
    /* une graine initiale par défaut est utilisée         */
    if (initf == 0) init_genrand(5489UL);

    left = N;
    next = state;
    
    for (j = N - M + 1; --j; p++) 
        *p = p[M] ^ TWIST(p[0], p[1]);

    for (j = M; --j; p++) 
        *p = p[M - N] ^ TWIST(p[0], p[1]);

    *p = p[M - N] ^ TWIST(p[0], state[0]);
}

/* génère un nombre aléatoire dans l'intervalle [0,0xffffffff] */
unsigned long genrand_int32(void) {
    unsigned long y;

    if (--left == 0) next_state();
    y = *next++;

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}

/* génère un nombre aléatoire dans l'intervalle [0,0x7fffffff] */
long genrand_int31(void) {
    unsigned long y;

    if (--left == 0) next_state();
    y = *next++;

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return (long)(y >> 1);
}

/* génère un nombre aléatoire dans l'intervalle [0,1]-réel */
double genrand_real1(void) {
    unsigned long y;

    if (--left == 0) next_state();
    y = *next++;

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return (double)y * (1.0 / 4294967295.0); 
}

/* génère un nombre aléatoire dans l'intervalle [0,1)-réel */
double genrand_real2(void) {
    unsigned long y;

    if (--left == 0) next_state();
    y = *next++;

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return (double)y * (1.0 / 4294967296.0); 
}

/* génère un nombre aléatoire dans l'intervalle (0,1)-réel */
double genrand_real3(void) {
    unsigned long y;

    if (--left == 0) next_state();
    y = *next++;

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return ((double)y + 0.5) * (1.0 / 4294967296.0); 
}


double genrand_res53(void) { 
    unsigned long a = genrand_int32() >> 5, b = genrand_int32() >> 6; 
    return (a * 67108864.0 + b) * (1.0 / 9007199254740992.0); 
}

double uniform(double a, double b) {
    return a + (b - a) * genrand_real1();
}

double negExp(double mean) {
    double random_number = genrand_real1();
    return -mean * log(1 - random_number);
}

/* Simule une distribution discrète */
void simulate_discrete(int n) {
    int countA = 0, countB = 0, countC = 0;

    for (int i = 0; i < n; i++) {
        double r = genrand_real1();
        if (r < 0.35) countA++;
        else if (r < 0.80) countB++;
        else countC++;
    }

    printf("Classe A: %.2f%%\n", (double)countA / n * 100);
    printf("Classe B: %.2f%%\n", (double)countB / n * 100);
    printf("Classe C: %.2f%%\n", (double)countC / n * 100);
}

double f_normal(double x) {
    // Fonction de densité pour la loi normale centrée réduite N(0,1)
    return (1.0 / sqrt(2 * M_PI)) * exp(-0.5 * x * x);
}

double rejection_method(double minX, double maxX, double maxY) {
    double x, y;
    while (1) {
        double Na1 = genrand_real1();
        double Na2 = genrand_real1();
        x = minX + Na1 * (maxX - minX);
        y = maxY * Na2;

        if (y <= f_normal(x)) {
            return x;
        }
    }
}

void box_muller(double *x1, double *x2) {
    double Rn1 = genrand_real1();
    double Rn2 = genrand_real1();
    *x1 = sqrt(-2.0 * log(Rn1)) * cos(2.0 * M_PI * Rn2);
    *x2 = sqrt(-2.0 * log(Rn1)) * sin(2.0 * M_PI * Rn2);
}

/* Fonction pour vérifier si un point est dans le cercle unité */
bool Verify_Point(double x_r, double y_r){
    return (x_r * x_r + y_r * y_r <= 1.0);
}

/* Fonction pour approximer Pi en utilisant la méthode de Monte-Carlo */
double Approximation_Of_Pi(int nombre_of_iterations){
    int conter = 0;
    for (int i = 0; i < nombre_of_iterations; i++) {
        double x_r = genrand_real1();
        double y_r = genrand_real1();
        if(Verify_Point(x_r, y_r)){
            conter++;
        }    
    }
    double pi = 4.0 * ((double)conter / (double)nombre_of_iterations);
    return pi;
}

int main(void) {
    // Initialisation du générateur de nombres aléatoires avec une graine personnalisée
    unsigned long init[4] = {0x123, 0x234, 0x345, 0x456};
    int length = 4;
    init_by_array(init, length);

    // Tests de la partie 2
    printf("Génération de 10 nombres uniformes entre -98 et 57.7 :\n");
    for (int i = 0; i < 10; i++) {
        printf("%.2f\n", uniform(-98.0, 57.7));
    }

    // Tests de la partie 4
    printf("\nGénération de 10 nombres suivant une loi exponentielle négative avec moyenne 10 :\n");
    for (int i = 0; i < 10; i++) {
        printf("%.2f\n", negExp(10.0));
    }

    // Tests de la partie 3
    printf("\nSimulation de la distribution discrète pour différents nombres d'échantillons :\n");
    simulate_discrete(1000);
    simulate_discrete(10000);
    simulate_discrete(100000);
    simulate_discrete(1000000);

    // Partie 5 : Algorithme de rejet pour la loi normale
    printf("\nGénération de 10 nombres suivant une loi normale avec la méthode de rejet :\n");
    for (int i = 0; i < 10; i++) {
        printf("%.2f\n", rejection_method(-5.0, 5.0, f_normal(0)));
    }

    // Partie 5 : Méthode de Box-Muller pour la loi normale
    printf("\nGénération de 10 nombres suivant une loi normale avec la méthode de Box-Muller :\n");
    for (int i = 0; i < 5; i++) {
        double x1, x2;
        box_muller(&x1, &x2);
        printf("x1 = %.2f, x2 = %.2f\n", x1, x2);
    }

    // Approximation de Pi
    printf("\nApproximation de Pi par la méthode de Monte-Carlo :\n");
    int iterations;
    printf("Entrez le nombre d'itérations pour approximer Pi : ");
    if (scanf("%d", &iterations) != 1 || iterations <= 0) {
        printf("Nombre d'itérations invalide. Utilisation de 1000000 par défaut.\n");
        iterations = 1000000;
    }
    double pi = Approximation_Of_Pi(iterations);
    printf("Approximation de Pi avec %d itérations = %.6f\n", iterations, pi);

    return 0;
}
