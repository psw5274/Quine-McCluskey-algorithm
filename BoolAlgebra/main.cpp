#include <iostream>
#include <vector>
#include <string>
#include <conio.h> //getch()사용
#include <Windows.h>
using namespace std;
//system("cls"); //콘솔 초기화

int BtD(long* a, int bit);
long* DtB(int n, int bit);

class Term {
protected:
	char *term;
	int num_var;	//변수의 개수
	long size;		//2^변수의 개수
	long num_term;	//항의 개수


public:	

	Term(){}
	Term(int v) {		
		num_var = v;
		setSize();
		num_term = 0;
		//term
	}

	int getNum_var() { return num_var; }

	long getNum_term() { return num_term; }

	void setSize() {
		size = 1;
		for (int i = 0; i < num_var; i++)
			size *= 2;
	}

	void setTerm(long* a) {
		for(int i = 0; i < num_term; i++)

		num_term++;
	}
};

class MinTerm : public Term { //MinTerm 클래스
	friend long* DtB(int n);
private:
	int *term;

public:
	MinTerm(int n) { //변수의 개수 n
		num_var = n;
		setSize();
		term = new int[size];
	}

	void setTerm() {
		string tmp;
		
		for (int i = 0; i < size; i++)
		{
			cin >> tmp;

			if (tmp == ".")
			{
				num_term = i;
				break;
			}
			else
				term[i] = stoi(tmp); //stoi() 문자열을 정수로 변환
		}
	}

	int getTerm(int i) { return term[i]; }
};

class BinaryTerm : public Term {
private:
	long **term;	//이차원 배열의 포인터
public:
	BinaryTerm(int a, long b) { //변수의 개수 a, 항의 개수 b
		num_var = a;
		num_term = b;
		setSize();

		term = new long*[size];
		for (int i = 0; i < size; i++) //이차원 배열의 할당
			term[i] = new long[30];

	}

	void setTerm(long* a, int i) {
		term[i] = a;
	}

	void printTerm() {
		cout << "===BinaryTerm===" << endl;
		for (int i = 0; i < num_term; i++)
		{			
			for (int j = 0; j < num_var; j++)
			{
				cout << term[i][j];
			}
			cout << endl;
		}
		cout << endl;
	}


	void Quine_McCluskey();
	int checkRepet(long*** column, long* tmp, int* num);
	void checkOutput(long***column, long** output, int* num);
	long** combination(long** output, int* num);
	void InsertToTable(int** table, int k, int n); //combination 하위 함수
	void DtB2(int **table, long *output, int k);//combination 하위 함수
	void printOutput(long** output, int* num);
};

void BinaryTerm::Quine_McCluskey()
{
	long*** column = new long**[num_var];
	int* num = new int[num_var + 1]; //num[num_var]는 output의 원소의 개수
	long* tmp = new long[num_var];
	int count, status, equality;
	long dump;

	for (int i = 0; i < num_var; i++) //column 할당
	{
		column[i] = new long*[size];
		for (int j = 0; j < size; j++)
		{
			column[i][j] = new long[num_var];
		}
	}

	num[0] = 0;
	for (int i = 0; i < num_term; i++) //column[0]
	{
		for (int j = 0; j < num_var; j++)
		{
			dump = term[i][j];
			column[0][i][j] = (int)dump;
		}
		num[0]++;
	}

	
	for (int i = 0; i < num_var; i++)	//column[1] ~ column[num_var - 1]
	{
		num[i + 1] = 0;

		for (int j = 0; j < num[i] - 1; j++)	//column[i][0] ~ column[i][num[i] - 2] //column[i][j]
		{
			for (int k = j + 1; k < num[i]; k++)	//column[i][j]와 column[i][k]의 비교
			{
				count = 0;
				for (int l = 0; l < num_var; l++)
				{
					if (column[i][j][l] == column[i][k][l])
						equality = 1;
					else if ((column[i][j][l] == 0 && column[i][k][l] == 1)
						|| (column[i][j][l] == 1 && column[i][k][l] == 0))
					{
						count++;
						status = l;
					}
					else
					{
						equality = 0;
						break;
					}
				}

				if (equality == 1 && count == 1)
				{
					for (int x = 0; x < num_var; x++)
					{
						dump = column[i][j][x];
						if (x == status)
							tmp[x] = 2;

						else
							tmp[x] = (int)dump;
					}

					if (checkRepet(column, tmp, num) == 0)
					{
						for (int x = 0; x < num_var; x++)
						{
							dump = column[i][j][x];
							if (x == status)
								column[i + 1][num[i + 1]][x] = 2;

							else
								column[i + 1][num[i + 1]][x] = (int)dump;
						}
						num[i + 1]++;
					}
				}
			}
		}
	}



	for (int z = 0; z < num_var; z++)
	{
		cout << "column" << z << endl;
		for (int i = 0; i < num[z]; i++)
		{
			for (int j = 0; j < num_var; j++)
			{
				cout << column[z][i][j];
			}
			cout << endl;
		}
		cout << "=========" << endl;
	}

	//for(int i = 0; i < 

	long** output = new long*[size];	//결과 할당
	for (int i = 0; i < size; i++)
		output[i] = new long[num_var];

	//마무리
	//column을 output에 중복을 검사 하면서 저장

	checkOutput(column, output, num);

	cout << "\n=====필수항=====" << endl;
	for (int i = 0; i < num[num_var]; i++)
	{
		for (int j = 0; j < num_var; j++)
		{
			cout << output[i][j];
		}
		cout << endl;
	}
		
	printOutput(combination(output, num), num);
}

