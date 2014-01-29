
/************************************************************************
* Set Associative Cache Simulator                                       *
* Author: Ariunjargal Batjargal                                         *
*                                                                       *
* This is a simulator for set-associative caches under LRU remplacement *
* strategy. The inputs to the simulator will be the line length, line   *
* per set, and the cache size. The number of sets will be computed form *
* these inputs.															*
* 																		*
* 0 = read data 														*
* 1 = write data				 										*
* 2 = instruction fetch 												*
* 3 = unknown access type 												*
* 4 = causes a flushing of cache 										*
*			     														*
************************************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#define maxTrace 100000
#define maxCacheSet 4096
#define maxLinePerSet 16

int C;						// Total cache size in bytes
int K;						// Number of line per set 
int L;						// Line size in bytes
char cacheSetting;			 
int numberOfAccess;			// Number of access made to the cache
int offset;					// Offset bits for current cache
int indexBits;				// Index bits for current cache
int numSets;				// Number of sets in current cache
int mask;					// The mask bits for the current cache
int hitCount;				// Total hit count to the cache
int dataHitCount;			// Hit count for the data cache
int instHitCount;			// Hit count for the instruction cache

double dataAccessCount;		// Number of accesses to the data cache
double instAccessCount;		// Number of accesses to the instruction cache

/****************** CACHE ARRAYS ***********************/

int Cache[maxCacheSet][maxLinePerSet];		// Array to store cache tags
int Valid[maxCacheSet][maxLinePerSet];		// Array to store cache valid bits
int LRU[maxCacheSet][maxLinePerSet];		// Array to store cache least recently used
int instCache[64]; 							// Array to store instruction cache tags
int instValid[64];							// Array to store instruction cache valid bits
int dataCache[64];							// Array to store data cache tags
int dataValid[64];							// Array to store data cache valid bits

/********************** START CACHE *********************/

struct trace{
	int accessType;
	int address;
	int index;
	int tag;
};

struct trace traceArray[maxTrace];

// Return number of sets for the current cache
int getNumSets(int c, int k, int l){
	return c/(k*l);
};

// Return the log base 2 of a given number
int logBase2Off(int num){
	return log10(num)/log10(2);
};

int shiftRight(int address, int num){
	return (address>>x);
};

int getMask(int bits){
	return pow(2, bits) -1;
};

int getIndex(int address){
	offset = logBase2Off(L);
	numSets = getNumSets(C,K,L);
	indexBits = logBase2Off(numSets);
	mask = getMask(indexBits);
	int addr = shiftRight(address, offset);
	return (addr & mask);
};

void initialize(){
	if(cacheSetting != 'd'){
		for(int i=0; i < maxCacheSet; i++){
			for(int j=0; j < maxLinePerSet; j++){
				Cache[i][j] = -1;
				Valid[i][j] = 0;
				LRU[i][j] = 0;
			}
		}
	}else{
		for(int i=0; i < 64; i++){
			instCache[i] = -1;
			dataCache[i] = -1;
			instValid[i] = 0;
			dataValid[i] = 0;
		}
	}
}

void readTrace(){
	int x;
	int y;
	int i=0;

	FILE *ifp;
	switch(cacheSetting){
		case 'e':
			ifp = fopen("randomTrace.txt", "r");
			break;
		case 'f':
			ifp = fopen("seqTrace.txt", "r");
			break;
		default:
			ifp = fopen("trace.txt", "r");
			break;
	}

	while(!feof(ifp)){
		fscanf(ifp, "%d %x", &x, &y);
		struct trace temp;
		temp.accessType = x;
		temp.address = y;
		temp.index = getIndex(y);
		temp.tag = shiftRight(y,(offset+indexBits));
		assert(i < maxTrace);
		traceArray[i] = temp;
		i++;
		numberOfAccess++;
	}

	fclose(ifp);
	return;
};

void hitLRUReplace(int index, int tag, int age){				
	int tempTag;													
	switch(age){
		case 0:													
			break;
		default:												
			tempTag = Cache[index][age];						
			for (int i=age; i>0; i--){							
				Cache[index][i] = Cache[index][i-1];				
				Valid[index][i] = 1;								
				LRU[index][i] = i;								
			}
			Cache[index][0] = tempTag;								
			Valid[index][0] = 1;
			LRU[index][0] = 0;	
			break;
	}
};

void missLRUReplace (int index, int tag){						
	for (int i=0; i<(K-1); i++){									
		Cache[index][i+1] = Cache[index][i];
		Valid[index][i] = 1;
		LRU[index][i] = i;
	}
	Cache[index][0] = tag;										
	Valid[index][0] = 1;
	LRU[index][0] = 0;
};

void cacheAccess(struct trace A){
	int hit = 0;													
	int age;													
	if (A.accessType !=4){
		for(int i=0; i<K; i++){
			if(Cache[A.index][i] == A.tag && Valid[A.index][i] == 1){
				hit = 1;		
				age = i;
			}
		
		}
		if (hit == 0){
			missLRUReplace(A.index, A.tag);
		}
		else{
			hitLRUreplace(A.index, A.tag, age);
		}
		HitCount = HitCount + hit; 
	}
	else {
		initialize();		
	}
};

void separateCacheAccess(struct trace A){
	int hit = 0;
	if (A.accessType == 0 || A.accessType == 1){				
		dataAccessCount +=1;
		if(dataCache[A.index] == A.tag && dataValid[A.index] == 1){
			h=1;		
		}
		else{
			dataCache[A.index] = A.tag;	
			dataValid[A.index] = 1;
				
		}
		dataHitCount = dataHitCount + hit;
	}
	else{
		if(A.accessType == 2){			
			instAccessCount +=1;
			if(instCache[A.index] == A.tag && instValid[A.index] == 1){
				hit = 1;
			}
			else
			{
				instCache[A.index] = A.tag;
				instValid[A.index] = 1;
			}
			instHitCount += hit;
		}
		if(A.accessType == 4){			
			Initialize();
		}
	}
};

void getCacheInfo(){
	printf("Select the simulation to run ( valid selection a, b, c, d, e and f): \n");
	scanf("%c", &cacheSetting);
	assert(cacheSetting == 'a' || cacheSetting == 'b' || cacheSetting == 'c' || cacheSetting == 'd' ||
		cacheSetting == 'e' || cacheSetting == 'f');
	if(cacheSetting != 'd'){
		printf("Enter the cache size in bytes: \n");
		scanf("%d", &C);
		printf("Enter the number of lines per set: \n");
		scanf("%d", &K);
		printf("Enter line size in bytes: \n");
		scanf("%d", &L);
	}else{
		C = 1024;
		K = 1;
		L = 16;
	}
};





