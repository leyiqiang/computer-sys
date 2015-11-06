#include <stdio.h>
#include <stdlib.h>
#include <string.h>
  int test_set[] = {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 0, 4, 8, 12, 16, 71, 3, 41, 81, 39, 38, 71, 15, 39, 11, 51, 57, 41};
  int test_set_length = 39;
  void cacherun(int s, int l, int n);
  struct caches {int tag; int set;};
  struct caches old_cache[39];
  void buildContent(int s, int l, int n);
  int isMissOrHit(struct caches old_cache[], int tag, int set,int idx);  

// run sample test
  int main() {
        // sample 1
        // 128-byte direct-mapped, 8-byte cache lines
        printf("Test Run 1: \n");
        cacherun(128, 8, 1);
        // sample 2
        // 64-byte, 2-way set associated, 8-byte cache lines
        printf("Test Run 2: \n");
	cacherun(64, 8, 2);
	// sample 3
	// 128-byte, direct-mapped cache, 16-byte cache lines
	printf("Text Run 3: \n");
	cacherun(128, 16, 1);
	// sample 4
	// 64-byte, fully associative cache, 8-byte cache lines
	printf("Test Run 4: \n");
	cacherun(64, 8, 8);
  }

  struct row {int valid; int tag;}; 
  struct row content[5000];
  // cacherun
  // s: cache size
  // l: cache lines of size
  // n: n-way set
  void cacherun(int s, int l, int n) {
    for(int i = 0; i < test_set_length; i++) { 
      int addr = test_set[i];
      int offSet = addr % l;
      int blockAddr = addr / l;
      int setIndex = blockAddr;
      int tag = addr / s;
      int miss = isMissOrHit(old_cache, tag, setIndex, i);
      content[addr/l].valid = 1;
      if(content[addr/l].tag < tag) {
         content[addr/l].tag = tag ;
      }
      if(content[addr/l].tag == '\0') {
         content[addr/l].tag = tag ;
      }
      char isMiss[4];
      if(miss) {
        strcpy(isMiss, "MISS");
      } else {
       strcpy(isMiss, "HIT");
      }
      old_cache[i].tag = tag;
      old_cache[i].set = setIndex;
      printf("%d: %s tag:%d,setIndex: %d, offSet:%d \n",addr, isMiss, tag, setIndex, offSet);
     }
    buildContent(s, l, n);
  }

  int isMissOrHit(struct caches old_cache[], int tag, int set, int idx) {
    int b = 1;
    int i = 0;
    for(int i = 0; i < idx; i++) {
        b = b * ((tag != old_cache[i].tag) +  (set != old_cache[i].set));
      }
    return b; 
  }
  void buildContent(int s, int l, int n) {
   printf("Cache Content: \n");
   int count = 0;
   for(int i = 0; i < s / l /n; i++) {
     for(int j = 0; j < n; j++) {
       if(content[count].valid == 0) {
       printf("%d: valid: false, tag: -, setNum: %d\n", count, i); 
       } else{
       printf("%d: valid: true, tag: %d, setNum: %d\n", count, content[count].tag, i);
       }
       count++;
     }
   }
  }
