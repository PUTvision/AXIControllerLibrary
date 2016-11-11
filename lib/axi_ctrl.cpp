#include "axi_ctrl.hpp"

AXIController::AXIController(std::experimental::string_view device_name) {
  auto device_dir = get_device_dir(device_name);
	registers_size = get_registers_size(device_dir);

	fs::path chrdev_path("/dev");
	chrdev_path /= device_dir.path().filename();
	if (fd = open(chrdev_path.c_str(), O_RDWR); fd < 1) {
		throw "Opening device failed";
	}

	std::cout << registers_size << std::endl;

	registers = static_cast<volatile std::uint32_t *>(
		mmap(NULL, registers_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)
	);
	if (not registers) {
		throw "mmap failed";
	}
}

AXIController::~AXIController() {
	munmap(static_cast<void *>(const_cast<std::uint32_t *>(registers)),
	       registers_size);
	close(fd);
}
