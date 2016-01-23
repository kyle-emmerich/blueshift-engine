#pragma once

/*! \page enginelayout Engine Layout
	\tableofcontents
	This page describes the layout of the engine and the core concepts in use.
	\section el_sec1 The Core
	The core of the engine resides in Core::Engine. This class encapsulates all of the
	state of the engine: past, present, and future. Each subsystem is initialized and
	stored in the Core::Engine singleton object. 

	As a game engine, Blueshift must maintain two things in its core execution step:

	* **Speed:** Each frame must be executed in 1/framerate seconds or less.  

	* **Consistency:** The deviation from the average frame throughput cannot be too high; it is better to be
					   consistently slow than to be fast but choppy.
	
	Since modern CPUs are increasingly parallel, it is best to utilize all cores whenever possible.
	However, the number of cores varies wildly across CPUs, so it is not a good idea to simply create
	a "physics" thread, a "render" thread, and a "logic" thread. The inverse is better: create a pool
	of threads and dole out work when available.

	Now we need to define what work is. There is some overhead to communicating the preconditions and
	results of processing work. Therefore, work must be sufficiently expensive to be worth splitting.
	How does this fit into the process of running a game?

	It is obvious that the game state is the main thing we need to worry about. Each frame can be
	processed independently, but there are conditions to when a frame can be processed; input must be
	ready or logic cannot be processed. However, we need to feed the GPU enough or we're wasting its
	potential. Therefore, the game state need not be processed at the same rate as it is rendered.

	Rendering can be decoupled. We can render the state as it appears at a given time. If there is
	no change in the state as it exists in memory, we may use portions of the state to infer the state
	at a given subinterval: for example, using the velocity of an object to infer its position in between
	two logical frames. To encapsulate this, we have Core::Frame. This object contains the game state at a given point in time
	as it is processed. If objects exist outside of the simulation range, they do not need to be referenced
	in the frame. The rendering system can take a frame when it is ready and work on executing render
	commands as necessary.

	\section el_sec2 Frames
	In order to understand how Blueshift represents the game world, one must imagine two timelines.
	First, we have real physical time; it flows constantly and linearly (for our purposes) and does
	not stop and wait for the game to do its thing. Second, we have Blueshift time. This timeline is
	much more important and has a few distinctions from real-time. Most importantly, it occurs in discrete
	steps called Frames, represented by Core::Frame. We will attempt to schedule a frame for a given
	point in real-time, so the frame must be ready by then. If it is not, the engine is in a state of
	"lag" and this is undesirable. The engine must somehow compensate to eliminate lag, either by
	reducing the set of game objects included in a frame or by adjusting the expected time to process
	a frame. The synchronization of real-time and Blueshift-time is very important, or the player who
	lives in real-time might become extremely confused or frustrated.

	The engine can be thought of as a collection of systems working together to produce a frame in
	the least amount of time possible. The scene system must submit only as many objects as it determines
	the engine to be capable of processing, the input system must have the input state ready, and the
	logic system must take the input and decide what to tell the physics system. Individually, these
	are serial operations, but can be parallel across frames. The physics system can process as many
	frames as the logic system has produced. The rendering system does not care about any of the others
	and can simply interpolate between finished logical frames. If the rendering system becomes
	starved of finished frames, a serious problem has occurred and the engine will be considered to be
	in a state of lag.

	\section el_sec3 Game State
	Since Blueshift uses an entity-component system to represent the game state, it is very easy to 
	encapsulate all the data since it needs no transformation to use it. Components of a given type
	are simply stored in a list and processed by the relevant systems.

	However, making sure that the scene can select a certain subset of game objects to process and
	getting them into a form usable by the engine is a big challenge. Therefore, components must be
	stored in many lists. The granularity of the lists depends on the scene division technique. For
	Blueshift's purposes, this will be an octree. Each leaf node will contain the components that exist
	within it spatially, so there is no copying involved in selecting subsets of data to process. The
	scene system simply reports a list of octree leaves to process.

	This presents another challenge: how to determine the location of a component. It must be based on
	both the visual representation of the entity the component belongs to, and on the physical representation.
	Incorrectly placing an entity within an octree leaf is unacceptable behavior because it could result in
	physical interactions being missed or objects disappearing when a leaf is considered out of view. Some
	game objects might also need to be processed even when not visible, such as AI or other complex 
	systems that run all of the time.
	
	In order to solve this, we can keep two octrees! One for visual systems, and one for logical systems.
	The visual octree will use the object's visual representation, and the logical system will use the
	object's sphere of influence. Octrees are incredibly useful for this purpose and provide a simple
	and understandable way to keep track of when objects are relevant. When the scene system determines
	a leaf node is relevant, it must also determine that all of its parent nodes are relevant. That does
	not imply that all of the sibling nodes are relevant. This means that an object that always needs to be
	processed can simply stipulate that it must exist on the root node!

	The major drawback of this approach is that objects must be copied around to different nodes when moving.
	The scene system must evaluate that all objects are in the correct node at the beginning of a frame, which
	may end up being an unacceptable cost. However, this is a very simple operation, assuming that axis-aligned
	bounding boxes are used to determine the spatial location of an object. It may end up being desirable
	to simply treat component lists as linked lists.

*/