#ifndef SymCode_h
#define SymCode_h

#include <string>
#include <vector>

class Sym {

private:

	struct elem {
		char c;                 // символ
		string code;            // статический код

		elem() {
			c = '\0';
			code = "00000000";
		}
	};

	int num = 0;                    // длина вектора arr
	vector <elem> arr;              // вектор хранит символы и их коды


public:
	//--------------------------------------- задает код символу
	string code(char c) {
		int i = 0;
		int n = ++num;
		int j;
		elem a;
		a.c = c;

		for (i = 0; n > 0; i++) {        // код символа задается как его индекс
										// в векторе arr в двоичной системе
			j = n % 2;

			n /= 2;

			if (j)
				a.code[i] = '1';

		}

		arr.push_back(a);

		return a.code;
	}

	//--------------------------------------- записывает в файл символы и их коды

	void writeFile(ofstream& file) {

		int i;

		for (i = 0; i < num; i++) {

			file << arr[i].code << " " << arr[i].c << endl;
		}

	}
	//--------------------------------------- считывает из фала символы и их коды

	int readFile(ifstream& file) {
		char sym;
		elem a;

		while (!file.eof()) {                       //производится считывание ключей
			char tmp = NULL;    //для считывания ненужных символов
			string codee;        //считывание кода символа
			file.get(sym);
			if (sym == '\n') {
				file.get(sym);
				if (sym != ' ') break;
				else sym = '\n';
			}
			else file.get(tmp);
			file >> codee;
			if (!isdigit(codee[0])) return 1;
			file.get(tmp);

			a.code = codee;
			a.c = sym;
			num++;
			arr.push_back(a);
		}

		return 0;
	
	}


	//--------------------------------------- находит символ по коду

	char whatSym(string code) {

		int i;

		for (i = 0; i < num; i++) {

			if (arr[i].code == code)
				return arr[i].c;


		}

		return '\0';

	}

	//--------------------------------------- печатает коды символов

	void print_Codes(ofstream & out) {
		int i = 0;

		while (i < num) {
			out << arr[i].c << " " << arr[i].code << endl;
			i++;
		}
	}
};

#endif