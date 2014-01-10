/*
 A C-program for MT19937, with initialization improved 2002/1/26.
 Coded by Takuji Nishimura and Makoto Matsumoto.

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

#ifndef MTWIST_H_
#define MTWIST_H_

// Mersenne twister random number generator
class MTwist {
public:
    enum {
        N = 624
    };

    void init_genrand(unsigned int s);
    void init_by_array(unsigned int init_key[], int key_length);

    MTwist() :
            mti(N + 1) {
    }

    MTwist(unsigned int s) :
            mti(N + 1) {
        init_genrand(s);
    }
    MTwist(unsigned int init_key[], int key_length) :
            mti(N + 1) {
        init_by_array(init_key, key_length);
    }

    /* generates a random number on [0,0xffffffff]-interval */
    unsigned int genrand_int32(void);

    /* generates a random number on [0,0x7fffffff]-interval */
    int genrand_int31(void);

    /* generates a random number on [0,1]-real-interval */
    double genrand_real1(void);

    /* generates a random number on [0,1)-real-interval */
    double genrand_real2(void);

    /* generates a random number on (0,1)-real-interval */
    double genrand_real3(void);

    /* generates a random number on [0,1) with 53-bit resolution*/
    double genrand_res53(void);

// AD: Added for Twitter simulation

    /* Grab an integer from 0 to max, non-inclusive (ie appropriate for array lengths). */
    int rand_int(int max) {
        //AD: Modified to remove modulo-bias problem. Inspired by Java's nextInt implementation.
        int raw = genrand_int31();

        if ((max & -max) == max) { // i.e., max is a power of 2
            return (int) ((max * (long long)raw) >> 31);
        }
        int val;
        do {
            val = raw % max;
            // Reject values within a small, problematic range:
        } while (raw - val + (max - 1) < 0);

        return val;
    }
    int rand_int(int min, int max) {
        int range = max - min;
        return rand_int(range) + min;
    }

    /* Grab a real number within [0,1) with 53-bit resolution */
    double rand_real_not1() {
        return genrand_res53();
    }
    /* Grab a real number within (0,1] with 53-bit resolution */
    double rand_real_not0() {
        return 1.0 - rand_real_not1();
    }
    /* Using Mersenne-twister, grab a real number within [0,1] */
    double rand_real_with01() {
        return genrand_real1();
    }

private:
    unsigned int mt[N];
    int mti;
};

#endif
