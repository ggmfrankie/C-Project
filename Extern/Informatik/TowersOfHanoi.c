//
// Created by Stefan on 04.01.2026.
//

#include "TowersOfHanoi.h"

#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>

#include "../Libraries/WinAdapt.h"
#include "../../Utils/Vector.h"

#define NUMFLOORS 12
#define NUMTOWERS 3

int g_towers[NUMTOWERS][NUMFLOORS] = {};

pthread_mutex_t towerMutex = PTHREAD_MUTEX_INITIALIZER;

void VtlInit(void);
void VtlPaint(int xl, int yo, int xr, int yu);
void drawTowers(int towers[][NUMFLOORS], int xLeft, int xRight, int yTop, int yBottom);
void drawBase(Vec2i pos, int baseWidth, int poleWidth);
void drawPole(Vec2i pos, int poleWidth, int poleHeight);
void drawDisc(Vec2i pos, int discWidth, int discHeight);

int removeDisc(int towers[][NUMFLOORS], int sourceTower);
void addDisk(int towers[][NUMFLOORS], int destTower, int discSize);
void moveDisk(int towers[][NUMFLOORS], int sourceTower, int destTower);

void drawTowers(int towers[][NUMFLOORS], const int xLeft, const int xRight, const int yTop, const int yBottom) {
    const int baseWidth = (xRight - xLeft)/4;
    const int baseDistance = baseWidth/4;
    const int poleHeight = (yBottom - yTop)*8/10;
    const int discUnit = baseWidth / (NUMFLOORS + 1);
    const int poleWidth = min(baseWidth/10, discUnit/2);
    const int discHeight = poleHeight/(NUMFLOORS);

    int offsetX = baseDistance;
    const int offsetY = yBottom - baseDistance;

    for (int i = 0; i < NUMTOWERS; i++) {
        const Vec2i posBase = {
            offsetX,
            offsetY
        };
        drawBase(posBase, baseWidth, poleWidth);

        const Vec2i posPole = {
            posBase.x + (baseWidth/2)-(poleWidth/2),
            posBase.y - poleHeight
        };
        drawPole(posPole, poleWidth, poleHeight);

        const int middle = posPole.x + (poleWidth/2);
        int yPos = posBase.y - discHeight;
        for (int ii = 1; ii <= towers[i][0]; ii++) {
            const int discSize = towers[i][ii];
            if (!discSize) break;

            const Vec2i posDisc = {
                middle - (discSize * discUnit)/2,
                yPos
            };
            yPos -= discHeight;
            drawDisc(posDisc, discUnit * discSize, discHeight);
        }

        offsetX += baseWidth + baseDistance;
    }
}

inline void drawBase(const Vec2i pos, const int baseWidth, const int poleWidth) {
    FillCol(0, 0, 0);
    Rect(pos.x, pos.y, pos.x+baseWidth, pos.y+poleWidth);
}

inline void drawPole(const Vec2i pos, const int poleWidth, const int poleHeight) {
    FillCol(0, 0, 0);
    Rect(pos.x, pos.y, pos.x+poleWidth, pos.y+poleHeight);
}

inline void drawDisc(const Vec2i pos, const int discWidth, const int discHeight) {
    FillCol(44, 77, 0);
    Rect(pos.x, pos.y, pos.x+discWidth, pos.y+discHeight);
}

int moveTower2(int towers[][NUMFLOORS], const int sourceTower, const int numDiscs, const int destTower) {
    if (numDiscs == 0) return 0;
    int moves = 1;
    const int tempTower = 0x03^sourceTower^destTower;

    moves += moveTower2(towers, sourceTower, numDiscs-1, tempTower);

    pthread_mutex_lock(&towerMutex);
    moveDisk(towers, sourceTower, destTower);
    pthread_mutex_unlock(&towerMutex);
    //Sleep(50);
    moves += moveTower2(towers, tempTower, numDiscs-1, destTower);
    return moves;
}

int moveTower(int towers[][NUMFLOORS], const int sourceTower, const int numDiscs, const int destTower, const int tempTower) {
    if (numDiscs == 0) return 0;
    int moves = 1;
    moves += moveTower(towers, sourceTower, numDiscs-1, tempTower, destTower);

    pthread_mutex_lock(&towerMutex);
    moveDisk(towers, sourceTower, destTower);
    pthread_mutex_unlock(&towerMutex);
    //Sleep(50);
    moves += moveTower(towers, tempTower, numDiscs-1, destTower, sourceTower);
    return moves;
}

void moveDisk(int towers[][NUMFLOORS], const int sourceTower, const int destTower) {
    addDisk(towers, destTower, removeDisc(towers, sourceTower));
}

int removeDisc(int towers[][NUMFLOORS], const int sourceTower) {
    return towers[sourceTower][towers[sourceTower][0]--];
}

void addDisk(int towers[][NUMFLOORS], const int destTower, const int discSize) {
    towers[destTower][++towers[destTower][0]] = discSize;
}

void* runTowersAsync(void* nix) {
    const int moves = moveTower2(g_towers, 0, NUMFLOORS-1, 2);
    printf("Required moves for %i discs: %i\n", NUMFLOORS-1, moves);
    return 0;
}

void VtlZyk(void)
{

}

void VtlMouse(int X, int Y)
{

}

void VtlKeyHit(int key)
{
    runAsThread(runTowersAsync);
}

void VtlInit(void)
{
    setWindowTitle("Towers of Hanoi");
    setUpdateInterval(200);
    int i = 1;
    for (; i < NUMFLOORS; i++) {
        g_towers[0][i] = NUMFLOORS-i;
    }
    g_towers[0][0] = --i;
}

void VtlPaint(int xl, int yo, int xr, int yu)
{
    pthread_mutex_lock(&towerMutex);
    drawTowers(g_towers, xl, xr, yo, yu);
    pthread_mutex_unlock(&towerMutex);
}