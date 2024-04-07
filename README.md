# Smol Blit2D

A C++20 framework for low resolution 2D software rendering.

## Installation

This library uses CMake as its build system, so you can simply git clone it and then build & install it locally.

## Usage

Although it is possible to use the headers of individual classes and structs, it is recomended to simply include the entire library at once.

```C++
#include <smol/blit2d.hpp>
// You may now use Smol Blit2D
```

Once this is done, you may use the `Renderer` class to draw things to a target bitmap, then feed the pixel data of that bitmap to the platform dependent/independent API of your choice.

```C++
using Smol::Blit2D::Renderer;

void main()
{
	// The renderer contains an internal bitmap it uses as a target by default.
	// You (the user) initialize this bitmap through the renderer's constructor.
	Renderer renderer(256, 192);
	
	// Clear the entire target to black.
	renderer.SetColor(Smol::Blit2D::ColorName::Black);
	renderer.DrawFill();
	
	// Draw a red rectangle.
	renderer.SetColor(Smol::Blit2D::ColorName::Red);
	renderer.DrawRect({ 8, 8, 248, 32 }); // left, top, right, bottom
	
	// Feed our target's pixel data to some OS function.
	auto ptr = &renderer.GetMainTarget().At({ 0, 0 });
	SomeFunctionThatUpdatesAWindowOrSomething(ptr);
}
```

Complete documentation of this library is not yet available, but it will come at a later date.

## Dependencies

Smol Blit2D has both development dependencies and runtime dependencies, as listed below.

### Runtime Dependencies

- [Smol Core2D](https://github.com/miss-programgamer/SmolCore2D)

### Development Dependencies:

- [Catch2](https://github.com/catchorg/Catch2) (for unit tests)