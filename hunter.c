#include "defs.h"
/*  
  Function: initHunter()
  Purpose:  Initiate a hunter
       in:  HunterType *hunter, RoomType *room, EvidenceClassType evidence, char *lname
  return:   The correctly initiated hunter
*/
void initHunter(HunterType *hunter, RoomType *room, EvidenceClassType evidence, char *lname){
    hunter->room = room;
    hunter->evidenceClass = evidence;
    hunter->evidenceCollection = (EvidenceListType*)malloc(sizeof(EvidenceListType));
    initEvidenceList(hunter->evidenceCollection);
    hunter->fearLevel = 0;
    strcpy(hunter->name, lname);
    hunter->timer = BOREDOM_MAX;
    hunter->numUniqueEvidences = 0;
}
/*  
  Function: initEvidenceList()
  Purpose:  Initiate an evidence list
       in:  EvidenceListType *list
  return:   The correctly initiated evidence list
*/
void initEvidenceList(EvidenceListType *list){
    list->head = NULL;
    list->tail = NULL;
}

/*  
  Function: removeHunterFromRoom()
  Purpose:  Remove a hunter from a room 
       in:  HunterType *hunter
  return:   Room hunter list without the hunter given correctly
*/void removeHunterFromRoom(HunterType *hunter) {
    for (int i = 0; i < hunter->room->numHunters; i++) {
        if (hunter->room->hunters[i] != NULL) {
            if (hunter->room->hunters[i] == hunter) {
                hunter->room->hunters[i] = NULL;
                hunter->room->numHunters -= 1;
            }
        }
    }
}
/*  
  Function: moveHunter()
  Purpose:  Move hunter to a random room it is connected to
       in:  HunterType *hunter
  return:   Hunter moves to a connected room
*/
void moveHunter(HunterType *hunter){
    int size = hunter->room->numConnectedRooms;
    size += 1;
    int randomInt = randInt(0,size);

    RoomNodeType* curr = hunter->room->connectedRooms->head;
    if (randomInt != 0 && curr != NULL) {
    
        int counter = 1;
        while(counter < randomInt){
            curr = curr->next;
            counter += 1;
        }
    }
    // Remove hunter from room hunter list by comparing evidence type
    for(int i = 0; i < hunter->room->numHunters; i++){
        if (hunter->room->hunters[i] != NULL) { 
            if (hunter->room->hunters[i]->evidenceClass == hunter->evidenceClass) {
                hunter->room->hunters[i] = NULL;
            }
        } 
    }

    hunter->room->numHunters -= 1;
    hunter->room = curr->room;
    hunter->room->numHunters += 1;

    // Add hunter to new room's hunter list
    for(int i = 0; i < hunter->room->numHunters; i++){
        if (hunter->room->hunters[i] == NULL) { 
            hunter->room->hunters[i] = hunter;
        } 
    }
    printf("%s moved to: %s\n", hunter->name, hunter->room->name);
    hunter->timer -= 1;

}

/*  
  Function: roomContainsGhost()
  Purpose:  Checks if a room contains a ghost
       in:  RoomType* room
      out:  TRUE/FALSE int value
  return:   1 if room contains hunter, 0 if not
*/
int roomContainsGhost(RoomType* room) {
    if (room->ghostPointer != NULL) {
        return 1;
    }
    return 0;
}

/*  
  Function: updateFear()
  Purpose:  Checks if a room contains a ghost, updates fear accordingly
       in:  HunterType* hunter
  return:   Correctly updated fear level
*/
void updateFear(HunterType* hunter) {
    if (roomContainsGhost(hunter->room) == 1) {
        hunter->fearLevel += 1;
    }
}

/*  
  Function: removeEvidence()
  Purpose:  Removes evidence from a given evidence list 
       in:  EvidenceListType* list, EvidenceType* evidence
  return:   Correctly updated evidence list without old evidence
*/
void removeEvidence(EvidenceListType* list, EvidenceType* evidence) {
    EvidenceNodeType* curr = list->head;
    EvidenceNodeType* prev = NULL;
    while (curr != NULL) {
        if (curr->data->classType == evidence->classType && curr->data->value == evidence->value) {
            if (prev == NULL) {
                list->head = curr->next;
            } 
            else {
                prev->next = curr->next;
            }
        }
        prev = curr;
        curr = curr->next;
    }
    return;
}

