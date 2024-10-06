// Grid.h


#pragma once

#include <iostream>
#include <exception>
#include <stdexcept>
#include <algorithm>

using namespace std;

template<typename T>
class Grid {
private:
  struct ROW {
    T*  Cols;     // array of column elements
    size_t NumCols;  // total # of columns (0..NumCols-1)
  };

  ROW* Rows;     // array of ROWs
  size_t  NumRows;  // total # of rows (0..NumRows-1)

public:
  //
  // default constructor:
  //
  // Called automatically by to construct a 4x4 Grid. 
  // All elements initialized to default value of T.
  //
  Grid() {
    Rows = new ROW[4];
    NumRows = 4;

    // initialize each row to have 4 columns:
    for (size_t r = 0; r < NumRows; ++r) {
      Rows[r].Cols = new T[4];
      Rows[r].NumCols = 4;

      // initialize the elements to their default value:
      for (size_t c = 0; c < Rows[r].NumCols; ++c) {
        Rows[r].Cols[c] = T();  // default value for type T:
      }
    }
  }

  //
  // parameterized constructor:
  //
  // Called automatically to construct a Grid 
  // with R rows, where each row has C columns. 
  // All elements initialized to default value of T.
  //
  Grid(size_t R, size_t C) {
    Rows = new ROW[R];  // Initialize the Rows array with space for R ROWs.
    NumRows = R;

    // Iterate through each row in the grid.
    for (size_t r = 0; r < NumRows; ++r) {
        // For each row, create a dynamic array of type T (columns) to store elements.
        Rows[r].Cols = new T[C];  // Initialize the dynamic array with space for C columns.
        Rows[r].NumCols = C;      // Set the total number of columns for this row to C.

        // Initialize each element in the row's columns with the default value for type T.
        for (size_t c = 0; c < Rows[r].NumCols; ++c) {
            Rows[r].Cols[c] = T();  // Default value for type T.
        }
    }

  }

  //
  // destructor:
  //
  // Called automatically to free memory for this Grid.
  //
  virtual ~Grid() {
    // Iterate through each row in the grid.
    for (size_t r = 0; r < NumRows; ++r) {
        delete[] Rows[r].Cols;  // Delete the dynamic column arrays for each row.
    }
    delete[] Rows;  // Delete the dynamic row array.

  }


  //
  // copy constructor:
  //
  // Called automatically to construct a Grid that contains a
  // copy of an existing Grid.  
  Grid(const Grid<T>& other) {
    Rows = new ROW[other.NumRows];  // Initialize the Rows array with the same number of rows as 'other'.
    NumRows = other.NumRows;        // Set the total number of rows to match 'other'.

    // Iterate through each row in the 'other' grid.
    for (size_t r = 0; r < other.NumRows; ++r) {
        size_t other_numcols = other.Rows[r].NumCols;

        // For each row, create a dynamic array of type T (columns) in the new grid to store elements.
        Rows[r].Cols = new T[other_numcols];  // Initialize the dynamic array with the same number of columns as 'other'.
        Rows[r].NumCols = other_numcols;      // Set the total number of columns for this row to match 'other'.

        // Copy the elements from the 'other' grid to the new grid.
        for (size_t c = 0; c < other_numcols; ++c) {
            Rows[r].Cols[c] = other.Rows[r].Cols[c];  // Copy each element from 'other' to the new grid.
        }
    }

  }

  //
  // copy operator=
  //
  // Called when one Grid is assigned into another
  //
  Grid& operator=(const Grid& other) {
      Grid<T> temp;

        // Check for self-assignment by comparing 'this' pointer with 'other'.
        if (this == &other) {
            return *this;  // If 'this' and 'other' are the same, no need to copy, return the current object.
        }

        // Deallocate memory for the existing grid.
        for (size_t r = 0; r < NumRows; ++r) {
            delete[] Rows[r].Cols;  // Delete the dynamic column arrays for each row.
        }
        delete[] Rows;  // Delete the dynamic row array.

        // Create a new dynamic array of ROW structures to represent the rows in the new grid.
        this->Rows = new ROW[other.NumRows];  // Initialize the Rows array with the same number of rows as 'other'.
        this->NumRows = other.NumRows;        // Set the total number of rows to match 'other'.

        // Iterate through each row in the 'other' grid.
        for (size_t r = 0; r < other.NumRows; ++r) {
            size_t other_numcols = other.Rows[r].NumCols;

            // For each row, create a dynamic array of type T (columns) in the new grid to store elements.
            this->Rows[r].Cols = new T[other_numcols];  // Initialize the dynamic array with the same number of columns as 'other'.
            this->Rows[r].NumCols = other_numcols;      // Set the total number of columns for this row to match 'other'.

            // Copy the elements from the 'other' grid to the new grid.
            for (size_t c = 0; c < other_numcols; ++c) {
                this->Rows[r].Cols[c] = other.Rows[r].Cols[c];  // Copy each element from 'other' to the new grid.
            }
        }
        return *this;  // Return a reference to the updated object.
  }

  //
  // numrows
  //
  // Returns the # of rows in the Grid.  
  // The indices for these rows are 0..numrows-1.
  //
  size_t numrows() const {


    return NumRows;  // TODO:  update this, it is only here so code compiles.
  }


  //
  // numcols
  //
  // Returns the # of columns in row r.  
  // The indices for these columns are 0..numcols-1.  
  //
  size_t numcols(size_t r) const {

      if (r >= NumRows) {
            throw out_of_range("Grid::numcols: Invalid row index");
      }
      return Rows[r].NumCols;

  }


  //
  // size
  //
  // Returns the total # of elements in the Grid.
  //
  size_t size() const {

      size_t totalSize = 0;
      for (size_t r = 0; r < NumRows; ++r) {
          totalSize += Rows[r].NumCols;
      }
      return totalSize;
  }


  //
  // ()
  //
  // Returns a reference to the element at location (r, c);
  T& operator()(size_t r, size_t c) {
      if (r >= NumRows || c >= Rows[r].NumCols) {
          throw out_of_range("Grid::operator(): Invalid row or column index");
      }
      return Rows[r].Cols[c];
  }

  //
  // _output
  //
  // Outputs the contents of the grid
  void _output() {
      for (size_t r = 0; r < NumRows; ++r) {
        cout << "| ";
        for (size_t c = 0; c < Rows[r].NumCols; ++c) {
            cout << Rows[NumRows - r - 1].Cols[c] << " | ";
        }
        cout << endl;
    }


  }

};
