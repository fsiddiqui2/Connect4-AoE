#initialize grid
cols = 5
rows = 4
grid = [[0 for x in range(cols)] for y in range(rows)]

for row in grid:
    print(*row)

turn = 0
boardFull = False
win = False
winRows = []
winCols = []

while not(boardFull or win):
    # -- switch player -- #
    if turn % 2 == 0:
        player = 1
    else:
        player = 2

    # -- read in move -- #
    move = int(input("Select a column: ")) 

    while grid[0][move] != 0:
        print("Column is full, select another column")
        move = int(input("Select a column: "))

    for row in range(rows - 1, -1, -1):
        if grid[row][move] == 0:
            grid[row][move] = player
            #"light up corresponding slot"
            break
          
          
    # -- check for win -- #

    #check horizontal
    for c in range(cols - 3):
        if grid[row][c] == player and grid[row][c + 1] == player and grid[row][c + 2] == player and grid[row][c + 3] == player:
            win = True
            for i in range(4):
                winRows.append(row)
                winCols.append(c + i)
            break

    #check vertical
    for r in range(rows - 3):
        if grid[r][move] == player and grid[r + 1][move] == player and grid[r + 2][move] == player and grid[r + 3][move] == player:
            win = True
            for i in range(4):
                winRows.append(r + i)
                winCols.append(move)
            break

    #check left to right diagonal (start from top left)
    startRow = row - min(move - max(0, move - 3), row - max(0, row - 3))
    startCol = move - min(move - max(0, move - 3), row - max(0, row - 3))
    while (not win) and startRow + 3 < rows and startCol + 3 < cols:
        if grid[startRow][startCol] == player and grid[startRow + 1][startCol + 1] == player and grid[startRow + 2][startCol + 2] == player and grid[startRow + 3][startCol + 3] == player:
            win = True
            for i in range(4):
                winRows.append(startRow + i)
                winCols.append(startCol + i)
        startRow += 1
        startCol += 1

    #check right to left diagonal (start from bottom left)
    startRow = row + min(move - max(0, move - 3),  min(rows-1, row + 3) - row)
    startCol = move - min(move - max(0, move - 3), min(rows-1, row + 3) - row)

    while (not win) and startRow - 3 >=0 and startCol + 3 < cols:
        if grid[startRow][startCol] == player and grid[startRow - 1][startCol + 1] == player and grid[startRow - 2][startCol + 2] == player and grid[startRow - 3][startCol + 3] == player:
            win = True
            for i in range(4):
                winRows.append(startRow - i)
                winCols.append(startCol + i)
        startRow -= 1
        startCol += 1
    

    # -- check for tie -- #
    if not (0 in grid[0]):
        boardFull = True
    
     # -- toggle turn -- #
    turn += 1

    for row in grid:
        print(*row)


if boardFull and not win:
    print("Tie!")
else:
    print(f"Player {player} wins!")

for i in range(4):
    grid[winRows[i]][winCols[i]] = "X"
    #flash the 4-in-a-row

for row in grid:
    print(*row)
