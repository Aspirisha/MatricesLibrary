#include <iostream>
#include <string.h>
#include "matrices.hpp"
using namespace std;

int main(int argc, char **argv) 
{
  if (argc == 1)
    return 0;

  Matrix result(argv[1]);

  if (PrintError(Matrix::GetLastError()))
    return 1;

  if (argc & 1)
  {
    cout << "Wrong number of arguments in command line." << endl;
    return 1;
  }

  for (int i = 2; i < argc; i += 2)
  {
    char *command = argv[i];
    char *fileName = argv[i + 1];

    Matrix rhs(fileName);
    if (PrintError(Matrix::GetLastError()))
      return 1;

    if (!strcmp(command, "--add"))
      result.Add(rhs);
    else if (!strcmp(command, "--mult"))
      result.MultRight(rhs);
    else
    {
      cout << "Unknown command in command line." << endl;
      return 1;
    }

    if (PrintError(Matrix::GetLastError()))
      return 1;
  }

  if (PrintError(Matrix::GetLastError()))
    return 1;

  result.Print();

  return 0;
}