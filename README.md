# RedNoise - Graphics Rendering Toolkit

## Description

The RedNoise Graphics Rendering Toolkit is a C++-based project focused on the exploration and implementation of fundamental graphics rendering techniques. It leverages the SDL2 library for window management and rendering, alongside glm for vector and matrix operations, essential in graphics programming. This toolkit is designed as an educational tool for those interested in the basics of computer graphics, offering a hands-on approach to learning rendering algorithms through practical examples and interactive experimentation. The project encapsulates a variety of rendering techniques, from pixel manipulation and geometric primitives to more complex operations like texture mapping and OBJ model rendering. It aims to provide a foundation for understanding how digital images are created, manipulated, and displayed using programming, serving as a stepping stone towards more advanced graphics programming concepts.

## Features

### Red Noise Generation

The toolkit includes a function to generate and display red noise across the entire rendering window. This feature is intended to demonstrate direct pixel manipulation, showcasing how individual pixels can be set to specific colors to create effects or patterns. Red noise generation is one of the simplest forms of graphics rendering, making it an excellent starting point for beginners.

### Interpolation Algorithms

Interpolation is a crucial concept in graphics, used in various applications from animations to texture mapping. This project implements two types of interpolation:

- **Single-float Interpolation:** Demonstrates linear interpolation between two values over a series of steps. This is fundamental for generating gradients or varying intensity values across pixels.
- **Three-Element Vector Interpolation:** Extends the concept to three-dimensional vectors, allowing for color interpolation and more complex transformations necessary in 3D rendering.

### Color and Greyscale Interpolation

Building on the interpolation algorithms, this feature showcases practical applications by generating color and greyscale gradients. Users can see firsthand how interpolation is used to create smooth transitions between colors or intensity values, which is essential in image processing, background generation, and more.

### Basic Drawing Primitives

A key aspect of graphics programming is drawing basic shapes. This toolkit provides functions to draw:

- **Straight Lines:** Using an algorithm that calculates the steps required to draw a line from one point to another.
- **Stroked and Filled Triangles:** Demonstrates more complex line drawing algorithms and filling techniques, which are foundational for rendering shapes and models.

### OBJ Loading and Rendering

The toolkit can load and render 3D models from OBJ files, a common model format in 3D graphics. This feature introduces users to the basics of 3D model parsing, vertex manipulation, and rendering. It lays the groundwork for understanding more complex 3D graphics concepts, such as mesh rendering and texture application.

### Event Handling and Interactive Graphics

Interactivity is implemented using SDL2's event handling, allowing users to interact with the graphics window through keyboard and mouse inputs. This introduces the concept of real-time rendering and interactive applications, crucial in game development and interactive simulations.

