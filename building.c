#include "defs.h"
/*  
  Function: initBuilding()
  Purpose:  Initiate a building
       in:  BuildingType* b
  return:   The correctly initiated building
*/
void initBuilding(BuildingType* b){
    b->rooms = (RoomListType*)malloc(sizeof(RoomListType));
    initRoomList(b->rooms);
}
/*  
  Function: initRoomList()
  Purpose:  Initiate a room list
       in:  RoomListType *list
  return:   The correctly initiated room List
*/
void initRoomList(RoomListType *list){
    list->head = NULL;
    list->tail = NULL;
}

// function provided in base code
void populateRooms(BuildingType* building) {
    RoomType* van = calloc(1, sizeof(RoomType));
    initRoom(van, "Van");
    RoomType* hallway = calloc(1, sizeof(RoomType));
    initRoom(hallway, "Hallway");
    RoomType* master_bedroom = calloc(1, sizeof(RoomType));
    initRoom(master_bedroom, "Master Bedroom");
    RoomType* boys_bedroom = calloc(1, sizeof(RoomType));
    initRoom(boys_bedroom, "Boy's Bedroom");
    RoomType* bathroom = calloc(1, sizeof(RoomType));
    initRoom(bathroom, "Bathroom");
    RoomType* basement = calloc(1, sizeof(RoomType));
    initRoom(basement, "Basement");
    RoomType* basement_hallway = calloc(1, sizeof(RoomType));
    initRoom(basement_hallway, "Basement Hallway");
    RoomType* right_storage_room = calloc(1, sizeof(RoomType));
    initRoom(right_storage_room, "Right Storage Room");
    RoomType* left_storage_room = calloc(1, sizeof(RoomType));
    initRoom(left_storage_room, "Left Storage Room");
    RoomType* kitchen = calloc(1, sizeof(RoomType));
    initRoom(kitchen, "Kitchen");
    RoomType* living_room = calloc(1, sizeof(RoomType));
    initRoom(living_room, "Living Room");
    RoomType* garage = calloc(1, sizeof(RoomType));
    initRoom(garage, "Garage");
    RoomType* utility_room = calloc(1, sizeof(RoomType));
    initRoom(utility_room, "Utility Room");

    RoomNodeType* van_node = calloc(1, sizeof(RoomNodeType));
    van_node->room = van;
    RoomNodeType* hallway_node = calloc(1, sizeof(RoomNodeType));
    hallway_node->room = hallway;
    RoomNodeType* master_bedroom_node = calloc(1, sizeof(RoomNodeType));
    master_bedroom_node->room = master_bedroom;
    RoomNodeType* boys_bedroom_node = calloc(1, sizeof(RoomNodeType));
    boys_bedroom_node->room = boys_bedroom;
    RoomNodeType* bathroom_node = calloc(1, sizeof(RoomNodeType));
    bathroom_node->room = bathroom;
    RoomNodeType* basement_node = calloc(1, sizeof(RoomNodeType));
    basement_node->room = basement;
    RoomNodeType* basement_hallway_node = calloc(1, sizeof(RoomNodeType));
    basement_hallway_node->room = basement_hallway;
    RoomNodeType* right_storage_room_node = calloc(1, sizeof(RoomNodeType));
    right_storage_room_node->room = right_storage_room;
    RoomNodeType* left_storage_room_node = calloc(1, sizeof(RoomNodeType));
    left_storage_room_node->room = left_storage_room;
    RoomNodeType* kitchen_node = calloc(1, sizeof(RoomNodeType));
    kitchen_node->room = kitchen;
    RoomNodeType* living_room_node = calloc(1, sizeof(RoomNodeType));
    living_room_node->room = living_room;
    RoomNodeType* garage_node = calloc(1, sizeof(RoomNodeType));
    garage_node->room = garage;
    RoomNodeType* utility_room_node = calloc(1, sizeof(RoomNodeType));
    utility_room_node->room = utility_room;

    // Building->rooms might be a linked list structre, or maybe just a node.
    initRoomList(building->rooms);

    appendRoom(building->rooms, van_node);
    appendRoom(building->rooms, hallway_node);
    appendRoom(building->rooms, master_bedroom_node);
    appendRoom(building->rooms, boys_bedroom_node);
    appendRoom(building->rooms, bathroom_node);
    appendRoom(building->rooms, basement_node);
    appendRoom(building->rooms, basement_hallway_node);
    appendRoom(building->rooms, right_storage_room_node);
    appendRoom(building->rooms, left_storage_room_node);
    appendRoom(building->rooms, kitchen_node);
    appendRoom(building->rooms, living_room_node);
    appendRoom(building->rooms, garage_node);
    appendRoom(building->rooms, utility_room_node);

    // Now connect the rooms. It is possible you do not need a separate
    // function for this, but it is provided to give you a starting point.
    connectRooms(van, hallway);
    connectRooms(hallway, master_bedroom);
    connectRooms(hallway, boys_bedroom);
    connectRooms(hallway, bathroom);
    connectRooms(hallway, kitchen);
    connectRooms(hallway, basement);
    connectRooms(basement, basement_hallway);
    connectRooms(basement_hallway, right_storage_room);
    connectRooms(basement_hallway, left_storage_room);
    connectRooms(kitchen, living_room);
    connectRooms(kitchen, garage);
    connectRooms(garage, utility_room);
}