int BinaryTerm::checkRepet(long*** column, long* tmp, int* num) //column을 위한 중복 검사 함수
{//중복이면 1, 중복 아니면 0 리턴
	int repet;

	for (int i = 0; i <= num_var; i++) //i < num_var?
	{
		for (int j = 0; j < num[i]; j++)
		{
			for (int k = 0; k < num_var; k++)
			{
				if (tmp[k] == column[i][j][k])
					repet = 1;
				else if (column[i][j][k] == 2)
					repet = 1;
				else
				{
					repet = 0;
					goto here;
				}
			}
		here:
			if (repet == 1)
			{
				return 1;
			}
		}
	}

	return 0;
}

void BinaryTerm::checkOutput(long*** column, long** output, int* num) //output을 위한 중복 검사 함수
{
	int repet, status;
	num[num_var] = 0;

	for (int i = 0; i < num_var - 1; i++)
	{
		for (int j = 0; j < num[i]; j++) //column[i][j]
		{
			if (num[i] == 0)
				break;

			status = 0;

			for (int k = i + 1; k < num_var; k++) //column[k][l]
			{
				for (int l = 0; l < num[k]; l++)
				{
					if (num[k] == 0)
						break;
					for (int m = 0; m < num_var; m++) //column[i][j][m], column[k][l][m] 비교
					{
						if (column[i][j][m] == column[k][l][m])
							repet = 1;
						else if (column[k][l][m] == 2)
							repet = 1;
						else
						{
							repet = 0;
							break;
						}
					}
					if (repet == 1) //중복
					{
						status = 1;
					}
				}
			}

			if (status == 0)
			{
				output[num[num_var]] = column[i][j];
				num[num_var]++;
			}
		}
	}

}

long** BinaryTerm::combination(long** output, int* num)
{	//후보항 표를 이용하여 최소항을 남기는 함수
	//BinaryTerm클래스의 Term을 이용한다
	//후보항 표 할당
	int** table = new int*[num_term];
	for (int i = 0; i < num_term; i++)
		table[i] = new int[num[num_var] + 1];

	for (int i = 0; i < num_term; i++) //2진수인 term을 정수로 변환하여 후보항 표의 테이블에 입력
	{
		table[i][0] = BtD(term[i], num_var);
	}

	for (int i = 0; i < num_term; i++)
	{
		for (int j = 1; j < num[num_var] + 1; j++)
		{
			table[i][j] = 0;
		}
	}
	
	for (int i = 0; i < num[num_var]; i++) //output[i]의 모든 경우의 수를 2진수로 바꿔 테이블에 입력
	{
		for (int j = 0; j < num_var; j++)
		{
			DtB2(table, output[i], i + 1);
		}
	}
	
	cout << "\n=====필수항 표=====" << endl;
	cout << "   ";
	for(int i = 0; i < num[num_var]; i++)
	{
		cout << i << "  ";
	}
	cout << endl;

	for (int i = 0; i < num_term; i++)
	{
		for (int j = 0; j < num[num_var] + 1; j++)
			cout << table[i][j] << "  ";
		cout << endl;
	}

	int *count = new int[num_term];
	int *S = new int[num[num_var]];
	int n = 0; //S의 원소 개수

	for (int i = 0; i < num_term; i++) //유일한 후보항을 찾는다
	{
		count[i] = 0;

		for (int j = 1; j < num[num_var] + 1; j++)
		{
			if (table[i][j] == 1)
				count[i]++;
		}
	}
	
	for (int i = 0; i < num_term; i++) //유일한 term을 가지고 있는 항의 번호(j - 1)를 집합 S에 저장
	{
		if (count[i] == 1)
			for(int j = 1; j < num[num_var] + 1; j++)
				if (table[i][j] == 1)
				{
					int flag = 0;
					for (int x = 0; x < n; x++)
					{
						if (S[x] == j - 1)
						{
							flag = 1;
							break;
						}
					}
					if (flag == 0)
					{
						S[n] = j - 1;
						n++;
					}
				}
	}

	//S 원소 선택 정렬
	for (int i = 0; i < n - 1; i++)
	{
		int MIN = i;

		for (int j = i + 1; j < n; j++)
			if (S[j] < S[MIN])
				MIN = j;
		int temp = S[MIN];
		S[MIN] = S[i];
		S[i] = temp;
	}

	//new_output을 집합S에 따라 생성한다
	long** new_output = new long*[n];
	for (int i = 0; i < n; i++)
		new_output[i] = new long[num_var];

	for (int i = 0; i < n; i++)
		new_output[i] = output[S[i]];
	num[num_var] = n;

	delete[]table;
	delete[]count;
	delete[]S;

	return new_output;
}

