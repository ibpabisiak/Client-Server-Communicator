// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

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
#include <sstream>

#define	PORT			1234 //port do polaczenia sie
#define BACKLOG			10 // max polaczen w kolejce w lsiten()
#define MAX_BUFFER_SIZE	256