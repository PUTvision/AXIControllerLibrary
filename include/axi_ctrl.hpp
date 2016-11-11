#ifndef AXICONTROLLER_H
#define AXICONTROLLER_H

#include <string>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <experimental/filesystem>
#include <experimental/string_view>

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

namespace fs = std::experimental::filesystem;

template<std::uint32_t RegisterNumber, std::uint8_t Begin = 0, std::uint8_t End = 32>
struct RegisterMapping{};

class AXIController {
private:
	int fd;
	std::uint32_t registers_size = 0;
	volatile std::uint32_t *registers = nullptr;

	inline fs::directory_entry get_device_dir(std::experimental::string_view device_name) {
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

	inline std::uint32_t get_registers_size(const fs::directory_entry &device_dir) {
		std::ifstream size_file(device_dir.path() / "maps/map0/size");
		if (size_file.is_open()) {
			std::uint32_t size;
			size_file >> std::hex >> size;

			return size;
		}

		throw "Not found";
	}

public:
	AXIController(std::experimental::string_view device_name);
	~AXIController();

	template<std::uint32_t RegisterNumber, std::uint8_t Begin, std::uint8_t End>
	std::uint32_t get_property(const RegisterMapping<RegisterNumber, Begin, End> &) {
    constexpr std::uint32_t mask = (1l << (1 + End - Begin)) - 1;
    return (registers[RegisterNumber] >> Begin) & mask;
  }

  template<std::uint32_t RegisterNumber, std::uint8_t Begin, std::uint8_t End>
	void set_property(const RegisterMapping<RegisterNumber, Begin, End> &, std::uint32_t value) {
    constexpr std::uint32_t mask = (1l << (1 + End - Begin)) - 1;
    registers[RegisterNumber] = value & mask << Begin;
  }
};

#endif // AXICONTROLLER_H
