#include "../../include/VFS/VFSController.hpp"

namespace tmn_vfs {

uint16_t tmn_vfs::VFSController::free_sector = 0; 

bool VFSController::IsValidUsername(const std::string& username) {
  if (username.length() < 3 || username.length() > 20) {
    return false;
  }

  for (char c : username) {
    if (!isalnum(c) && c != '-' && c != '_') {
      return false;
    }
    if (isalpha(c) && !islower(c) && !isupper(c)){
        return false;
    }
  }
  return true;
}

bool VFSController::IsValidFullname(const std::string& fullname) {
  if (fullname.length() < 3 || fullname.length() > 20) {
    return false;
  }

  for (char c : fullname) {
    if (isdigit(c)) {
      return false;
    }
    if (!isalpha(c) && c != ' ') {
      return false;
    }
    if (isalpha(c) && !islower(c) && !isupper(c)){
        return false;
    }
  }

  return true;
}

bool tmn_vfs::VFSController::IsValidFileName(const std::string& filename){
  if (filename.empty()) {
    return false;
  }
  for (char c : filename) {
    if (!std::isalnum(c) && !(c == '-') && !(c == '_')) {
      return false;
    }
  }

  if (filename.length() > 20) {
    return false;
  }
  
  return true;
}

void VFSController::WrongAuthorization(std::string& authorization_error){
  authorization_error  = "Something wrong... Please, try again.";
}

void tmn_vfs::VFSController::WrongCreatingFile(std::string& creating_file_error){
  creating_file_error  = "Something wrong (most likely an error in the file name - it should be unique)";
}

std::string tmn_vfs::VFSController::GetFreeSector(){
    return tmn_vfs::MOUNT_DIR + tmn_vfs::VFS_DIR + std::to_string(free_sector);
    free_sector++;
}

}