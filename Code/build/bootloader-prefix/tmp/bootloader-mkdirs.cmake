# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Espressif/frameworks/esp-idf-v4.4/components/bootloader/subproject"
  "C:/ece445/Phone_Audio_FM_Transmitter/Code/build/bootloader"
  "C:/ece445/Phone_Audio_FM_Transmitter/Code/build/bootloader-prefix"
  "C:/ece445/Phone_Audio_FM_Transmitter/Code/build/bootloader-prefix/tmp"
  "C:/ece445/Phone_Audio_FM_Transmitter/Code/build/bootloader-prefix/src/bootloader-stamp"
  "C:/ece445/Phone_Audio_FM_Transmitter/Code/build/bootloader-prefix/src"
  "C:/ece445/Phone_Audio_FM_Transmitter/Code/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/ece445/Phone_Audio_FM_Transmitter/Code/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
