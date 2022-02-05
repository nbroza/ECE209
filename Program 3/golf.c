// ECE 209 - Fall 2021 - Program 3
// This program reads information about a golf tournament
// and allows a user to view information about the course,
// the overall scores, and the individual players.
//
// Implementation of Course ADT and functions for Player and PlayerNode types
//
// Nick Broza

/* whoever is reading this please read me:
 * this code is terrible, i know. i am sorry that you have to read through this.
 * i made my own functions to get parts working and then used them in weird ways to get
 * other functions working. its terrible and i hate it. i no longer possess the brain cells to fix
 * this any more though so this is what i am submitting. i truly am sorry, please forgive me. it does
 * work though, albeit terribly inefficiently. this code should not work however somehow it does.
 */

#include "golf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

PlayerNode * newNode(const char * name, const char * strokes, int hole, Course course);
PlayerNode * existingNode(Course course, const char * name);

struct golfCourse{
    char name[41];
    int par[18];
    PlayerNode * head;
};

//Course Functions
//begin the actually decent section of code
Course readCourse(const char * filename) {
    struct golfCourse * newCourse = malloc(sizeof(struct golfCourse));
    newCourse->head  = NULL;

    FILE * fp;
    fp = fopen(filename, "r");

    if (fp == NULL) {
        return(NULL);
    }

    fgets(newCourse->name, 41, fp);
    strtok(newCourse->name, "\n");

    int n;
    int c = 0;
    while (fscanf(fp, "%d", &n) == 1) {
        newCourse->par[c] = n;
        c++;
    }

    char input[30];
    while (fscanf(fp, "%s", input) != EOF) {
        const char * name = strtok(input, ":");
        int hole = atoi(strtok(NULL, ":"));
        const char * strokes = strtok(NULL, ":");

        if (newCourse->head == NULL) {
            newCourse->head = newNode(name, strokes, hole, newCourse);
        } else {
            if (existingNode(newCourse, name) != NULL) {
                PlayerNode * node = malloc(sizeof(PlayerNode));
                node = existingNode(newCourse, name);
                node->player->strokes[hole-1] = malloc(sizeof(strokes));
                strcpy(node->player->strokes[hole-1], strokes);
            } else {
                PlayerNode * node = newNode(name, strokes, hole, newCourse);
                node->next = newCourse->head;
                newCourse->head = node;
            }
        }
    }
    return newCourse;
}

PlayerNode * newNode(const char * name, const char * strokes, int hole, Course course) {
    Player * player = malloc(sizeof(Player));
    player->strokes[hole-1] = malloc(sizeof(strokes));
    player->course = course;

    strcpy(player->name, name);
    strcpy(player->strokes[hole-1], strokes);

    PlayerNode * node = malloc(sizeof(PlayerNode));
    node->player = player;
    node->next = NULL;

    return node;
}

PlayerNode * existingNode(Course course, const char * name) {
    PlayerNode * tmpHead = course->head;
    while (tmpHead != NULL) {
        if (strcmp(tmpHead->player->name, name) == 0) {
            return tmpHead;
        }
        tmpHead = tmpHead->next;
    }
    return NULL;
}
//end the actually decent section of this code

//begin terrible workarounds
const char * courseName(Course c) {
    return c->name;
}

const int * courseHoles(Course c) {
    return c->par;
}

const PlayerNode * coursePlayers(Course c) {
    return c->head;
}

PlayerNode * courseLeaders(Course c, int n) {
    PlayerNode * topHead = malloc(sizeof(PlayerNode));
    PlayerNode * currentNode = malloc(sizeof(PlayerNode));

    int minscore = INT_MAX;
    int maxscore = INT_MIN;
    for (int i = 0; i < n; i++) {
        PlayerNode * tmpHead = c->head;
        while (tmpHead != NULL) {
            int score = totalStrokeScore(tmpHead->player);
            if (score < minscore) {
                minscore = score;
            } else if (score > maxscore) {
                maxscore = score;
            }
            tmpHead = tmpHead->next;
        }

        int count = 0;
        int numfound = 0;
        int found = 0;
        for (int i = minscore; i <= maxscore && count < n ; i++) {
            PlayerNode * tmpHead = c->head;
            while (tmpHead != NULL) {
                int score = totalStrokeScore(tmpHead->player);
                if (i == score) {
                    found = 1;
                    if (count == 0) {
                        count++;
                        memcpy(topHead, tmpHead, sizeof(PlayerNode));
                        topHead->next = NULL;
                    } else if (count == 1) {
                        count++;
                        memcpy(currentNode, tmpHead, sizeof(PlayerNode));
                        currentNode->next = NULL;
                        topHead->next = currentNode;
                    } else {
                        count++;
                        PlayerNode * newNode= malloc(sizeof(PlayerNode));
                        memcpy(newNode, tmpHead, sizeof(PlayerNode));
                        currentNode->next = newNode;
                        currentNode = newNode;
                        currentNode->next = NULL;
                    }
                }
                tmpHead = tmpHead->next;
            }
            if (found == 1) {
                numfound++;
                found = 0;
            }
        }
    }
    return topHead;
}

