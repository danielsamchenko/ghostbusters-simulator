#include "defs.h"
/*  
  Function: initRoom()
  Purpose:  Initiate a room
       in:  RoomType *room, char *name
  return:   The correctly initiated room
*/
void initRoom(RoomType *room, char *name){
    strcpy(room->name, name);
    room->connectedRooms = (RoomListType*)malloc(sizeof(RoomListType));
    initRoomList(room->connectedRooms);
    room->evidences = (EvidenceListType*)malloc(sizeof(EvidenceListType));
    initEvidenceList(room->evidences);
    room->ghostPointer = NULL;
    room->numConnectedRooms = 0;
    room->numHunters = 0;
}