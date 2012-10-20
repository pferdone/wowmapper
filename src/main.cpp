/*
 * main.cpp
 *
 *  Created on: Oct 20, 2012
 *      Author: pferdone
 */


#include "chunk.hpp"
#include "mpqutil.hpp"

int main(int argc, char **argv)
{
  MPQUtil_c mpq_util;

  mpq_util.openArchive("expansion1.MPQ");

  return 0;
}

