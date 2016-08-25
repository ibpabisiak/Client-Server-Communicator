/**
 * stdafx.h : include file for standard system include files,
 * or project specific include files that are used frequently, but
 * are changed infrequently
 */

#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <WinSock2.h>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <regex>
#include <sstream>
#include <condition_variable>
#include <mutex>

//defines of the server configuration
#define QUEUE_LISTEN_LIMIT	10
#define MAX_BUFFER_SIZE		256
#define MAX_CLIENTS			32
#define WELCOME_MSG			"> Connection successful, welcome!\n> Please, enter your nickname.."
