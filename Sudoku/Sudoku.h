#pragma once
#include <iostream>
#include <stdexcept>
#include <random>
#include <chrono>
#include <unordered_set>

class Sudoku
{
	int grid[9][9] { 0 };
	int solution { 0 };

	bool solve();

	bool isValidRow(int row) const;
	bool isValidColumn(int col) const;
	bool isValidBlock(int block) const;

public:

	Sudoku();
	Sudoku(std::istream& stream);

	int CountSolutions();
	Sudoku GetSolved(int solution) const;

	static Sudoku GetRandomSolved();

	/// <returns>Number of blank spaces added</returns>
	int AddBlanks(int blanks);

	bool IsValid(int row, int col) const;
	bool IsValid() const;

	bool operator ==(const Sudoku& other) const;


	friend std::ostream& operator << (std::ostream& stream, const Sudoku& grid);
	friend std::istream& operator >> (std::istream& stream, Sudoku& grid);
};

