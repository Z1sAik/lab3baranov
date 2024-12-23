#include <iostream>
#include <string>
#include <fstream>
#include "Pipe.h"
#include "CS.h"
#include <unordered_map>
#include "functions.h"
#include <format>
#include <chrono>

using namespace std;
using namespace chrono;

int main() {
    redirect_output_wrapper cerr_out(cerr);
    string time = std::format("{:%d_%m_%Y %H_%M_%OS}", system_clock::now());
    ofstream logfile("log_" + time +".txt");
    if (logfile)
        cerr_out.redirect(logfile);

    setlocale(LC_ALL, "RU"); 
    unordered_map<int, Pipe> Pipes = {};
    unordered_map<int, compressor_station> Stations = {};
    while (true) {
        int k = menu();
        if (k == 0) {
            break;
        }
        else if (k == 1) {
            Pipe P = Pipe::newPipe();
            Pipes.emplace(P.getID(), P);
        }
        else if (k == 2) {
            compressor_station CS = compressor_station::newCS();
            Stations.emplace(CS.getID(), CS);
        }
        else if (k == 3) {
            if (Pipes.empty() && Stations.empty()) {
                cout << "Вы ещё не добавили ни одного объекта" << endl;
            }
            else
            {
                cout << "Все трубы:" << endl; 
                view(Pipes);
                cout << "Все компрессорные станции:" << endl; 
                view(Stations);
            }
        }
        else if (k == 4) {
            selectObjects(Pipes);
        }
        else if (k == 5) {
            selectObjects(Stations);
        }
        else if (k == 6) {
            save(Pipes, Stations);
        }
        else if (k == 7) {
            load(Pipes, Stations);
        }
        else if (k == 8) {
            deleteAll(Pipes, Stations);
        }
    }
    return 0;
}