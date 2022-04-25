#include "cachelab.h"

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

int hits = 0, misses = 0, evictions = 0;
int s=0, E=0, b=0;
int S=0,B=0;
uint64_t now = 0;

int debug_flag = 0;
#define DEBUG_PRINTF(args...) \
        if(debug_flag)printf(args);
FILE* file;
#define READ 0b1
#define WRITE 0b10
struct cache_line{
    //more
    uint8_t valid;
    int tag;
    uint64_t counter;
};
struct cache_line* cache; 

void getCacheOps(int argc, char* const* argv, int* sp, int* Ep, int* bp, FILE** filep){
    // extern char *optarg;
    // extern int optind;
    // extern int optopt;
    // extern int opterr;
    // extern int optreset;
    int ch;
    while( (ch = getopt(argc, argv, "s:E:b:t:v")) != -1 ){
        switch (ch) {
        case 's': *sp = atoi(optarg); break;
        case 'E': *Ep = atoi(optarg); break;
        case 'b': *bp = atoi(optarg); break;
        case 't': *filep = fopen(optarg, "r");if(*filep == NULL)
            DEBUG_PRINTF("failed opening file %s\n",optarg);
            break;
        case 'v': debug_flag = 1; break;
        // default: 
        }
    }
}

void perform(int op, uint64_t addr){
    //get tag, set. 
    uint64_t tag, set;
    uint64_t set_flag = S-1;
    set = (addr >> b) & set_flag;
    tag = (addr >> b) >> s;
    DEBUG_PRINTF("set:%lx tag:%lx ",set,tag);
    struct cache_line* free = NULL;
    struct cache_line* victim = NULL;
    uint8_t found_free = 0; // flag for free
    uint64_t counter = (uint64_t)-1;
    //Walk through a set
    for (struct cache_line* itr = &cache[set*E]; itr<&cache[set*E+E]; itr++){
        // DEBUG_PRINTF("itr: %p\n",itr);
        // itr->tag = tag;
        if(!found_free && !itr->valid){
            found_free = 1;
            free = itr;
        }
        if(itr->valid && tag == itr->tag){
            // hit
            itr->counter = now;
            DEBUG_PRINTF("hit ");
            hits++;
            if(op == (READ|WRITE)){
                hits++;
            } 
            return;
        }
        if(itr->valid && itr->counter < counter){
            counter = itr->counter;
            victim = itr;
        }
    }
    DEBUG_PRINTF("miss ");
    misses++;
    //Find if hit. 
    //not hit, find one empty
    // no empty, evict according to LRU
    if(found_free){
        if(op == (READ|WRITE)){
            DEBUG_PRINTF("hit ");
            hits++;
        }
        free->tag = tag;
        free->valid = 1;
        free->counter = now;
        return;
    }
    else{
        DEBUG_PRINTF("evictions ");
        evictions++;
        if(op == (READ|WRITE)){
            DEBUG_PRINTF("hit ");
            hits++;
        }
        victim->counter = now;
        victim->tag = tag;
    }

}

void simulate(int hits, int misses, int evictions){
    char op[2];  //using string for scanf to skip spaces.
    uint64_t addr;
    uint64_t size; //to be ignored according to writeup.
    // read input in a while loop
    while( EOF != fscanf(file,"%s %lx,%lx",op,&addr,&size) ){
        now ++;
        DEBUG_PRINTF("%s %lx,%lx ",op,addr,size);
        // within each loop, perform emu accordingly
        switch(op[0]){
            case 'L':
                perform(READ,addr);
                break;
            case 'M':
                perform(READ | WRITE,addr);
                break;
            case 'S':
                perform(WRITE,addr);
                break;
        }
        DEBUG_PRINTF("\n");
    }
}


int main(int argc, char * const argv[]){
    getCacheOps(argc, argv, &s, &E, &b, &file);
    DEBUG_PRINTF("%d, %d, %d\n", s,E,b);
    S = 1<<s;
    B = 1<<b;
    cache = malloc( S*E*sizeof(struct cache_line) );   //struct cache_line cache[S][E] 
    memset(cache,0,S*E*sizeof(struct cache_line));
    simulate(hits, misses, evictions);
    printSummary(hits, misses, evictions);
    return 0;
}

