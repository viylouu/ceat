# ear (end all rendering)
ear has 2 different types of rendering things.
- high level
- low level

## high level
most things drawn in the high level library will be drawn relative to the top left of the screen.  
the window draw space is in the range of 0 to width or height.

#### flush
```c
void
ear_flush(
    void
    );
```
flush just commits all the rect/tex calls and draws the batch to the framebuffer.  
this is usually not needed unless youre using both the high and low level rendering api.  

#### rect
```c
void
ear_rect(
    float x, float y,
    float w, float h,
    float col[4],
    eau_align align
    );
```
rect draws a rectangle to the framebuffer (wow who couldve... uhh... see below).  
![img](orange_geusse.png "this is a picture of a foot")

#### tex
```c
void
ear_tex(
    ear_texture* tex,
    float x, float y,
    float w, float h,
    float sx, float sy, float sw, float sh,
    float col[4],
    eau_align align
    );
```
tex draws a texture to the framebuffer (wow who co- i already did this joke.).).).).).).).).  
the sx, sy, sw, and sh variables denote the sample position in the texture.  
these are in the range of 0 to the texture width and height.  

## low level

go away cone again another day
