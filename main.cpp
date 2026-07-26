#include <raylib.h>
#include <iostream>

using namespace std;

enum Block {BarBlock, BoxBlock, TBlock, LBlock, JBlock, ZBlock, SBlock};
enum Orientation {Up, Right, Down, Left};

const int BOARD_WIDTH = 300;
const int BOARD_HEIGHT = 600;
const int ROWS = 20;
const int COLS = 10;
const int CELL_WIDTH = BOARD_WIDTH / COLS;
const int CELL_HEIGHT = BOARD_HEIGHT / ROWS;

const int INFO_AREA_WIDTH = 250;

const Color WINDOW_BG_COLOR = WHITE;
const Color GRID_LINE_COLOR = LIGHTGRAY;
const Color LOCKED_CELL_COLOR = DARKGRAY;

struct ActiveBlock {
    Block block;
    Orientation orientation;
    int x;
    int y;
    Color color;
};

int cellInfo[ROWS][COLS];
int score = 0;
bool isGameOver = false;

void init();
void initCells();

void spawnBlock(ActiveBlock &activeBlock);
int findMiddle(Block block, Orientation Orientation);

Block chooseRandomBlock();
Orientation chooseRandomOrientation();

void drawGrid();

void drawInfoArea();
void clearFullRows();
bool isGameOverCondition(ActiveBlock activeBlock);
void showGameOver();

void playGame(ActiveBlock &activeBlock);

void drawActiveBlock(ActiveBlock activeBlock);
void drawBarBlock(ActiveBlock activeBlock); 
void drawBoxBlock(ActiveBlock activeBlock); 
void drawTBlock(ActiveBlock activeBlock); 
void drawLBlock(ActiveBlock activeBlock); 
void drawJBlock(ActiveBlock activeBlock); 
void drawZBlock(ActiveBlock activeBlock); 
void drawSBlock(ActiveBlock activeBlock); 

bool canBlockGoDown(ActiveBlock activeBlock);
bool canBarGoDown(ActiveBlock activeBlock);
bool canBoxGoDown(ActiveBlock activeBlock);
bool canTGoDown(ActiveBlock activeBlock);
bool canLGoDown(ActiveBlock activeBlock);
bool canJGoDown(ActiveBlock activeBlock);
bool canZGoDown(ActiveBlock activeBlock);
bool canSGoDown(ActiveBlock activeBlock);

void lockActiveBlock(ActiveBlock activeBlock);
void lockBarBlock(ActiveBlock activeBlock);
void lockBoxBlock(ActiveBlock activeBlock);
void lockTBlock(ActiveBlock activeBlock);
void lockLBlock(ActiveBlock activeBlock);
void lockJBlock(ActiveBlock activeBlock);
void lockZBlock(ActiveBlock activeBlock);
void lockSBlock(ActiveBlock activeBlock);
void drawLockedCells();

bool canBlockGoLeft(ActiveBlock activeBlock);
bool canBarGoLeft(ActiveBlock activeBlock);
bool canBoxGoLeft(ActiveBlock activeBlock);
bool canTGoLeft(ActiveBlock activeBlock);
bool canLGoLeft(ActiveBlock activeBlock);
bool canJGoLeft(ActiveBlock activeBlock);
bool canZGoLeft(ActiveBlock activeBlock);
bool canSGoLeft(ActiveBlock activeBlock);

bool canBlockGoRight(ActiveBlock activeBlock);
bool canBarGoRight(ActiveBlock activeBlock);
bool canBoxGoRight(ActiveBlock activeBlock);
bool canTGoRight(ActiveBlock activeBlock);
bool canLGoRight(ActiveBlock activeBlock);
bool canJGoRight(ActiveBlock activeBlock);
bool canZGoRight(ActiveBlock activeBlock);
bool canSGoRight(ActiveBlock activeBlock);

bool canBlockRotate(ActiveBlock activeBlock);
bool canBarRotate(ActiveBlock activeBlock);
bool canTRotate(ActiveBlock activeBlock);
bool canLRotate(ActiveBlock activeBlock);
bool canJRotate(ActiveBlock activeBlock);
bool canZRotate(ActiveBlock activeBlock);
bool canSRotate(ActiveBlock activeBlock);
void rotateBlock(ActiveBlock &activeBlock);

