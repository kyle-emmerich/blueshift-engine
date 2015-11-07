![Blueshift Engine](blueshift.png)

# Blueshift Engine
Blueshift is a high performance game engine written for the modern game industry. It must be held to high standards, as it will serve as the engine for Titan Interactive's flagship game, NoxAstra.

# Twitch Stream
Kyle will stream Blueshift/NoxAstra development almost daily. There's no set time yet, so just check it out occasionally.

[Twitch Link](http://twitch.tv/noxastra)

## Dependencies
Blueshift seeks to minimize dependencies in order to maximize portability and keep the codebase size down. Right now, we're using the following dependencies:

 - [bgfx](https://github.com/bkaradzic/bgfx)
 - [bx](https://github.com/bkaradzic/bx)
 - [bullet](https://github.com/bulletphysics/bullet3)
 - [physfs](https://icculus.org/physfs)

For all libraries with compiled binaries in the repository, one may expect binaries compiled for x64 systems in debug and release mode with MSVC 2015 build tools.

###64-bit
Blueshift is currently 64-bit only. This might change if absolutely necessary, but it's unlikely. Most of the engine will be written using double precision floats and would run badly on 32-bit systems.

###Code Style
Blueshift's codebase exemplifies the best practices in modern C++. Everything must be exception-safe, const-correct, and free of memory leaks. Raw pointers are not shyed away from, but their use
in the public API is minimal and ownership is well-documented.

For more information, see the Code Style file under Documentation in the repository.

###Coordinate System
We're using a left-handed coordinate system where +X is to the right, +Y is up, and +Z is away. If the camera is moving in the positive Z direction, it is moving forward. The sine of a 45 degree angle up from the camera's Z axis is positive. Unreal Engine 4 is wrong. There is no debate. End of discussion.

###Matrix Operations
Blueshift uses row-major matrices. There's no real reason to choose one or the other unless you're using plain old C-style multidimensional arrays (row major by specification.) Blueshift uses a C union to provide multiple ways to access the contained data, so row-major makes sense. Here's what our matrices look like laid out:

Index order:

    0   1   2   3
    4   5   6   7
    8   9   10  11
    12  13  14  15

This means the base vectors are laid out like so:

    X.x X.y X.z 0 //right-vector
    Y.x Y.y Y.z 0 //up-vector
    Z.x Z.y Z.z 0 //forward-vector
    P.x P.y P.z 1 //position

There is some confusion in how much this actually matters. The real answer is that it really doesn't matter that much, but it does carry some implications when dealing with multiplication order. Recall that matrix multiplication is not commutative, so `a * b` is not the same as `b * a`. Usually, you'll want to multiply a parent space by the local transform to get the local transform into world space:

    Matrix4 world_transform = parent_transform * local_transform;

However, keep in mind that points and vectors are treated differently. Specifically, translating a vector makes no sense and will produce a "wrong" result. In order to explicitly handle vectors vs points, convert them 4-vectors and use W=0 for vectors and W=1 for points. Helper functions might be added later in order to clarify the procedure.

###Documentation
As of right now (October 25th, 2015), there is no documentation other than the source code itself. Don't be afraid to dive in and start learning; the API is really clean and simple so far. When the project continues to grow and when Kyle has free time, documentation will begin.

However, ReadTheDocs is terrible and Doxygen is terrible. A custom solution is probably in order, probably based on the [NoxAstra](https://noxastra.com) website.
