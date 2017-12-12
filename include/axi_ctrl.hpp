#pragma once

#include <experimental/filesystem>
#include <string_view>

namespace fs = std::experimental::filesystem;

template <std::uint32_t RegisterNumber, std::uint8_t Begin = 0, std::uint8_t End = 32>
struct RegisterMapping {};

class AXIController {
private:
  int fd;
  std::uint32_t registers_size = 0;
  volatile std::uint32_t *registers = nullptr;

  fs::directory_entry get_device_dir(std::string_view device_name);
  std::uint32_t get_registers_size(const fs::directory_entry &device_dir);

public:
  AXIController(std::string_view device_name);
  ~AXIController();

  template <std::uint32_t RegisterNumber, std::uint8_t Begin, std::uint8_t End>
  std::uint32_t get_property(const RegisterMapping<RegisterNumber, Begin, End> &) {
    constexpr std::uint32_t mask = (1l << (1 + End - Begin)) - 1;
    return (registers[RegisterNumber] >> Begin) & mask;
  }

  template <std::uint32_t RegisterNumber, std::uint8_t Begin, std::uint8_t End>
  void set_property(const RegisterMapping<RegisterNumber, Begin, End> &, std::uint32_t value) {
    constexpr std::uint32_t mask = (1l << (1 + End - Begin)) - 1;
    registers[RegisterNumber] &= ~(mask << Begin);
    registers[RegisterNumber] |= (value & mask) << Begin;
  }
};
