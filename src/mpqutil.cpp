/*
 * mpqutil.cpp
 *
 *  Created on: Oct 20, 2012
 *      Author: pferdone
 */

#include "mpqutil.hpp"

MPQUtil_c::MPQUtil_c()
{
  // TODO Auto-generated constructor stub
}

MPQUtil_c::~MPQUtil_c()
{
  // TODO Auto-generated destructor stub
}

bool MPQUtil_c::openArchive(const std::string& filename, bool is_read_only)
{
  std::cout << "Open file: " << filename << std::endl;

  // check if file is already open
  HandleMap_t::iterator found = _fileHandles.find(filename);
  if (found != _fileHandles.end()) {
    std::cout << filename << " already open" << std::endl;
    return true;
  }

  // open archive
  uint32_t open_flags = is_read_only ? STREAM_FLAG_READ_ONLY : 0;
  HANDLE archive_handle;
  if (!SFileOpenArchive(filename.c_str(), 0, open_flags, &archive_handle)) {
    std::cout << "cannot open " << filename << std::endl;
    return false;
  }

  // open listfile
  HANDLE lf_handle;
  if (!SFileOpenFileEx(archive_handle, "(listfile)", 0, &lf_handle)) {
    std::cout << "cannot open listfile" << std::endl;
    return false;
  }

  // read listfile
  uint32_t file_size, bytes_read;
  file_size = SFileGetFileSize(lf_handle, 0);
  std::string file_buffer(file_size, 0);
  if (!SFileReadFile(lf_handle, (char*)file_buffer.data(), file_size, &bytes_read, 0)) {
    std::cout << "cannot read listfile" << std::endl;
    return false;
  }

  // store filenames in archive to map
  std::istringstream lf_stream(file_buffer);
  std::string line;
  while(std::getline(lf_stream, line)) {
    std::string clean(line.begin(), std::find(line.begin(), line.end(), '\n')-1);
    _fileMap[filename].push_back(clean);
  }

  // close listfile and save archive handle to filename
  SFileCloseFile(lf_handle);
  _fileHandles[filename] = archive_handle;

  std::cout << "Number of files in " << filename << ": " << _fileMap[filename].size() << std::endl;

  return true;
}


