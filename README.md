![Blueshift Engine](blueshift.png)

# Blueshift Engine
Blueshift is a high performance game engine written for the modern game industry. It must be held to high standards, as it will serve as the engine for Titan Interactive's flagship game, NoxAstra.

## Dependencies
Blueshift seeks to minimize dependencies in order to maximize portability and keep the codebase size down. Right now, we're using the following dependencies:

 - [bgfx](https://github.com/bkaradzic/bgfx)
 - [bx](https://github.com/bkaradzic/bx)
 - [bullet](https://github.com/bulletphysics/bullet3)

For all libraries with compiled binaries in the repository, one may expect binaries compiled for x64 systems in debug and release mode with MSVC 2015 build tools.

###Matrix Operations
Blueshift uses column-major matrices. There's no real reason to choose one or the other unless you're using plain old C-style multidimensional arrays (row major by specification.) However, Blueshift's math library uses a 1-dimensional array and provides a convenient helper function to index the array (`matrix(i, j)`). Here's what our matrices look like laid out:

Index order:

    0  4  8  12
    1  5  9  13
    2  6  10 14
    3  7  11 15

This means the base vectors are laid out like so:

    X.x X.y X.z 0
    Y.x Y.y Y.z 0
    Z.x Z.y Z.z 0
    P.x P.y P.z 1

Because Blueshift is column-major, remember to keep all matrix multiplication operations in the correct order:

    Matrix4 world_space = parent_transform * local_transform;
