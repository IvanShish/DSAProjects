#include "header.h"

using namespace std;
using namespace binTree_modul;

typedef unsigned int unInt;

binTree enterBT(char c, int curr, ifstream& f, int* check, int* openingBracket, int* endingBracket);       //������� ��� ����� ��������� ������
void outBT(binTree b, char elem, int *check, int *count, int length, int* min);          //������� ��� ������ ��������� ������ � ��� �������� ������ �������� � ������������� ��������
char reading(ifstream &f);        //�������, ������������ �������� � ���������/����� �������

int main()
{
	binTree b;
	int how;
	int curr = 0;
	string s;
	ifstream in;
	char elem;
	char c = NULL;
	int check = 1;
	int checkForElem = 0;
	int count = 0;        //������� ��������� ���������
	int length = 1;        //����� �� ������� ���������� ��������
	int min = 100000;    //��� ������� ������������ ����
	int openingBracket = 0;
	int endingBracket = 0;

	/*SetConsoleCP(1251);			// ��� ������ ��������
	SetConsoleOutputCP(1251);	// ��� ������ ��������*/
	setlocale(LC_ALL, "RUS");

	cout << "��� �� ������ ������ �������� ������?\n1 - �� �����\n2 - �� ���������\n";
	cin >> how;
	if (how != 1 && how != 2) {
		cout << "����� ������ 1 ��� 2!";
		return 0;
	}
	
	if (how == 1) {
		string nameOfFile;

		cout << "��������� ����� ��� ������:\n";
		WIN32_FIND_DATA FindFileData;
		HANDLE hf;
		hf = FindFirstFile(".\\*.txt", &FindFileData);
		if (hf != INVALID_HANDLE_VALUE) {
			do {
				cout << FindFileData.cFileName << endl;
			} while (FindNextFile(hf, &FindFileData) != 0);
			FindClose(hf);
		}

		cout << "������� �������� �����\n";
		cin >> nameOfFile;
		in.open(nameOfFile);

		if (!in.is_open()) {
			cout << "�������� �������� �����!\n";
			return 0;
		}

		getline(in, s);
		cout << "������ � �����:\n" << s << endl;
		in.close();
		in.open(nameOfFile);
	}

	cin.sync();
	cin.get();

	b = enterBT(c, curr, in, &check, &openingBracket, &endingBracket);
	cout << endl;

	if (openingBracket != endingBracket || check == 4) {
		cout << "��� �� �������� ������!" << endl;
		return 0;
	}

	cout << "������� �������\n";
	cin >> elem;
	if (!isalpha(elem)) {
		cout << "������� ������ ������� ��� ��� �� �������� ������\n";
		return 0;
	}

	outBT(b, elem, &checkForElem, &count, length, &min);
	if (checkForElem) {
		cout << "\n������������� ������� ��� ������\n";
		cout << "����� ��������� �������� � ������ - " << count << endl;
		cout << "����� ���� �� ����� �� ���������� �������������� �������� - " << min << endl;
	}
	else {
		cout << "\n������������� ������� �� ��� ������\n";
		return 0;
	}
	cout << endl;

	destroy(b);
	cout << endl;
	return (0);
}

binTree enterBT(char c, int curr, ifstream& f, int* check, int* openingBracket, int* endingBracket) {         //������� ��� ����� ��������� ������
	binTree p, q;

	if (c == EOF || c == '\n') return NULL;
	c = reading(f);

	if (curr == 0) {
		if (c == '(') {
			(*openingBracket)++;
			c = reading(f);
			if (c == ')') {
				(*endingBracket)++;
				return NULL;
			}
		}
		else {
			*check = 4;
			return NULL;
		}
	}

	else if (c == ')') {
		*check = 0;
		(*endingBracket)++;
		return NULL;
	}

	else if (isspace(c)) {
		*check = 1;
		return NULL;
	}

	else if (c == '(') {
		(*openingBracket)++;
		*check = 2;
		return NULL;
	}

	if (isalpha(c)) {
		p = enterBT(c, curr + 1, f, check, openingBracket, endingBracket);

		if (*check == 2) {                       //���� ������������� ������
			p = enterBT(c, curr + 1, f, check, openingBracket, endingBracket);
			if (*check != 5) {
				*check = 3;
			}
			else q = NULL;
		}
		else if (*check == 1) {                  //���� ������
			p = enterBT(c, curr + 1, f, check, openingBracket, endingBracket);
		}

		if (*check == 0) {                        //���� ������������� ������
			binTree tmp = NULL;
			if (curr != 0) {
				tmp = enterBT(c, curr + 1, f, check, openingBracket, endingBracket);
			}
			if (*check == 2) q = NULL;
			else if (*check == 0) {
				q = tmp;
				*check = 5;
			}
		}
		else if (*check != 5) {
			q = enterBT(c, curr + 1, f, check, openingBracket, endingBracket);
			if (*check == 0) q = enterBT(c, curr + 1, f, check, openingBracket, endingBracket);
			if (*check == 2) q = enterBT(c, curr + 1, f, check, openingBracket, endingBracket);
			else if (*check == 5) enterBT(c, curr + 1, f, check, openingBracket, endingBracket);
		}
		else if (*check == 5) {
			*check = 3;
			q = NULL;
		}

		return ConsBT(c, p, q);
	}

	return 0;
}

void outBT(binTree b, char elem, int *check, int *count, int length, int *min) {         //������� ������ ������������� �������� ��� ��������
	if (b != NULL) {
		if (b->info == elem) {               //���� ������ ������������� �������
			if (length < *min) {
				*min = length;
			}
			(*count)++;
			*check = 1;
			cout << "������ ������������� ������� " << RootBT(b) << " ";
		}
		else cout << RootBT(b) << " ";
		cout << " LEFT:";
		outBT(Left(b), elem, check, count, length+1, min);
		cout << " RIGHT:";
		outBT(Right(b), elem, check, count, length+1, min);
	}
	else cout << "�";
}

char reading(ifstream& f) {
	char c = NULL;
	if (f.is_open()) c = f.get();
	else c = cin.get();
	cout << c << endl;
	return c;
}