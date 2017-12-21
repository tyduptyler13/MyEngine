MyEngine
========

Welcome to MyEngine, this is a completely open source (MIT License) game engine written in C++14 with Electron and Node.js attached
for easy modding and UI design.

I created this engine because I found a large lackluster of existing game engines and wasn't particularly impressed by the large cost on
getting a mainstream engine. This engine is designed after some of the best (also open source) tools in the industry. This engine is
special because it is designed to be extremely modular and extremely cross platform. The target operating systems should include unix
based systems, windows, and browser technologies like emscripten, ppapi, and eventually webassembly. Mobile support will come through
web support but may eventually become native apps (currently this is limited by electron and nodejs being ported to mobile or the engine
gaining more abstraction to support those platforms and simulate electron and nodejs support).

__Graphics__: Completely portable graphics using SDL2. Different render classes can be written for every driver if someone wants that driver. The windows will be powered by Electron (desktop) and browsers (mobile). Electron/browsers will also power the UI allowing highly customizable UIs, which will be overlayed on the game.

__Physics__: [Bullet](http://bulletphysics.org/wordpress/)

__Networking__: [Nodejs](https://nodejs.org/en/)/[Cap'nProto](https://capnproto.org/) This mixture of networking allows networking to be cross platform, fast, and easy. No matter where the networking is happening.

__Sound__: In Planning stage ([YSE](http://www.attr-x.net/yse/) is a major contender for our internal use)

__Mod API/Scripting__: [Nodejs](https://nodejs.org/en/). All of our mods will be javascript driven through a shielded api that enforces permissions and reduces security risks. Scripts only get permissions that they are granted by the user running the mods and even then must go through the api defined in the engine to gain access to any external resources.


News
====

End of 2017
-----------
Hi, I want to keep people posted, seeing as I have a couple stars and watchers. I haven't abandoned this project, I have just been busy with work and life. Since my last commit I have learned several useful coding techniques that I will probably integrate into the engine. Additionally, I have been experimenting with unreal engine and would like to take features I like and try to implement them here as well.

My plans for the engine will be to use a better organization strategy for code and attempt to implement/use a dependency injection library to see if c++ is friendly to it. After that I will finish polishing the new renderer and get everything compiling again, then push hard to get some of the skeleton parts for writing an actual game. *Maybe a course on using the engine would be a good way to test the engine and teach people how to use it most effectively.*

Anyways, I will be back to coding soon.

Older
-----

We have finally released version Alpha-1 which includes the first working version of the GLES2Renderer and the Shader compiler.

I am really proud of the shader compiler as it is node based, aka you can drag and drop nodes to create shaders without even having
to know any glsl. This will eventually have its own GUI similar to blender.

TODO List
=========

This list comprises everything still needing to be done before this engine can be used for any sort of release of any game.

* Abstracted keybinding for game input with default values. (wasd for movement, etc)
* Sound - Was originally targetting sdl but not so much anymore.
* Physics - Need to actually hook in the physics library. (Special Object3D type class that allows you to apply physics to objects)
* Electron Overlay - Need to setup the steps to include it in the render process.
* Electron Launcher/Menu - Games will implement these on their own

Not as required but still TODO

* Electron patcher - Not required, but eventually this will be part of the engine, will patch all files over p2p and html.
* GLES3Renderer - Future work (Can optimize somethings and specialize a few others)
* VulkanRenderer - Max performance out of rendering.

Coding Style
============

All code uses a very simple coding style called Modified K&R. You can find this style referenced in eclipse or you can just follow the linux kernels coding standards.

* We use tabs, not spaces, because its what tab was made for. (If you don't like tabs, you can set your editor to display tabs as two spaces but we would really appreciate that all commits are tab indented. Git provides tools to have the code automatically converted to your preferred indentation format and back if you really want spaces.)
* Header file include guard names should be of the format MYUPLAY_MYENGINE_NAME. We don't need to know that it is a HPP or H file, we don't care where the file is located, and we don't need the trailing underscore to waste time/space. The convention follows similar to java, we list who provides the code (MyUPlay), what project it is (MyEngine), and the name of the primary class in the file. If for some reason your naming is not unique, consider why you have an overlapping name and if it could be better named or combined.

Some extensions to the style that are vague but we would appreciate if you at least try to maintain these concepts throughout the code.

* Keep it simple stupid, we don't want the code to be optimized because its rare that you know better than the compiler. Usually the compiler will take the lame version of a method and spruce it up to whatever the processor can do in a manor that is likely far better than c code can express. Another reason to avoid optimization is that it can very quickly become a rats nest for bugs, I never want to have to untangle some obscure code because someone was "making it run faster". I will just rewrite the function and get mad at whoever did it.
* Passing by reference is your friend. This somewhat contradicts the earlier statement but mastery of this helps weed out bugs because it forces the use of const or non const parameters so we know exactly what the parameter is intended for. It also helps reduce copying but that is something the compiler ~~will~~ *should* optimize out anyways.
* Documentation is optional unless the function is particularly complex. Leaving comments on things that you don't want removed for some reason is a good idea, leaving comments on strange math is a good idea, however, as most of the [original project](https://github.com/mrdoob/three.js) is undocumented but also straighforward enough not to require it, we won't require it either (yet).
* Please provide examples for any code you create. Ideally your examples are also the test cases. This will be enforced in the future and all classes must have test cases. We will have a testing suite format that works nicely with examples. (Functionality ported from Three.js should still be tested/examples should be provided)

Any questions or things outside of this document should be directed to us on the issue tracker or our [forum](https://forum.myuplay.com/) (broken at the time or writing but will probably be fixed eventually.)

