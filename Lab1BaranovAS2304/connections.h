#pragma once
struct connections {
	int id_entry;
	int id_pipe;
	int id_out;
	connections(int entry = 0, int pipe = 0, int out = 0)
		: id_entry(entry), id_pipe(pipe), id_out(out) {}
	friend ostream& operator<<(ostream& os, const connections& conn) {
		os << conn.id_entry << " " << conn.id_pipe << " " << conn.id_out << endl;
		return os;
	}
	friend istream& operator>>(istream& is, connections& conn) {
		is >> conn.id_entry >> conn.id_pipe >> conn.id_out;
		return is;
	}
};
