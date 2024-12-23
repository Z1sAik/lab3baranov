#include "functions.h"
#include "CS.h"
#include "Pipe.h"
#include <iostream>
#include <fstream>

using namespace std;

int compressor_station::maxID = 0;

std::ostream& operator<<(std::ostream& out, const compressor_station& CS) {
    out << "ID: " << CS.id
        << "; Имя: " << CS.Name 
        << "; Кол-во цехов: " << CS.workshops
        << "; Кол-во цехов в работе: " << CS.workshopsinwork 
        << "; Эффективности: " << CS.effectiveness << "%" << endl;
    return out;
}

std::ofstream& operator<<(std::ofstream& fout, const compressor_station& CS) {
    if (fout.is_open()) {
        fout << CS.id << endl
            << CS.Name << endl
            << CS.workshops << endl
            << CS.workshopsinwork << endl
            << CS.effectiveness << endl;
    }
    return fout;
}

std::istream& operator>>(std::istream& in, compressor_station& CS) {
    CS.id = ++CS.maxID;
    cout << "Создание новой КС" << endl;
    cout << "ID: " << CS.id << endl;
    cout << "Введите название компрессорной станции (на английском язык): ";
    CS.Name = get_line(in);
    cout << "Введите кол-во цехов: ";
    CS.workshops = check<int>(0, INT_MAX);
    cout << "Введите кол-во цехов в работе: ";
    CS.workshopsinwork = check<int>(0, CS.workshops);
    cout << "Введите коэффициент эффективности КС(от 0 до 100): ";
    CS.effectiveness = check<int>(0, 100);
    return in;
}

std::ifstream& operator>>(std::ifstream& fin, compressor_station& CS) {
    if (fin.is_open()) {
        fin >> CS.id;
        fin.ignore();
        getline(fin >> ws, CS.Name);
        fin >> CS.workshops;
        fin >> CS.workshopsinwork;
        fin >> CS.effectiveness;
    }
    return fin;
}

compressor_station compressor_station::newCS()
{
    compressor_station CS;
    cin >> CS;
    return CS;
}

void compressor_station::resetMaxID()
{
    maxID = 0;
}

bool compressor_station::runworkshop()
{
    if (workshopsinwork < workshops)
    {
        workshopsinwork +=1;
        return 1;
    }
    else
        return 0;
}

bool compressor_station::stopworkshop()
{
    if (workshopsinwork > 0)
    {
        workshopsinwork -=1;
        return 1;
    }
    else
        return 0;
}

int compressor_station::Get_Id_in() const {
    return id_in;
}

int compressor_station::Get_Id_out() const {
    return id_out;

}

void compressor_station::Set_Id_in(int in) {
    id_in = in;
}

void compressor_station::Set_Id_out(int out) {
    id_out = out;
}

