#pragma once

#include "../../include/VFS/VFS.hpp"

namespace tmn_vfs {

class VFSController {
public:
    static uint16_t free_sector;
public:    
    static bool IsValidUsername(const std::string& username);
    static bool IsValidFullname(const std::string& fullname);
    static bool IsValidFileName(const std::string& filename);
    static void WrongAuthorization(std::string& authorization_error);
    static void WrongCreatingFile(std::string& creating_file_error);

    static std::string GetFreeSector();
};

}