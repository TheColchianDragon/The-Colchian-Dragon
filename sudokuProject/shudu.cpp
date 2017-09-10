#include<iostream>
#include<cstring>
#include<set>
#include<bitset>
#include<time.h>
#include <fstream>

using namespace std;

int first = -1;
bitset<81> flags[9][9];
bitset<10> second[9][9];
int choosenum[9][9];
int result;
int n;

int a[9][9] = {
	{ 4,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0 },
};

void AddElement(int r, int c, int num)
{
	++first;
	a[r][c] = num;
	for (int i = 0; i<9; ++i)
	{
		if (a[r][i] == 0 && second[r][i].test(num))
		{
			second[r][i].reset(num);
			--choosenum[r][i];
			flags[r][i].set(first);
		}

		if (a[i][c] == 0 && second[i][c].test(num))
		{
			second[i][c].reset(num);
			--choosenum[i][c];
			flags[i][c].set(first);
		}
	}

	int palaceRow = r>2 ? (r>5 ? 6 : 3) : 0;
	int palaceColumn = c>2 ? (c>5 ? 6 : 3) : 0;

	for (int i = 0; i<3; ++i)
	{
		for (int j = 0; j<3; ++j)
		{
			r = palaceRow + i;
			c = palaceColumn + j;
			if (a[r][c] == 0 && second[r][c].test(num))
			{
				second[r][c].reset(num);
				--choosenum[r][c];
				flags[r][c].set(first);
			}
		}
	}
}

void RecoverElement(int row, int column, int num)
{
	a[row][column] = 0;
	for (int i = 0; i<9; ++i)
	{
		if (a[row][i] == 0 && flags[row][i].test(first))
		{
			second[row][i].set(num);
			++choosenum[row][i];
			flags[row][i].reset(first);
		}

		if (a[i][column] == 0 && flags[i][column].test(first))
		{
			second[i][column].set(num);
			++choosenum[i][column];
			flags[i][column].reset(first);
		}
	}

	int palaceRow = row>2 ? (row>5 ? 6 : 3) : 0;
	int palaceColumn = column>2 ? (column>5 ? 6 : 3) : 0;

	for (int i = 0; i<3; ++i)
	{
		for (int j = 0; j<3; ++j)
		{
			row = palaceRow + i;
			column = palaceColumn + j;
			if (a[row][column] == 0 && flags[row][column].test(first))
			{
				second[row][column].set(num);
				++choosenum[row][column];
				flags[row][column].reset(first);
			}
		}
	}

	--first;
}

void Init()
{
	for (int i = 0; i<9; ++i)
	{
		for (int j = 0; j<9; ++j)
		{
			second[i][j].set();
			choosenum[i][j] = 10;
		}
	}

	for (int i = 0; i<9; ++i)
	{
		for (int j = 0; j<9; ++j)
		{
			if (a[i][j] != 0)
				AddElement(i, j, a[i][j]);
		}
	}
}

bool find(int &r, int &c)
{
	int min = 999;
	for (int i = 0; i<9; ++i)
	{
		for (int j = 0; j<9; ++j)
		{
			if (a[i][j] == 0 && choosenum[i][j]>1 && choosenum[i][j]<min)
			{
				r = i;
				c = j;
				min = choosenum[i][j];
			}
		}
	}

	if (min == 999)
		return false;
	return true;
}

bool check()
{
	set<int> e1;
	set<int> e2;

	for (int i = 0; i<9; ++i)
	{
		for (int j = 0; j<9; ++j)
		{
			e1.insert(a[i][j]);
			e2.insert(a[j][i]);
		}
		if (e1.size() != 9)
			return false;
		if (e2.size() != 9)
			return false;
		e1.clear();
		e2.clear();
	}

	e1.clear();
	int r, c;
	for (int i = 0; i<3; ++i)
	{
		for (int j = 0; j<3; ++j)
		{
			r = i * 3;
			c = j * 3;
			for (int k = 0; k<9; ++k)
			{
				e1.insert(a[r + k / 3][c + k % 3]);
			}
			if (e1.size() != 9)
				return false;

			e1.clear();
		}
	}
	return true;
}

void output(ofstream &mycout)
{
	//ofstream mycout("shudu.txt");
	mycout << endl;
	for (int i = 0; i<9; ++i)
	{
		for (int j = 0; j<9; ++j)
		{
			mycout << a[i][j] << " ";
		}
		mycout << endl;
	}
	//mycout.close();
}

bool go(ofstream &ofs)
{
	int row, column;
	if (!find(row, column))
		return true;

	for (int i = 1; i<10; ++i)
	{
		if (!second[row][column].test(i))
			continue;

		AddElement(row, column, i);

		if (go(ofs))
		{
			if (first == 80 && check())
			{
				++result;
				output(ofs);
				if (result>= n)
					return false;

			}
		}
		else
			return false;

		RecoverElement(row, column, i);
	}

	return true;
}

int main()
{
	ofstream mycout("sudoku.txt");
	cin >> n;
	Init();
	go(mycout);
	return 0;


}