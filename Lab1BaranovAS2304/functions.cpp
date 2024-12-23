#include "functions.h"
#include "CS.h"
#include "Pipe.h"
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

int menu() {
    int k = -1;
    while (true) {
        cout << "///////  Меню  ///////" << endl
            << "1) Добавить трубу" << endl
            << "2) Добавить КС" << endl
            << "3) Просмотр объектов" << endl
            << "4) Редактировать трубу" << endl
            << "5) Редактировать КС" << endl
            << "6) Сохранить данные" << endl
            << "7) Загрузить данные" << endl
            << "8) Удаление объектов" << endl
            << "0) Выход" << endl
            << "Введите команду которую вы бы хотели выполнить(от 0 до 8): ";
        k = check<int>(0, 9);
        return k;
    }
}

string get_line(istream& in) {
    string input;
    getline(in >> ws, input);
    cerr << input << endl;
    return input;
}

void save(unordered_map<int, Pipe>& Pipes, unordered_map<int, compressor_station>& Stations) {
    string filename;
    cout << "Введите имя файла для сохранения: ";
    filename = get_line(cin);
    filename += ".txt";
    ofstream out;
    out.open(filename);
    if (out.is_open()) {
        out << "data Pipe:" << endl;
        saveMap(out, Pipes);
        out << "end data" << endl;
        out << "data CS:" << endl;
        saveMap(out, Stations);
        out << "end data" << endl;
        cout << "Данные сохранены в файл: " << filename << endl;
    }
    else {
        cout << "Ошибка при открытии файла для записи!" << endl;
    }
    out.close();
}

void load(unordered_map<int, Pipe>& Pipes, unordered_map<int, compressor_station>& Stations) {
    string filename;
    cout << "Введите имя файла для загрузки: ";
    filename = get_line(cin);

    if (filename.find(".txt") == string::npos) {
        filename += ".txt";
    }

    ifstream in(filename);
    if (!in.is_open()) {
        cout << "Файл " << filename << " не найден!" << endl;
        return;
    }
    Pipe::resetMaxID();
    compressor_station::resetMaxID();
    Stations.clear();
    Pipes.clear();
    string line;
    while (getline(in >> ws, line)) {
        if (line == "data Pipe:") {
            loadMap(in, Pipes);
        }
        if (line == "data CS:") {
            loadMap(in, Stations);
        }
    }
    if (!Pipes.empty() || !Stations.empty()) {
        cout << "Данные загружены из файла: " << filename << endl;
    }

    in.close();
}

bool filterByNameP(const Pipe& P, string name) {
    size_t pos = P.getName().find(name);
    return pos != string::npos;
}

bool filterByRepairP(const Pipe& P, int repair) {
    return P.getRepair() == repair;
}

bool filterByNameCS(const compressor_station& CS, string name) {
    size_t pos = CS.getName().find(name);
    return pos != string::npos;
}

bool filterByWork(const compressor_station& CS, float work) {
    return CS.notinwork() == work;
}

unordered_set<int> selectByChosenFilter(unordered_map<int, compressor_station>& Stations) {
    unordered_set<int> res;
    cout << "Выберите фильтр:" << endl << "1) По имени" << endl << "2) По проценту нерабочих цехов (0–100): " << endl << "0) Выход" << endl;
    int m = 0;
    while (true) {
        m = check<int>(0, 2);
        if (m == 0) {
            break;
        }
        else if (m == 1) {
            cout << "Введите часть имени: ";
            string name = get_line(cin);
            res = findwithFilter(Stations, filterByNameCS, name);
            break;
        }
        else if (m == 2) {
            cout << "Введите процент нерабочих цехов (0–100): "; float status = check<float>(0, 100);
            res = findwithFilter(Stations, filterByWork, status);
            break;
        }
    }
    return res;
}

unordered_set<int> selectByChosenFilter(unordered_map<int, Pipe>& Pipes) {
    unordered_set<int> res;
    cout << "Выберите фильтр:" << endl << "1) По имени" << endl << "2) По статус 'в ремонте'" <<endl << "0) Выход" << endl;
    int m = 0;
    while (true) {
        m = check<int>(0, 2);
        if (m == 0) {
            break;
        }
        else if (m == 1) {
            cout << "Введите часть имени: ";
            string name = get_line(cin);
            res = findwithFilter(Pipes, filterByNameP, name);
            break;
        }
        else if (m == 2) {
            cout << "Введите статус 'в ремонте'(0 = Не в ремонте, 1 = В ремонте): "; int status = check<int>(0, 1);
            res = findwithFilter(Pipes, filterByRepairP, status);
            break;
        }
    }
    return res;
}

void edit(unordered_map<int, Pipe>& Pipes, int id)
{
    auto it = Pipes.find(id);
    it->second.editPipe();
    if (!Pipes.empty()) {
        cout << "Статус 'в ремонте был изменён!" << endl;
    }
    else {
        cout << "Нет объектов для редактирования!" << endl;
    }
}

void deleteAll(unordered_map<int, Pipe>& Pipes, unordered_map<int, compressor_station>& Stations){
    if (Pipes.empty() && Stations.empty())
    {
        cout << "Вы ещё не добавили ни одного объекта!" << endl;
        return;
    }
    else {
        Pipe::resetMaxID();
        compressor_station::resetMaxID();
        Pipes.clear();
        Stations.clear();
        cout << "Все объекты были удалены" << endl;
    }
    return;
}


void edit(unordered_map<int, compressor_station>& Stations, int id) {
    if (!Stations.empty()) {
        cout << "Что вы хотите сделать с КС" << endl
            << "1) Запустить 1 цех" << endl
            << "2) Остановить 1 цех" << endl
            << "0) Выход" << endl;
    }
    else {
        cout << "Нет объектов для редактирования!" << endl;
    }
    int m = 0;
    while (true) {
        m = check<int>(0, 2);
        if (m == 0) {
            break;
        }
        if (m == 1) {
                auto it = Stations.find(id);
                it->second.runworkshop();
                cout << "Увеличено кол-во цехов в работе на один у КС c ID: " << id << endl;
                
        }
        if (m == 2) {
                auto it = Stations.find(id);
                it->second.stopworkshop();
                cout << "Невозможно изменить CS " << id << endl;
        }
    }
    if (!Stations.empty()) {
        cout << "Редактирование выполнено!" << endl;
    }
}