
![Hex](https://github.com/JoshuaDiaz/Hex/blob/master/Images/HexImg.PNG "Image Source: Wikipedia")
# Hex
The game of Hex played in the terminal.

## The Game of Hex

The game of Hex was invented in 1942 by Piet Hein, and independently reinvented in 1948 by John Nash. Hex got its name in 1952 from a commercial distribution by Parker Brothers and has been popularized by Martin Gardner in 1957. 

![Img Source: Wikipedia](https://github.com/JoshuaDiaz/Hex/blob/Images/master/HexBoard.jpg "Image Source: Wikipedia")

The blue player must make a connected set of blue hexagons from east to west. The red player must do the same from north to south. At each turn a player chooses any unoccupied hexagon and gives it their color.

## Playing This Game
This implementation of the game allows any dimension of board to be played on. In fact this is the first thing prompted to the player:

![Choose the board size!](https://github.com/JoshuaDiaz/Hex/blob/Images/master/BoardSize.PNG "Choose the board size!")

The player is than prompted with the option of whether to play solo (1 Player) or with somebody else (2 Player). Choosing 1 Player will put you up against an AI as Player 2. The game is turn based, so Player 1 and Player 2 will alternate playing their moves on the same board.

The player who goes first is decided by an in-game coin flip.

![The AI won the coin flip](https://github.com/JoshuaDiaz/Hex/blob/Images/master/coinflip.PNG "The AI won the coin flip")

Player 1 is always represented by 'X' and Player 2 by 'O'. The borders around the game board designate which ends of the board each player is trying to connect. In the above picture, The top and bottom borders are meant for Player 1 ('X') to connect and the left and right borders are meant for Player 2 ('O') to connect.

On your turn, you will be prompted to input an x and y coordinate. This corresponds to the location on the grid that your token will be placed at. The top left corner is (x = 0, y= 0) and the bottom left corner is (x = boardSize, y = boardSize).

![x = 3, y = 1](https://github.com/JoshuaDiaz/Hex/blob/master/Images/input.PNG "x = 3, y = 1")

The game will continue until one player connects their ends!

