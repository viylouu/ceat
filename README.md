# ceat
ceat is a game engine that was originally a direct translation of [eat-engine](https://github.com/viylouu/eat-engine/tree/main) to c.  
it was made to be used in other languages, and also has bindings for odin again so you can just use this instead since its newer and faster.  
heres a snipped from the eat-engine readme:  
```md
eat is a game engine that stands for "End All To (be all)".  
it is the (hopefully) final game engine i will have to make.
```

## building
to build ceat, you can use cmake.  
```bash
zig build shaders # only needed on shader update, first run, or when using vulkan
zig build
```
this will create either a `libceat.a` file or a `ceat.lib` file in the build folder  
to build examples, run `zig build examples`.  

## docs
if you dont know how to do stuff see the [docs](https://viylouu.github.io/ceat)
