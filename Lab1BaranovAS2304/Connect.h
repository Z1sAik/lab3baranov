#pragma once
#include <unordered_map>
#include <unordered_set>
#include "Pipe.h"
#include "CS.h"
#include "Connections.h"
#pragma once
class GTS
{
public:
	unordered_map<int, Pipe> Pipes = {};
	unordered_map<int, compressor_station> Stations = {};
	unordered_map<int, connections> Connections = {};
	void ConnectInGTS(unordered_map<int, Pipe>& Pipes, unordered_map<int, compressor_station>& Stations, unordered_map<int, connections>& Connections);
	int AddNewPipeConnect(unordered_map<int, Pipe>& Pipes, int diameter);
	void ShowGTS(unordered_map<int, connections>& Connections);
	void DeleteConnection(unordered_map<int, connections>& Connections, unordered_map<int, Pipe>& Pipes);
	vector<int> topologSort(unordered_map<int, Pipe>& Pipes, unordered_map<int, compressor_station>& Stations, unordered_map<int, connections>& Connections);
	vector<int> GetFreeCompressorStations(unordered_map<int, compressor_station>& Stations);
	vector<int> GetFreePipes(unordered_map<int, Pipe>& Pipes);
	int GetCompressorStationId(const vector<int>& free_cs, const string& type, int exclude_id = -1);
	int GetPipeId(const vector<int>& free_pipes, unordered_map<int, Pipe>& Pipes);
	void delCS(unordered_map<int, compressor_station>& Stations, unordered_map<int, connections>& Connections, unordered_map<int, Pipe>& Pipes);
	void delPipe(unordered_map<int, Pipe>& Pipes, unordered_map<int, connections>& Connections, unordered_map<int, compressor_station>& Stations);

	template <typename K>
	unordered_map<int, K> removeKeyIfExists(unordered_map<int, K>& notes, int key) {
		while (true) {
			auto it = notes.find(key);
			if (it != notes.end()) {
				notes.erase(it);
				cout << "Удаление прошло успешно!" << endl;
				return notes;
			}
			else {
				cout << "Ключ " << key << " не существует!" << endl << "Пожалуйста, введите существующий ключ: ";
				key = check<int>(1, findMaxId(notes));
			}
		}
	}

	template<typename K, typename V>
	K findMaxId(const std::unordered_map<K, V>& map) {
		if (map.empty()) {
			throw std::runtime_error("Вы ещё ничего не добавили");
		}

		K maxId = std::numeric_limits<K>::min();

		for (const auto& pair : map) {
			if (pair.first > maxId) {
				maxId = pair.first;
			}
		}
		return maxId;
	}

private:
	void dfs(int stationId, unordered_map<int, connections>& Connections, unordered_map<int, int>& state, vector<int>& result, bool& hasCycle);
};