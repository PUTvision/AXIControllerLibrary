cmake_minimum_required(VERSION 3.7)

include_directories("xilinx_axidma/include")

add_library(libaxidma SHARED "xilinx_axidma/library/libaxidma.c")
