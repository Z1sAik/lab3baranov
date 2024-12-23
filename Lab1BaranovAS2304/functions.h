#pragma once
#include "CS.h"
#include "Pipe.h"
#include "connections.h"
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
using namespace std;

template <typename T, typename Param>
using Filter = bool(*)(const T&, Param);

template <typename T>
T check(T low, T max) {
    T z;
    cin >> z;
    cin.ignore();
    while (true) {
        cerr << z << endl;
        if (cin.fail()) {
            cout << "Ошибка. Введено не целое число или символ! Попробуйте ещё раз: ";
            cerr << z << endl;
            cin.clear();
            while (cin.get() != '\n');
        }
        else if (z < low) {
            cout << "Ошибка. Введено число меньше минимального! Попробуйте ещё раз: ";
            cerr << z  << endl;
        }
        else if (z > max) {
            cout << "Ошибка. Введено число больше максимального! Попробуйте ещё раз: ";
            cerr << z << endl;
        }
        else {
            break;
        }
        cin >> z;
        cin.ignore();
    }
    return z;
}

template <typename T>
void view(const unordered_map<int, T>& map) {
    if (!map.empty()) {
        for (auto& [id, val] : map)
        {
            cout << val;
        }
    }
    else {
        cout << "Вы ещё не добавили ни одного объекта!" << endl;
    }
}

template<typename T>
void saveMap(ofstream& fout, const unordered_map<int, T>& map)
{
	for (auto& [id, val] : map)
	{
		fout << val;
	}
}

template<typename T>
void loadMap(ifstream& fin, unordered_map<int, T>& map) {
    T val;
    while (fin >> val) {
        int id = val.getID();
        map.emplace(id, val);
        if (id > T::getmaxID()) {
            T::setmaxID(id);
        }
    }
    fin.clear();
}

bool filterByNameP(const Pipe& P, string name);
bool filterByRepairP(const Pipe& P, int repair);
bool filterByNameCS(const compressor_station& CS, string name);
bool filterByWork(const compressor_station& CS, float work);

template<typename T, typename U>
unordered_set<int> findwithFilter(const unordered_map<int, T>& map, bool (*f)(const T&, U), U param)
{
    unordered_set<int> res;
    for (const auto& [id, val] : map) {
        if (f(val, param)) {
            res.emplace(id);
        }
    }
    return res;
}

template<typename T>
unordered_set<int> selectByChosenID(unordered_map<int, T>& map, unordered_set<int>& set = {})
{
    unordered_set<int> res;
    cout << "Введите нужные вам ID (для остановки введите -1)" << endl;

    while (true)
    {
        int id = check<int>(-1, INT_MAX);
        if (id == -1)
            break;

        if (set.size() == 0)
        {
            if (map.contains(id)) {
                res.emplace(id);
                cout << "ID " << id << " добавлен в фильтр" << endl;
            }
            else {
                cout << "ID " << id << " не найдено во всех id" << endl;
            }
        }
        else
        {
            if (map.contains(id) && set.contains(id)) {
                res.emplace(id);
                cout << "ID " << id << " добавлен в фильтр" << endl;
            }
            else {
                if (!map.contains(id)) {
                    cout << "ID " << id << " не найдено во всех id" << endl;
                }
                if (!set.contains(id)) {
                    cout << "ID " << id << " не найдено в фильтре" << endl;
                }
            }
        }
    }
    if (res.empty()) {
        cout << "Не найдено ни одного подходящего ID." << endl;
    }
    return res;
}

template<typename T>
void editSelected(unordered_map<int, T>& map, unordered_set<int>& set) {
    int m = 0;
    while (true) {
        cout << "Выберите что вы хотите сделать:" << endl << "1) Посмотреть что содержится в фильтре" << endl << "2) Отредактировать отфильтрованные объекты" << endl << "3) Удалить отфильтрованные объекты" << endl << "0) Выход" << endl << "Введите команду: ";
        int m = check<int>(0, 3);
        if (m == 0) {
            break;
        }
        if (m == 1) {
            for (int id : set)
                cout << map[id];
        }
        if (m == 2) {
            for (int id : set)
            edit(map,id);
        }
        if (m == 3) {
            for (int id : set)
                map.erase(id);
        }
    }
}

template<typename T>
void selectObjects(unordered_map<int, T>& map) {
    unordered_set<int> res;
    if (map.empty()) {
        cout << "Вы ещё не добавили ни одного объекта!" << endl;
        return;
    }

    cout << "Все объекты: " << endl;
    view(map);
    cout << "Выберите как вы хотите отфильтровать объекты?" << endl << "1) По id" << endl << "2) По характеристикам" << endl << "Введите команду: ";
    int m = check<int>(1, 2);
    if (m == 1)
        res = selectByChosenID(map, res);
    else if (m == 2)
        res = selectByChosenFilter(map);
    if (res.size() != 0) {
        cout << "Найденные объекты с ID: ";
        for (int id : res)
            cout << id << " ";
        cout << endl;
        cout << "Вы хотите выбрать все или по ID?" << endl << "1) Вcе объекты" << endl << "2) По ID" << endl << "Введите команду: ";
        int choice = check<int>(1, 2);
        if (choice == 2) {
            res = selectByChosenID(map, res);
        }
        if (res.size() != 0)
            editSelected(map, res);
        else
            cout << "Вы ещё не добавили ни одного объекта!" << endl;
    }
    else {
        cout << "Вы ещё не добавили ни одного объекта!" << endl;
    }
}

class redirect_output_wrapper
{
    std::ostream& stream;
    std::streambuf* const old_buf;
public:
    redirect_output_wrapper(std::ostream& src)
        :old_buf(src.rdbuf()), stream(src)
    {
    }

    ~redirect_output_wrapper() {
        stream.rdbuf(old_buf);
    }
    void redirect(std::ostream& dest)
    {
        stream.rdbuf(dest.rdbuf());
    }
};

int menu();
string get_line(istream& in);
void save(unordered_map<int, Pipe>& Pipes, unordered_map<int, compressor_station>& Stations, unordered_map<int, connections>& Conns);
void load(unordered_map<int, Pipe>& Pipes, unordered_map<int, compressor_station>& Stations, unordered_map<int, connections>& Conns);
unordered_set<int> selectByChosenFilter(unordered_map<int, Pipe>& Pipes);
unordered_set<int> selectByChosenFilter(unordered_map<int, compressor_station>& Stations);
void edit(unordered_map<int, compressor_station>& Stations, int id);
void edit(unordered_map<int, Pipe>& Pipes, int id);
