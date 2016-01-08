#pragma once

#include <inttypes.h>
#include <string>
#include <vector>

#include "Core/Exceptions.h"

/*! \mainpage Main Page
 *
 * \section intro_sec Introduction
 *
 *	Blueshift is a high-performance game engine written in C++ with embedded LuaJIT for scripting.
 *  It is a full package for the creation of modern games and other immersive visual applications
 *	that require exceptional responsiveness.
 *
 * \section Documentation
 *	
 *	Please note that this documentation is only for the C++ API and does not include Lua API information.
 */

/*! \page porting Porting Blueshift
	
	\section porting_sec Porting

	If you're reading this, you're probably cursing me for not porting Blueshift to your favored operating system.
	However, I'm just one guy and I have a full-time job developing someone else's game engine entirely! Blueshift
	remains my favorite personal project of all time and while I do love dedicating lots of time to it, the simple
	fact is that I really don't know much about operating systems other than Windows.

	If you would be interested in helping to port Blueshift to another operating system, it would be much appreciated!
	In order to port Blueshift, there are quite a few things that need to be added. Fortunately, I've written everything
	to take drop-in replacements, so the code you need to write is very contained.

	\section components Components to be replaced

	As a rough overview, Blueshift requires platform-specific code for the Core::Timing::IStopwatch interface, 
	for Graphics::IWindow, for the entire Platform namespace, and most of the Input namespace.

	I will update this page as time goes on with more information, including how to port these components. For now,
	feel free to jump in and figure out how my existing Windows code works, and let me know if you have any questions
	using Github's Issues for the project's repository.

*/