int numPlayers(Course c) {
    int acc = 0;
    PlayerNode * tmpHead = c->head;
    while (tmpHead != NULL) {
        acc++;
        tmpHead = tmpHead->next;
    }
    return acc;
}

Player * findPlayer(const char * name, Course c) {
    if (existingNode(c, name) == NULL) {
        newPlayer(name, c);
        return NULL;
    }
    return existingNode(c, name)->player;
}

void addPlayer(Player * p, Course c) {
    PlayerNode * node = malloc(sizeof(PlayerNode));
    node->player = p;
    node->next = c->head;
    c->head = node;

    for (int i = 0; i < 18; i++) {
        node->player->strokeScore[i] = 0;
        node->player->parScore[i] = 0;
        node->player->strokes[i] = 0;
    }
}

double avgTotalScore(Course c) {
    int acc = 0;
    PlayerNode * tmpHead = c->head;
    while (tmpHead != NULL) {
        acc += totalStrokeScore(tmpHead->player);
        tmpHead = tmpHead->next;
    }
    return (double) acc / numPlayers(c);
}

double avgParScore(Course c) {
    int acc = 0;
    PlayerNode * tmpHead = c->head;
    while (tmpHead != NULL) {
        int totalpar =0;
        for (int i = 0; i < 18; i++) {
            totalpar += c->par[i];
        }
        acc += totalStrokeScore(tmpHead->player) - totalpar;
        tmpHead = tmpHead->next;
    }
    return (double) acc / numPlayers(c);
}

double avgHoleScore(Course c, int hole) {
    int acc = 0;
    PlayerNode * tmpHead = c->head;
    while (tmpHead != NULL) {
        acc += strlen(tmpHead->player->strokes[hole-1]);
        tmpHead = tmpHead->next;
    }
    return (double) acc / numPlayers(c);
}
//End course functions

Player * newPlayer(const char * name, Course course) {
    Player * player = malloc(sizeof(Player));
    player->course = course;
    strcpy(player->name, name);
    char * strokes[18];
    for (int i = 0; i < 18; i++) {
        strokes[i] = malloc(sizeof(strokes));
        strokes[i] = 0;
    }
    memcpy(player->strokes, strokes, sizeof(strokes));
    addPlayer(player, course);
    return player;
}

int scoreHole(Player * p, int hole, const char * strokes) {
    p->strokes[hole-1] = malloc(sizeof(strokes));
    strcpy(p->strokes[hole-1], strokes);
    p->strokeScore[hole-1] = strlen(p->strokes[hole-1]);
    p->parScore[hole-1] = p->strokeScore[hole-1] - p->course->par[hole-1];
    return p->parScore[hole-1];
}

int totalStrokeScore(const Player * p) {
    int acc = 0;
    for (int i = 0; i < 18; i++) {
        if (p->strokes[i] == 0) {
            return acc;
        }
        acc += strlen(p->strokes[i]);
    }
    return acc;
}

int totalParScore(const Player * p) {
    int totalpar = 0;
    for (int i = 0; i < 18; i++) {
        totalpar += p->parScore[i];
    }
    return totalpar;
}

int greensInReg(const Player * p) {
    int gir = 0;
    for (int i = 0; i < 18; i++) {
        int num = 0;

        if (p->strokes[i] == 0) {
            return gir;
        }

        for (int j = 0; j <= strlen(p->strokes[i]); j++) {
            num++;
            if (p->strokes[i][j] == 'g' || p->strokes[i][j] == 'h') {
                break;
            }

        }
        if (p->course->par[i] - 2 >= num) {
            gir++;
        }
    }
    return gir;
}

void fairwaysHit(const Player * p, int * hit, int * holes) {
    int num = 0;
    int den = 0;
    for (int i = 0; i < 18; i++) {
        if (p->strokes[i] == 0) {
            break;
        }
        if (p->course->par[i] >= 4) {
            den++;
            if (p->strokes[i][0] == 'f'){
                num++;
            }
        }
    }
    *hit = num;
    *holes = den;
}

int countScores(const Player * p, int parScore) {
    int num = 0;
    for (int i = 0; i < 18; i++) {
        if (p->strokes[i] == 0) {
            return num;
        }
        if (strlen(p->strokes[i]) - p->course->par[i] == parScore) {
            num++;
        }
    }
    return num;
}
