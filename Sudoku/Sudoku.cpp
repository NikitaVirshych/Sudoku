#include "Sudoku.h"

bool Sudoku::solve()
{
	static int index = 0;
	static auto rand = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

	if (index == 81)
	{
		this->solution--;

		if (this->solution == 0)
			return true;
		else
			return false;
	}

	if (this->grid[0][index] != 0)
	{
		index++;
		bool result = this->solve();
		index--;
		return result;
	}
	else
	{

		int possibleValues[]{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		std::shuffle(std::begin(possibleValues), std::end(possibleValues), rand);

		for (int val : possibleValues)
		{
			this->grid[0][index] = val;

			if (!this->IsValid(index / 9, index % 9))
				continue;

			if (this->solve())
				return true;
		}

		this->grid[0][index] = 0;
		return false;
	}
}

bool Sudoku::isValidRow(int row) const
{
	std::unordered_set<int> existingNumbers{ grid[row][0] };
	existingNumbers.reserve(9);

	for (int i = 1; i < 9; i++)
	{
		if (grid[row][i] != 0)
		{
			if (existingNumbers.contains(grid[row][i]))
				return false;
			existingNumbers.insert(grid[row][i]);
		}
	}
	return true;
}

bool Sudoku::isValidColumn(int col) const
{
	std::unordered_set<int> existingNumbers{ grid[0][col] };
	existingNumbers.reserve(9);

	for (int i = 1; i < 9; i++)
	{
		if (grid[i][col] != 0)
		{
			if (existingNumbers.contains(grid[i][col]))
				return false;
			existingNumbers.insert(grid[i][col]);
		}
	}
	return true;
}

bool Sudoku::isValidBlock(int block) const
{
	std::unordered_set<int> existingNumbers;
	existingNumbers.reserve(9);

	int colOffset = (block % 3) * 3;
	int rowOffset = (block / 3) * 3;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (grid[rowOffset + i][colOffset + j] != 0)
			{
				if (existingNumbers.contains(grid[rowOffset + i][colOffset + j]))
					return false;
				existingNumbers.insert(grid[rowOffset + i][colOffset + j]);
			}
		}
	}

	return true;
}

Sudoku::Sudoku()
{
}

Sudoku::Sudoku(std::istream& stream)
{
	stream >> *this;
}

int Sudoku::CountSolutions()
{
	this->solution = 0;
	this->solve();

	return -this->solution;
}

Sudoku Sudoku::GetSolved(int solution = 0) const
{
	Sudoku tmp{ *this };

	tmp.solution = solution + 1;
	tmp.solve();

	return tmp;
}

Sudoku Sudoku::GetRandomSolved()
{
	Sudoku tmp;
	tmp.solution = 1;
	tmp.solve();
	return tmp;
}

int Sudoku::AddBlanks(int blanks)
{
	auto rand = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());
	std::unordered_set<int> skip;

	int i = 0;
	while (i < blanks)
	{
		int id = rand() % 81;
		int val = this->grid[0][id];

		if (val == 0 or skip.contains(id))
		{
			continue;
		}

		this->grid[0][id] = 0;

		Sudoku secondSolution = this->GetSolved(1);

		if (secondSolution == *this)
		{
			skip.clear();
			i++;
		}
		else
		{
			this->grid[0][id] = val;
			skip.insert(id);

			if (skip.size() + i == 81)
				break;

		}
	}

	return i;
}

bool Sudoku::IsValid(int row, int col) const
{
	if (!this->isValidRow(row))
		return false;

	if (!this->isValidColumn(col))
		return false;

	int block = (row / 3) * 3 + (col / 3);

	if (!this->isValidBlock(block))
		return false;

	return true;
}

bool Sudoku::IsValid() const
{
	for (int i = 0; i < 9; i++)
	{
		if (!this->isValidRow(i))
			return false;

		if (!this->isValidColumn(i))
			return false;

		if (!this->isValidBlock(i))
			return false;
	}

	return true;
}

bool Sudoku::operator==(const Sudoku& other) const
{
	return memcmp(this->grid, other.grid, 81 * sizeof(int)) == 0 ? true : false;
}

std::ostream& operator<<(std::ostream& stream, const Sudoku& grid)
{
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			stream << grid.grid[row][col] << " ";
		}
		stream << std::endl;
	}

	return stream;
}

std::istream& operator>>(std::istream& stream, Sudoku& sudoku)
{
	int buf;
	int i = 0;

	while (i < 81 and !stream.eof()) 
	{
		stream >> buf;

		if (stream.fail())
		{
			stream.clear();
			stream.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
			continue;
		}

		if (buf < 0 or buf > 9)
			continue;

		sudoku.grid[0][i++] = buf;
	}

	return stream;
}