int main(){
    init();
    initCells();
    ActiveBlock activeBlock;
    spawnBlock(activeBlock);
    while(!WindowShouldClose()){
        BeginDrawing();
            ClearBackground(WINDOW_BG_COLOR);
            drawGrid();
            if(isGameOver){
                showGameOver();
            } else {
                playGame(activeBlock);
                drawInfoArea();
                clearFullRows();
                if(isGameOverCondition(activeBlock)){
                    isGameOver = true;
                }
            }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

void init(){
    InitWindow(BOARD_WIDTH + INFO_AREA_WIDTH, BOARD_HEIGHT, "TETRIS");
}

void initCells(){
    for(int row = 0; row < ROWS; row ++){
        for(int col = 0; col < COLS; col ++){
            cellInfo[row][col] = 0;
        }
    }
}

void spawnBlock(ActiveBlock &activeBlock){
    activeBlock.block = chooseRandomBlock();
    activeBlock.orientation = chooseRandomOrientation();
    activeBlock.x = findMiddle(activeBlock.block, activeBlock.orientation);
    activeBlock.y = 0;
    activeBlock.color = GetRandomValue(0,1) == 0 ? RED : MAGENTA;
}

int findMiddle(Block block, Orientation orientation){
    switch (block){
        case BarBlock:
            if(orientation == Up || orientation == Down){
                return COLS/2;
            }
            else{
                return (COLS/2) - 2;
            }
            break;
        case BoxBlock:case TBlock:
        case JBlock:case LBlock:
        case ZBlock:case SBlock:
            return (COLS/2) - 1;
            break;
    }
    return COLS/2;
}

Block chooseRandomBlock(){
    return Block(GetRandomValue(BarBlock,SBlock));
}

Orientation chooseRandomOrientation(){
    return Orientation(GetRandomValue(Up,Left));
}

void drawGrid(){
    int verticalLineStart = 0;
    int horizontalLineStart = 0;

    for(int row = 0; row <= ROWS; row ++){
        DrawLine(horizontalLineStart, row * CELL_HEIGHT, BOARD_WIDTH, row * CELL_HEIGHT, GRID_LINE_COLOR);
    }

    for(int col = 0; col <= COLS; col ++){
        DrawLine(col * CELL_WIDTH, verticalLineStart, col * CELL_WIDTH, BOARD_HEIGHT, GRID_LINE_COLOR);
    }
}

void drawInfoArea()
{

    DrawRectangle(BOARD_WIDTH,0,INFO_AREA_WIDTH,BOARD_HEIGHT,BLACK);

    int center = BOARD_WIDTH + INFO_AREA_WIDTH/2;



    DrawText("TETRIS",center - MeasureText("TETRIS",36)/2,60,36,RAYWHITE);

    DrawRectangle(center - MeasureText("Score",20)/2,170,MeasureText("Score",20),20,WHITE);

    DrawText("Score",center - MeasureText("Score",20)/2,170,20,GRAY);

    const char* scoreText =TextFormat("%06d", score);

    DrawText(scoreText,center - MeasureText(scoreText,40)/2,210,40,WHITE);

    DrawText("Left OR Right : Move",BOARD_WIDTH + 5,BOARD_HEIGHT - 120,18,GRAY);

    DrawText(" UP : Rotate",BOARD_WIDTH + 1,BOARD_HEIGHT - 90,18,GRAY);

    DrawText("SPACE : Drop",BOARD_WIDTH + 5,BOARD_HEIGHT - 60,18,GRAY);
}

void clearFullRows(){
    int rowsRemoved = 0;

    for(int row = ROWS - 1; row >= 0; row--){
        bool full = true;
        for(int col = 0; col < COLS; col++){
            if(cellInfo[row][col] == 0){ 
                full = false;
                break; }
        }
        if(full){
            for(int r = row; r > 0; r--){
                for(int col = 0; col < COLS; col++){
                    cellInfo[r][col] = cellInfo[r-1][col];
                }
            }
            rowsRemoved++;
            row++; 
        }
    }
    
    score += rowsRemoved * 100;
    
}

bool isGameOverCondition(ActiveBlock activeBlock){
    for(int col = 0; col < COLS; col++){
        if(cellInfo[0][col] == 1) return true;
    }
    return false;
}

void showGameOver()
{
    DrawRectangle(0, 0,BOARD_WIDTH + INFO_AREA_WIDTH,BOARD_HEIGHT,{0,0,0,200});

    int center = (BOARD_WIDTH + INFO_AREA_WIDTH)/2;

    DrawText("GAME OVER",center - MeasureText("GAME OVER",40)/2,BOARD_HEIGHT/2 - 70,40,WHITE);

    DrawText("FINAL SCORE",center - MeasureText("FINAL SCORE",18)/2,BOARD_HEIGHT/2 - 10,18,GRAY);

    const char* scoreText = TextFormat("%06d", score);

    DrawText(scoreText,center - MeasureText(scoreText,42)/2,BOARD_HEIGHT/2 + 20,42,WHITE);

    DrawText("Press ENTER",center - MeasureText("Press ENTER",20)/2,BOARD_HEIGHT/2 + 80,20,{150,150,150,255});

    if(IsKeyPressed(KEY_ENTER))
    {
        score = 0;
        isGameOver = false;

        for(int r=0;r<ROWS;r++)
            for(int c=0;c<COLS;c++)
                cellInfo[r][c]=0;
    }
}

void playGame(ActiveBlock &activeBlock){
    drawActiveBlock(activeBlock);
    drawLockedCells();
    float gameSpeed = 0.4;
    static float fallDelay = 0;
    fallDelay += GetFrameTime();

    if(fallDelay > gameSpeed){
        if(canBlockGoDown(activeBlock)){
            activeBlock.y++;
            fallDelay = 0;
        }
        else {
            lockActiveBlock(activeBlock);
            spawnBlock(activeBlock);
        }
    }

    if(IsKeyPressed(KEY_LEFT)){
        if(canBlockGoLeft(activeBlock))
            activeBlock.x--;
    }

    if(IsKeyPressed(KEY_RIGHT)){
        if(canBlockGoRight(activeBlock))
            activeBlock.x++;
    }

    if(IsKeyPressed(KEY_UP)){
        if(canBlockRotate(activeBlock)){
            rotateBlock(activeBlock);
        }
    }

    if(IsKeyPressed(KEY_DOWN)){
        if(canBlockGoDown(activeBlock))
            activeBlock.y++;
    }

    if(IsKeyPressed(KEY_SPACE)){
        while (canBlockGoDown(activeBlock))
        {
            activeBlock.y++;
        }
    }
}

void drawActiveBlock(ActiveBlock activeBlock){
    switch(activeBlock.block){
        case BarBlock: drawBarBlock(activeBlock); break;
        case BoxBlock:drawBoxBlock(activeBlock); break;
        case TBlock: drawTBlock(activeBlock); break;
        case LBlock: drawLBlock(activeBlock); break;
        case JBlock:drawJBlock(activeBlock); break;
        case ZBlock: drawZBlock(activeBlock); break;
        case SBlock: drawSBlock(activeBlock); break;
    }
}

void drawBarBlock(ActiveBlock activeBlock){
    int barCells = 4;

    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        for(int y = 0; y < barCells; y ++){
            DrawRectangle((activeBlock.x) * CELL_WIDTH, (activeBlock.y + y) * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT, activeBlock.color);
        }
    }
    else {
        for(int x = 0; x < barCells; x ++){
            DrawRectangle((activeBlock.x + x) * CELL_WIDTH, (activeBlock.y) * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT, activeBlock.color);
        }
    }
}
void drawBoxBlock(ActiveBlock activeBlock){
    int barCells = 2;

    for (int y = 0; y < barCells; y++) {
        for (int x = 0; x < barCells; x++) {
            DrawRectangle((activeBlock.x + x) * CELL_WIDTH,(activeBlock.y + y) * CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
        }
    }
}
void drawSBlock(ActiveBlock activeBlock){
    int barCells = 3;
    int barCells2 = 2;

    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        for (int y = 0; y < barCells2; y++) {
            for (int x = 0; x < barCells; x++) {
                if((x==0&&y==0)||(x==2&&y==1)){
                    continue;
                }      
                else{
                    DrawRectangle((activeBlock.x + x) * CELL_WIDTH,(activeBlock.y + y) * CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
                }
            }
        }
    }
    else {
        for (int y = 0; y < barCells; y++) {
            for (int x = 0; x < barCells2; x++) {
                if((x==0&&y==0)||(x==1&&y==2)){
                    continue;
                }      
                else{
                    DrawRectangle((activeBlock.x + x) * CELL_WIDTH,(activeBlock.y + y) * CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
                }
            }
        }
    }
}
void drawZBlock(ActiveBlock activeBlock){
    int barCells = 3;
    int barCells2 = 2;

    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        for (int y = 0; y < barCells2; y++) {
            for (int x = 0; x < barCells; x++) {
                if((x==2&&y==0)||(x==0&&y==1)){
                    continue;
                }      
                else{
                    DrawRectangle((activeBlock.x + x) * CELL_WIDTH,(activeBlock.y + y) * CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
                }
            }
        }
    }
    else {
        for (int y = 0; y < barCells; y++) {
            for (int x = 0; x < barCells2; x++) {
                if((x==0&&y==0)||(x==1&&y==2)){
                    continue;
                }      
                else{
                    DrawRectangle((activeBlock.x + x) * CELL_WIDTH,(activeBlock.y + y) * CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
                }
            }
        }
    }
}
void drawLBlock(ActiveBlock activeBlock){
    int barCells = 3;
    int barCells2 = 2;

    if(activeBlock.orientation == Up ){
        for (int y = 0; y < barCells; y++) {
            for (int x = 0; x < barCells2; x++) {
                if((x==1&&y==0)||(x==1&&y==1)){
                    continue;
                }      
                else{
                    DrawRectangle((activeBlock.x + x) * CELL_WIDTH,(activeBlock.y + y) * CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
                }
            }
        }
    }
    else if(activeBlock.orientation == Down ){
        for (int y = 0; y < barCells; y++) {
            for (int x = 0; x < barCells2; x++) {
                if((x==0&&y==1)||(x==0&&y==2)){
                    continue;
                }      
                else{
                    DrawRectangle((activeBlock.x + x) * CELL_WIDTH,(activeBlock.y + y) * CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
                }
            }
        }
    }
    else if(activeBlock.orientation == Right ){
        for (int y = 0; y < barCells2; y++) {
            for (int x = 0; x < barCells; x++) {
                if((x==1&&y==1)||(x==2&&y==1)){
                    continue;
                }      
                else{
                    DrawRectangle((activeBlock.x + x) * CELL_WIDTH,(activeBlock.y + y) * CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
                }
            }
        }
    }
    else {
        for (int y = 0; y < barCells2; y++) {
            for (int x = 0; x < barCells; x++) {
                if((x==0&&y==0)||(x==1&&y==0)){
                    continue;
                }      
                else{
                    DrawRectangle((activeBlock.x + x) * CELL_WIDTH,(activeBlock.y + y) * CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
                }
            }
        }
    }
}
void drawJBlock(ActiveBlock activeBlock){
    int barCells = 3;
    int barCells2 = 2;

    if(activeBlock.orientation == Up ){
        for (int y = 0; y < barCells; y++) {
            for (int x = 0; x < barCells2; x++) {
                if((x==0&&y==0)||(x==0&&y==1)){
                    continue;
                }      
                else{
                    DrawRectangle((activeBlock.x + x) * CELL_WIDTH,(activeBlock.y + y) * CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
                }
            }
        }
    }
    else if(activeBlock.orientation == Down ){
        for (int y = 0; y < barCells; y++) {
            for (int x = 0; x < barCells2; x++) {
                if((x==1&&y==1)||(x==1&&y==2)){
                    continue;
                }      
                else{
                    DrawRectangle((activeBlock.x + x) * CELL_WIDTH,(activeBlock.y + y) * CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
                }
            }
        }
    }
    else if(activeBlock.orientation == Right ){
        for (int y = 0; y < barCells2; y++) {
            for (int x = 0; x < barCells; x++) {
                if((x==1&&y==0)||(x==2&&y==0)){
                    continue;
                }      
                else{
                    DrawRectangle((activeBlock.x + x) * CELL_WIDTH,(activeBlock.y + y) * CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
                }
            }
        }
    }
    else {
        for (int y = 0; y < barCells2; y++) {
            for (int x = 0; x < barCells; x++) {
                if((x==0&&y==1)||(x==1&&y==1)){
                    continue;
                }      
                else{
                    DrawRectangle((activeBlock.x + x) * CELL_WIDTH,(activeBlock.y + y) * CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
                }
            }
        }
    }
}
void drawTBlock(ActiveBlock activeBlock){
    int barCells = 3;
    int barCells2 = 2;

    if(activeBlock.orientation == Up ){
        for (int y = 0; y < barCells2; y++) {
            for (int x = 0; x < barCells; x++) {
                if((x==0&&y==1)||(x==2&&y==1)){
                    continue;
                }      
                else{
                    DrawRectangle((activeBlock.x + x) * CELL_WIDTH,(activeBlock.y + y) * CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
                }
            }
        }
    }
    else if(activeBlock.orientation == Down ){
        for (int y = 0; y < barCells2; y++) {
            for (int x = 0; x < barCells; x++) {
                if((x==0&&y==0)||(x==2&&y==0)){
                    continue;
                }      
                else{
                    DrawRectangle((activeBlock.x + x) * CELL_WIDTH,(activeBlock.y + y) * CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
                }
            }
        }
    }
    else if(activeBlock.orientation == Right ){
        for (int y = 0; y < barCells; y++) {
            for (int x = 0; x < barCells2; x++) {
                if((x==0&&y==0)||(x==0&&y==2)){
                    continue;
                }      
                else{
                    DrawRectangle((activeBlock.x + x) * CELL_WIDTH,(activeBlock.y + y) * CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
                }
            }
        }
    }
    else {
        for (int y = 0; y < barCells; y++) {
            for (int x = 0; x < barCells2; x++) {
                if((x==1&&y==0)||(x==1&&y==2)){
                    continue;
                }      
                else{
                    DrawRectangle((activeBlock.x + x) * CELL_WIDTH,(activeBlock.y + y) * CELL_HEIGHT,CELL_WIDTH,CELL_HEIGHT,activeBlock.color);
                }
            }
        }
    }
}
bool canBlockGoDown(ActiveBlock activeBlock){
    switch (activeBlock.block){
        case BarBlock: return canBarGoDown(activeBlock);
        break;
        case BoxBlock: return canBoxGoDown(activeBlock);
        break;
        case TBlock: return canTGoDown(activeBlock);
        break;
        case LBlock: return canLGoDown(activeBlock);
        break;
        case JBlock: return canJGoDown(activeBlock);
        break;
        case ZBlock: return canZGoDown(activeBlock);
        break;
        case SBlock: return canSGoDown(activeBlock);
        break;
    }
    return false;
}

bool canBarGoDown(ActiveBlock activeBlock){
    int barCells = 4;
    if (activeBlock.orientation == Up || activeBlock.orientation == Down){
        if(activeBlock.y + barCells >= ROWS) return false;
        if(cellInfo[activeBlock.y + barCells][activeBlock.x] == 1) return false;
    }
    else {
        if(activeBlock.y + 1 >= ROWS) return false;
        for(int x = 0; x < barCells; x ++){
            if(cellInfo[activeBlock.y + 1][activeBlock.x + x] == 1) return false;
        }
    }
    return true;
}
bool canBoxGoDown(ActiveBlock activeBlock) {
    int barCells = 2;
    if (activeBlock.y + barCells >= ROWS) return false;
    if (cellInfo[activeBlock.y + barCells][activeBlock.x] == 1 || 
        cellInfo[activeBlock.y + barCells][activeBlock.x + 1] == 1) return false;
    return true;
}

bool canTGoDown(ActiveBlock activeBlock) {
    int barCells = 3;
    int barCells2 = 2;
    if (activeBlock.orientation == Up) {
        if (activeBlock.y + barCells2 >= ROWS) return false;
        if (cellInfo[activeBlock.y + barCells-barCells2][activeBlock.x] == 1 || 
            cellInfo[activeBlock.y + barCells2][activeBlock.x + (barCells-barCells2)] == 1 || 
            cellInfo[activeBlock.y + (barCells-barCells2)][activeBlock.x + barCells2] == 1) return false;
    } 
    else if (activeBlock.orientation == Down) {
        if (activeBlock.y + barCells2 >= ROWS) return false;
        for (int x = 0; x < barCells; x++) {
            if (cellInfo[activeBlock.y + barCells2][activeBlock.x + x] == 1) return false;
        }
    } 
    else if (activeBlock.orientation == Right) {
        if (activeBlock.y + barCells >= ROWS) return false;
        if (cellInfo[activeBlock.y + barCells2][activeBlock.x] == 1 || 
            cellInfo[activeBlock.y + barCells][activeBlock.x + barCells-barCells2] == 1) return false;
    } 
    else { 
        if (activeBlock.y + barCells >= ROWS) return false;
        if (cellInfo[activeBlock.y + barCells][activeBlock.x] == 1 || 
            cellInfo[activeBlock.y + barCells2][activeBlock.x + barCells-barCells2] == 1) return false;
    }
    return true;
}

bool canLGoDown(ActiveBlock activeBlock) {
    int barCells = 3;
    int barCells2 = 2;
    if (activeBlock.orientation == Up) {
        if (activeBlock.y + barCells >= ROWS) return false;
        if (cellInfo[activeBlock.y + barCells][activeBlock.x] == 1 || 
            cellInfo[activeBlock.y + barCells][activeBlock.x + barCells-barCells2] == 1) return false;
    } 
    else if (activeBlock.orientation == Down) {
        if (activeBlock.y + barCells >= ROWS) return false;
        if (cellInfo[activeBlock.y + barCells-barCells2][activeBlock.x] == 1 || 
            cellInfo[activeBlock.y + barCells][activeBlock.x + barCells-barCells2] == 1) return false;
    } 
    else if (activeBlock.orientation == Right) {
        if (activeBlock.y + barCells2 >= ROWS) return false;
        if (cellInfo[activeBlock.y + barCells2][activeBlock.x] == 1 || 
            cellInfo[activeBlock.y + barCells-barCells2][activeBlock.x + barCells-barCells2] == 1 ||  
            cellInfo[activeBlock.y + barCells-barCells2][activeBlock.x + barCells2] == 1) return false;
    } 
    else { 
        if (activeBlock.y + barCells2 >= ROWS) return false;
        for (int x = 0; x < barCells; x++) {
            if (cellInfo[activeBlock.y + barCells2][activeBlock.x + x] == 1) return false;
        }
    }
    return true;
}

bool canJGoDown(ActiveBlock activeBlock) {
    int barCells = 3;
    int barCells2 = 2;
    if (activeBlock.orientation == Up) {
        if (activeBlock.y + barCells >= ROWS) return false;
        if (cellInfo[activeBlock.y + barCells][activeBlock.x] == 1 || 
            cellInfo[activeBlock.y + barCells][activeBlock.x + barCells-barCells2] == 1) return false;
    } 
    else if (activeBlock.orientation == Down) {
        if (activeBlock.y + barCells >= ROWS) return false;
        if (cellInfo[activeBlock.y + barCells][activeBlock.x] == 1 || 
            cellInfo[activeBlock.y + barCells-barCells2][activeBlock.x + 1] == 1) return false;
    } 
    else if (activeBlock.orientation == Right) {
        if (activeBlock.y + barCells2 >= ROWS) return false;
        for (int x = 0; x < barCells; x++) {
            if (cellInfo[activeBlock.y + 2][activeBlock.x + x] == 1) return false;
        }
    } 
    else { 
        if (activeBlock.y + barCells2 >= ROWS) return false;
        if (cellInfo[activeBlock.y + barCells-barCells2][activeBlock.x] == 1 || 
            cellInfo[activeBlock.y + barCells-barCells2][activeBlock.x + barCells-barCells2] == 1 || 
            cellInfo[activeBlock.y + barCells2][activeBlock.x + barCells2] == 1) return false;
    }
    return true;
}

bool canZGoDown(ActiveBlock activeBlock) {
    int barCells = 3;
    int barCells2 = 2;
    if (activeBlock.orientation == Up || activeBlock.orientation == Down) {
        if (activeBlock.y + barCells2 >= ROWS) return false;
        if (cellInfo[activeBlock.y + barCells-barCells2][activeBlock.x] == 1 || 
            cellInfo[activeBlock.y + barCells2][activeBlock.x + barCells-barCells2] == 1 || 
            cellInfo[activeBlock.y + barCells2][activeBlock.x + barCells2] == 1) return false;
    } 
    else { 
        if (activeBlock.y + barCells >= ROWS) return false;
        if (cellInfo[activeBlock.y + barCells2][activeBlock.x + barCells-barCells2] == 1 || 
            cellInfo[activeBlock.y + barCells][activeBlock.x] == 1) return false;
    }
    return true;
}

bool canSGoDown(ActiveBlock activeBlock) {
    int barCells = 3;
    int barCells2 = 2;
    if (activeBlock.orientation == Up || activeBlock.orientation == Down) {
        if (activeBlock.y + barCells2 >= ROWS) return false;
        if (cellInfo[activeBlock.y + barCells2][activeBlock.x] == 1 || 
            cellInfo[activeBlock.y + barCells2][activeBlock.x + barCells-barCells2] == 1 || 
            cellInfo[activeBlock.y + barCells-barCells2][activeBlock.x + barCells2] == 1) return false;
    } 
    else {
        if (activeBlock.y + barCells >= ROWS) return false;
        if (cellInfo[activeBlock.y + barCells][activeBlock.x ] == 1 || 
            cellInfo[activeBlock.y + barCells2][activeBlock.x + barCells-barCells2] == 1) return false;
    }
    return true;
}

void lockActiveBlock(ActiveBlock activeBlock){
    switch (activeBlock.block){
        case BarBlock: lockBarBlock(activeBlock); break;
        case BoxBlock: lockBoxBlock(activeBlock); break;
        case TBlock: lockTBlock(activeBlock); break;
        case LBlock: lockLBlock(activeBlock); break;
        case JBlock: lockJBlock(activeBlock); break;
        case ZBlock: lockZBlock(activeBlock); break;
        case SBlock: lockSBlock(activeBlock); break;
    }
}

void lockBarBlock(ActiveBlock activeBlock){
    int barCells = 4;
    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        for(int y = 0; y < barCells; y++){
            cellInfo[activeBlock.y + y][activeBlock.x] = 1;
        }
    }
    else {
        for(int x = 0; x < barCells ; x++){
            cellInfo[activeBlock.y][activeBlock.x + x] = 1;
        }
    }
}
void lockBoxBlock(ActiveBlock activeBlock){
    int barCells = 2;
    for (int y = 0; y < barCells; y++) {
        for (int x = 0; x < barCells; x++) {
            cellInfo[activeBlock.y + y][activeBlock.x + x] = 1;
        }
    }
}
void lockTBlock(ActiveBlock activeBlock){
    int barCells = 3;
    int barCells2 = 2;

    if(activeBlock.orientation == Up ){
        for (int y = 0; y < barCells2; y++) {
            for (int x = 0; x < barCells; x++) {
                if((x==0&&y==1)||(x==2&&y==1)){
                    continue;
                }      
                else{
                    cellInfo[activeBlock.y + y][activeBlock.x + x] = 1;
                }
            }
        }
    }
    else if(activeBlock.orientation == Down ){
        for (int y = 0; y < barCells2; y++) {
            for (int x = 0; x < barCells; x++) {
                if((x==0&&y==0)||(x==2&&y==0)){
                    continue;
                }      
                else{
                    cellInfo[activeBlock.y + y][activeBlock.x + x] = 1;
                }
            }
        }
    }
    else if(activeBlock.orientation == Right ){
        for (int y = 0; y < barCells; y++) {
            for (int x = 0; x < barCells2; x++) {
                if((x==0&&y==0)||(x==0&&y==2)){
                    continue;
                }      
                else{
                    cellInfo[activeBlock.y + y][activeBlock.x + x] = 1;
                }
            }
        }
    }
    else {
        for (int y = 0; y < barCells; y++) {
            for (int x = 0; x < barCells2; x++) {
                if((x==1&&y==0)||(x==1&&y==2)){
                    continue;
                }      
                else{
                    cellInfo[activeBlock.y + y][activeBlock.x + x] = 1;
                }
            }
        }
    }
}
void lockLBlock(ActiveBlock activeBlock){
    int barCells = 3;
    int barCells2 = 2;

    if(activeBlock.orientation == Up ){
        for (int y = 0; y < barCells; y++) {
            for (int x = 0; x < barCells2; x++) {
                if((x==1&&y==0)||(x==1&&y==1)){
                    continue;
                }      
                else{
                    cellInfo[activeBlock.y + y][activeBlock.x + x] = 1;
                }
            }
        }
    }
    else if(activeBlock.orientation == Down ){
        for (int y = 0; y < barCells; y++) {
            for (int x = 0; x < barCells2; x++) {
                if((x==0&&y==1)||(x==0&&y==2)){
                    continue;
                }      
                else{
                    cellInfo[activeBlock.y + y][activeBlock.x + x] = 1;
                }
            }
        }
    }
    else if(activeBlock.orientation == Right ){
        for (int y = 0; y < barCells2; y++) {
            for (int x = 0; x < barCells; x++) {
                if((x==1&&y==1)||(x==2&&y==1)){
                    continue;
                }      
                else{
                    cellInfo[activeBlock.y + y][activeBlock.x + x] = 1;
                }
            }
        }
    }
    else {
        for (int y = 0; y < barCells2; y++) {
            for (int x = 0; x < barCells; x++) {
                if((x==0&&y==0)||(x==1&&y==0)){
                    continue;
                }      
                else{
                    cellInfo[activeBlock.y + y][activeBlock.x + x] = 1;
                }
            }
        }
    }
}

void lockJBlock(ActiveBlock activeBlock){
    int barCells = 3;
    int barCells2 = 2;

    if(activeBlock.orientation == Up ){
        for (int y = 0; y < barCells; y++) {
            for (int x = 0; x < barCells2; x++) {
                if((x==0&&y==0)||(x==0&&y==1)){
                    continue;
                }      
                else{
                    cellInfo[activeBlock.y + y][activeBlock.x + x] = 1;
                }
            }
        }
    }
    else if(activeBlock.orientation == Down ){
        for (int y = 0; y < barCells; y++) {
            for (int x = 0; x < barCells2; x++) {
                if((x==1&&y==1)||(x==1&&y==2)){
                    continue;
                }      
                else{
                    cellInfo[activeBlock.y + y][activeBlock.x + x] = 1;
                }
            }
        }
    }
    else if(activeBlock.orientation == Right ){
        for (int y = 0; y < barCells2; y++) {
            for (int x = 0; x < barCells; x++) {
                if((x==1&&y==0)||(x==2&&y==0)){
                    continue;
                }      
                else{
                    cellInfo[activeBlock.y + y][activeBlock.x + x] = 1;
                }
            }
        }
    }
    else {
        for (int y = 0; y < barCells2; y++) {
            for (int x = 0; x < barCells; x++) {
                if((x==0&&y==1)||(x==1&&y==1)){
                    continue;
                }      
                else{
                    cellInfo[activeBlock.y + y][activeBlock.x + x] = 1;
                }
            }
        }
    }
}
void lockZBlock(ActiveBlock activeBlock){
    int barCells = 3;
    int barCells2 = 2;

    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        for (int y = 0; y < barCells2; y++) {
            for (int x = 0; x < barCells; x++) {
                if((x==2&&y==0)||(x==0&&y==1)){
                    continue;
                }      
                else{
                    cellInfo[activeBlock.y + y][activeBlock.x + x] = 1;
                }
            }
        }
    }
    else {
        for (int y = 0; y < barCells; y++) {
            for (int x = 0; x < barCells2; x++) {
                if((x==0&&y==0)||(x==1&&y==2)){
                    continue;
                }      
                else{
                    cellInfo[activeBlock.y + y][activeBlock.x + x] = 1;
                }
            }
        }
    }
}
void lockSBlock(ActiveBlock activeBlock){
    int barCells = 3;
    int barCells2 = 2;

    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        for (int y = 0; y < barCells2; y++) {
            for (int x = 0; x < barCells; x++) {
                if((x==0&&y==0)||(x==2&&y==1)){
                    continue;
                }      
                else{
                    cellInfo[activeBlock.y + y][activeBlock.x + x] = 1;
                }
            }
        }
    }
    else {
        for (int y = 0; y < barCells; y++) {
            for (int x = 0; x < barCells2; x++) {
                if((x==0&&y==0)||(x==1&&y==2)){
                    continue;
                }      
                else{
                    cellInfo[activeBlock.y + y][activeBlock.x + x] = 1;
                }
            }
        }
    }
}
void drawLockedCells(){
    for(int row = 0; row < ROWS; row++){
        for( int col = 0; col < COLS; col++){
            if(cellInfo[row][col] == 1){
                DrawRectangle(col * CELL_WIDTH, row * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT, LOCKED_CELL_COLOR);
            }
        }
    }
}

bool canBlockGoLeft(ActiveBlock activeBlock){
    switch(activeBlock.block){
        case BarBlock: return canBarGoLeft(activeBlock);
        case BoxBlock: return canBoxGoLeft(activeBlock);
        case TBlock: return canTGoLeft(activeBlock);
        case LBlock: return canLGoLeft(activeBlock);
        case JBlock: return canJGoLeft(activeBlock);
        case ZBlock: return canZGoLeft(activeBlock);
        case SBlock: return canSGoLeft(activeBlock);
    }
    return false;
}

bool canBarGoLeft(ActiveBlock activeBlock){
    int barCells = 4;
    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        if(activeBlock.x - 1 < 0) return false;
        for(int y = 0; y < barCells; y++){
            if(cellInfo[activeBlock.y + y][activeBlock.x - 1] == 1) return false;
        }
    }
    else {
        if(activeBlock.x - 1 < 0) return false;
        if(cellInfo[activeBlock.y][activeBlock.x - 1] == 1) return false;
    }
    return true;
}
bool canBoxGoLeft(ActiveBlock activeBlock){
    int barCells = 2;
    if(activeBlock.x - 1 < 0) return false;
    for (int y = 0; y < barCells; y++) {
        if(cellInfo[activeBlock.y + y][activeBlock.x - 1] == 1) return false;
    }
    return true;
}
bool canTGoLeft(ActiveBlock activeBlock){
    int barCells = 3;
    int barCells2 = 2;
    if(activeBlock.x - 1 < 0) return false;
    if(activeBlock.orientation == Up ){
        for (int y = 0; y < barCells2; y++) {
            if(cellInfo[activeBlock.y + y][activeBlock.x + y - 1] == 1) return false;
        }
    }
    else if(activeBlock.orientation == Down ){
        for (int y = 0; y < barCells2; y++) {
            if(cellInfo[activeBlock.y + y][activeBlock.x - y ] == 1) return false;
        }
    }
    else if(activeBlock.orientation == Right ){
        if(cellInfo[activeBlock.y][activeBlock.x] == 1 ||
           cellInfo[activeBlock.y + barCells-barCells2][activeBlock.x - barCells-barCells2] == 1 ||
           cellInfo[activeBlock.y + barCells2][activeBlock.x] == 1 ) return false;
    }
    else {
        for (int y = 0; y < barCells; y++) {
            if(cellInfo[activeBlock.y + y][activeBlock.x - 1] == 1) return false;
        }
    }
    return true;
}
bool canLGoLeft(ActiveBlock activeBlock){
    int barCells = 3;
    int barCells2 = 2;
    if(activeBlock.x - 1 < 0) return false;
    if(activeBlock.orientation == Up ){
        for (int y = 0; y < barCells; y++) {
            if(cellInfo[activeBlock.y + y][activeBlock.x - 1] == 1) return false;
        }
    }
    else if(activeBlock.orientation == Down ){
        if(cellInfo[activeBlock.y][activeBlock.x - (barCells-barCells2)] == 1 ||
        cellInfo[activeBlock.y + (barCells-barCells2)][activeBlock.x] == 1 ||
        cellInfo[activeBlock.y + barCells2][activeBlock.x] == 1) return false;
    }
    else if(activeBlock.orientation == Right ){
        for (int y = 0; y < barCells2; y++) {
            if(cellInfo[activeBlock.y + y][activeBlock.x - 1] == 1) return false;
        }
    }
    else {
        if(cellInfo[activeBlock.y][activeBlock.x + (barCells-barCells2)] == 1 ||
        cellInfo[activeBlock.y + (barCells-barCells2)][activeBlock.x - (barCells-barCells2)] == 1 ) return false;
    }
    return true;
}
bool canJGoLeft(ActiveBlock activeBlock){
    int barCells = 3;
    int barCells2 = 2;

    if(activeBlock.x - 1 < 0) return false;
    if(activeBlock.orientation == Up ){
        if(cellInfo[activeBlock.y][activeBlock.x] == 1 ||
        cellInfo[activeBlock.y + (barCells-barCells2)][activeBlock.x] == 1 ||
        cellInfo[activeBlock.y + barCells2][activeBlock.x - (barCells-barCells2)] == 1) return false;
    }
    else if(activeBlock.orientation == Down ){
        for (int y = 0; y < barCells; y++) {
            if(cellInfo[activeBlock.y + y][activeBlock.x - 1] == 1) return false;
        }
    }
    else if(activeBlock.orientation == Right ){
        for (int y = 0; y < barCells2; y++) {
            if(cellInfo[activeBlock.y + y][activeBlock.x - 1] == 1) return false;
        }
    }
    else {
        if(cellInfo[activeBlock.y][activeBlock.x - (barCells-barCells2)] == 1 ||
        cellInfo[activeBlock.y + (barCells-barCells2)][activeBlock.x + (barCells-barCells2)] == 1 ) return false;
    }
    return true;
}
bool canZGoLeft(ActiveBlock activeBlock){
    int barCells = 3;
    int barCells2 = 2;
    if(activeBlock.x - 1 < 0) return false;
    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        if(cellInfo[activeBlock.y][activeBlock.x - (barCells-barCells2)] == 1 ||
        cellInfo[activeBlock.y + (barCells-barCells2)][activeBlock.x] == 1 ) return false;
    }
    else {
        if(cellInfo[activeBlock.y][activeBlock.x] == 1 ||
        cellInfo[activeBlock.y + (barCells-barCells2)][activeBlock.x - (barCells-barCells2)] == 1 ||
        cellInfo[activeBlock.y + barCells2][activeBlock.x - (barCells-barCells2)] == 1) return false;
    }
    return true;
}
bool canSGoLeft(ActiveBlock activeBlock){
    int barCells = 3;
    int barCells2 = 2;
    if(activeBlock.x - 1 < 0) return false;
    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        if(cellInfo[activeBlock.y][activeBlock.x] == 1 ||
        cellInfo[activeBlock.y + (barCells-barCells2)][activeBlock.x - (barCells-barCells2)] == 1 ) return false;
    }
    else {
        if(cellInfo[activeBlock.y][activeBlock.x - (barCells-barCells2)] == 1 ||
        cellInfo[activeBlock.y + (barCells-barCells2)][activeBlock.x - (barCells-barCells2)] == 1 ||
        cellInfo[activeBlock.y + barCells2][activeBlock.x] == 1) return false;
    }
    return true;
}

bool canBlockGoRight(ActiveBlock activeBlock){
    switch(activeBlock.block){
        case BarBlock: return canBarGoRight(activeBlock);break;
        case BoxBlock: return canBoxGoRight(activeBlock);break;
        case TBlock: return canTGoRight(activeBlock);break;
        case LBlock: return canLGoRight(activeBlock);break;
        case JBlock: return canJGoRight(activeBlock);break;
        case ZBlock: return canZGoRight(activeBlock);break;
        case SBlock: return canSGoRight(activeBlock);break;
    }
    return false;
}

bool canBarGoRight(ActiveBlock activeBlock){
    int barCells = 4;
    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        if(activeBlock.x + 1 >= COLS) return false;
        for(int y = 0; y < barCells; y++){
            if(cellInfo[activeBlock.y + y][activeBlock.x + 1] == 1) return false;
        }
    }
    else {
        if(activeBlock.x + barCells >= COLS) return false;
        if(cellInfo[activeBlock.y][activeBlock.x + barCells] == 1) return false;
    }
    return true;
}
bool canBoxGoRight(ActiveBlock activeBlock){
    int barCells = 2;
    if(activeBlock.x + barCells >= COLS) return false;
    for (int y = 0; y < barCells; y++) {
        if(cellInfo[activeBlock.y + y][activeBlock.x + 2] == 1) return false;
    }
    return true;
}
bool canTGoRight(ActiveBlock activeBlock){
    int barCells = 3;
    int barCells2 = 2;
    if(activeBlock.orientation == Up ){
        if(activeBlock.x + barCells >= COLS) return false;
        for (int y = 0; y < barCells2; y++) {
            if(cellInfo[activeBlock.y + y][activeBlock.x + barCells - y] == 1) return false;
        }
    }
    else if(activeBlock.orientation == Down ){
        if(activeBlock.x + barCells >= COLS) return false;
        for (int y = 0; y < barCells2; y++) {
            if(cellInfo[activeBlock.y + y][activeBlock.x + barCells2 + y ] == 1) return false;
        }
    }
    else if(activeBlock.orientation == Right ){
        if(activeBlock.x + barCells2 >= COLS) return false;
        for (int y = 0; y < barCells; y++) {
            if(cellInfo[activeBlock.y + y][activeBlock.x + (barCells-barCells2)] == 1) return false;
        }
    }
    else {
        if(activeBlock.x + barCells2 >= COLS) return false;
        if(cellInfo[activeBlock.y][activeBlock.x + (barCells-barCells2)] == 1 ||
           cellInfo[activeBlock.y + barCells-barCells2][activeBlock.x + barCells2] == 1 ||
           cellInfo[activeBlock.y + barCells2][activeBlock.x + (barCells-barCells2)] == 1 ) return false;
    }
    return true;
}
bool canLGoRight(ActiveBlock activeBlock){
    int barCells = 3;
    int barCells2 = 2;
    if(activeBlock.orientation == Up ){
        if(activeBlock.x + barCells2 >= COLS) return false;
        if(cellInfo[activeBlock.y][activeBlock.x + (barCells-barCells2)] == 1 ||
        cellInfo[activeBlock.y + (barCells-barCells2)][activeBlock.x + (barCells-barCells2)] == 1 ||
        cellInfo[activeBlock.y + barCells2][activeBlock.x + barCells2] == 1) return false;
    }
    else if(activeBlock.orientation == Down ){
        if(activeBlock.x + barCells2 >= COLS) return false;
        for (int y = 0; y < barCells; y++) {
            if(cellInfo[activeBlock.y + y][activeBlock.x + barCells2] == 1) return false;
        }
    }
    else if(activeBlock.orientation == Right ){
        if(activeBlock.x + barCells >= COLS) return false;
        if(cellInfo[activeBlock.y][activeBlock.x + barCells] == 1 ||
        cellInfo[activeBlock.y + (barCells-barCells2)][activeBlock.x + (barCells-barCells2)] == 1 ) return false;
    }
    else {
        if(activeBlock.x + barCells >= COLS) return false;
        for (int y = 0; y < barCells2; y++) {
            if(cellInfo[activeBlock.y + y][activeBlock.x + barCells] == 1) return false;
        }
    }
    return true;
}
bool canJGoRight(ActiveBlock activeBlock){
    int barCells = 3;
    int barCells2 = 2;
    if(activeBlock.orientation == Up ){
        if(activeBlock.x + barCells2 >= COLS) return false;
        for (int y = 0; y < barCells; y++) {
            if(cellInfo[activeBlock.y + y][activeBlock.x + barCells2] == 1) return false;
        }
    }
    else if(activeBlock.orientation == Down ){
        if(activeBlock.x + barCells2 >= COLS) 
            return false;
        if(cellInfo[activeBlock.y][activeBlock.x + barCells2] == 1 ||
        cellInfo[activeBlock.y + (barCells-barCells2)][activeBlock.x + (barCells-barCells2)] == 1 ||
        cellInfo[activeBlock.y + barCells2][activeBlock.x + (barCells-barCells2)] == 1) 
        return false;
    }
    else if(activeBlock.orientation == Right ){
        if(activeBlock.x + barCells >= COLS) return false;
        if(cellInfo[activeBlock.y][activeBlock.x + (barCells-barCells2)] == 1 ||
        cellInfo[activeBlock.y + (barCells-barCells2)][activeBlock.x + barCells] == 1 ) return false;
    }
    else {
        if(activeBlock.x + barCells >= COLS) return false;
        for (int y = 0; y < barCells2; y++) {
            if(cellInfo[activeBlock.y + y][activeBlock.x + barCells] == 1) return false;
        }
    }
    return true;
}
bool canZGoRight(ActiveBlock activeBlock){
    int barCells = 3;
    int barCells2 = 2;
    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        if(activeBlock.x + barCells >= COLS) return false;
        if(cellInfo[activeBlock.y][activeBlock.x + barCells2] == 1 ||
        cellInfo[activeBlock.y + (barCells-barCells2)][activeBlock.x + barCells] == 1 ) return false;
    }
    else {
        if(activeBlock.x + barCells2 >= COLS) return false;
        if(cellInfo[activeBlock.y][activeBlock.x + barCells2] == 1 ||
           cellInfo[activeBlock.y + (barCells-barCells2)][activeBlock.x + barCells2] == 1 ||
           cellInfo[activeBlock.y + barCells2][activeBlock.x + (barCells-barCells2)] == 1) return false;
    }
    return true;
}
bool canSGoRight(ActiveBlock activeBlock){
    int barCells = 3;
    int barCells2 = 2;
    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        if(activeBlock.x + barCells >= COLS) return false;
        if(cellInfo[activeBlock.y][activeBlock.x + barCells] == 1 ||
        cellInfo[activeBlock.y + (barCells-barCells2)][activeBlock.x + barCells2] == 1 ) return false;
    }
    else {
        if(activeBlock.x + barCells2 >= COLS) return false;
        if(cellInfo[activeBlock.y][activeBlock.x + (barCells-barCells2)] == 1 ||
        cellInfo[activeBlock.y + (barCells-barCells2)][activeBlock.x + barCells2] == 1 ||
        cellInfo[activeBlock.y + barCells2][activeBlock.x + barCells2] == 1) return false;
    }
    return true;
}

bool canBlockRotate(ActiveBlock activeBlock){
    switch(activeBlock.block){
        case BarBlock: return canBarRotate(activeBlock);
        break;
        case BoxBlock:
        break;
        case TBlock: return canTRotate(activeBlock);
        break;
        case LBlock: return canLRotate(activeBlock);
        break;
        case JBlock: return canJRotate(activeBlock);
        break;
        case ZBlock: return canZRotate(activeBlock);
        break;
        case SBlock: return canSRotate(activeBlock);
        break;
    }
    return false;
}

bool canBarRotate(ActiveBlock activeBlock){
    int barCells = 4;
    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        if((activeBlock.x -1) < 0) return false;
        if((activeBlock.x-1) + barCells >= COLS) return false;
        for(int x = -1; x < barCells -1; x++){
            if(cellInfo[activeBlock.y][activeBlock.x + x] == 1) return false;
        }
    }
    else {
        if(activeBlock.y + barCells >= ROWS) return false;
        for(int y = 0; y < barCells; y++){
            if(cellInfo[activeBlock.y + y][activeBlock.x + 1] == 1) return false;
        }
    }
    return true;
}
bool canTRotate(ActiveBlock activeBlock){
    int barCells = 3;
    int barCells2 = 2;
    if(activeBlock.orientation == Up ){
        if(activeBlock.y + barCells >= ROWS) return false;
        for (int y = -1; y < barCells-1; y++) {
            if(cellInfo[activeBlock.y - y][activeBlock.x] == 1) return false;
        }
    }
    else if(activeBlock.orientation == Down ){
        if(activeBlock.y + barCells >= ROWS) return false;
        for (int x = 1; x < barCells; x++) {
            if(cellInfo[activeBlock.y+x][activeBlock.x+barCells2] == 1) return false;
        }
    }
    else if(activeBlock.orientation == Right ){
        if(activeBlock.x+barCells > COLS) return false;
        if((activeBlock.x-(barCells-barCells2)) < 0) return false;
        for (int x = -1; x < barCells2-1; x++) {
            if(cellInfo[activeBlock.y+barCells2][activeBlock.x+x] == 1) return false;
        }
    }
    else {
        if((activeBlock.x-(barCells-barCells2)) < 0) return false;
        if(activeBlock.x+barCells > COLS) return false;
        for (int x = 1; x < barCells; x++) {
            if(cellInfo[activeBlock.y][activeBlock.x+x] == 1) return false;
        }
    }
    return true;
}
bool canLRotate(ActiveBlock activeBlock){
    int barCells = 3;
    int barCells2 = 2;
    if(activeBlock.orientation == Up ){
        if((activeBlock.x+barCells) > COLS) return false;
        for (int y = 1; y < barCells; y++) {
            if(cellInfo[activeBlock.y][activeBlock.x+y] == 1) return false;
        }
    }
    else if(activeBlock.orientation == Down ){
        if((activeBlock.x-1) < 0) return false;
        for (int x = -1; x < barCells2-1; x++) {
            if(cellInfo[activeBlock.y+barCells2][activeBlock.x+x] == 1) return false;
        }
    }
    else if(activeBlock.orientation == Right ){
        if(activeBlock.y+barCells > ROWS) return false;
        for (int x = 1; x < barCells; x++) {
            if(cellInfo[activeBlock.y+x][activeBlock.x+barCells2] == 1) return false;
        }
    }
    else {
        for (int x = -1; x < barCells2-1; x++) {
            if(cellInfo[activeBlock.y+x][activeBlock.x] == 1) return false;
        }
    }
    return true;
}
bool canJRotate(ActiveBlock activeBlock){
    int barCells = 3;
    int barCells2 = 2;
    if(activeBlock.orientation == Up ){
        if((activeBlock.x-1) < 0) return false;
        for (int y = 1; y < barCells; y++) {
            if(cellInfo[activeBlock.y+y][activeBlock.x-1] == 1) return false;
        }
    }
    else if(activeBlock.orientation == Down ){
        if((activeBlock.x+barCells) > COLS) return false;
        for (int x = 0; x < barCells2; x++) {
            if(cellInfo[activeBlock.y+x][activeBlock.x+2] == 1) return false;
        }
    }
    else if(activeBlock.orientation == Right ){
        for (int x = 0; x < barCells2; x++) {
            if(cellInfo[activeBlock.y-1][activeBlock.x+x] == 1) return false;
        }
    }
    else {
        if(activeBlock.y+barCells > ROWS) return false;
        for (int x = 1; x < barCells; x++) {
            if(cellInfo[activeBlock.y+2][activeBlock.x+x] == 1) return false;
        }
    }
    return true;
}
bool canZRotate(ActiveBlock activeBlock){
    int barCells = 3;
    int barCells2 = 2;
    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        if(activeBlock.y+barCells >= ROWS) return false;
        if(cellInfo[activeBlock.y][activeBlock.x + barCells2] == 1 ||
        cellInfo[activeBlock.y + barCells2][activeBlock.x + (barCells-barCells2)] == 1 ) return false;
    }
    else {
        if(activeBlock.x - 1 < 0) return false;
        if(cellInfo[activeBlock.y + barCells2][activeBlock.x + 1] == 1 ||
        cellInfo[activeBlock.y + (barCells-barCells2)][activeBlock.x - 1] == 1 ) return false;
    }
    return true;
}
bool canSRotate(ActiveBlock activeBlock){
    int barCells = 3;
    int barCells2 = 2;
    if(activeBlock.orientation == Up || activeBlock.orientation == Down){
        if(activeBlock.y+barCells > ROWS) return false;
        if(cellInfo[activeBlock.y][activeBlock.x + barCells2] == 1 ||
        cellInfo[activeBlock.y + barCells2][activeBlock.x + (barCells-barCells2)] == 1 ) return false;
    }
    else {
        if(activeBlock.x - 1 < 0) return false;
        if(cellInfo[activeBlock.y + 1][activeBlock.x - 1] == 1 ||
        cellInfo[activeBlock.y][activeBlock.x + 1] == 1 ) return false;
    }
    return true;
}

void rotateBlock(ActiveBlock &activeBlock){
    switch(activeBlock.block){
        case BarBlock:
            switch(activeBlock.orientation){
                case Up:
                case Down:
                    activeBlock.x--;
                    activeBlock.orientation = Right;
                    break;
                case Right:
                case Left:
                    activeBlock.x++;
                    activeBlock.y--;
                    activeBlock.orientation = Up;
                    break;
            }
            break;
        case BoxBlock: break;
        case TBlock:
            switch(activeBlock.orientation){
                case Up:
                    activeBlock.x++;
                    activeBlock.y--;
                    activeBlock.orientation = Right;
                    break;
                case Down:
                    activeBlock.y++;
                    activeBlock.orientation = Left;
                    break;
                case Right:
                    activeBlock.x--;
                    activeBlock.orientation = Down;
                    break;
                case Left:
                    activeBlock.y--;
                    activeBlock.orientation = Up;
                    break;
            }
            break;
        case LBlock:
            switch(activeBlock.orientation){
                case Up:
                    activeBlock.orientation = Right;
                    break;
                case Down:
                    activeBlock.x--;
                    activeBlock.orientation = Left;
                    break;
                case Right:
                    activeBlock.orientation = Down;
                    break;
                case Left:
                    activeBlock.y--;
                    activeBlock.x++;
                    activeBlock.orientation = Up;
                    break;
            }
            break;
        case JBlock:
            switch(activeBlock.orientation){
                case Up:
                    activeBlock.x--;
                    activeBlock.orientation = Right;
                    break;
                case Down:
                    activeBlock.orientation = Left;
                    break;
                case Right:
                    activeBlock.y--;
                    activeBlock.x++;
                    activeBlock.orientation = Down;
                    break;
                case Left:
                    activeBlock.orientation = Up;
                    break;
            }
            break;
        case SBlock:
            switch(activeBlock.orientation){
                case Up:
                case Down:
                    activeBlock.x++;
                    activeBlock.orientation = Right;
                    break;
                case Right:
                case Left:
                    activeBlock.x--;
                    activeBlock.orientation = Up;
                    break;
            }
            break;
        case ZBlock:
            switch(activeBlock.orientation){
                case Up:
                case Down:
                    activeBlock.x++;
                    activeBlock.orientation = Right;
                    break;
                case Right:
                case Left:
                    activeBlock.x--;
                    activeBlock.orientation = Up;
                    break;
            }
            break;
    }
}