/*  
  Function: appendRoom()
  Purpose:  Add a room node to a room list
       in:  RoomListType *list, RoomNodeType *room
  return:   Updated room list with new node
*/
void appendRoom(RoomListType *list, RoomNodeType *room){
    room->next = NULL;

    if(list->head == NULL){
        list->head = room;
        list->tail = room;
    }
    else{
        list->tail->next = room;
        list->tail = room;
    }
}

/*  
  Function: connectRooms()
  Purpose:  Connect two rooms by adding each room as a node to eachother's roomlist
       in:  RoomType *roomOne, RoomType *roomTwo
  return:   Updated room lists with each new room node
*/
void connectRooms(RoomType *roomOne, RoomType *roomTwo){
    //room one node
    RoomNodeType *nodeOne = (RoomNodeType*)calloc(1, sizeof(RoomNodeType));
    nodeOne->room = roomOne;
    nodeOne->next = NULL;
    //room two node
    RoomNodeType *nodeTwo = (RoomNodeType*)calloc(1, sizeof(RoomNodeType));
    nodeTwo->room = roomTwo;
    nodeTwo->next = NULL;
    //add room two node to room one list
    if(roomOne->connectedRooms->head == NULL){
        roomOne->connectedRooms->head = nodeTwo;
        roomOne->connectedRooms->tail = nodeTwo;
    }
    else{
        roomOne->connectedRooms->tail->next = nodeTwo;
        roomOne->connectedRooms->tail = nodeTwo;
    }
    //add room one node to room two list
    if(roomTwo->connectedRooms->head == NULL){
        roomTwo->connectedRooms->head = nodeOne;
        roomTwo->connectedRooms->tail = nodeOne;
    }
    else{
        roomTwo->connectedRooms->tail->next = nodeOne;
        roomTwo->connectedRooms->tail = nodeOne;
    }
    // increase number of connected rooms in each room
    roomOne->numConnectedRooms += 1;
    roomTwo->numConnectedRooms += 1;
}

/*  
  Function: printRooms()
  Purpose:  Print out all rooms in a room list
       in:  RoomListType *list
  return:   Printed rooms to terminal
*/
void printRooms(RoomListType *list){
    RoomNodeType* curr = list->head;
    while(curr != NULL){
        printRoom(curr->room);
        curr = curr->next;
    }
}
/*  
  Function: printRoom()
  Purpose:  Print out all connected rooms in a room
       in:  RoomType* room
  return:   Printed connected rooms to terminal
*/
void printRoom(RoomType* room){
    printf("%s\n", room->name);
    printf("Rooms connected:\n");
    RoomNodeType* curr = room->connectedRooms->head;
    while(curr != NULL){
        printf("%s\n", curr->room->name);
        curr = curr->next;
    }
    printf("%d\n",room->numConnectedRooms);
    printf("\n");
}

/*  
  Function: freeBuilding()
  Purpose:  Frees the dynamically allocated memory in a building
       in:  BuildingType* building
  return:   Freed memory
*/
void freeBuilding(BuildingType* building) {
    // Free all rooms
    RoomNodeType* curr = building->rooms->head;
    while (curr != NULL) {
        freeRoom(curr->room);
        curr = curr->next;
    }
    freeRoomList(building->rooms);
}
/*  
  Function: freeRoom()
  Purpose:  Frees the dynamically allocated memory in a room
       in:  RoomType* room
  return:   Freed memory
*/
void freeRoom(RoomType* room) {
    freeRoomList(room->connectedRooms);
    freeEvidenceList(room->evidences);
    free(room);
}

/*  
  Function: freeRoomList()
  Purpose:  Frees the dynamically allocated memory in a room list
       in:  RoomListType* list
  return:   Freed memory
*/
void freeRoomList(RoomListType* list) {
    RoomNodeType* curr = list->head;
    while (curr != NULL) {
        RoomNodeType* next = curr->next;
        free(curr);
        curr = next;
    }
    free(list);
}
/*  
  Function: freeEvidenceList()
  Purpose:  Frees the dynamically allocated memory in a evidence list
       in:  EvidenceListType* list
  return:   Freed memory
*/
void freeEvidenceList(EvidenceListType* list) {
    EvidenceNodeType* curr = list->head;
    while (curr != NULL) {
        EvidenceNodeType* next = curr->next;
        free(curr);
        curr = next;
    }
    free(list);
}