# Install script for directory: C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/mi_websocket_cpp")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/msys64/mingw64/bin/objdump.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/alexk/Documents/github/PAE-2025/build/third_party/IXWebSocket/libixwebsocket.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ixwebsocket" TYPE FILE FILES
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXBase64.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXBench.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXCancellationRequest.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXConnectionState.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXDNSLookup.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXExponentialBackoff.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXGetFreePort.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXGzipCodec.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXHttp.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXHttpClient.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXHttpServer.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXNetSystem.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXProgressCallback.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXSelectInterrupt.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXSelectInterruptFactory.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXSelectInterruptPipe.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXSelectInterruptEvent.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXSetThreadName.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXSocket.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXSocketConnect.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXSocketFactory.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXSocketServer.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXSocketTLSOptions.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXStrCaseCompare.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXUdpSocket.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXUniquePtr.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXUrlParser.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXUuid.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXUtf8Validator.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXUserAgent.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXWebSocket.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXWebSocketCloseConstants.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXWebSocketCloseInfo.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXWebSocketErrorInfo.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXWebSocketHandshake.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXWebSocketHandshakeKeyGen.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXWebSocketHttpHeaders.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXWebSocketInitResult.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXWebSocketMessage.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXWebSocketMessageType.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXWebSocketOpenInfo.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXWebSocketPerMessageDeflate.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXWebSocketPerMessageDeflateCodec.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXWebSocketPerMessageDeflateOptions.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXWebSocketProxyServer.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXWebSocketSendData.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXWebSocketSendInfo.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXWebSocketServer.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXWebSocketTransport.h"
    "C:/Users/alexk/Documents/github/PAE-2025/third_party/IXWebSocket/ixwebsocket/IXWebSocketVersion.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/ixwebsocket" TYPE FILE FILES "C:/Users/alexk/Documents/github/PAE-2025/build/third_party/IXWebSocket/ixwebsocket-config.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Users/alexk/Documents/github/PAE-2025/build/third_party/IXWebSocket/ixwebsocket.pc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ixwebsocket/ixwebsocket-targets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ixwebsocket/ixwebsocket-targets.cmake"
         "C:/Users/alexk/Documents/github/PAE-2025/build/third_party/IXWebSocket/CMakeFiles/Export/dbc99e06a99e696141dafd40631f8060/ixwebsocket-targets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ixwebsocket/ixwebsocket-targets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ixwebsocket/ixwebsocket-targets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/ixwebsocket" TYPE FILE FILES "C:/Users/alexk/Documents/github/PAE-2025/build/third_party/IXWebSocket/CMakeFiles/Export/dbc99e06a99e696141dafd40631f8060/ixwebsocket-targets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/ixwebsocket" TYPE FILE FILES "C:/Users/alexk/Documents/github/PAE-2025/build/third_party/IXWebSocket/CMakeFiles/Export/dbc99e06a99e696141dafd40631f8060/ixwebsocket-targets-noconfig.cmake")
  endif()
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/alexk/Documents/github/PAE-2025/build/third_party/IXWebSocket/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
