include(GNUInstallDirs)

if (CMAKE_CUDA_COMPILER) # Ensure we have proper compile definitions for torch import
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CUDA_STANDARD 17)
  set(CMAKE_CUDA_STANDARD_REQUIRED TRUE)
  set(module_compile_definitions WITH_CUDA)
  set(module_source_files tensor_buffer_kernel.cu)
endif()

# Require input libraries
find_library(libcaer NAMES caer libcaer REQUIRED)
# find_library(MetavisionSDK NAMES metavision_sdk_core REQUIRED)

find_package(PythonExtensions REQUIRED)
find_package(Torch REQUIRED)

# Import PyTorch's Python library to ensure correct linking
# Thanks to https://github.com/pytorch/pytorch/issues/38122
find_library(TORCH_PYTHON_LIBRARY torch_python PATHS "${TORCH_INSTALL_PREFIX}/lib")
message(STATUS "TORCH_PYTHON_LIBRARY: ${TORCH_PYTHON_LIBRARY} - ${PYTHON_INCLUDE_DIRS}")
message(STATUS "TORCH_LIBRARIES: ${TORCH_LIBRARIES} - ${TORCH_INCLUDE_DIRS}")

set(PYTHON_MODULE_NAME aestream_ext)
add_library(${PYTHON_MODULE_NAME} MODULE
      module.cpp 
      udp.cpp 
      usb.cpp 
      udp_client.cpp 
      udp_client.hpp 
      convert.cpp 
      convert.hpp 
      tensor_buffer.hpp 
      tensor_buffer.cpp
      ${module_source_files})

python_extension_module(${PYTHON_MODULE_NAME})

target_compile_definitions(${PYTHON_MODULE_NAME} PUBLIC ${module_compile_definitions})
target_compile_features(${PYTHON_MODULE_NAME} PUBLIC cxx_std_20)
target_link_libraries(${PYTHON_MODULE_NAME} "${TORCH_LIBRARIES}" "${TORCH_PYTHON_LIBRARY}" aedat4 aestream_input aestream_output)
target_include_directories(${PYTHON_MODULE_NAME} PUBLIC "${TORCH_INCLUDE_DIRS}" ${CMAKE_INSTALL_LIBDIR})
set_target_properties(${PYTHON_MODULE_NAME} PROPERTIES 
  INSTALL_RPATH "${PYTHON_SITE_PACKAGES_DIR}/aestream;${PYTHON_SITE_PACKAGES_DIR}/torch/" # Force searching in Python's packages
  INSTALL_RPATH_USE_LINK_PATH TRUE
)

install(TARGETS aedat4 aestream_input aestream_output DESTINATION aestream)
install(TARGETS ${PYTHON_MODULE_NAME} LIBRARY DESTINATION aestream)