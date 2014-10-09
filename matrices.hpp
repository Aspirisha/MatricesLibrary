#pragma once

// it's easier to use exceptions but it's not that big problem here
enum ErrorCode
{
  ERR_ZERO_MATRIX_SIZE,
  ERR_WRONG_MATRIX_SIZES,
  ERR_NO_MEMORY,
  ERR_NO_ERROR,
  ERR_FILE_NOT_FOUND
};

#define nullptr 0

class Matrix
{
public:
  Matrix();
  ~Matrix();
  Matrix(size_t rows, size_t cols);
  Matrix(const char *fileName);
  Matrix operator =(const Matrix &m);
  ErrorCode Add(const Matrix &m);
  // uses given operand as right operand
  ErrorCode MultRight(const Matrix &m); 
  // Load doesn't check if the actual sizes of matrix in file are as they declared 
  // in first line. If needed, this check can be added
  ErrorCode Load(const char *fileName);
  void Print() const;
  size_t GetRowsNumber() const { return m_rows; }
  size_t GetColsNumber() const { return m_cols; }
  static ErrorCode GetLastError() { return s_lastError; }
  static void ResetLastError() { s_lastError = ERR_NO_ERROR; }
private:
  size_t m_rows;
  size_t m_cols;
  size_t m_size; // m_rows * m_cols
  double *m_data;
  static ErrorCode s_lastError;
};

/**
 * @return true, if there was some error, i.e. er != ERR_NO_ERROR
 */
bool PrintError(const ErrorCode &er);