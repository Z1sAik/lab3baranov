#include "functions.h"
#include "Pipe.h"
#include <iostream>
#include <fstream>

using namespace std;

int Pipe::maxID = 0;

std::ostream& operator<<(std::ostream& out, const Pipe& P) {
    out << "ID: " << P.id  
        << "; Имя: " << P.Name 
        << "; Длина: " << P.length 
        << "; Диаметр: " << P.diameter 
        << "; Статус 'в ремонте': " << boolalpha << P.repair << endl;
    return out;
}

std::ofstream& operator<<(std::ofstream& fout, const Pipe& P) {
    if (fout.is_open()) {
        fout << P.id << endl
            << P.Name << endl
            << P.length << endl
            << P.diameter << endl
            << P.repair << endl;
    }
    return fout;
}

std::istream& operator>>(std::istream& in, Pipe& P) {
    P.id = ++P.maxID;
    cout << "Создание новой трубы" << endl;
    cout << "ID: " << P.id << endl;
    cout << "Введите название трубы (на английском язык): ";
    P.Name = get_line(in);
    cout << "Введите длину трубы в километрах: ";
    P.length = check<float>(0.1, INT_MAX);
    cout << "Введите диаметр трубы в миллиметрах: ";
    P.diameter = check<int>(1, INT_MAX);
    cout << "Выберите в каком состоянии труба: " << endl << "0)Не в ремонте" << endl << "1)В ремонте" << endl;
    P.repair = check<int>(0, 1);
    return in;
}

std::ifstream& operator>>(std::ifstream& fin, Pipe& P) {
    if (fin.is_open()) {
        fin >> P.id;
        fin.ignore();
        getline(fin >> ws, P.Name);
        fin >> P.length;
        fin >> P.diameter;
        fin >> P.repair;
    }
    return fin;
}

Pipe Pipe::newPipe()
{
    Pipe P;
    cin >> P;
    return P;
}

void Pipe::resetMaxID()
{
    maxID = 0;
}

void Pipe::editPipe()
{
    repair = !repair;
}



