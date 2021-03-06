# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "tmapping: 0 messages, 5 services")

set(MSG_I_FLAGS "-Istd_msgs:/opt/ros/melodic/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(tmapping_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/zzw/zzw/tmap/src/Tmapping/srv/GateMovement.srv" NAME_WE)
add_custom_target(_tmapping_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "tmapping" "/home/zzw/zzw/tmap/src/Tmapping/srv/GateMovement.srv" ""
)

get_filename_component(_filename "/home/zzw/zzw/tmap/src/Tmapping/srv/NewExp.srv" NAME_WE)
add_custom_target(_tmapping_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "tmapping" "/home/zzw/zzw/tmap/src/Tmapping/srv/NewExp.srv" ""
)

get_filename_component(_filename "/home/zzw/zzw/tmap/src/Tmapping/srv/arucoId.srv" NAME_WE)
add_custom_target(_tmapping_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "tmapping" "/home/zzw/zzw/tmap/src/Tmapping/srv/arucoId.srv" ""
)

get_filename_component(_filename "/home/zzw/zzw/tmap/src/Tmapping/srv/SetSurviverMapsNum.srv" NAME_WE)
add_custom_target(_tmapping_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "tmapping" "/home/zzw/zzw/tmap/src/Tmapping/srv/SetSurviverMapsNum.srv" ""
)

