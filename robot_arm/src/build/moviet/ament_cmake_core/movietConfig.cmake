# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_moviet_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED moviet_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(moviet_FOUND FALSE)
  elseif(NOT moviet_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(moviet_FOUND FALSE)
  endif()
  return()
endif()
set(_moviet_CONFIG_INCLUDED TRUE)

# output package information
if(NOT moviet_FIND_QUIETLY)
  message(STATUS "Found moviet: 0.3.0 (${moviet_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'moviet' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${moviet_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(moviet_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${moviet_DIR}/${_extra}")
endforeach()
