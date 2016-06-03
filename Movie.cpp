#include "Movie.hpp"
#include <vector>
#include <string>

using namespace std;

Movie::Movie(string movieName, int year) {
  this->movieName = movieName;
  this->year = year;
}

