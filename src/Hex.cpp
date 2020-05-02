/* The game of Hex played in the terminal
 * 
 * Compile with:  
 * g++ -std=c++11 Hex.cpp -o Hex.exe
**/

#include <iostream>
#include <iterator>
#include <vector>
#include <queue>
#include <ctime>
#include "Node.h"

using namespace std;

/* Graph of nodes representing the Hex board **/
class Board
{
    private:
    // Board is boardDim * boardDim connected hexagons
    int boardDim;
    // vector of all of boardDim*boardDim nodes in the board
    vector<Node<char> > boardNodes;

    public: 
    /* Constructor
     * Initialize all boardDim*boardDim nodes with the specified occupier and connect adjacent hexagons to one another.
     * Add these nodes to boardNodes vector.
     **/
    Board(int dim, char occupier = '*'): boardDim(dim)
    {
        for(int i = 0; i < dim*dim; i++){
            boardNodes.push_back(Node<char>(i, occupier));
        }
        for(int i = 0; i < dim; i++){ // y coordinate
            for(int j = 0; j < dim; j++){ // x-coordinate
                int nodeID = getNodeIDFromCoords(j, i);
                if(j > 0) // try adding West hexagon
                    boardNodes[nodeID].addEdge(&boardNodes[getNodeIDFromCoords(j-1, i)]); 
                if(i > 0) // try adding NW hexagon
                    boardNodes[nodeID].addEdge(&boardNodes[getNodeIDFromCoords(j, i-1)]);
                if((i > 0) && (j < dim - 1)) // try adding NE hexagon
                    boardNodes[nodeID].addEdge(&boardNodes[getNodeIDFromCoords(j+1, i-1)]);
                if(j < dim -1) // try adding East hexagon
                    boardNodes[nodeID].addEdge(&boardNodes[getNodeIDFromCoords(j+1, i)]);
                if(i < dim - 1) // try adding SE hexagon
                    boardNodes[nodeID].addEdge(&boardNodes[getNodeIDFromCoords(j, i+1)]);
                if((j > 0) && (i < dim - 1)) // try adding SW hexagon
                    boardNodes[nodeID].addEdge(&boardNodes[getNodeIDFromCoords(j-1, i+1)]);   
            }
        }
    }

    /* Return dimension of the board **/
    int getDim(void) const
    {
        return boardDim;
    }

    /* Return number of nodes in this board **/
    int getNumNodes(void) const
    {
        return boardNodes.size();
    }

    /* Draw the gameBoard with a border
     * param[in] northSouthBorder : character to draw on top and bottom border of graph
     * param[in] westEastBorder : character to draw on left and right border of graph
     **/
    void drawBoard(char northSouthBorder, char westEastBorder)
    {
        // initial empty padding
        cout << endl;
        // draw north border
        cout << "  "; // inital whitespace
        for(int i = 0; i < boardDim*2 + 1; i ++)
            cout << northSouthBorder << " ";
        cout << endl << endl;

        // draw middle rows
        for(int i = 0; i<boardDim; i++){

            // draw whitespace on left
            for(int j = 0; j < 2*i; j++)
                    cout << " ";
            //draw west border
            cout << westEastBorder << "   ";
            // draw row w/ occupiers
            for(int j = 0; j < boardDim; j++){
                cout << boardNodes[getNodeIDFromCoords(j, i)];
                // draw left link only if not on right end of board
                if(j < boardDim-1)
                    cout << " - ";
            }
            //draw east border and start next line
            cout << "   " << westEastBorder << endl;

            // draw whitespace on left
            for(int j = 0; j < (2*i + 1); j++)
                cout << " ";
            

            // draw bottom links if not on last row, otherwise draw south border
            if (i < boardDim - 1){ // bottom links
                // draw west border
                cout << westEastBorder << "   ";
                // draw bottom links
                for(int j = 0; j < boardDim; j++){
                    cout << "\\";
                    if(j < boardDim -1)
                    cout << " / ";
                }
                // draw east border
                cout << "   " << westEastBorder;
            }
            else{ //south border
                cout << endl;
                // draw whitespace on left
                for(int j = 0; j < (2*i + 3); j++)
                    cout << " ";
                for(int i = 0; i < boardDim*2 + 1; i ++)
                    cout << northSouthBorder << " ";
            }
            cout << endl;
        }
        //final empty padding
        cout << endl;     
    }

    /* set the occupier for the node at the specified coordinates **/
    void setOccupierAtCoord(int x, int y, char occ)
    {
        boardNodes[getNodeIDFromCoords(x,y)].setOccupier(occ);
    }

    /* get the occupier for the node at the specified coordinates **/
    char getOccupierAtCoord(int x, int y) const
    {
        return boardNodes[getNodeIDFromCoords(x,y)].getOccupier();
    }

    /* Return a reference to th Node at board coordinates x, y **/
    Node<char>& getNodeAtCoords(int x, int y)
    {
        return boardNodes[getNodeIDFromCoords(x, y)];
    }

    /* Convert x & y coordinate of a node to its position in the boardNodes vector **/
    inline int getNodeIDFromCoords(int x, int y) const { return (x + boardDim*y); }

    /* Use the node ID to get the node's coordinates in this board **/
    inline pair<int, int> getCoordsFromNodeID(int nodeId)
    {
        return (make_pair((nodeId % boardDim), (nodeId / boardDim)));
    }
};

