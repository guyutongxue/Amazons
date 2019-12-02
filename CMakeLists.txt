# Copyright (c) 2019 Guyutongxue
# 
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(amazons)

SET(CMAKE_CXX_STANDARD 17)
ADD_COMPILE_OPTIONS(-static-libstdc++ -finput-charset=UTF-8 -fexec-charset=UTF-8)
ADD_COMPILE_OPTIONS(-ggdb)
# ADD_COMPILE_OPTIONS(-O2)

AUX_SOURCE_DIRECTORY(src DIR_SRCS)

SET(EXECUTABLE_OUTPUT_PATH build)
ADD_EXECUTABLE(amazons ${DIR_SRCS})