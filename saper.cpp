#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

int razmer_polya = 9;
int kol_minn = 10;

vector<vector<char>> pole;
vector<vector<char>> mini;

bool proverka_granic(int x, int y) {
    return x >= 0 && x < razmer_polya && y >= 0 && y < razmer_polya;
}

int schitat_miny(int x, int y) {
    int count = 0;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            int nx = x + dx, ny = y + dy;
            if (proverka_granic(nx, ny) && mini[nx][ny] == '*') {
                count++;
            }
        }
    }
    return count;
}

void raspolozhit_miny(int perv_x, int perv_y) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, razmer_polya - 1);

    int razmestili = 0;
    while (razmestili < kol_minn) {
        int x = dis(gen), y = dis(gen);
        if ((x != perv_x || y != perv_y) && mini[x][y] != '*') {
            mini[x][y] = '*';
            razmestili++;
        }
    }

    for (int i = 0; i < razmer_polya; i++) {
        for (int j = 0; j < razmer_polya; j++) {
            if (mini[i][j] != '*') {
                int c = schitat_miny(i, j);
                mini[i][j] = c > 0 ? c + '0' : '0';
            }
        }
    }
}

void otkryt_pustye(int x, int y) {
    if (!proverka_granic(x, y) || pole[x][y] != '.') return;
    pole[x][y] = mini[x][y];
    if (mini[x][y] == '0') {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                otkryt_pustye(x + dx, y + dy);
            }
        }
    }
}

bool proverit_pobedu() {
    for (int i = 0; i < razmer_polya; i++) {
        for (int j = 0; j < razmer_polya; j++) {
            if (mini[i][j] != '*' && pole[i][j] == '.') {
                return false;
            }
        }
    }
    return true;
}

void pokazat_pole() {
    cout << "   ";
    for (int i = 0; i < razmer_polya; i++) {
        cout << setw(2) << i << " ";
    }
    cout << endl;
    cout << "  +" << string(razmer_polya * 3, '-') << endl;
    for (int i = 0; i < razmer_polya; i++) {
        cout << setw(2) << i << "| ";
        for (int j = 0; j < razmer_polya; j++) {
            cout << pole[i][j] << "  ";
        }
        cout << endl;
    }
}

int main() {
    cout << "Dobro pozhalovat v igru Saper!" << endl;
    cout << "Vyberite slozhnost:" << endl;
    cout << "1. Legkaya (9x9)" << endl;
    cout << "2. Srednyaya (16x16)" << endl;
    cout << "3. Slozhnaya (24x24)" << endl;
    cout << "Vvedite nomer slozhnosti: ";

    int vybor;
    cin >> vybor;
    cin.ignore();

    if (vybor == 2) {
        razmer_polya = 16;
        kol_minn = 40;
    }
    else if (vybor == 3) {
        razmer_polya = 24;
        kol_minn = 99;
    }
    else {
        razmer_polya = 9;
        kol_minn = 10;
    }

    pole.assign(razmer_polya, vector<char>(razmer_polya, '.'));
    mini.assign(razmer_polya, vector<char>(razmer_polya, '0'));

    cout << "Pole: " << razmer_polya << "x" << razmer_polya << ", min: " << kol_minn << endl;
    cout << "Vvedite khod v formate: stroka stolbec [f]" << endl;
    cout << "Dobav'te 'f' ili 'f', chtoby postavit/ubrat flazhok." << endl;

    bool perviy_hod = true;
    bool igra_zakonchena = false;

    while (!igra_zakonchena) {
        pokazat_pole();
        cout << "\nVash khod: ";
        string vhod;
        getline(cin, vhod);
        istringstream iss(vhod);
        int x, y;
        char deistvie = 'o';

        if (!(iss >> x >> y)) {
            cout << "Oshibka vvoda. Povtorite popytku." << endl;
            continue;
        }

        if (iss >> deistvie) {
            deistvie = tolower(deistvie);
        }

        if (!proverka_granic(x, y)) {
            cout << "Koordinaty vne polya!" << endl;
            continue;
        }

        if (deistvie == 'f') {
            if (pole[x][y] == '.') {
                pole[x][y] = 'F';
            }
            else if (pole[x][y] == 'F') {
                pole[x][y] = '.';
            }
            else {
                cout << "Nel'zya stavit flazhok na otkrytuyu kletku." << endl;
            }
        }
        else {
            if (perviy_hod) {
                raspolozhit_miny(x, y);
                perviy_hod = false;
            }

            if (pole[x][y] == 'F') {
                cout << "Uberite flazhok, chtoby otkryt kletku." << endl;
                continue;
            }

            if (mini[x][y] == '*') {
                pole[x][y] = '*';
                pokazat_pole();
                cout << "\nVi popali na minu! Igra okonchena." << endl;
                igra_zakonchena = true;
            }
            else {
                otkryt_pustye(x, y);
                if (proverit_pobedu()) {
                    pokazat_pole();
                    cout << "\nPozdravlyaem! Vi pobedili!" << endl;
                    igra_zakonchena = true;
                }
            }
        }
    }

    system("pause");
    return 0;
}
