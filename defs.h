#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_STR            64
#define FEAR_RATE           1
#define MAX_FEAR          100
#define MAX_HUNTERS         4
#define USLEEP_TIME     50000
#define BOREDOM_MAX        99

// Evidence definitions
#define MIN_EMF_STANDARD 0.00
#define MAX_EMF_STANDARD 4.90
#define MIN_EMF_GHOSTLY_RES 4.70
#define MAX_EMF_GHOSTLY_RES 5.00 

#define MIN_TEMP_STANDARD 0.00
#define MAX_TEMP_STANDARD 27.00
#define MIN_TEMP_GHOSTLY_RES -10.00
#define MAX_TEMP_GHOSTLY_RES 1.00 

#define FINGERPRINT_STANDARD 0.00
#define FINGERPRINT_GHOSTLY_RES 1.00 

#define MIN_SOUND_STANDARD 40.00
#define MAX_SOUND_STANDARD 70.00
#define MIN_SOUND_GHOSTLY_RES 65.00 
#define MAX_SOUND_GHOSTLY_RES 75.00

// You may rename these types if you wish
typedef enum { EMF, TEMPERATURE, FINGERPRINTS, SOUND } EvidenceClassType;
typedef enum { POLTERGEIST, BANSHEE, BULLIES, PHANTOM } GhostClassType;


// Room struct def
typedef struct{
  char name[MAX_STR];
  struct RoomList *connectedRooms;
  struct EvidenceList *evidences;
  struct HunterType *hunters[MAX_HUNTERS];
  struct GhostType *ghostPointer;
  int numConnectedRooms;
  int numHunters;
} RoomType;

// Room node struct def
typedef struct RoomNode {
  RoomType* room;
  struct RoomNode* next;
} RoomNodeType;

// Room linked list struct def
typedef struct RoomList {
  RoomNodeType* head;
  RoomNodeType* tail;
  //int size; 
} RoomListType;

//Evidence struct def
typedef struct{
  EvidenceClassType classType;
  float value;
} EvidenceType;

// Evidence node struct def
typedef struct EvidenceNode {
  EvidenceType *data;
  struct EvidenceNode* next;
} EvidenceNodeType;

// Evidence linked list struct def
typedef struct EvidenceList {
  EvidenceNodeType* head;
  EvidenceNodeType* tail;
  //int size;
} EvidenceListType;

// Ghost struct def
typedef struct GhostType {
  GhostClassType ghostClass;
  RoomType *room;
  int boredomTimer;
} GhostType;

// Hunter struct def
typedef struct HunterType {
  RoomType *room;
  EvidenceClassType evidenceClass;
  EvidenceListType *evidenceCollection;
  char name[MAX_STR];
  int fearLevel;
  int timer;
  EvidenceClassType typesPickedUp[3];
  int numUniqueEvidences;

} HunterType;

// Building struct def 
typedef struct Building {
  GhostType ghost;
  HunterType *hunters[MAX_HUNTERS];
  RoomListType *rooms;
} BuildingType;


int randInt(int, int);          // Generates a pseudorandom integer between the parameters
float randFloat(float, float);  // Generates a pseudorandom float between the parameters

void populateRooms(BuildingType*);   // Populates the building with sample data for rooms

//inits
void initRoom(RoomType*, char*);
void initRoomList(RoomListType*);
void initEvidenceList(EvidenceListType*);
void initBuilding(BuildingType*);
void initGhost(GhostType*,GhostClassType,RoomType*);
void initHunter(HunterType*, RoomType*, EvidenceClassType, char*);
void appendRoom(RoomListType*, RoomNodeType*);
void connectRooms(RoomType*, RoomType*);
void printRooms(RoomListType*);
void printRoom(RoomType*);
RoomType randomRoom(RoomListType*);
GhostClassType randomClass();
void moveGhost(GhostType*);
EvidenceClassType randomEvidenceClass(GhostType*);
float generateRandomEvidenceValue(EvidenceClassType*);
void* ghostAction(void*);
void printEvidenceClassType(EvidenceClassType*);
int containsHunter(RoomType*);
void updateBoredom(GhostType*);
void dropEvidence(GhostType*);
int isGhostBored(GhostType*);
void printGhostClassType(GhostClassType*);
void moveHunter(HunterType*);
int roomContainsGhost(RoomType*);
void updateFear(HunterType*);
void removeEvidence(EvidenceListType*, EvidenceType*);
void addEvidence(EvidenceListType*, EvidenceType*); 
void collectEvidence(RoomType*, HunterType*);
void hunterCommunicates(RoomType*, HunterType*);
void removeHunterFromRoom(HunterType*);
void* hunterAction(void*);
void addEvidenceClassType(HunterType*,EvidenceClassType);
void printEvidence(HunterType*);
void printRoomEvidenceList(RoomType*);
void freeBuilding(BuildingType*);
void freeRoom(RoomType*);
void freeRoomList(RoomListType*);
void freeEvidenceList(EvidenceListType*);
void freeHunterEvidenceList(HunterType*);