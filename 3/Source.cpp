#include "Headder.h"

using namespace std;

void inStack(STACK <char>& st, string s, int curr, int max);           //функция для записывания каждого литерала строки в стек
void outStack(STACK <char> &st, ofstream& f);       //функция для записывания каждого литерала из стека в файл

int main() {
	setlocale(LC_ALL, "RUS");
	int check = 1;
	string file;      //переменная, хранящая в себе путь к файлу, из которого будут считаны строки
	string file2;     //переменная, хранящая в себе путь к файлу, в который будет выведен результат
	string fileContents;   //переменная, предназначенная для вывода содержимого начального файла и результата на экран
	ifstream in;       //начальный файл
	ofstream out;      //файл с результатом
	int t = 0;
	int curr = 0;

	cout << "Доступные файлы для выбора:\n";
	WIN32_FIND_DATA FindFileData;
	HANDLE hf;
	hf = FindFirstFile(".\\files\\*.txt", &FindFileData);
	if (hf != INVALID_HANDLE_VALUE) {
		do {
			cout << FindFileData.cFileName << endl;
		} while (FindNextFile(hf, &FindFileData) != 0);
		FindClose(hf);
	}
	
	cout << "Введите файл, который нужно считать\n";
	cin >> file;
	file.insert(0, ".\\files\\");
	in.open(file);
	if (!in) {
		cout << "WRONG FILE!\n";
		return 0;
	}
	else {
		cout << "Содержимое начального файла:\n";
		while (!in.eof()) {
			getline(in, fileContents);
			cout << fileContents << endl;
		}
		in.close();
		in.open(file);
	}

	fileContents.clear();
	cout << "Введите файл, в который нужно записать результат\n";
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
	cout << "\nСодержимое конечного файла:\n";
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