/*  
  Function: addEvidence()
  Purpose:  Adds evidence from a given evidence list 
       in:  EvidenceListType* list, EvidenceType* evidence
  return:   Correctly updated evidence list with new evidence
*/
void addEvidence(EvidenceListType* list, EvidenceType* evidence) {
    EvidenceNodeType* node = calloc(1, sizeof(EvidenceNodeType));
    node->data = evidence;
    node->next = NULL;
    if (list->head == NULL) {
        list->head = node;
    }
    else {
        EvidenceNodeType* curr = list->head;
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = node;
    }
    return;
}
/*  
  Function: collectEvidence()
  Purpose:  Look through rooms evidence list and add to the hunters evidences list
       in:  RoomType* room, HunterType* hunter
  return:   Hunter evidence list with new evidence, room evidence list without old evidence
*/
void collectEvidence(RoomType* room, HunterType* hunter) {
    EvidenceNodeType* curr = room->evidences->head;
    while (curr != NULL) {
        if (curr->data->classType == hunter->evidenceClass) {
            printf("%s found evidence: %f\n", hunter->name, curr->data->value);
            addEvidence(hunter->evidenceCollection, curr->data);
            addEvidenceClassType(hunter, curr->data->classType);
            removeEvidence(room->evidences, curr->data);
        }
        curr = curr->next;
    }
    return;
}

/*  
  Function: hunterCommunicates()
  Purpose:  Look through rooms ghost list and add other ghosts evidence to its own evidence list
       in:  RoomType* room, HunterType* hunter
  return:   Hunter evidence list with new evidence
*/
void hunterCommunicates(RoomType* room, HunterType* hunter) {
    for (int iter = 0; iter < MAX_HUNTERS; iter++) {
        if (room->hunters[iter] != NULL && room->hunters[iter] != hunter) {
            // Add the other hunters entire evidence list to the current hunters evidence list
            EvidenceNodeType* curr = room->hunters[iter]->evidenceCollection->head;
            while (curr != NULL) {
                addEvidence(hunter->evidenceCollection, curr->data);
                addEvidenceClassType(hunter, curr->data->classType);
                removeEvidence(room->hunters[iter]->evidenceCollection, curr->data);
                curr = curr->next;
            }
            
        }
    }
    hunter->timer -= 1;
    return;
}

/*  
  Function: hunterAction()
  Purpose:  Generates random hunter actions for the hunter thread to follow in a loop until a break condition is met
       in:  void* arg
  return:   Hunter action
*/
void* hunterAction(void* arg) {

    HunterType* hunter = (HunterType*) arg;
    
    while (1)
    {
        int randomInt = randInt(1, 4);
        if (randomInt == 1) {
            hunterCommunicates(hunter->room, hunter);
            printf("%s communicated\n",hunter->name);
        }
        else if (randomInt == 2) {
            collectEvidence(hunter->room, hunter);
        }
        else if (randomInt == 3) {
            moveHunter(hunter);
        }
        updateFear(hunter);
        if(hunter->fearLevel <= MAX_FEAR){
            removeHunterFromRoom(hunter);     
        }
        if(hunter->timer <= 0){
            break;
        }
        if(hunter->numUniqueEvidences == 3){
            break; 
        }
        usleep(USLEEP_TIME);
    }
    return NULL;
}
/*  
  Function: addEvidenceClassType()
  Purpose:  Updates the number of unique evidence class types the ghost has, so if it has 3 the thread can stop
       in:  HunterType *hunter,EvidenceClassType classType
  return:   Correctly updated number of unique evidence types
*/
void addEvidenceClassType(HunterType *hunter,EvidenceClassType classType){
    if(hunter->numUniqueEvidences == 0){
        hunter->typesPickedUp[0] = classType;
        hunter->numUniqueEvidences += 1;
    }
    else{
        int addEvidence = 1;
        for(int i = 0; i < hunter->numUniqueEvidences; i++){
            if(hunter->typesPickedUp[i] == classType){
                addEvidence = 0;
            }
        }
        if(addEvidence == 1){
            hunter->typesPickedUp[hunter->numUniqueEvidences] = classType;
            hunter->numUniqueEvidences += 1;
        }
    }
}
/*  
  Function: freeHunterEvidenceList()
  Purpose:  Frees the dynamically allocated memory in a hunter evidence list
       in:  HunterType* hunter
  return:   Freed memory
*/
void freeHunterEvidenceList(HunterType* hunter) {
    EvidenceNodeType* curr = hunter->evidenceCollection->head;
    while (curr != NULL) {
        EvidenceNodeType* temp = curr;
        curr = curr->next;
        free(temp);
    }
    free(hunter->evidenceCollection);
}