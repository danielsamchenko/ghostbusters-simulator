#include "defs.h"

/*  
  Function: initGhost()
  Purpose:  Initiate a ghost
       in:  GhostType *ghost, GhostClassType class, RoomType *room
  return:   The correctly initiated ghost
*/
void initGhost(GhostType *ghost, GhostClassType class, RoomType *room){
    ghost->ghostClass = class;
    ghost->boredomTimer = BOREDOM_MAX;
    ghost->room = room;
}
/*  
  Function: initEvidence()
  Purpose:  Initiate an evidence
       in:  EvidenceType *evidence, EvidenceClassType class, float value
  return:   The correctly initiated evidence
*/
void initEvidence(EvidenceType *evidence, EvidenceClassType class, float value){
    evidence->classType = class;
    evidence->value = value;
}
/*  
  Function: randomRoom()
  Purpose:  Pick a random room for the ghost to start in 
       in:  RoomListType *list
  return:   Random RoomType room
*/
RoomType randomRoom(RoomListType *roomList){
    int randomInt = randInt(1,13);
    RoomNodeType* curr = roomList->head;
    int counter = 0;
    RoomType rndmRoom;
    while(counter <= randomInt && curr!=NULL){
        rndmRoom = *curr->room;
        curr = curr->next;
        counter += 1;
    }
    return rndmRoom;
}
/*  
  Function: randomClass()
  Purpose:  Pick a class room for the ghost to init with 
  return:   Random RoomType room
*/
GhostClassType randomClass() {
    GhostClassType class;
    int randomInt = randInt(1,5);
    if (randomInt == 1){
        class = POLTERGEIST;
    }
    if (randomInt == 2){
        class = BANSHEE;
    }
    if (randomInt == 3){
        class = BULLIES;
    }
    if (randomInt == 4){
        class = PHANTOM;
    }
    return class;
}
/*  
  Function: moveGhost()
  Purpose:  Move ghost to a random room it is connected to
       in:  GhostType *ghost
  return:   Ghost moves to a connected room
*/
void moveGhost(GhostType *ghost){
    ghost->room->ghostPointer = NULL;

    int size = ghost->room->numConnectedRooms;
    size += 1;
    int randomInt = randInt(0,size);

    RoomNodeType* curr = ghost->room->connectedRooms->head;
    if (randomInt!=0 && curr!=NULL){
        int counter = 1;
        while(counter < randomInt){
            curr = curr->next;
            counter += 1;
        }
    }
    ghost->room = curr->room;
    ghost->room->ghostPointer = ghost;
    printf("Ghost moved to: %s\n",ghost->room->name);
}
/*  
  Function: randomEvidenceClass()
  Purpose:  Pick a evidence class room for the ghost to init with, based off its ghost class
       in:  GhostType *ghost
  return:   Random evidence class type
*/
EvidenceClassType randomEvidenceClass(GhostType *ghost){
    EvidenceClassType class;
    int randomInt = randInt(1,4);
    if(ghost->ghostClass == POLTERGEIST){
        if (randomInt == 1){
            class = EMF;
        }
        if (randomInt == 2){
            class = TEMPERATURE;
        }
        if (randomInt == 3){
            class = FINGERPRINTS;
        }
        return class;
    }
    else if(ghost->ghostClass == BANSHEE){
        if (randomInt == 1){
            class = EMF;
        }
        if (randomInt == 2){
            class = TEMPERATURE;
        }
        if (randomInt == 3){
            class = SOUND;
        }
        return class;
    }
    else if(ghost->ghostClass == BULLIES){
        if (randomInt == 1){
            class = EMF;
        }
        if (randomInt == 2){
            class = FINGERPRINTS;
        }
        if (randomInt == 3){
            class = SOUND;
        }
        return class;
    }
    else if(ghost->ghostClass == PHANTOM){
        if (randomInt == 1){
            class = TEMPERATURE;
        }
        if (randomInt == 2){
            class = FINGERPRINTS;
        }
        if (randomInt == 3){
            class = SOUND;
        }
        return class;
    }
}
/*  
  Function: generateRandomEvidenceValue()
  Purpose:  Pick a random float value based off the evidence class type
       in:  EvidenceClassType* ev
  return:   Random evidence value
*/
float generateRandomEvidenceValue(EvidenceClassType* ev) {
    float value;
    if (*ev == EMF){
        value = randFloat(MIN_EMF_GHOSTLY_RES, MAX_EMF_GHOSTLY_RES);
    }
    else if (*ev == TEMPERATURE){
        value = randFloat(MIN_TEMP_GHOSTLY_RES, MAX_TEMP_GHOSTLY_RES);
    }
    else if (*ev == FINGERPRINTS){
        value = FINGERPRINT_GHOSTLY_RES;
    }
    else if (*ev == SOUND){
        value = randFloat(MIN_SOUND_GHOSTLY_RES, MAX_SOUND_GHOSTLY_RES);
    }
    return value;
}
/*  
  Function: ghostAction()
  Purpose:  Generates random ghost actions for the ghost thread to follow in a loop until a break condition is met
       in:  void* arg
  return:   Ghost action
*/
void* ghostAction(void* arg) {
    GhostType* ghost = (GhostType*) arg;

    while (1)
    {
        int randomInt = randInt(0, 3);

        if (randomInt == 1) {
            moveGhost(ghost);
        }
        else if (randomInt == 2){
            dropEvidence(ghost);
        }
        else{
            printf("Ghost did nothing\n");
        }
        updateBoredom(ghost);
        if(ghost->boredomTimer <= 0){
            break;
        }
    }
    usleep(USLEEP_TIME);
}