void BinaryTerm::DtB2(int **table, long *output, int k)
{//2를 처리하기 위한 DTB
	long *temp = new long[num_var];
	int n = 0;
	int tmp = 1;
	int dump;

	for (int i = num_var - 1; i >= 0; i--)
	{
		if (output[i] == 1)
			n += tmp;
		else if (output[i] == 2)
		{
			for (int j = 0; j < num_var; j++)
			{
				dump = output[j];
				temp[j] = (long)dump;
			}

			temp[i] = 0;
			DtB2(table, temp, k);
			temp[i] = 1;
			DtB2(table, temp, k);
		}
		tmp *= 2;
	}

	InsertToTable(table, k, n);
}

void BinaryTerm::InsertToTable(int** table, int k, int n)
{
	for (int i = 0; i < num_term; i++)
	{
		if (table[i][0] == n)
		{
			table[i][k] = 1;
		}
	}
}

void BinaryTerm::printOutput(long** output, int* num)
{
	cout << endl << "f(";
	for (int i = 0; i < num_var -  1; i++)
	{
		cout << (char)(i + 'a') << ", ";
	}
	cout << (char)(num_var - 1 + 'a') << ") = ";

	for (int i = 0; i < num[num_var]; i++)
	{
		for (int j = 0; j < num_var; j++)
		{
			if (output[i][j] == 0)
				cout << (char)(j + 'a') << "'";
			else if (output[i][j] == 1)
				cout << (char)(j + 'a');
			//2인 항은 무시
		}
		if (i != num[num_var] - 1)
		cout << " + ";
	}
	cout << endl;
}


int BtD(long* a, int bit) //Binary to Decimal
{
	int n = 0;
	int tmp = 1;

	for (int i = bit - 1; i >= 0; i--)
	{
		if (a[i] == 1)
		{
			n += tmp;
		}
		tmp *= 2;
	}

	return n;
}

long* DtB(int n, int bit) //Decimal to Binary
{									//long의 배열형으로 반환
	long* binary = new long[bit];
	int top = 0; //비트의 수

	if (n == 0) //0에 대한 DTB
	{
		long *result = new long[bit];

		for (int i = 0; i < bit; i++)
			result[i] = 0;

		return result;
	}

	while (n > 1)
	{
		if (n % 2 == 1)
			binary[top] = 1;

		else if (n % 2 == 0)
			binary[top] = 0;

		n /= 2;
		top++;
	}
	binary[top] = 1;

	for (int i = top + 1; i < bit; i++)
		binary[i] = 0;

	long *result = new long[bit];


	for (int i = 0; i < bit; i++)
	{
		result[i] = binary[bit - i - 1];
	}
	return result;
}

MinTerm Input() //변수의 개수를 입력받아 MinTerm 객체를 생성하는 함수
{
	system("cls");

	int n;
	string tmp;

	cout << "변수의 개수 n을 입력하시오 (최대 26) : ";
	cin >> n;

	MinTerm T(n);

	cout << "최소항을 입력하시오 (. : 입력 완료)\n" << "Σ(";
	for (int i = 0; i < n - 1; i++)
		cout << (char)('a' + i) << ", ";
	cout << (char)('a' + n) << ") = ";

	T.setTerm();

	return T;
}

void main()
{
	cout << "=========================" << endl;
	cout << "1. 부울대수 간소화" << endl;
	cout << "0. 프로그램 종료" << endl;
	cout << "=========================" << endl;


	switch (_getch())
	{
	case '1':
	{
		MinTerm input = Input();

		BinaryTerm B(input.getNum_var(), input.getNum_term());

		for (int i = 0; i < input.getNum_term(); i++)
			B.setTerm(DtB(input.getTerm(i), input.getNum_var()), i);
		B.printTerm();
		B.Quine_McCluskey();

		fflush(stdin);
		_getch();
	} break;

	case '0':
		break;
	}
}
