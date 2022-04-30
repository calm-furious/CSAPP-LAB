#include "cachelab.h"

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>

#define DECL_LIST(typename) struct typename *prev, *next;

#define INSERT_AFTER(head, pos, elem)                       \
    (elem)->prev = pos;                                     \
    if (pos)                                                \
        (elem)->next = (pos)->next;                         \
    else {                                                  \
        (elem)->next = *(head);                             \
        *(head) = elem;                                     \
    }                                                       \
    if((elem)->next)                                        \
        (elem)->next->prev = elem;                          \
    if((elem)->prev)                                        \
        (elem)->prev->next = elem;
    
#define DELETE_THIS(head, elem)             \
    if((elem)->next)                        \
        (elem)->next->prev = (elem)->prev;  \
    if((elem)->prev)                        \
        (elem)->prev->next = (elem)->next;  \
    if(!(elem)->prev)                       \
        *(head) = (elem)->next;

#define REPLACE_WITH(head, pos, elem)       \
    INSERT_AFTER(head, pos, elem);          \
    DELETE_THIS(head, pos)

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
    DECL_LIST(cache_line);
};
struct cache_line** cache; 

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


void init_cache(){
    for (int i=0;i<S;i++){
        cache[i] = (struct cache_line*)NULL;
        for (int j=0;j<E;j++){
            struct cache_line *cl = malloc(sizeof(struct cache_line));
            cl->next = NULL;
            cl->prev = NULL;
            cl->tag = 0;
            cl->valid = 0;
            struct cache_line **head = &cache[i];
            INSERT_AFTER(head,(struct cache_line*)NULL,cl);
        }
    }
}

void fini_cache(){
    for (int i=0;i<S;i++){
        for (int j=0;j<E;j++){
            struct cache_line *victim = cache[i];
            DELETE_THIS(&cache[i],victim);
            free(victim);
        }
    }
}

void perform(int op, uint64_t addr){
    //get tag, set. 
    uint64_t tag, set;
    uint64_t set_flag = S-1;
    set = (addr >> b) & set_flag;
    tag = (addr >> b) >> s;
    assert(set<S);

    struct cache_line* itr = cache[set];
    while(itr){
        if(!itr->valid){
            // free = itr;
            itr->valid = 1;
            itr->tag = tag;
            DELETE_THIS(&cache[set],itr);
            INSERT_AFTER(&cache[set],(struct cache_line*)NULL,itr);
            DEBUG_PRINTF("miss ");
            misses++;
            if(op == (READ|WRITE)){
                hits++;
            }
            return;
        }
        if(itr->valid && tag == itr->tag){
            // hit
            // bring to front
            DELETE_THIS(&cache[set],itr);
            INSERT_AFTER(&cache[set],(struct cache_line*)NULL,itr);
            DEBUG_PRINTF("hit ");
            hits++;
            if(op == (READ|WRITE)){
                hits++;
            }
            return;
        }
        if(itr->valid && itr->next==NULL){
            // miss & evict
            itr->tag = tag;
            DELETE_THIS(&cache[set],itr);
            INSERT_AFTER(&cache[set],(struct cache_line*)NULL,itr);

            DEBUG_PRINTF("miss eviction ");
            misses++;evictions++;
            if(op == (READ|WRITE)){
                hits++;
            }
            return;
        }
        itr = itr->next;
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
    cache = malloc( S*sizeof(struct cache_line*) );   //struct cache_line cache[S][E]
    init_cache();
    simulate(hits, misses, evictions);
    printSummary(hits, misses, evictions);
    fini_cache();
    free(cache);
    return 0;
}