/* Master class encapsulating all gamestates and gameplay processing functions **/
class Game
{
private:
    // character meant to represent no player
    const char nullChar = '*';
    // character representing player 1
    char player1;
    // character representing player 2
    char player2;
    // Control if player 2 will be played by an AI
    bool useAI;
    // which player starts the game
    char whoGoesFirst;
    // game board
    Board b;

    void p1PlaceToken(int x, int y)
    {
        b.setOccupierAtCoord(x, y, player1);
    }

    void p2PlaceToken(int x, int y)
    {
        b.setOccupierAtCoord(x, y, player2);
    }

    /* Check if the last played move connects the player's desired ends.
     * Performs a BFS to search for the winning ends.
     * Win Conditions for player 1: A path from top to bottom exists.
     * Win Conditions for player 2: A path from left to right exists.
     **/
    char checkWinner(int lastPlayedX, int lastPlayedY, char player)
    {
        //winCond1 is topReached and windCond2 is bottomReached for player1
        //winCond1 is leftReached and windCond2 is rightReached for player2
        bool winCond1, winCond2;
        // vector to ensure a node isn't checked twice when checking for win
        vector <bool> visited(b.getNumNodes(), false);
        // list of connected nodes
        queue<Node<char> > path;
        // start with the last played move
        path.emplace(b.getNodeAtCoords(lastPlayedX, lastPlayedY));
        // go through all connected nodes until a path through both desired ends is found, if no path, no win
        while(!path.empty()){
            Node<char> A = path.front();
            path.pop();
            visited[A.getNodeID()] = true;
            pair<int, int> nodeCoords = b.getCoordsFromNodeID(A.getNodeID());
            if(!winCond1)
                winCond1 = (player == player1) ? (nodeCoords.second == 0) : (nodeCoords.first == 0);
            if(!winCond2)
                winCond2 = (player == player1) ? (nodeCoords.second == (b.getDim()-1)) : (nodeCoords.first == (b.getDim()-1));
            if(winCond1 && winCond2)
                return player;
            for(auto i : A.getEdgeList()){
                if(i->getOccupier() == player && !visited[i->getNodeID()])
                    path.emplace(*i);   
            }
        }

        return nullChar;
    }

public:
    /* Constructor **/
    Game(int boardSize):player1('X'), player2('O'), b(boardSize, nullChar)
    {
    };

    /* Destructor **/
    ~Game()
    {
    };

    /* Initial game setup routine
     * sets up the play mode (1 player or 2 player) and decides who goes first
     **/
    void initGame(void)
    {
        int players = 0;
        while(players != 1 && players != 2){
            cout << "1Player or 2Player? (Enter 1 or 2): " << flush;
            cin >> players;
        }
        useAI = (players == 1);

        // Dice roll who goes first.
        srand(time(NULL));
        whoGoesFirst = (rand()%2 == 0) ? player1 : player2;

        b.drawBoard(player1, player2);
    }

    /* Main game loop 
     * Takes in and processes all input.
     * Manages all game logic.
    **/
    void processGame(void)
    {
        int x, y;
        bool playing = true;
        char whoseTurn = whoGoesFirst;
        char winner;
        while(playing){
            if(whoseTurn == player1)
            {
                cout << "PLAYER 1'S TURN ("<< player1 << ")" << endl;

                while(true){
                    cout << "Enter where to place token" << endl;
                    cout << "x: " << flush;
                    cin >> x;
                    cout << "y: "<< flush;
                    cin >> y;
                    if(b.getOccupierAtCoord(x, y) == '*'){
                        p1PlaceToken(x, y);
                        break;
                    }
                    else
                        cout << "INVALID MOVE" << endl;
                }
            }
            else{
                cout << "PLAYER 2'S TURN (" << player2 << ")" << endl;

                if(useAI){
                    while(true){
                        x = rand()%b.getDim();
                        y = rand()%b.getDim();
                        if(b.getOccupierAtCoord(x, y) == nullChar){
                            p2PlaceToken(x, y);
                            break;
                        }
                    }
                    cout << "PLAYER 2 PLAYED "<< x << ", " << y << " !" << endl;
                } 
                else{
                    while(true){
                        cout << "Enter where to place token" << endl;
                        cout << "x: " << flush;
                        cin >> x;
                        cout << "y: "<< flush;
                        cin >> y;
                        if(b.getOccupierAtCoord(x, y) == nullChar){
                            p2PlaceToken(x, y);
                            break;
                        }
                        else
                            cout << "INVALID MOVE" << endl;
                    }
                }
            }

            // check if anybody won
            b.drawBoard(player1, player2);
            winner = checkWinner(x, y, whoseTurn);
            if(winner != nullChar){
                if(winner == player1)
                    cout << "PLAYER 1 (" << player1 << ") WINS!" << endl;
                else
                    cout << "PLAYER 2 (" << player2 << ") WINS!" << endl;
                playing = false;
            }
            else{
                whoseTurn = (whoseTurn == player1) ? (player2) : (player1);
            }
        }
    }

};



/** Driver */
int main(void)
{
    cout << endl;
    cout << " _    _ ________   __ " << endl;
    cout << "| |  | |  ____\\ \\ / / " << endl;
    cout << "| |__| | |__   \\ V /  " << endl;
    cout << "|  __  |  __|   > <   " << endl;
    cout << "| |  | | |____ / . \\  " << endl;
    cout << "|_|  |_|______/_/ \\_\\ " << endl;
    cout <<endl;
    int n;
    cout << "Board size?: " << flush;
    cin >> n;
    Game g(n);
    g.initGame();
    g.processGame();
}
