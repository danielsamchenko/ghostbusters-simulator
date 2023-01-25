#include "defs.h"

// Daniel test commit

int main(int argc, char *argv[])
{
    // Initialize a random seed for the random number generators
    srand(time(NULL));

    // Get hunter names from user
    char nameOne[MAX_STR];
    char nameTwo[MAX_STR];
    char nameThree[MAX_STR];
    char nameFour[MAX_STR];

    printf("Enter first Hunter's name: ");
    scanf("%s", nameOne);
    printf("Enter second Hunter's name: ");
    scanf("%s", nameTwo);
    printf("Enter third Hunter's name: ");
    scanf("%s", nameThree);
    printf("Enter fourth Hunter's name: ");
    scanf("%s", nameFour);

    // initialize building 
    BuildingType building;
    initBuilding(&building);
    populateRooms(&building);

    // initialize the 4 hunters
    HunterType hunterOne;
    initHunter(&hunterOne, building.rooms->head->room, EMF, nameOne);

    HunterType hunterTwo;
    initHunter(&hunterTwo, building.rooms->head->room, TEMPERATURE, nameTwo);

    HunterType hunterThree;
    initHunter(&hunterThree, building.rooms->head->room, FINGERPRINTS, nameThree);

    HunterType hunterFour;
    initHunter(&hunterFour, building.rooms->head->room, SOUND, nameFour);

    // initialize the ghost
    RoomType ghostStartingRoom = randomRoom(building.rooms); // random starting room

    printf("\n");
    printf("Ghost starts in: %s\n", ghostStartingRoom.name);

    GhostClassType ghostClass = randomClass(); // random class

    GhostType ghost;
    initGhost(&ghost, ghostClass, &ghostStartingRoom);

    // add ghost and hunters to building
    building.ghost = ghost;
    building.hunters[0] = &hunterOne;
    building.hunters[1] = &hunterTwo;
    building.hunters[2] = &hunterThree;
    building.hunters[3] = &hunterFour;

    // van hunter collection and random room ghost pointer should be updated
    ghost.room->ghostPointer = &ghost;

    // add hunters to van
    building.rooms->head->room->hunters[0] = &hunterOne;
    building.rooms->head->room->hunters[1] = &hunterTwo;
    building.rooms->head->room->hunters[2] = &hunterThree;
    building.rooms->head->room->hunters[3] = &hunterFour;

    // Use threads for hunter and ghost actions
    pthread_t hunterOneThread;
    pthread_t hunterTwoThread;
    pthread_t hunterThreeThread;
    pthread_t hunterFourThread;
    pthread_t ghostThread;

    pthread_create(&hunterOneThread, NULL, hunterAction, &hunterOne);
    pthread_create(&hunterTwoThread, NULL, hunterAction, &hunterTwo);
    pthread_create(&hunterThreeThread, NULL, hunterAction, &hunterThree);
    pthread_create(&hunterFourThread, NULL, hunterAction, &hunterFour);
    pthread_create(&ghostThread, NULL, ghostAction, &ghost);

    pthread_join(hunterOneThread, NULL);
    pthread_join(hunterTwoThread, NULL);
    pthread_join(hunterThreeThread, NULL);
    pthread_join(hunterFourThread, NULL);
    pthread_join(ghostThread, NULL);

    // keep track whether or not each hunter has got scared and left
    int hunterOneExited = 0;
    int hunterTwoExited = 0;
    int hunterThreeExited = 0;
    int hunterFourExited = 0;

    if (hunterOne.fearLevel >= 100) {
        printf(" %s", hunterOne.name);
        hunterOneExited = 1;
    }
    if (hunterTwo.fearLevel >= 100) {
        printf(" %s", hunterTwo.name);
        hunterTwoExited = 1;
    }
    if(hunterThree.fearLevel >= 100){
        printf(" %s", hunterThree.name);
        hunterThreeExited = 1;
    }
    if(hunterFour.fearLevel >= 100){
        printf(" %s", hunterFour.name);
        hunterFourExited = 1;
    }
    printf("\n");
    printf("RESULTS\n");
    printf("==============================================================\n");
    // determine the winner
    if (hunterOneExited == 1 && hunterTwoExited == 1 && hunterThreeExited == 1 && hunterFourExited == 1){
        printf("All hunters left the building. Ghost wins!\n");

    }
    else {
        printf("Hunters gathered enough evidence. Hunters win!\n");
        printf("The ghost was a ");
        printGhostClassType(&ghost.ghostClass);
    } 

    freeHunterEvidenceList(&hunterOne);
    freeHunterEvidenceList(&hunterTwo);
    freeHunterEvidenceList(&hunterThree);
    freeHunterEvidenceList(&hunterFour);

    freeBuilding(&building);

    return 0;
}


/*
  Function:  randInt
  Purpose:   returns a pseudo randomly generated number, 
             in the range min to (max - 1), inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [min, max-1) 
*/
int randInt(int min, int max)
{
    return rand() % (max - min) + min;
}

/*
  Function:  randFloat
  Purpose:   returns a pseudo randomly generated number, 
             in the range min to max, inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [0, max-1) 
*/
float randFloat(float a, float b) {
    // Get a percentage between rand() and the maximum
    float random = ((float) rand()) / (float) RAND_MAX;
    // Scale it to the range we want, and shift it
    return random * (b - a) + a;
}