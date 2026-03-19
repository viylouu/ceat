# ear (end all rendering)
ear has 2 different types of rendering things.
- high level
- low level

## high level

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
![img](orange\ geusse.png "this is a picture of a foot")

## low level

go away cone again another day
