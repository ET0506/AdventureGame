#include "lab.h"

int main()
{
    int processCommand(adventureGame *ag);
    adventureGame ag ={ // Allocate memory a struct of type adventureGame
        .playerHP=12, // Initialize player HP be 12
        .playerXP=0, // Initialize player XP be 0
        .room=0, // Initialize player to start in room 0

         // Rooms Name
        .rooms = {"Main Hall", "Foyer", "Bedroom", "Dining Room",
                  "Kitchen", "Bathroom", "Pantry", "Laundry Room",
                  "Basement", "Courtyard", "Library", "Game Room",
                  "Study Room"},

        // Descriptions for each room
        .lookArray={// Put the description for the look in each room here.
            "You are in a Main Hall. There is a Foyer to the east, and a Bathroom lies to the south.",
            "You are in a Foyer. There is a Main Hall to the west, and a Bedroom lies to the east.",
            "You are in a Bedroom. There is a Foyer to the west, a Dining Room to the east, and a Laundry Room lies to the south.",
            "You are in a Dining Room. There is a Bedroom to the west, a Basement to the south, and a Kitchen lies to the east.",
            "You are in a Kitchen. There is a Dining Room to the west, and a Courtyard lies to the south.",
            "You are in a Bathroom. There is only a Main hall lies to the north.",
            "You are in a Pantry. There is a Laundry Room to the east, and a Library lies to the south.",
            "You are in a Laundry Room. There is a Basement to the east, a Game Room to the south, a Pantry to the west, and a Bedroom lies to the north.",
            "You are in a Basement. There is a Dining room to the north, and a Laundry Room lies to the west.",
            "You are in a Courtyard. There is a only a Kitchen lies to the north.",
            "You are in a Library. There is a Pantry to the north, and a Game Room lies to the east.",
            "You are in a Game Room. There is a Laundry Room to the north, a Library to the west, and a Study Room lies to the east.",
            "You are in a Study Room. There is only a Game Room to the west.",
        },

        // Movement options for each room
        .moveArray={
            {"You cannot go north.","You go south to the Bathroom.","You go east to the Foyer.","You cannot go to west."},
            {"You cannot go north.","You cannot go south.","You go east to the Bedroom.","You go west to the Main Hall."},
            {"You cannot go north.","You go south to the Laundry Room.","You go east to the Dining Room.","You go west to the Foyer."},
            {"You cannot go north.","You go south to the Basement.","You go east to the Kitchen.","You go west to the Bedroom."},
            {"You cannot go north.","You go south to the Courtyard.","You cannot go east.","You go west to the Dining Room."},
            {"You go north to the Main Hall.","You cannot go south.","You cannot go east.","You cannot go west."},
            {"You cannot go north.","You go south to the Library.","You go east to the Laundry Room.","You cannot go west."},
            {"You go north to the Bedrooom.","You go south to the Game Room.","You go east to the Basement.","You go west to the Pantry."},
            {"You go north to the Dining Room.","You cannot go south.","You cannot go east.","You go west to the Laundry Room."},
            {"You go north to the Kitchen.","You cannot go south.","You cannot go east.","You cannot go west."},
            {"You go north to the Pantry.","You cannot go south.","You go east to the Game Room.","You cannot go west."},
            {"You go north to the Laundry Room.","You cannot go south.","You go east to the Study Room.","You go west to the Library."},
            {"You cannot go north.","You cannot go south.","You go cannot go east.","You go west to the Game Room."},
        },

        // Array indicating room connections (North, South, East, West)
        .moveToArray={ // -1 indicate player is not able to move in this direction
            {-1,5,1,-1},    // Room 0
            {-1,-1,2,0},    // Room 1
            {-1,-1,-1,1},   // Room 2
            {-1,8,4,2},     // Room 3
            {-1,-1,-1,3},   // Room 4
            {0,-1,-1,-1},   // Room 5
            {-1,10,7,-1},   // Room 6
            {2,11,8,6},     // Room 7
            {3,-1,-1,7},    // Room 8
            {4,-1,-1,-1},   // Room 9
            {6,-1,11,-1},   // Room 10
            {7,-1,-1,10},   // Room 11
            {-1,-1,-1,11},  // Room 12

        },

        //Monster Array
        .N_monster=7,
        .monsterArray={"Zombie", "Werewolf", "Monster", "Dracula", "Lizard-man", "Bride of Dracula", "Dragon"},
        .monsterStatusArray={5,12,4,12,5,7,12},
        .monsterLoc={2,4,5,6,9,10,12},
        .monsterXP={35,50,35,50,35,45,50},

        //Objects or Artifacts array
        .N_artifact=9,
        .artifact={"Food", "Medpack", "Sword", "Copper Key", "Bronze Key", "Elixir", "Diamond Cross","Silver Spear", "Golden Chalice"},
        .artifactLoc={1,3,5,6,7,8,9,11,12},
        .artifactXP={5,5,10,15,5,5,20,10,25},
        .artifactXPScore={5,5,10,15,5,5,20,10,25},


    };

    // Print welcome message
    printf("Welcome to adventure game.\n");
    printf("To win this game, you need to get the Golden Chalice and return to this room with it.\n");
    printf("Enter commands to explore the game world. Type 'help' for a list of commands.\n");


     // Game loop: Read a command-line from the user and process it until the user decides to exit
        while (fgets(ag.command,1000,stdin)) {
            if (processCommand(&ag)==0) break;
        }

    return 0;
}
