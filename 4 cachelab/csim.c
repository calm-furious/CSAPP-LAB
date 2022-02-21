#include "cachelab.h"

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

struct cache_line{
    int tag;
    //more
};

// int stoi(const char * opt){

// }


void getCacheOps(int argc, char* const* argv, int* sp, int* Ep, int* bp, FILE** filep){
    // extern char *optarg;
    // extern int optind;
    // extern int optopt;
    // extern int opterr;
    // extern int optreset;
    int ch;
    while( (ch = getopt(argc, argv, "s:E:b:t:")) != -1 ){
        switch (ch) {
        case 's': *sp = atoi(optarg); break;
        case 'E': *Ep = atoi(optarg); break;
        case 'b': *bp = atoi(optarg); break;
        case 't': *filep = fopen(optarg, "r"); break;
        // default: 
        }
    }
    
}

void simulate(int hits, int misses, int evictions){

}


int main(int argc, char * const argv[]){
    // int hits = 0, misses = 0, evictions = 0;
    int s=0, E=0, b=0;
    FILE* file;
    getCacheOps(argc, argv, &s, &E, &b, &file);
    // printf("%d, %d, %d\n", s,E,b);

    // struct cache_line* cache = malloc( s*E*sizeof(struct cache_line) );   //struct cache_line cache[S][E] 

    // simulate(hits, misses, evictions);
    // printSummary(hits, misses, evictions);
    return 0;
}

