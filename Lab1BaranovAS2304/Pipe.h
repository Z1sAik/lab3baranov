﻿#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
using namespace std;

class Pipe
{
private:
    int id;
    static int maxID;
    string Name; //название трубы
    float length; //километры 
    int diameter; //миллиметры
    bool repair; //признак "в ремонте"

public:
    Pipe() : id(0), Name(""), length(0.0f), diameter(0), repair(false) {
    } 

    Pipe(int P_id, string P_Name, float P_length, int P_diameter, bool P_repair){
        id = P_id;
        Name = P_Name;
        length = P_length;
        diameter = P_diameter;
        repair = P_repair;
    }

    int getID()
    {
        return id;
    }
    
    friend ostream& operator << (ostream& out, const Pipe& P);
    friend istream& operator >> (istream& in, Pipe& P);
    friend ofstream& operator << (ofstream& fout, const Pipe& P);
    friend ifstream& operator >> (ifstream& fin, Pipe& P);
    static void resetMaxID();
    static Pipe newPipe();
    void editPipe();
    string getName() const {
        return Name;
    }

    bool getRepair() const {
        return repair;
    }

    static int getmaxID() {
        return maxID;
    }

    static void setmaxID(int newID) {
        maxID = newID;
    }
};
