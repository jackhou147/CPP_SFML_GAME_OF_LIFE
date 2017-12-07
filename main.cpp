#include <iostream>
#include <SFML/Graphics.hpp>
const int MAX_ROWS = 10;
const int MAX_COLS = 10;
const int MAX_WINDOW_WIDTH = 800;
const int MAX_WINDOW_HEIGHT = 800;
//SQUARES have equal width and length
const float MAX_RECT_WIDTH = MAX_WINDOW_WIDTH / MAX_COLS;
const float MAX_RECT_HEIGHT = MAX_RECT_WIDTH;
sf::RenderWindow window(sf::VideoMode(MAX_WINDOW_WIDTH, MAX_WINDOW_HEIGHT), "My window");

using namespace std;

//============================================
//RED IS ALIVE; BLUE IS DEAD
//1 IS ALIVE; 0 IS DEAD
//============================================


//PROTOTYPE
//initiate all elements in grid to 0
void init_grid(int grid[][MAX_COLS]);

//print grid to screen
void print_grid(int grid[][MAX_COLS], sf::RectangleShape arr[][MAX_COLS]);

//copy grid to a temperary grid
void copy_grid(int source[][MAX_COLS], int target[][MAX_COLS]);

//number of live cells around a given cell
int count_neighbors(int grid[][MAX_COLS], int cell_row, int cell_col);

//get next generation
//dependency: next_gen(), cooy_grid()
void step(int grid[][MAX_COLS]);

//calculate next generation
void next_gen(int grid[][MAX_COLS], int temp_grid[][MAX_COLS]);

//output menu on screen
void print_menu();



int main()
{
    //fill up array of rectangles
    sf::RectangleShape arr[MAX_ROWS][MAX_COLS];
    for(int i=0; i<MAX_ROWS; i++){
        for(int j=0; j<MAX_COLS; j++){
            sf::RectangleShape shape;
            shape.setSize(sf::Vector2f(MAX_RECT_WIDTH, MAX_RECT_HEIGHT));   //fill with squares
            shape.setFillColor(sf::Color::Blue);
            shape.setOutlineColor(sf::Color::Black);  //black outline
            shape.setOutlineThickness(3.f); //outline 1 px thick
            arr[i][j] = shape;
        }
    }

    //======variables=========
    //2d array of  game cells(NOT the actual rectangles to be drawn on screen)
    int grid[MAX_ROWS][MAX_COLS];




    //=======process===========
    //set default grid configuration
    init_grid(grid);
    grid[3][4] = 1;
    grid[4][5] = 1;
    grid[5][3] = 1;
    grid[5][4] = 1;
    grid[5][5] = 1;


    // create the window
    window.setFramerateLimit(15);

    // run the program as long as the window is open
    while (window.isOpen())
    {

        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        // update.....
        step(grid);

        // draw everything here...
        print_grid(grid,arr);

        // end the current frame
        window.display();
    }

    cout << "Hello World!" << endl;
    return 0;
}


//DEFINITIONS
void init_grid(int grid[][MAX_COLS])
{
    /*
      Purpose: initiate all elements in grid to 0
      Pre-condition: valid argument values
      Post-condition: grid is initiated to zeros
      Tested:
    */

    //process...
    //1. each row
    for (int i = 0; i < MAX_ROWS; i++) {
        //2. each column
        for (int j = 0; j < MAX_COLS; j++) {
            grid[i][j] = 0;
        }
    }
}

void print_grid(int grid[][MAX_COLS], sf::RectangleShape arr[][MAX_COLS])
{
    /*
      Purpose: draw the grid to window object
      Pre-condition: an array containing rectangle objects; an array containing inte
      gers indicating whether a cell is alive or dead
      Post-condition: grid is drawn on window object, ready to be displayed to screen
      Tested:
    */

    //variables...
    float posX = 0.f;
    float posY = 0.f;


    //process...
    //loop through each element in array
    for (int i = 1; i < MAX_ROWS; i++) {
        cout << endl;
        for (int j = 1; j < MAX_COLS; j++) {
            //====set color for each cell=======
            if(grid[i][j] == 0) // if the cell is dead, make it blue
                arr[i][j].setFillColor(sf::Color::Blue);
            else        //if the cell is alive, make it red
                arr[i][j].setFillColor(sf::Color::Red);


            //========draw on window object====
            arr[i][j].setPosition(posX,posY);

            //after drawing each box, update x and y position for the next box
            posX += (MAX_RECT_WIDTH);
            if(j == (MAX_COLS-1)){  //if this is the last box in current row
                cout<<endl<<"last box of the row";
                posY += MAX_RECT_HEIGHT;    //start drawing from the beginning of next line
                posX = 0.f;
            }
            window.draw(arr[i][j]);
        };
    };
}

