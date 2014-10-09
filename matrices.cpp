#include <fstream>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include "matrices.hpp"

ErrorCode Matrix::s_lastError = ERR_NO_ERROR;

Matrix::Matrix() : m_data(nullptr), m_cols(0), m_rows(0), m_size(0)
{ 
}

Matrix::Matrix(size_t rows, size_t cols) : m_rows(rows), m_cols(cols), m_size(cols * rows)
{
  m_data = new double[rows * cols];
  if (!m_data)
    s_lastError = ERR_NO_MEMORY;
  else
    memset(m_data, 0, m_size * sizeof(double));
}

Matrix::Matrix(const char *fileName) : m_data(nullptr), m_size(0), m_rows(0), m_cols(0)
{
  Load(fileName);
}

Matrix::~Matrix()
{
  if (m_data)
    delete[] m_data;
  m_data = nullptr;
}

// we assume input file contains  correct matrix. It's not interesting if it doesn't
ErrorCode Matrix::Load(const char *fileName)
{
  if (!fileName)
  {
    s_lastError = ERR_FILE_NOT_FOUND;
    return s_lastError;
  }

  std::ifstream in(fileName);
  if (m_data)
    delete[] m_data;

  in >> m_rows >> m_cols;
  m_size = m_rows * m_cols;
  if (!m_size)
  {
    m_data = nullptr;
    m_rows = m_cols = 0;
    s_lastError = ERR_ZERO_MATRIX_SIZE;
    return s_lastError;
  }

  m_data = new double[m_size];
  memset(m_data, 0, m_size * sizeof(double));
  for (size_t i = 0, index = 0; i < m_rows; ++i)
  {
    for (size_t j = 0; j < m_cols; ++j, ++index)
    {
      in >> m_data[index];
    }
  }
  
  return ERR_NO_ERROR;
}

ErrorCode Matrix::Add(const Matrix &m)
{
  if (m.GetColsNumber() != m_cols || m.GetRowsNumber() != m_rows)
  {
    s_lastError = ERR_WRONG_MATRIX_SIZES;
    return s_lastError;
  }

  for (size_t i = 0; i < m_size; ++i)
    m_data[i] += m.m_data[i];

  return ERR_NO_ERROR;
}

ErrorCode Matrix::MultRight(const Matrix &m)
{
  if (m_cols != m.m_rows)
  {
    s_lastError = ERR_WRONG_MATRIX_SIZES;
    return s_lastError;
  }

  double *temp = new double[m_rows * m.m_cols];
  if (!temp)
  {
    s_lastError = ERR_NO_MEMORY;
    return s_lastError;
  }

  m_size = m_rows * m.m_cols;
  memset(temp, 0, sizeof(double) * m_size);
  for (size_t i = 0, index = 0; i < m_rows; ++i)
  {
    for (size_t j = 0; j < m.m_cols; ++j, ++index)
    {
      for (size_t k = 0; k < m_cols; ++k)
      {
        temp[index] += m_data[i * m_cols + k] * m.m_data[k * m.m_cols + j];
      }
    }
  }

  delete[] m_data;
  m_data = temp;
  m_cols = m.m_cols;

  return ERR_NO_ERROR;
}

Matrix Matrix::operator =(const Matrix &m)
{
  if (m_data)
    delete[] m_data;
  if (m.m_size == 0)
  {
    m_size = 0;
    m_rows = 0;
    m_cols = 0;
    m_data = nullptr;
    s_lastError = ERR_ZERO_MATRIX_SIZE;
    return *this;
  }
  m_data = new double[m.m_size];
  if (m_data == nullptr)
  {
    s_lastError = ERR_NO_MEMORY;
    m_rows = 0;
    m_cols = 0;
    m_size = 0;
    return *this;
  }
  m_size = m.m_size;
  m_cols = m.m_cols;
  m_rows = m.m_rows;
  memcpy(m_data, m.m_data, m_size);

  return *this;
}

void Matrix::Print() const
{
  size_t ind = 0;
  for (size_t i = 0; i < m_rows; ++i)
  {
    for (size_t j = 0; j < m_cols; ++j, ++ind)
    {
      std::cout << m_data[ind] << " ";
    }
    std::cout << "\n";
  }
}

bool PrintError(const ErrorCode &er)
{
  const int messageSize = 255;
  char message[messageSize + 1];

  switch (er)
  {
  case ERR_ZERO_MATRIX_SIZE:
    snprintf(message, messageSize, "%s", "Error: matrix has zero size.");
    break;
  case ERR_FILE_NOT_FOUND:
    snprintf(message, messageSize, "%s", "Error: file not found while loading matrix.");
    break;
  case ERR_NO_MEMORY:
    snprintf(message, messageSize, "%s", "Error: not enough memory to load matrix.");
    break;
  case ERR_WRONG_MATRIX_SIZES:
    snprintf(message, messageSize, "%s", "Error: incompatible matrix sizes for matrix operation.");
    break;
  case ERR_NO_ERROR:
    return false;
  }

  std::cout << message << std::endl;
  return true;
}