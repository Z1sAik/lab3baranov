#include "Connect.h"
#include "Pipe.h"
#include "CS.h"
#include "functions.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include "Connections.h"


void GTS::ConnectInGTS(unordered_map<int, Pipe>& Pipes, unordered_map<int, compressor_station>& Stations, unordered_map<int, connections>& Connections) {
	if (Stations.size() == 0 || Pipes.size() == 0)
	{
		cout << "Недостаточно КС или труб!" << endl;
		return;
	}

	vector<int> free_cs = GetFreeCompressorStations(Stations);
	if (free_cs.size() < 2)
	{
		cout << "У вас нет незанятых КС!" << endl;
		return;
	}

	vector<int> free_pipes = GetFreePipes(Pipes);
	if (free_pipes.size() < 1)
	{
		cout << "У вас нет незанятых труб!" << endl;
		return;
	}

	connections pipe_connect;
	pipe_connect.id_entry = GetCompressorStationId(free_cs, "входной");

	pipe_connect.id_out = GetCompressorStationId(free_cs, "выходной", pipe_connect.id_entry);

	pipe_connect.id_pipe = GetPipeId(free_pipes, Pipes);

	Connections[pipe_connect.id_entry] = pipe_connect;
}

vector<int> GTS::GetFreeCompressorStations(unordered_map<int, compressor_station>& Stations)
{
	vector<int> free_cs;
	for (auto& elem : Stations)
	{
		if (elem.second.Get_Id_in() + elem.second.Get_Id_out() < 2)
		{
			free_cs.push_back(elem.first);
		}
	}
	return free_cs;
}

vector<int> GTS::GetFreePipes(unordered_map<int, Pipe>& Pipes)
{
	vector<int> free_pipes;
	for (auto& elem : Pipes)
	{
		if (elem.second.Get_free())
		{
			free_pipes.push_back(elem.first);
		}
	}
	return free_pipes;
}

int GTS::GetCompressorStationId(const vector<int>& free_cs, const string& type, int exclude_id)
{
	int cs_id;
	while (true)
	{
		cout << "Введите Id " << type << " КС: ";
		cs_id = check<int>(0, INT_MAX);
		if (find(free_cs.begin(), free_cs.end(), cs_id) != free_cs.end() && cs_id != exclude_id)
		{
			return cs_id;
		}
		else
		{
			cout << "Нет свободной КС с таким Id или неправильный Id, попробуйте еще раз: ";
		}
	}
}

int GetCorrectDiameter(int min, int max)
{
	int diam;
	cin >> diam;
	while (((diam != 500) && (diam != 700) && (diam != 1000) && (diam != 1400)) || diam < min || diam > max || cin.fail() || cin.peek() != '\n')
	{
		cin.clear();
		cin.ignore(1000000, '\n');
		cout << "Пожалуйста введите корректный диаметр для трубы [500, 700, 1000, 1400]: ";
		cin >> diam;
	}
	cerr << diam << endl;
	return diam;
}

int GTS::GetPipeId(const vector<int>& free_pipes, unordered_map<int, Pipe>& Pipes) {
	int pipe_diameter;
	while (true)
	{
		cout << "Введите диаметр труб, которые вы хотите использовать (500, 700, 1000, 1400): ";
		pipe_diameter = GetCorrectDiameter(500, 1400);
		for (auto& idp : free_pipes)
		{
			if (Pipes[idp].GetDiameter() == pipe_diameter)
			{
				Pipes[idp].Set_free(0);
				return idp;
			}
		}

		cout << "Нет свободных труб с таким диаметром!" << endl;
		int new_id_pipe = AddNewPipeConnect(Pipes, pipe_diameter);
		return new_id_pipe;
	}
}


int GTS::AddNewPipeConnect(unordered_map<int, Pipe>& Pipes, int diameter)
{
	cout << "Отсутствующий диаметр будет установлен:" << endl;
	Pipe P;
	cin >> P;
	P.SetDiameter(diameter);
	int new_pipe_id = P.getID();
	Pipes.insert({ new_pipe_id,P});
	return new_pipe_id;
}

void GTS::ShowGTS(unordered_map<int, connections>& Connections){
	if (Connections.empty())
	{
		cout << "Отсутствуют соединения!" << endl;
	}
	else
	{
		for (const auto& conn : Connections) {
			const connections& conn0 = conn.second;

			if (conn0.id_entry != 0 && conn0.id_pipe != 0 && conn0.id_out != 0) {
				cout << "ID ВХОДНОЙ  КС: " << conn0.id_entry << "; ID ТРУБЫ: " << conn0.id_pipe << "; ID ВЫХОДНОЙ КС: " << conn0.id_out << endl;
			}
		}
	}
}

