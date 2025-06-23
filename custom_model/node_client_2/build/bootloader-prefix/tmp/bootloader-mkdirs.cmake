# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/ASUS/esp/v5.3.1/esp-idf/components/bootloader/subproject"
  "D:/Study/PTIT/Do an tot nghiep/project_BLE_Mesh/ESP-IDF_BLE_MEsh/esp_ble_mesh/custom_model/node_client_2/build/bootloader"
  "D:/Study/PTIT/Do an tot nghiep/project_BLE_Mesh/ESP-IDF_BLE_MEsh/esp_ble_mesh/custom_model/node_client_2/build/bootloader-prefix"
  "D:/Study/PTIT/Do an tot nghiep/project_BLE_Mesh/ESP-IDF_BLE_MEsh/esp_ble_mesh/custom_model/node_client_2/build/bootloader-prefix/tmp"
  "D:/Study/PTIT/Do an tot nghiep/project_BLE_Mesh/ESP-IDF_BLE_MEsh/esp_ble_mesh/custom_model/node_client_2/build/bootloader-prefix/src/bootloader-stamp"
  "D:/Study/PTIT/Do an tot nghiep/project_BLE_Mesh/ESP-IDF_BLE_MEsh/esp_ble_mesh/custom_model/node_client_2/build/bootloader-prefix/src"
  "D:/Study/PTIT/Do an tot nghiep/project_BLE_Mesh/ESP-IDF_BLE_MEsh/esp_ble_mesh/custom_model/node_client_2/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Study/PTIT/Do an tot nghiep/project_BLE_Mesh/ESP-IDF_BLE_MEsh/esp_ble_mesh/custom_model/node_client_2/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/Study/PTIT/Do an tot nghiep/project_BLE_Mesh/ESP-IDF_BLE_MEsh/esp_ble_mesh/custom_model/node_client_2/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