/*  
  Function: dropEvidence()
  Purpose:  Generates a random ghost evidence for the ghost to drop
       in:  GhostType* ghost
  return:   Evidence placed in current room evidence list
*/
void dropEvidence(GhostType* ghost){
    EvidenceClassType* ev = calloc(1, sizeof(EvidenceClassType));
    *ev = randomEvidenceClass(ghost);
    float value = generateRandomEvidenceValue(ev);
    EvidenceType* evidence = calloc(1, sizeof(EvidenceType));
    evidence->classType = *ev;
    evidence->value = value;
    addEvidence(ghost->room->evidences, evidence);
    printf("Ghost dropped evidence: ");
    printEvidenceClassType(ev);
    printf(" %f \n",value);
}

/*  
  Function: printEvidenceClassType()
  Purpose:  Prints evidence type to console given the evidence class type 
       in:  EvidenceClassType* ev
  return:   Type printed to console
*/
void printEvidenceClassType(EvidenceClassType* ev) {
    if (*ev == EMF) {
        printf("EMF");
    }
    else if (*ev == TEMPERATURE) {
        printf("TEMPERATURE");
    }
    else if (*ev == FINGERPRINTS) {
        printf("FINGERPRINTS");
    }
    else if (*ev == SOUND) {
        printf("SOUND");
    }
}

/*  
  Function: containsHunter()
  Purpose:  Checks if a room contains a hunter
       in:  RoomType* room
      out:  TRUE/FALSE int value
  return:   1 if room contains hunter, 0 if not
*/
int containsHunter(RoomType* room) {
    for (int iter = 0; iter < MAX_HUNTERS; iter++) {
        if (room->hunters[iter] != NULL) {
            return 1; 
        }
    }
    return 0; 
}

/*  
  Function: updateBoredom()
  Purpose:  Checks if a room contains a hunter, updates boredom accordingly
       in:  GhostType* ghost
  return:   Correctly updated boredom timer
*/
void updateBoredom(GhostType* ghost) {
    if (containsHunter(ghost->room) == 0) {
        ghost->boredomTimer -= 1;
    }
}

/*  
  Function: printGhostClassType()
  Purpose:  Prints host type to console given the ghost class type 
       in:  GhostClassType* ghostClass
  return:   Type printed to console
*/void printGhostClassType(GhostClassType* ghostClass) {
    if (*ghostClass == POLTERGEIST) {
        printf("POLTERGEIST\n");
    }
    else if (*ghostClass == BANSHEE) {
        printf("BANSHEE\n");
    }
    else if (*ghostClass == BULLIES) {
        printf("BULLIES\n");
    }
    else if (*ghostClass == PHANTOM) {
        printf("PHANTOM\n");
    }
}

