#include <iostream>
#include <vector>
#include <unistd.h>
#include <stack>
#include <iomanip>

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;

#define SPACE "         "
#define LINE "------------------------------------------------------"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

#define FEKETE 0
#define FEHER 1

#define ORA_IRANY true
#define ORA_FORDITOTT false

#define FRONT 1
#define UP 2
#define RIGHT 3
#define DOWN 4
#define LEFT 5
#define BACK 6

#define TRIG_POZ true
#define TRIG_NEG false
#define X 1
#define Y 2
#define Z 3

vi reverse(vi v)
{
    vi uj(4);
    for (int i = 1; i <= 3; i++) {
        uj[i] = v[4 - i];
    }
    return uj;
}

struct Turn {
    int kulso;
    bool irany;
    int oldal;
    int xyz;
};

typedef vector<Turn> vt;

int get_random_number(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

class Oldal
{
private:
    vvi oldal;
public:
    Oldal(int szam) {
        oldal.resize(4, vi(4, FEKETE));
        switch (szam)
        {
        case 1:
            oldal[2][2] = FEHER;
            break;
        case 2:
            oldal[1][1] = FEHER;
            oldal[3][3] = FEHER;
            break;
        case 3:
            oldal[1][1] = FEHER;
            oldal[2][2] = FEHER;
            oldal[3][3] = FEHER;
            break;
        case 4:
            oldal[1][1] = FEHER;
            oldal[1][3] = FEHER;
            oldal[3][1] = FEHER;
            oldal[3][3] = FEHER;
            break;
        case 5:
            oldal[1][1] = FEHER;
            oldal[1][3] = FEHER;
            oldal[2][2] = FEHER;
            oldal[3][1] = FEHER;
            oldal[3][3] = FEHER;
            break;
        case 6:
            oldal[1][1] = FEHER;
            oldal[1][3] = FEHER;
            oldal[2][1] = FEHER;
            oldal[2][3] = FEHER;
            oldal[3][1] = FEHER;
            oldal[3][3] = FEHER;
            break;
        default:
            break;
        }
    }

    Oldal() {
        oldal.resize(4, vi(4, FEKETE));
    }

    int& operator()(int i, int j) {
        return oldal[i][j];
    }

    vi getSor(int i) {
        return oldal[i];
    }

    vi getOszlop(int j) {
        vi oszlop(4);
        for (int i = 1; i <= 3; i++) {
            oszlop[i] = oldal[i][j];
        }
        return oszlop;
    }

    void setSor(int i, vi sor) {
        oldal[i] = sor;
    }
    
    void setOszlop(int j, vi oszlop) {
        for (int i = 1; i <= 3; i++) {
            oldal[i][j] = oszlop[i];
        }
    }
    
    void forgat(bool irany = ORA_IRANY) {
        vvi uj = oldal;
        if (irany == ORA_IRANY) {
            for (int i = 1; i <= 3; i++) {
                uj[1][i] = oldal[4 - i][1];
                uj[2][i] = oldal[4 - i][2];
                uj[3][i] = oldal[4 - i][3];
            }
        }
        else {
            for (int i = 1; i <= 3; i++) {
                uj[1][i] = oldal[i][4 - 1];
                uj[2][i] = oldal[i][4 - 2];
                uj[3][i] = oldal[i][4 - 3];
            }
        }
        oldal = uj;
    }
    
    Oldal& operator=(Oldal& o) {
        for (int i = 1; i <= 3; i++) {
            oldal[i] = o.getSor(i);
        }
        return *this;
    }

    int get_nr_of_dots() {
        int nrOfDots = 0;
        for (int i = 1; i <= 3; i++) {
            for (int j = 1; j <= 3; j++) {
                if (oldal[i][j] == FEHER) {
                    nrOfDots++;
                }
            }
        }
        return nrOfDots;
    }
    
    void printSor(int i, bool focus = false) {
        for (int j = 1; j <= 3; j++) {
            cout << "[";
            if (oldal[i][j] == FEHER) {
                if (focus) {
                    cout << GREEN;
                }
                else {
                    cout << RED;
                }
                cout << "O" << RESET;
            }
            else {
                cout << " ";
            }
            cout << "]";
        }
        cout << "  ";
    }
};

class Kocka
{
private:
    vector<Oldal> oldalak;
public:
    Kocka() {
        oldalak.resize(7);
        Oldal temp1 = Oldal(1);
        Oldal temp2 = Oldal(2);
        Oldal temp3 = Oldal(3);
        Oldal temp4 = Oldal(4);
        Oldal temp5 = Oldal(5);
        Oldal temp6 = Oldal(6);
        oldalak[FRONT] = temp2;
        oldalak[UP] = temp1;
        oldalak[RIGHT] = temp3;
        oldalak[DOWN] = temp6;
        oldalak[LEFT] = temp4;
        oldalak[BACK] = temp5;
    }

    Kocka& operator=(Kocka& k) {
        for (int i = 1; i <= 6; i++) {
            oldalak[i] = k[i];
        }
        return *this;
    }

    Oldal& operator[](int i) {
        return oldalak[i];
    }

    void forgatKivul(int oldal, bool irany = ORA_IRANY) {
        Kocka masolat = *this;
        switch (oldal)
        {
            case FRONT:
                oldalak[FRONT].forgat(irany);
                if (irany == ORA_IRANY) {
                    oldalak[UP].setSor(3, reverse(masolat[LEFT].getOszlop(3)));
                    oldalak[RIGHT].setOszlop(1, masolat[UP].getSor(3));
                    oldalak[DOWN].setSor(1, reverse(masolat[RIGHT].getOszlop(1)));
                    oldalak[LEFT].setOszlop(3, masolat[DOWN].getSor(1));
                } else {
                    oldalak[UP].setSor(3, masolat[RIGHT].getOszlop(1));
                    oldalak[RIGHT].setOszlop(1, reverse(masolat[DOWN].getSor(1)));
                    oldalak[DOWN].setSor(1, masolat[LEFT].getOszlop(3));
                    oldalak[LEFT].setOszlop(3, reverse(masolat[UP].getSor(3)));
                }
                break;
            case UP:
                oldalak[UP].forgat(irany);
                if (irany == ORA_IRANY) {
                    oldalak[FRONT].setSor(1, masolat[RIGHT].getSor(1));
                    oldalak[RIGHT].setSor(1, masolat[BACK].getSor(1));
                    oldalak[BACK].setSor(1, masolat[LEFT].getSor(1));
                    oldalak[LEFT].setSor(1, masolat[FRONT].getSor(1));
                } else {
                    oldalak[FRONT].setSor(1, masolat[LEFT].getSor(1));
                    oldalak[RIGHT].setSor(1, masolat[FRONT].getSor(1));
                    oldalak[BACK].setSor(1, masolat[RIGHT].getSor(1));
                    oldalak[LEFT].setSor(1, masolat[BACK].getSor(1));
                }
                break;
            case RIGHT:
                oldalak[RIGHT].forgat(irany);
                if (irany == ORA_IRANY) {
                    oldalak[FRONT].setOszlop(3, masolat[DOWN].getOszlop(3));
                    oldalak[UP].setOszlop(3, masolat[FRONT].getOszlop(3));
                    oldalak[BACK].setOszlop(1, reverse(masolat[UP].getOszlop(3)));
                    oldalak[DOWN].setOszlop(3, reverse(masolat[BACK].getOszlop(1)));
                } else {
                    oldalak[FRONT].setOszlop(3, masolat[UP].getOszlop(3));
                    oldalak[UP].setOszlop(3, reverse(masolat[BACK].getOszlop(1)));
                    oldalak[BACK].setOszlop(1, reverse(masolat[DOWN].getOszlop(3)));
                    oldalak[DOWN].setOszlop(3, masolat[FRONT].getOszlop(3));
                }
                break;
            case DOWN:
                oldalak[DOWN].forgat(irany);
                if (irany == ORA_IRANY) {
                    oldalak[FRONT].setSor(3, masolat[LEFT].getSor(3));
                    oldalak[LEFT].setSor(3, masolat[BACK].getSor(3));
                    oldalak[BACK].setSor(3, masolat[RIGHT].getSor(3));
                    oldalak[RIGHT].setSor(3, masolat[FRONT].getSor(3));
                } else {
                    oldalak[FRONT].setSor(3, masolat[RIGHT].getSor(3));
                    oldalak[RIGHT].setSor(3, masolat[BACK].getSor(3));
                    oldalak[BACK].setSor(3, masolat[LEFT].getSor(3));
                    oldalak[LEFT].setSor(3, masolat[FRONT].getSor(3));
                }
                break;
            case LEFT:
                oldalak[LEFT].forgat(irany);
                if (irany == ORA_IRANY) {
                    oldalak[FRONT].setOszlop(1, masolat[UP].getOszlop(1));
                    oldalak[UP].setOszlop(1, reverse(masolat[BACK].getOszlop(3)));
                    oldalak[BACK].setOszlop(3, reverse(masolat[DOWN].getOszlop(1)));
                    oldalak[DOWN].setOszlop(1, masolat[FRONT].getOszlop(1));
                } else {
                    oldalak[FRONT].setOszlop(1, masolat[DOWN].getOszlop(1));
                    oldalak[UP].setOszlop(1, masolat[FRONT].getOszlop(1));
                    oldalak[BACK].setOszlop(3, reverse(masolat[UP].getOszlop(1)));
                    oldalak[DOWN].setOszlop(1, reverse(masolat[BACK].getOszlop(3)));
                }
            case BACK:
                oldalak[BACK].forgat(irany);
                if (irany == ORA_IRANY) {
                    oldalak[UP].setSor(1, masolat[RIGHT].getOszlop(3));
                    oldalak[RIGHT].setOszlop(3, reverse(masolat[DOWN].getSor(3)));
                    oldalak[DOWN].setSor(3, masolat[LEFT].getOszlop(1));
                    oldalak[LEFT].setOszlop(1, reverse(masolat[UP].getSor(1)));
                } else {
                    oldalak[UP].setSor(1, reverse(masolat[LEFT].getOszlop(1)));
                    oldalak[RIGHT].setOszlop(3, masolat[UP].getSor(1));
                    oldalak[DOWN].setSor(3, reverse(masolat[RIGHT].getOszlop(3)));
                    oldalak[LEFT].setOszlop(1, masolat[DOWN].getSor(3));
                }
            default:
                break;
        }
    }

    void forgatBelul(int xyz, bool irany = ORA_IRANY) {
        Kocka masolat = *this;
        switch (xyz)
        {
        case X:
            if (irany = ORA_IRANY) {
                oldalak[FRONT].setOszlop(2, masolat[DOWN].getOszlop(2));
                oldalak[DOWN].setOszlop(2, reverse(masolat[BACK].getOszlop(2)));
                oldalak[BACK].setOszlop(2, reverse(masolat[UP].getOszlop(2)));
                oldalak[UP].setOszlop(2, masolat[FRONT].getOszlop(2));
            } else {
                oldalak[FRONT].setOszlop(2, masolat[UP].getOszlop(2));
                oldalak[UP].setOszlop(2, reverse(masolat[BACK].getOszlop(2)));
                oldalak[BACK].setOszlop(2, reverse(masolat[DOWN].getOszlop(2)));
                oldalak[DOWN].setOszlop(2, masolat[FRONT].getOszlop(2));
            }
            break;
        case Y:
            if (irany = ORA_IRANY) {
                oldalak[FRONT].setSor(2, masolat[RIGHT].getSor(2));
                oldalak[RIGHT].setSor(2, masolat[BACK].getSor(2));
                oldalak[BACK].setSor(2, masolat[LEFT].getSor(2));
                oldalak[LEFT].setSor(2, masolat[FRONT].getSor(2));
            } else {
                oldalak[FRONT].setSor(2, masolat[LEFT].getSor(2));
                oldalak[LEFT].setSor(2, masolat[BACK].getSor(2));
                oldalak[BACK].setSor(2, masolat[RIGHT].getSor(2));
                oldalak[RIGHT].setSor(2, masolat[FRONT].getSor(2));
            }
            break;
        case Z:
            if (irany = ORA_IRANY) {
                oldalak[UP].setSor(2, reverse(masolat[LEFT].getOszlop(2)));
                oldalak[LEFT].setOszlop(2, masolat[DOWN].getSor(2));
                oldalak[DOWN].setSor(2, reverse(masolat[RIGHT].getOszlop(2)));
                oldalak[RIGHT].setOszlop(2, masolat[UP].getSor(2));
            } else {
                oldalak[UP].setSor(2, masolat[RIGHT].getOszlop(2));
                oldalak[RIGHT].setOszlop(2, reverse(masolat[DOWN].getSor(2)));
                oldalak[DOWN].setSor(2, masolat[LEFT].getOszlop(2));
                oldalak[LEFT].setOszlop(2, reverse(masolat[UP].getSor(2)));
            }
            break;
        default:
            break;
        }
    }

    void displayFromSide(int side, bool refresh = false) {
        int currentFront, currentUp, currentRight, currentDown, currentLeft, currentBack;
        string focusSide;
        bool needToRotate = false;
        int arrowDir;
        switch (side)
        {
            case FRONT:
                focusSide = "FRONT";
                arrowDir = BACK;
                currentFront = FRONT;
                currentUp = UP;
                currentRight = RIGHT;
                currentDown = DOWN;
                currentLeft = LEFT;
                currentBack = BACK;
                break;
            case UP:
                focusSide = "UP";
                arrowDir = DOWN;
                currentFront = UP;
                currentUp = BACK;
                currentRight = RIGHT;
                currentDown = FRONT;
                currentLeft = LEFT;
                currentBack = DOWN;
                break;
            case RIGHT:
                focusSide = "RIGHT";
                arrowDir = LEFT;
                currentFront = RIGHT;
                currentUp = UP;
                currentRight = BACK;
                currentDown = DOWN;
                currentLeft = FRONT;
                currentBack = LEFT;
                needToRotate = true;
                break;
            case DOWN:
                focusSide = "DOWN";
                arrowDir = UP;
                currentFront = DOWN;
                currentUp = FRONT;
                currentRight = RIGHT;
                currentDown = BACK;
                currentLeft = LEFT;
                currentBack = UP;
                break;
            case LEFT:
                focusSide = "LEFT";
                arrowDir = RIGHT;
                currentFront = LEFT;
                currentUp = UP;
                currentRight = FRONT;
                currentDown = DOWN;
                currentLeft = BACK;
                currentBack = RIGHT;
                needToRotate = true;
                break;
            case BACK:
                focusSide = "BACK";
                arrowDir = FRONT;
                currentFront = BACK;
                currentUp = UP;
                currentRight = LEFT;
                currentDown = DOWN;
                currentLeft = RIGHT;
                currentBack = FRONT;
                break;
            default:
                break;
        };
        if (refresh) {
            system("clear");
        }
        cout << endl << LINE << endl << endl;
        cout << SPACE << "  FROM " << focusSide << endl << endl;
        Oldal tempUp = (*this)[currentUp];
        Oldal tempDown = (*this)[currentDown];
        if (needToRotate) {
            tempUp.forgat(ORA_IRANY);
            tempDown.forgat(ORA_FORDITOTT);
        }
        for (int i = 1; i <= 3; i++) {
            cout << SPACE << "  ";
            tempUp.printSor(i);
            cout << endl;
        }
        cout << endl;
        for (int i = 1; i <= 3; i++) {
            (*this)[currentLeft].printSor(i);
            (*this)[currentFront].printSor(i, true);
            (*this)[currentRight].printSor(i);
            (*this)[currentBack].printSor(i);
            cout << endl;
        }
        cout << endl;
        for (int i = 1; i <= 3; i++) {
            cout << SPACE << "  ";
            tempDown.printSor(i);
            cout << endl;
        }
        cout << endl << endl << LINE << endl << endl;
    }

    void scramble(int n) {
        int oldal;
        int xyz;
        bool irany;
        for (int i = 0; i < n; i++) {
            oldal = get_random_number(1, 6);
            xyz = get_random_number(1, 3);
            irany = get_random_number(0, 1);
            if (xyz == 1) {
                forgatKivul(oldal, irany);
            } else {
                forgatBelul(xyz, irany);
            }
        }
    }
};

struct Case {
    Kocka startingPosition;
    vt forgatasok;
    int index;
};

vector<Case> successfulCases;

void visualize_example(Case& mySelection)
{
    cout << endl << LINE << endl << endl;
    cout << SPACE << "VISUALIZING EXAMPLE " << mySelection.index << endl;
    cout << endl;
    cout << SPACE << "  STARTING POSITION" << endl;
    mySelection.startingPosition.displayFromSide(FRONT);
    Kocka k = mySelection.startingPosition;
    for (int i = 0; i < mySelection.forgatasok.size(); i++) {
        switch (mySelection.forgatasok[i].kulso)
        {
        case 0:
            k.forgatKivul(mySelection.forgatasok[i].oldal, mySelection.forgatasok[i].irany);
            break;
        case 1:
            k.forgatBelul(mySelection.forgatasok[i].xyz, mySelection.forgatasok[i].irany);
            break;
        default:
            break;
        }
        int side;
        cout << SPACE << "  TURN " << i + 1 << ": ";
        if (mySelection.forgatasok[i].kulso == 0) {
            side = mySelection.forgatasok[i].oldal;
            cout << "OUTSIDE ";
            switch (mySelection.forgatasok[i].oldal)
            {
            case FRONT:
                cout << "FRONT ";
                break;
            case UP:
                cout << "UP ";
                break;
            case RIGHT:
                cout << "RIGHT ";
                break;
            case DOWN:
                cout << "DOWN ";
                break;
            case LEFT:
                cout << "LEFT ";
                break;
            case BACK:
                cout << "BACK ";
                break;
            default:
                break;
            }
            if (mySelection.forgatasok[i].irany == ORA_IRANY) {
                cout << "CLOCKWISE";
            } else {
                cout << "COUNTERCLOCKWISE";
            }
        } else {
            side = 4 - mySelection.forgatasok[i].xyz;
            cout << "INSIDE ";
            switch (mySelection.forgatasok[i].xyz)
            {
            case X:
                cout << "X (-) ";
                break;
            case Y:
                side = UP;
                cout << "Y (|) ";
                break;
            case Z:
                side = FRONT;
                cout << "Z (.) ";
                break;
            default:
                break;
            }
            if (mySelection.forgatasok[i].irany == ORA_IRANY) {
                cout << "CLOCKWISE";
            } else {
                cout << "COUNTERCLOCKWISE";
            }
        }
        cout << endl;
        // k.displayFromSide(side);
        k.displayFromSide(FRONT);
    }
    cout << endl << SPACE << "  FINAL POSITION" << endl;
    k.displayFromSide(FRONT);
    cout << endl << SPACE << "  TOTAL TURNS: " << mySelection.forgatasok.size() << endl;
    cout << endl << LINE << endl << endl;
}

bool there_are_4_dots_on_any_3_sides(Kocka& k)
{
    int kedv = 0;
    for (int i = 1; i <= 6; i++) {
        if (k[i].get_nr_of_dots() == 4) {
            kedv++;
        }
    }
    return kedv == 3;
}

double chance(int n, int k, bool storeCases)
{
    int kedv = 0;
    int maxTurns = k;
    int counter = 0;
    for (int i = 0; i < n; i++) {
        Kocka k, startingPosition;
        k.scramble(rand() % 100);
        startingPosition = k;
        int j = 0;
        vt forgatasok;
        while (j < maxTurns) {
            int kulsoVagyBelso = get_random_number(0, 1);
            int oldal;
            int xyz;
            bool irany;
            if (kulsoVagyBelso == 0) {
                oldal = get_random_number(1, 6);
                irany = get_random_number(0, 1);
                Turn t = {kulsoVagyBelso, irany, oldal, 0};
                forgatasok.push_back(t);
                k.forgatKivul(oldal, irany);
            } else {
                xyz = get_random_number(1, 3);
                irany = get_random_number(0, 1);
                Turn t = {kulsoVagyBelso, irany, 0, xyz};
                forgatasok.push_back(t);
                k.forgatBelul(xyz, irany);
            }
            j++;
        }
        if (there_are_4_dots_on_any_3_sides(k)) {
            kedv++;
            if (storeCases) {
                Case c = {startingPosition, forgatasok, ++counter};
                successfulCases.push_back(c);
            }
        }
    }
    return (double)kedv / n;
}

int main(int argc, char** argv)
{
    system("clear");
    int n = 10000;
    int k = 10;
    if (argc == 3) {
        n = atoi(argv[1]);
        k = atoi(argv[2]);
    } else {
        cout << "No argument given, using default value of n = 10000 and k = 10." << endl;
    }
    bool storeCases = n <= 100000;
    srand(time(NULL));
    cout << "The probability of 3 sides having 4 dots after " << k;
    cout << "turns from a random starting position is: " << endl;
    cout << "n = " << n << ", k = " << k << endl;
    cout << fixed << setprecision(3) << chance(n,k,storeCases) * 100 << "%" << endl;
    cout << endl << LINE << endl << endl;
    if (!storeCases) return 0;
    while (true) {
        cout << "Would you like to visualize an example? (y/n)" << endl;
        char c;
        cin >> c;
        if (c == 'y' || c == 'Y') {
            cout << "There are " << successfulCases.size() << " successful cases." << endl;
            cout << "Which one would you like to visualize?" << endl;
            int index;
            cin >> index;
            visualize_example(successfulCases[index - 1]);
        } else {
            break;
        }
    }
    return 0;
}
