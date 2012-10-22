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
#include <vector>
#include <algorithm>
#include <iostream>

typedef std::map<std::string, HANDLE> HandleMap_t;
typedef std::list<std::string> StringList_t;
typedef std::map<std::string, StringList_t> FileMap_t;
typedef std::vector<uint8_t> Buffer_t;

/**
 * MPQ utility class to ease interaction with archives.
 */
class MPQUtil_c {
  public:
    MPQUtil_c();
    virtual ~MPQUtil_c();

    /**
     * Opens an archive and adds its HANDLE to the file handle map on success.
     * Also retrieves the listfile from a MPQ.
     *
     * \param filename MPQ filename
     * \param is_read_only wether or not to open it with read only flag
     * \return Returns true if file can be opened and listfile could be read or if file is already open
     */
    bool openArchive(const std::string &filename, bool is_read_only = true);

    /**
     * Read a file of an open archive and output it to buffer.
     *
     * \param filename filename of a file inside any open archive
     * \param buffer buffer to receive file data
     * \return true if file could be read or false if it cannot be read or does not exist in any archive
     */
    bool readFile(const std::string &filename, Buffer_t *buffer);

  private:
    HandleMap_t _fileHandles;
    FileMap_t _fileMap;
};

#endif /* MPQUTIL_HPP_ */
