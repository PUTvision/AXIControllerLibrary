#include <iostream>
#include <axi_ctrl.hpp>

namespace input_registers {
  constexpr const RegisterMapping<0, 0, 0> software_reset;
  constexpr const RegisterMapping<1, 0, 9> number_of_coprocessors;
  constexpr const RegisterMapping<1, 10, 19> number_of_reads;
  constexpr const RegisterMapping<2, 0, 9> fill;
  constexpr const RegisterMapping<2, 10, 19> number_of_writes;
  constexpr const RegisterMapping<3> fill_value;
}

namespace output_registers {
  constexpr const RegisterMapping<9> test;
  constexpr const RegisterMapping<10, 0, 9> number_of_reads;
  constexpr const RegisterMapping<11, 0, 9> number_of_fills;
  constexpr const RegisterMapping<12, 0, 9> number_of_coprocessors;
  constexpr const RegisterMapping<13, 0, 9> number_of_writes;
  constexpr const RegisterMapping<14, 0, 10> input_buffer_data_count;
  constexpr const RegisterMapping<15, 0, 10> output_buffer_data_count;
}

int main(int argc, char **argv) {
		try {
			AXIController controller{"adder_axi_ctrl"};
      controller.get_property(input_registers::number_of_reads);
		}
		catch (const char *e) {
			std::cout << e << std::endl;
			return -1;
		}

    return 0;
}