void GTS::DeleteConnection(unordered_map<int, connections>& Connections, unordered_map<int, Pipe>& Pipes)
{
	cout << "Все трубы:" << endl;
	view(Pipes);
	cout << "Пожалуйста, введите id трубы, от которой хотите разорвать соединение: ";
	int idp = check<int>(1, int(Pipes.size()));
	for (auto it = Connections.begin(); it != Connections.end();)
	{
		if (it->second.id_pipe == idp)
		{
			it = Connections.erase(it);
		}
		else
		{
			++it;
		}
	}
}
void GTS::dfs(int stationId, unordered_map<int, connections>& Connections, unordered_map<int, int>& state, vector<int>& result, bool& hasCycle) {
	if (state[stationId] == 1) {
		hasCycle = true;
		return;
	}
	if (state[stationId] == 0) {
		state[stationId] = 1;
		for (auto& elem : Connections) {
			if (elem.second.id_entry == stationId) {
				dfs(elem.second.id_out, Connections, state, result, hasCycle);
			}
		}
		state[stationId] = 2;
		result.push_back(stationId);
	}
}

vector<int> GTS::topologSort(unordered_map<int, Pipe>& Pipes, unordered_map<int, compressor_station>& Stations, unordered_map<int, connections>& Connections)
{
	vector<int> result;
	unordered_map<int, int> enterEdges;
	unordered_map<int, int> outEdges;
	unordered_map<int, int> state;
	bool hasCycle = false;

	for (auto& elem : Connections)
	{
		if (elem.second.id_out != 0)
			enterEdges[elem.second.id_out]++;
	}
	for (auto& elem : Connections)
	{
		if (elem.second.id_entry != 0)
			outEdges[elem.second.id_entry]++;
	}

	for (auto& elem : Stations) {
		state[elem.first] = 0;
	}

	for (auto& elem : Stations) {
		if (enterEdges.find(elem.first) == enterEdges.end() && outEdges.find(elem.first) != outEdges.end()) {
			dfs(elem.first, Connections, state, result, hasCycle);
		}
	}

	for (auto& elem : Stations) {
		if (state[elem.first] == 0) {
			dfs(elem.first, Connections, state, result, hasCycle);
		}
	}

	if (hasCycle) {
		return {};
	}

	reverse(result.begin(), result.end());
	return result;
}

void GTS::delPipe(unordered_map<int, Pipe>& Pipes, unordered_map<int, connections>& Connections, unordered_map<int, compressor_station>& Stations) {
	if (Pipes.size() == 0) {
		cout << "0 труб доступно!" << endl;
	}
	else {
		cout << "Все трубы:" << endl;
		view(Pipes);
		cout << "Введите ID (какую трубу вы хотите удалить): ";
		int key0;
		key0 = check<int>(1, findMaxId(Pipes));
		for (auto it = Connections.begin(); it != Connections.end(); ) {
			if (it->second.id_pipe == key0) {

				int id_entry = it->second.id_entry;
				int id_out = it->second.id_out;

				Stations[id_entry].Set_Id_in(Stations[id_entry].Get_Id_in() - 1);
				Stations[id_out].Set_Id_out(Stations[id_out].Get_Id_out() - 1);
				it = Connections.erase(it);
			}
			else {
				++it;
			}
		}

		removeKeyIfExists(Pipes, key0);
	}
}
void GTS::delCS(unordered_map<int, compressor_station>& Stations, unordered_map<int, connections>& Connections, unordered_map<int, Pipe>& Pipes) {
	if (Stations.size() == 0) {
		cout << "0 станций доступно!" << endl;
	}
	else {
		cout << "Все компрессорные станции:" << endl;
		view(Stations);
		cout << "Введите ID (какую станцию вы хотите удалить): ";
		int key0;
		key0 = check<int>(1, findMaxId(Stations));
		for (auto it = Connections.begin(); it != Connections.end(); ) {
			if (it->second.id_entry == key0 || it->second.id_out == key0) {
				int pipeId = it->second.id_pipe;
				if (Pipes.find(pipeId) != Pipes.end()) {
					Pipes[pipeId].Set_free(true);
				}
				if (it->second.id_entry == key0) {
					int id_out = it->second.id_out;
					Stations[id_out].Set_Id_in(Stations[id_out].Get_Id_in() - 1);
				}

				if (it->second.id_out == key0) {
					int id_entry = it->second.id_entry;
					Stations[id_entry].Set_Id_out(Stations[id_entry].Get_Id_out() - 1);
				}
				it = Connections.erase(it);
			}
			else {
				++it;
			}
		}
		removeKeyIfExists(Stations, key0);
	}
}