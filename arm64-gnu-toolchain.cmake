# Define the system name and processor
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_PROCESSOR aarch64)

# Specify the cross compiler locations
SET(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
SET(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)

# Adjust the default behavior of the FIND_XXX() commands:
# search headers and libraries in the target environment, directive from here.
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)