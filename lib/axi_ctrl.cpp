#include "axi_ctrl.hpp"

#include <fstream>
#include <iostream>
#include <string>

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

AXIController::AXIController(std::string_view device_name) {
  auto device_dir = get_device_dir(device_name);
  registers_size = get_registers_size(device_dir);

  fs::path chrdev_path("/dev");
  chrdev_path /= device_dir.path().filename();
  if (fd = open(chrdev_path.c_str(), O_RDWR); fd < 1) {
    throw "Opening device failed";
  }

  std::cout << registers_size << std::endl;

  registers =
      static_cast<volatile std::uint32_t *>(mmap(nullptr, registers_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
  if (not registers) {
    throw "mmap failed";
  }
}

AXIController::~AXIController() {
  munmap(static_cast<void *>(const_cast<std::uint32_t *>(registers)), registers_size);
  close(fd);
}

fs::directory_entry AXIController::get_device_dir(std::string_view device_name) {
  auto dir = fs::directory_iterator("/sys/class/uio");
  for (const auto &entry : dir) {
    if (std::ifstream name_file(entry.path() / "name"); name_file.is_open()) {
      std::string current_device_name;
      name_file >> current_device_name;

      if (current_device_name == device_name) {
        return entry;
      }
    }
  }

  throw "Not found";
}

std::uint32_t AXIController::get_registers_size(const fs::directory_entry &device_dir) {
  std::ifstream size_file(device_dir.path() / "maps/map0/size");
  if (size_file.is_open()) {
    std::uint32_t size;
    size_file >> std::hex >> size;

    return size;
  }

  throw "Not found";
}
