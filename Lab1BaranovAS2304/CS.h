#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
using namespace std;
class compressor_station 
{
private:
    int id;
    static int maxID;
    string Name; //название КС
    int workshops; //Кол-во цехов
    int workshopsinwork; //Кол-во цехов в работе
    int effectiveness; //Коэфф. эффективности

public:
    compressor_station() :id(0), Name(""), workshops(0), workshopsinwork(0), effectiveness(0) {
    }
    compressor_station(int CS_id, string CS_Name, int CS_workshops, int CS_workshopsinwork, int CS_effectiveness) {

        id = CS_id;
        Name = CS_Name;
        workshops = CS_workshops;
        workshopsinwork = CS_workshopsinwork;
        effectiveness = CS_effectiveness;
    }
    static compressor_station newCS();
    friend ostream& operator << (ostream& out, const compressor_station& cs);
    friend istream& operator >> (istream& in, compressor_station& cs);
    friend ofstream& operator << (ofstream& fout, const compressor_station& cs);
    friend ifstream& operator >> (ifstream& fin, compressor_station& cs);
    static void resetMaxID();
    bool runworkshop();
    bool stopworkshop();
    int getID()
    {
        return id;
    }
    
    string getName() const {
        return Name;
    }

    float notinwork() const {
        if (workshops == 0) {
            return 0.0f;
        }
        return ((float)(workshops - workshopsinwork) / workshops) * 100;
    }

    static int getmaxID() {
        return maxID;
    }

    static void setmaxID(int newID) {
        maxID = newID;
    }
};