/*
 * Movie.cpp
 * Author: Kyle Sit, Louis Leung
 * Date: 6/2/16
 *
 * This file contains the implementation of the movie class used in ufind
 */

#include "Movie.hpp"
#include <vector>
#include <string>

using namespace std;

//constructor
Movie::Movie(string movieName, int year) {
  this->movieName = movieName;
  this->year = year;
}

