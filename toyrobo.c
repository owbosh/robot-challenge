#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_X 4
#define MAX_Y 4

typedef struct {
    short x;
    short y;
} Direction;

typedef struct {
    unsigned short x;
    unsigned short y;
    Direction *dir;
} Robot;

char *dir_to_string_convert(Direction *direction) {
    if (direction->x == 1)
        return "EAST";
    if (direction->x == -1)
        return "WEST";
    if (direction->y == 1)
        return "NORTH";
    if (direction->y == -1)
        return "SOUTH";
}

Direction string_to_dir_convert(char *direction) {
    Direction converted_dir;
    if (strcmpi(direction, "EAST") == 0) {
        converted_dir.x = 1;
        converted_dir.y = 0;
    }
    else if (strcmpi(direction, "WEST") == 0) {
        converted_dir.x = -1;
        converted_dir.y = 0;
    }
    else if (strcmpi(direction, "NORTH") == 0) {
        converted_dir.x = 0;
        converted_dir.y = 1;
    }
    else if (strcmpi(direction, "SOUTH") == 0) {
        converted_dir.x = 0;
        converted_dir.y = -1;
    }
    return converted_dir;
}

bool valid_place(short x, short y) {
    if (x >= 0 && x <= MAX_X && y >= 0 && y <= MAX_Y)
        return true;
    else {
        printf("This place (%d, %d) is out of bounds!\n", x, y);
        return false;
    }
}

void place(Robot *robot, short new_x, short new_y, Direction new_dir) {
    if (valid_place(new_x, new_y)) {
        robot->x = new_x;
        robot->y = new_y;
        robot->dir->x = new_dir.x;
        robot->dir->y = new_dir.y;
    }
}

void move(Robot *robot) {
    // do nothing if robot hasn't been placed
    if (robot->dir == NULL) {
        printf("No robot has been placed yet!\n");
        return;
    }
    if (valid_place(robot->x + robot->dir->x, robot->y + robot->dir->y)) {
        robot->x += robot->dir->x;
        robot->y += robot->dir->y;
    }
}

void rotate(Robot *robot, short rotation) {
    // do nothing if robot hasn't been placed
    if (robot->dir == NULL) {
        printf("No robot has been placed yet!\n");
        return;
    }
    // 'rotation' must either be 1 or -1,
    // 1 for clockwise rotation, -1 for counter-clockwise
    if (robot->dir->x == 1) {
        robot->dir->x = 0;
        robot->dir->y = -rotation;
    }
    else if (robot->dir->x == -1) {
        robot->dir->x = 0;
        robot->dir->y = rotation;
    }
    else if (robot->dir->y == 1) {
        robot->dir->y = 0;
        robot->dir->x = rotation;
    }
    else if (robot->dir->y == -1) {
        robot->dir->y = 0;
        robot->dir->x = -rotation;
    }
}

void report(Robot *robot) {
    // do nothing if robot hasn't been placed
    if (robot->dir == NULL) {
        printf("No robot has been placed yet!\n");
        return;
    }
    printf("%d,%d,%s\n", robot->x, robot->y, dir_to_string_convert(robot->dir));
}

int main () {
    // set up commands text file
    char *filename = "commands.txt";
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Couldn't find file \"%s\", make sure the filename is spelt correctly \
        and is in the same working directory as this binary.", filename);
        return 1;
    }
    // init robot
    Robot robot;
    // each line from the commands text file is read into here
    char line_buffer[16];
    while (fgets(line_buffer, 16, file)) {
        // remove trailing newline characters
        line_buffer[strcspn(line_buffer, "\n")] = 0;
        // check first word of line_buffer for start of PLACE command
        char *token = strtok(line_buffer, " ");
        if (strcmpi(token, "PLACE") == 0) {
            // seperate each of the command arguments into an array for reading
            char command[3][6];
            for (short i = 0; i <= 3; i++) {
                strcpy(command[i], token);
                token = strtok(NULL, ",");
            }
            Direction new_dir = string_to_dir_convert(command[3]);
            // with the Direction being initialized, it indicates that the robot has been placed
            if (robot.dir == NULL)
                robot.dir = malloc(sizeof(Direction));
            // finally call the place function with all the parsed info
            place(&robot, strtol(command[1], NULL, 10), strtol(command[2], NULL, 10), new_dir);
        }
        // the rest of the commands do not need as much parsing
        else if (strcmpi(line_buffer, "MOVE") == 0)
            move(&robot);
        else if (strcmpi(line_buffer, "LEFT") == 0)
            rotate(&robot, -1);
        else if (strcmpi(line_buffer, "RIGHT") == 0)
            rotate(&robot, 1);
        else if (strcmpi(line_buffer, "REPORT") == 0)
            report(&robot);
    }
    // prepare to exit by closing opened text file and freeing allocated memory
    fclose(file);
    free(robot.dir);
    return 0;
}