# Celestial Dance

Broj indeksa: 255/2019  
Ime i prezime: Djordje Aksentijevic

Celestial dance is an interactive simulation of a simplified solar system, showcasing Earths rotation around its own
axis and orbiting the Sun alongside the Moon.

## Controls

{WASD / Mouse} -> {Moving the camera}  
{F2} -> {Toggle GUI}  
{KEY_1} -> {Toggle Earth rotation (around own axis)}  
{KEY_2} -> {Toggle Earth orbit (around Sun)}  
{KEY_3} -> {Toggle Sun rotation (around own axis)}  
{KEY_4} -> {Toggle Moon rotation (around own axis)}  
{KEY_5} -> {Toggle Moon orbit (around Earth)}  
{R} -> {Reset to the state before the event with triggers}

## Features

### Fundamental:

[x] Model with lighting  
[x] Two types of lighting with customizable colors and movement through GUI or ACTIONS  
[x] {Press T} --- AFTER_3_SECONDS---Triggers---> {Moon disappearing} ---> AFTER_6_SECONDS---Triggers---> {Light color of
the Sun changes}

### Group A:

[x] Frame-buffers with post-processing   
[ ] Instancing  
[ ] Off-screen Anti-Aliasing  
[ ] Parallax Mapping

### Group B:

[ ] Bloom with the use of HDR  
[ ] Deferred Shading  
[ ] Point Shadows  
[ ] SSAO

### Engine improvement:

Added PostProcess class which creates framebuffer and can only be made from GraphicsController.

## Models:

1. https://www.cgtrader.com/free-3d-models/space/planet/the-earth-727809ef-fa6b-460a-9f79-ca71facec2d2 - planet Earth
2. https://free3d.com/3d-model/moon-photorealistic-2k-853071.html - Moon
3. https://drive.google.com/drive/folders/1LGF4Zs_RHb4Ya7OZ-0UWpOr0nQgqgW-9 - Sun

## Textures

1. https://drive.google.com/drive/folders/1v5Tng9aLg26spNiojTBrbSjdJhWcHq_D - space texture

## Other resources

1. https://en.cppreference.com/w/cpp/standard_library.html - cppreference
