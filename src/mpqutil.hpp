/*
 * mpqutil.hpp
 *
 *  Created on: Oct 20, 2012
 *      Author: pferdone
 */

#ifndef MPQUTIL_HPP_
#define MPQUTIL_HPP_

#include <StormLib.h>
#include <string>
#include <sstream>
#include <map>
#include <list>
#include <algorithm>
#include <iostream>

typedef std::map<std::string, HANDLE> HandleMap_t;
typedef std::list<std::string> StringList_t;
typedef std::map<std::string, StringList_t> FileMap_t;

/**
 * MPQ utility class to ease interaction with archives.
 */
class MPQUtil_c {
  public:
    MPQUtil_c();
    virtual ~MPQUtil_c();

    /**
     * Opens an archive and adds its HANDLE to the file handle map on success.
     */
    bool openArchive(const std::string &filename, bool is_read_only = true);

  private:
    HandleMap_t _fileHandles;
    FileMap_t _fileMap;
};

#endif /* MPQUTIL_HPP_ */
