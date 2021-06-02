#include <iostream>
#include <cassert>

bool checkPlaceForShip(int field[][10], int row, int col, int shipLength, bool isHorizontal)
{
    assert(shipLength >= 1 && shipLength <= 4);
    if (isHorizontal && (col + shipLength > 10)) return false;
    else if (!isHorizontal && (row + shipLength > 10)) return false;
    else
    {
        for (int i = 0; i < shipLength; i++)
        {
            if (isHorizontal && field[row][col + i] != 0) return false;
            else if (!isHorizontal && field[row + i][col] != 0) return false;
        }
        return true;
    }
}

bool placeTheShip(int field [][10], int row, int col, int shipLength, bool isHorizontal)
{
    assert(shipLength > 0 && shipLength <= 4);
    if (checkPlaceForShip(field, row, col, shipLength, isHorizontal))
    {
        for (int i = 0; i < shipLength; i++)
        {
            if (isHorizontal) field[row][col + i] = shipLength; //Marking ships with their length.
            else field[row + i][col] = shipLength;
        }
        return true;
    }
    else
    {
        std::cout << "This ship can't be placed here.. Try another coords.\n";
        return false;
    }
}

void printField(int field[][10], int player)
{
    std::cout << "Field of player " << player + 1 << ":\n";
    std::cout << " \t";
    for (int i = 0; i < 10; i++) std::cout << i << "\t";
    std::cout << "\n\t";
    for (int i = 0; i < 10; i++) std::cout << "_" << "\t";
    std::cout << "\n";
    for (int i = 0; i < 10; i++)
    {
        std::cout << i << "|\t";
        for (int j = 0; j < 10; j++)
        {
            if (field[i][j] != 0) std::cout << field[i][j];
            else std::cout << ".";
            std::cout << "\t";
        }
        std::cout << "\n";
    }
}

void getCoordFromInput(int coords[][2], int player)
{
    assert(player == 0 || player == 1);
    bool badInput;
    do
    {
        std::cout << "Please enter the coords (format: X,Y):\n";
        std::string input;
        std::cin >> input;

        std::string left = "";
        std::string right = "";
        bool commaFound = false;

        for (int i = 0; i < input.length(); i++)
        {
            if (input[i] == ',') commaFound = true;
            else if (!commaFound) left += input[i];
            else right += input[i];
        }

        badInput = left.length() > 1 ||
                right.length() > 1 ||
                left[0] < '0' ||
                left[0] > '9' ||
                right[0] < '0' ||
                right[0] > '9';

        if (badInput) std::cout << "Bad input. Use numbers from 0 to 9. Try again.\n";
        else
        {
            coords[player][1] = std::stoi(left);    // X coordinate
            coords[player][0] = std::stoi(right);   // Y coordinate
        }

    } while (badInput);

}

bool askIfHorizontal()
{
    std::string result;
    std::cout << "Horizontal? (0 / 1)\n";
    std::cin >> result;
    if (result[0] == '0') return false;
    else return true;
}

bool attack(int field[][10], int coords[][2], int player)
{
    assert(player == 0 || player == 1);
    if (field[coords[player][0]][coords[player][1]] != 0)
    {
        std::cout << "Hit!\n";
        field[coords[player][0]][coords[player][1]] = 0;
        return true;
    }
    else
    {
        std::cout << "Miss...\n";
        return false;
    }
}

void placeAllShips(int player, int playerCoords[][2], int field[][10])
{
    assert(player == 0 || player == 1);
    std::cout << "Player " << player + 1 << ": set your ships!\n";
    for (int i = 1; i <= 4; i++)
    {
        for (int shipsQuantity = 5 - i; shipsQuantity > 0; shipsQuantity--)
        {
            bool shipIsPlaced;
            do
            {
                std::cout << "Ship with length " << i << " is waiting for command.\n";
                getCoordFromInput(playerCoords, 0);
                shipIsPlaced = placeTheShip(field,
                                            playerCoords[0][0],
                                            playerCoords[0][1], i,
                                            i > 1 ? askIfHorizontal() : false);
                if (shipIsPlaced) printField(field, player);

            } while(!shipIsPlaced);
        }
    }
    std::cout << "All ships are placed!\n";
    std::cout << "----------------------------\n";
}

bool checkForVictory(int field[][10])
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (field[i][j] != 0) return false;
        }
    }
    return true;
}


int main() {

    int player_coords[2][2] = {0}; //array for both players coordinates storage;

    int field_1[10][10] = {0};
    int field_2[10][10] = {0};

    //Use for fast test:
    //1,1 2,2 3,3 4,4 5,5 1 6,6 1 0,8 1 4,6 0 8,0 0 9,6 0
    placeAllShips(0, player_coords, field_1);
    placeAllShips(1, player_coords, field_2);

    std::cout << "Ships are placed. Time to destroy some boats!\n";
    std::cout << "---------------------------------------------\n";

    //Game start:
    int actualPlayer = 0;
    bool continueTurn;
    while (true)
    {
        printField(actualPlayer == 0 ? field_2 : field_1, actualPlayer == 0 ? 1 : 0);
        std::cout << "Player " << actualPlayer + 1 << " - your turn. Enter coords and fire!\n";
        getCoordFromInput(player_coords, actualPlayer);
        continueTurn = attack(actualPlayer == 0 ? field_2 : field_1,
                              player_coords,
                              actualPlayer);

        if (checkForVictory(actualPlayer == 0 ? field_2 : field_1)) break;

        if (!continueTurn) actualPlayer == 0 ? actualPlayer = 1 : actualPlayer = 0;
    }
    //End of game:
    std::cout << "Player " << actualPlayer + 1 << " is the winner!\n";

    return 0;
}
