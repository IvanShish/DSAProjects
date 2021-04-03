#include "Headder.h"

using namespace std;

void inStack(STACK <char>& st, string s, int curr, int max);           //������� ��� ����������� ������� �������� ������ � ����
void outStack(STACK <char> &st, ofstream& f);       //������� ��� ����������� ������� �������� �� ����� � ����

int main() {
	setlocale(LC_ALL, "RUS");
	int check = 1;
	string file;      //����������, �������� � ���� ���� � �����, �� �������� ����� ������� ������
	string file2;     //����������, �������� � ���� ���� � �����, � ������� ����� ������� ���������
	string fileContents;   //����������, ��������������� ��� ������ ����������� ���������� ����� � ���������� �� �����
	ifstream in;       //��������� ����
	ofstream out;      //���� � �����������
	int t = 0;
	int curr = 0;

	cout << "��������� ����� ��� ������:\n";
	WIN32_FIND_DATA FindFileData;
	HANDLE hf;
	hf = FindFirstFile(".\\files\\*.txt", &FindFileData);
	if (hf != INVALID_HANDLE_VALUE) {
		do {
			cout << FindFileData.cFileName << endl;
		} while (FindNextFile(hf, &FindFileData) != 0);
		FindClose(hf);
	}
	
	cout << "������� ����, ������� ����� �������\n";
	cin >> file;
	file.insert(0, ".\\files\\");
	in.open(file);
	if (!in) {
		cout << "WRONG FILE!\n";
		return 0;
	}
	else {
		cout << "���������� ���������� �����:\n";
		while (!in.eof()) {
			getline(in, fileContents);
			cout << fileContents << endl;
		}
		in.close();
		in.open(file);
	}

	fileContents.clear();
	cout << "������� ����, � ������� ����� �������� ���������\n";
	cin >> file2;
	file2.insert(0, ".\\files\\");
	out.open(file2);

	while (!in.eof()) {
		getline(in, fileContents);
		STACK <char> st(fileContents.size());
		inStack(st, fileContents, curr, fileContents.size());
		outStack(st, out);
		t = st.getTop();
		for (int i = 0; i < t; i++) st.pop();
	}

	out.close();
	in.close();
	in.open(file2);
	cout << "\n���������� ��������� �����:\n";
	while (!in.eof()) {
		getline(in, fileContents);
		cout << fileContents << endl;
	}

	return 0;
}

void inStack(STACK <char>& st, string s, int curr, int max) {
	if (curr == max) return;
	st.push(s[curr]);
	inStack(st, s, curr + 1, max);
	//for (int i = 0; i < s.size(); i++) st.push(s[i]);
}

void outStack(STACK <char> &st, ofstream &f) {
	for (int i = 0; i < st.getTop(); i++) {
		f.put(st.peek(i));
	}
	f.put('\n');
}