get_filename_component(_filename "/home/zzw/zzw/tmap/src/Tmapping/srv/GetMaps.srv" NAME_WE)
add_custom_target(_tmapping_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "tmapping" "/home/zzw/zzw/tmap/src/Tmapping/srv/GetMaps.srv" ""
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages

### Generating Services
_generate_srv_cpp(tmapping
  "/home/zzw/zzw/tmap/src/Tmapping/srv/GateMovement.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/tmapping
)
_generate_srv_cpp(tmapping
  "/home/zzw/zzw/tmap/src/Tmapping/srv/NewExp.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/tmapping
)
_generate_srv_cpp(tmapping
  "/home/zzw/zzw/tmap/src/Tmapping/srv/arucoId.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/tmapping
)
_generate_srv_cpp(tmapping
  "/home/zzw/zzw/tmap/src/Tmapping/srv/SetSurviverMapsNum.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/tmapping
)
_generate_srv_cpp(tmapping
  "/home/zzw/zzw/tmap/src/Tmapping/srv/GetMaps.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/tmapping
)

### Generating Module File
_generate_module_cpp(tmapping
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/tmapping
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(tmapping_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(tmapping_generate_messages tmapping_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/zzw/zzw/tmap/src/Tmapping/srv/GateMovement.srv" NAME_WE)
add_dependencies(tmapping_generate_messages_cpp _tmapping_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/zzw/zzw/tmap/src/Tmapping/srv/NewExp.srv" NAME_WE)
add_dependencies(tmapping_generate_messages_cpp _tmapping_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/zzw/zzw/tmap/src/Tmapping/srv/arucoId.srv" NAME_WE)
add_dependencies(tmapping_generate_messages_cpp _tmapping_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/zzw/zzw/tmap/src/Tmapping/srv/SetSurviverMapsNum.srv" NAME_WE)
add_dependencies(tmapping_generate_messages_cpp _tmapping_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/zzw/zzw/tmap/src/Tmapping/srv/GetMaps.srv" NAME_WE)
add_dependencies(tmapping_generate_messages_cpp _tmapping_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(tmapping_gencpp)
add_dependencies(tmapping_gencpp tmapping_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS tmapping_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages

### Generating Services
_generate_srv_eus(tmapping
  "/home/zzw/zzw/tmap/src/Tmapping/srv/GateMovement.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/tmapping
)
_generate_srv_eus(tmapping
  "/home/zzw/zzw/tmap/src/Tmapping/srv/NewExp.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/tmapping
)
_generate_srv_eus(tmapping
  "/home/zzw/zzw/tmap/src/Tmapping/srv/arucoId.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/tmapping
)
_generate_srv_eus(tmapping
  "/home/zzw/zzw/tmap/src/Tmapping/srv/SetSurviverMapsNum.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/tmapping
)
_generate_srv_eus(tmapping
  "/home/zzw/zzw/tmap/src/Tmapping/srv/GetMaps.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/tmapping
)

### Generating Module File
_generate_module_eus(tmapping
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/tmapping
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(tmapping_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(tmapping_generate_messages tmapping_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/zzw/zzw/tmap/src/Tmapping/srv/GateMovement.srv" NAME_WE)
add_dependencies(tmapping_generate_messages_eus _tmapping_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/zzw/zzw/tmap/src/Tmapping/srv/NewExp.srv" NAME_WE)
add_dependencies(tmapping_generate_messages_eus _tmapping_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/zzw/zzw/tmap/src/Tmapping/srv/arucoId.srv" NAME_WE)
add_dependencies(tmapping_generate_messages_eus _tmapping_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/zzw/zzw/tmap/src/Tmapping/srv/SetSurviverMapsNum.srv" NAME_WE)
add_dependencies(tmapping_generate_messages_eus _tmapping_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/zzw/zzw/tmap/src/Tmapping/srv/GetMaps.srv" NAME_WE)
add_dependencies(tmapping_generate_messages_eus _tmapping_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(tmapping_geneus)
add_dependencies(tmapping_geneus tmapping_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS tmapping_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages

### Generating Services
_generate_srv_lisp(tmapping
  "/home/zzw/zzw/tmap/src/Tmapping/srv/GateMovement.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/tmapping
)
_generate_srv_lisp(tmapping
  "/home/zzw/zzw/tmap/src/Tmapping/srv/NewExp.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/tmapping
)
_generate_srv_lisp(tmapping
  "/home/zzw/zzw/tmap/src/Tmapping/srv/arucoId.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/tmapping
)
_generate_srv_lisp(tmapping
  "/home/zzw/zzw/tmap/src/Tmapping/srv/SetSurviverMapsNum.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/tmapping
)
_generate_srv_lisp(tmapping
  "/home/zzw/zzw/tmap/src/Tmapping/srv/GetMaps.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/tmapping
)

### Generating Module File
_generate_module_lisp(tmapping
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/tmapping
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(tmapping_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(tmapping_generate_messages tmapping_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/zzw/zzw/tmap/src/Tmapping/srv/GateMovement.srv" NAME_WE)
add_dependencies(tmapping_generate_messages_lisp _tmapping_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/zzw/zzw/tmap/src/Tmapping/srv/NewExp.srv" NAME_WE)
add_dependencies(tmapping_generate_messages_lisp _tmapping_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/zzw/zzw/tmap/src/Tmapping/srv/arucoId.srv" NAME_WE)
add_dependencies(tmapping_generate_messages_lisp _tmapping_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/zzw/zzw/tmap/src/Tmapping/srv/SetSurviverMapsNum.srv" NAME_WE)
add_dependencies(tmapping_generate_messages_lisp _tmapping_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/zzw/zzw/tmap/src/Tmapping/srv/GetMaps.srv" NAME_WE)
add_dependencies(tmapping_generate_messages_lisp _tmapping_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(tmapping_genlisp)
add_dependencies(tmapping_genlisp tmapping_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS tmapping_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages

### Generating Services
_generate_srv_nodejs(tmapping
  "/home/zzw/zzw/tmap/src/Tmapping/srv/GateMovement.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/tmapping
)
_generate_srv_nodejs(tmapping
  "/home/zzw/zzw/tmap/src/Tmapping/srv/NewExp.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/tmapping
)
_generate_srv_nodejs(tmapping
  "/home/zzw/zzw/tmap/src/Tmapping/srv/arucoId.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/tmapping
)
_generate_srv_nodejs(tmapping
  "/home/zzw/zzw/tmap/src/Tmapping/srv/SetSurviverMapsNum.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/tmapping
)
_generate_srv_nodejs(tmapping
  "/home/zzw/zzw/tmap/src/Tmapping/srv/GetMaps.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/tmapping
)

### Generating Module File
_generate_module_nodejs(tmapping
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/tmapping
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(tmapping_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(tmapping_generate_messages tmapping_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/zzw/zzw/tmap/src/Tmapping/srv/GateMovement.srv" NAME_WE)
add_dependencies(tmapping_generate_messages_nodejs _tmapping_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/zzw/zzw/tmap/src/Tmapping/srv/NewExp.srv" NAME_WE)
add_dependencies(tmapping_generate_messages_nodejs _tmapping_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/zzw/zzw/tmap/src/Tmapping/srv/arucoId.srv" NAME_WE)
add_dependencies(tmapping_generate_messages_nodejs _tmapping_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/zzw/zzw/tmap/src/Tmapping/srv/SetSurviverMapsNum.srv" NAME_WE)
add_dependencies(tmapping_generate_messages_nodejs _tmapping_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/zzw/zzw/tmap/src/Tmapping/srv/GetMaps.srv" NAME_WE)
add_dependencies(tmapping_generate_messages_nodejs _tmapping_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(tmapping_gennodejs)
add_dependencies(tmapping_gennodejs tmapping_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS tmapping_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages

### Generating Services
_generate_srv_py(tmapping
  "/home/zzw/zzw/tmap/src/Tmapping/srv/GateMovement.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/tmapping
)
_generate_srv_py(tmapping
  "/home/zzw/zzw/tmap/src/Tmapping/srv/NewExp.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/tmapping
)
_generate_srv_py(tmapping
  "/home/zzw/zzw/tmap/src/Tmapping/srv/arucoId.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/tmapping
)
_generate_srv_py(tmapping
  "/home/zzw/zzw/tmap/src/Tmapping/srv/SetSurviverMapsNum.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/tmapping
)
_generate_srv_py(tmapping
  "/home/zzw/zzw/tmap/src/Tmapping/srv/GetMaps.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/tmapping
)

### Generating Module File
_generate_module_py(tmapping
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/tmapping
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(tmapping_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(tmapping_generate_messages tmapping_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/zzw/zzw/tmap/src/Tmapping/srv/GateMovement.srv" NAME_WE)
add_dependencies(tmapping_generate_messages_py _tmapping_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/zzw/zzw/tmap/src/Tmapping/srv/NewExp.srv" NAME_WE)
add_dependencies(tmapping_generate_messages_py _tmapping_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/zzw/zzw/tmap/src/Tmapping/srv/arucoId.srv" NAME_WE)
add_dependencies(tmapping_generate_messages_py _tmapping_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/zzw/zzw/tmap/src/Tmapping/srv/SetSurviverMapsNum.srv" NAME_WE)
add_dependencies(tmapping_generate_messages_py _tmapping_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/zzw/zzw/tmap/src/Tmapping/srv/GetMaps.srv" NAME_WE)
add_dependencies(tmapping_generate_messages_py _tmapping_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(tmapping_genpy)
add_dependencies(tmapping_genpy tmapping_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS tmapping_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/tmapping)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/tmapping
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(tmapping_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/tmapping)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/tmapping
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(tmapping_generate_messages_eus std_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/tmapping)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/tmapping
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(tmapping_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/tmapping)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/tmapping
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(tmapping_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/tmapping)
  install(CODE "execute_process(COMMAND \"/usr/bin/python2\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/tmapping\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/tmapping
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(tmapping_generate_messages_py std_msgs_generate_messages_py)
endif()
