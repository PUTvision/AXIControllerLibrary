#include <iostream>
#include <thread>

#include <axi_ctrl.hpp>
#include <axidma_ioctl.h>
#include <libaxidma.h>

namespace input_registers {

constexpr const RegisterMapping<0, 0, 0> software_reset;
constexpr const RegisterMapping<1, 0, 9> number_of_coprocessor_operations;
constexpr const RegisterMapping<1, 10, 19> number_of_reads;
constexpr const RegisterMapping<2, 0, 9> number_of_fills;
constexpr const RegisterMapping<2, 10, 19> number_of_writes;
constexpr const RegisterMapping<3, 0, 19> fill_value;

} // namespace input_registers

namespace output_registers {

constexpr const RegisterMapping<9> test;
constexpr const RegisterMapping<10, 0, 9> number_of_reads;
constexpr const RegisterMapping<11, 0, 9> number_of_fills;
constexpr const RegisterMapping<12, 0, 9> number_of_coprocessor_operations;
constexpr const RegisterMapping<13, 0, 9> number_of_writes;
constexpr const RegisterMapping<14, 0, 10> input_buffer_data_count;
constexpr const RegisterMapping<15, 0, 10> output_buffer_data_count;

} // namespace output_registers

void transfer();

void transfer() {
  auto axidma_dev = axidma_init();
  if (axidma_dev == nullptr) {
    std::cerr << "Error: Failed to initialize the AXI DMA device." << std::endl;
    std::exit(1);
  }

  auto tx_chans = axidma_get_dma_tx(axidma_dev);
  if (tx_chans->len < 1) {
    std::cerr << "Error: No transmit channels were found." << std::endl;
    std::exit(1);
  }
  auto rx_chans = axidma_get_dma_rx(axidma_dev);
  if (rx_chans->len < 1) {
    std::cerr << "Error: No receive channels were found." << std::endl;
    std::exit(1);
  }

  auto input = reinterpret_cast<std::uint32_t *>(axidma_malloc(axidma_dev, 64));
  auto output = reinterpret_cast<std::uint32_t *>(axidma_malloc(axidma_dev, 64));

  for (std::uint32_t i = 0; i < 16; i++) {
    input[i] = i;
  }

  int rc = axidma_twoway_transfer(axidma_dev, tx_chans->data[0], input, 64, rx_chans->data[0], output, 64, true);
  if (rc < 0) {
    std::cerr << "DMA read write transaction failed." << std::endl;
    std::exit(1);
  }

  for (int i = 0; i < 16; i++) {
    std::cout << output[i] << std::endl;
  }

  axidma_free(axidma_dev, input, 64);
  axidma_free(axidma_dev, output, 64);
}

int main(int /*argc*/, char * /*argv*/ []) {
  try {
    AXIController controller{"adder_axi_ctrl"};
    controller.set_property(input_registers::number_of_coprocessor_operations, 20);
    controller.set_property(input_registers::number_of_reads, 16);
    controller.set_property(input_registers::number_of_writes, 16);
    controller.set_property(input_registers::number_of_fills, 4);
    controller.set_property(input_registers::fill_value, 100);

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    transfer();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "Stats:" << std::endl;
    std::cout << controller.get_property(output_registers::number_of_coprocessor_operations) << std::endl;
    std::cout << controller.get_property(output_registers::number_of_reads) << std::endl;
    std::cout << controller.get_property(output_registers::number_of_writes) << std::endl;
  } catch (const char *e) {
    std::cout << e << std::endl;
    return -1;
  }

  return 0;
}