void copy_grid(int source[][MAX_COLS], int target[][MAX_COLS])
{
    /*
      Purpose:copy grid to a temperary grid
      Pre-condition:valid argument values
      Post-condition:source grid is copied to target grid
      Tested:
    */

    //process...
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            target[i][j] = source[i][j];
        }
    }
}

int count_neighbors(int grid[][MAX_COLS], int cell_row, int cell_col)
{
    /*
      Purpose: number of live cells around a given cell
      Pre-condition:    0<cell_row<MAX_ROWS-1;
                        0<cell_col<MAX_COL-1;
      Post-condition: number of neighbors of the
      current cell is returned.
      Note: No circular, border is empty
      Tested:
    */

    //variables...
    int count = 0;

    //process...
    //neighbor top
    if (grid[cell_row - 1][cell_col] == 1) count++;
    //neighbor bottom
    if (grid[cell_row + 1][cell_col] == 1) count++;
    //neighbor left
    if (grid[cell_row][cell_col - 1] == 1) count++;
    //neighbor right
    if (grid[cell_row][cell_col + 1] == 1) count++;
    //neighbor top left
    if (grid[cell_row - 1][cell_col - 1] == 1) count++;
    //neighbor top right
    if (grid[cell_row - 1][cell_col + 1] == 1) count++;
    //neighbor bottom left
    if (grid[cell_row + 1][cell_col - 1] == 1) count++;
    //neighbor bottom right
    if (grid[cell_row + 1][cell_col + 1] == 1) count++;

    //output...
    return count;
}

void step(int grid[][MAX_COLS])
{
    /*
      Purpose:get next generation
      Pre-condition: valid argument values
      Post-condition:
      Note:
      Dependency:
      Tested:
    */

    //process...
    //1. define and initialize temp grid to all zeros
    int temp_grid[MAX_ROWS][MAX_COLS];
    init_grid(temp_grid);
    //2. copy current grid to temp grid
    copy_grid(grid, temp_grid);
    //3. process temp grid to get next generation
    next_gen(grid, temp_grid);
    //4. copy temp grid back to current grid
    copy_grid(temp_grid, grid);
}

void next_gen(int grid[][MAX_COLS], int temp_grid[][MAX_COLS])
{
    /*
      Purpose: calculate next generation
      Pre-condition: valid argument values
      Post-condition: temp_grid[] is initiated to the
      next generation based on grid[]
      Tested:
    */

    //variables...
    int neighbor_count = 0;

    //process...
    //1. loop through each row
    for (int i = 1; i < MAX_ROWS; i++) {
        //2. loop through each cell current row
        for (int j = 1; j < MAX_COLS; j++) {

            //3. count the neighbors of this cell
            neighbor_count = count_neighbors(grid, i, j);

            //3.1 if this is an alive cell
            if (grid[i][j] == 1) {
                //kill if less than 2  or more than 3 live neighbors
                if (neighbor_count < 2 || neighbor_count > 3)
                    temp_grid[i][j] = 0;
            }
            //3.2 if this is a dead cell
            else {
                //come alive if exactly 3 live neighbors
                if (neighbor_count == 3) {
                    temp_grid[i][j] = 1;
                }
            }
        }
    }

}

void print_menu(){
    cout<<endl<<"        [S]tep, [D]efault, [L]ast configuration,"
        <<endl<<"pattern [1], pattern[2], pattern[3],pattern[4],pattern[5], e[X]it"<<endl;
}
