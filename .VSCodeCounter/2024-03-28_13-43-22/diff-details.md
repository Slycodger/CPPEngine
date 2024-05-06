# Diff Details

Date : 2024-03-28 13:43:22

Directory c:\\Users\\gabri\\OneDrive\\Desktop\\MySpecialEngine\\MySpecialEngine\\Shaders

Total : 48 files,  -2867 codes, -197 comments, -329 blanks, all -3393 lines

[Summary](results.md) / [Details](details.md) / [Diff Summary](diff.md) / Diff Details

## Files
| filename | language | code | comment | blank | total |
| :--- | :--- | ---: | ---: | ---: | ---: |
| [Scripts/AObjectDraw.cpp](/Scripts/AObjectDraw.cpp) | C++ | -516 | -102 | -68 | -686 |
| [Scripts/AObjectDraw.h](/Scripts/AObjectDraw.h) | C++ | -65 | -7 | -12 | -84 |
| [Scripts/ApplicationMain.cpp](/Scripts/ApplicationMain.cpp) | C++ | -265 | -11 | -33 | -309 |
| [Scripts/ApplicationMain.h](/Scripts/ApplicationMain.h) | C++ | -20 | 0 | -5 | -25 |
| [Scripts/Button.h](/Scripts/Button.h) | C++ | -23 | 0 | -5 | -28 |
| [Scripts/Camera.h](/Scripts/Camera.h) | C++ | -83 | 0 | -3 | -86 |
| [Scripts/Constants.h](/Scripts/Constants.h) | C++ | -15 | -3 | -3 | -21 |
| [Scripts/DrawingPad.cpp](/Scripts/DrawingPad.cpp) | C++ | -78 | -16 | -9 | -103 |
| [Scripts/DrawingPad.h](/Scripts/DrawingPad.h) | C++ | -25 | -3 | -6 | -34 |
| [Scripts/EngineBase.h](/Scripts/EngineBase.h) | C++ | -16 | 0 | -1 | -17 |
| [Scripts/GUIText.cpp](/Scripts/GUIText.cpp) | C++ | -243 | -10 | -50 | -303 |
| [Scripts/GUIText.h](/Scripts/GUIText.h) | C++ | -56 | 0 | -12 | -68 |
| [Scripts/GlStuff.h](/Scripts/GlStuff.h) | C++ | -11 | -3 | -2 | -16 |
| [Scripts/GlobalVars.h](/Scripts/GlobalVars.h) | C++ | -32 | -3 | -1 | -36 |
| [Scripts/IntializeShaders.h](/Scripts/IntializeShaders.h) | C++ | -219 | -3 | -22 | -244 |
| [Scripts/KeyPresses.cpp](/Scripts/KeyPresses.cpp) | C++ | -58 | -3 | -7 | -68 |
| [Scripts/KeyPresses.h](/Scripts/KeyPresses.h) | C++ | -17 | -3 | -4 | -24 |
| [Scripts/LightingMain.cpp](/Scripts/LightingMain.cpp) | C++ | -379 | -24 | -92 | -495 |
| [Scripts/LightingMain.h](/Scripts/LightingMain.h) | C++ | -27 | 0 | -9 | -36 |
| [Scripts/Line.h](/Scripts/Line.h) | C++ | -105 | 0 | -6 | -111 |
| [Scripts/List.h](/Scripts/List.h) | C++ | -167 | 0 | -4 | -171 |
| [Scripts/MathFunctions.cpp](/Scripts/MathFunctions.cpp) | C++ | -44 | 0 | -9 | -53 |
| [Scripts/MathFunctions.h](/Scripts/MathFunctions.h) | C++ | -15 | 0 | -2 | -17 |
| [Scripts/Mouse.h](/Scripts/Mouse.h) | C++ | -16 | 0 | -4 | -20 |
| [Scripts/ObjectClasses.h](/Scripts/ObjectClasses.h) | C++ | -282 | 0 | -27 | -309 |
| [Scripts/Raycasting.h](/Scripts/Raycasting.h) | C++ | -53 | 0 | -19 | -72 |
| [Scripts/VAOCreate.h](/Scripts/VAOCreate.h) | C++ | -310 | -6 | -11 | -327 |
| [Scripts/Vector.h](/Scripts/Vector.h) | C++ | -186 | 0 | -12 | -198 |
| [Shaders/BasicFrag.frag](/Shaders/BasicFrag.frag) | GLSL | 12 | 0 | 5 | 17 |
| [Shaders/BasicVert.vert](/Shaders/BasicVert.vert) | GLSL | 16 | 0 | 4 | 20 |
| [Shaders/FractalFrag.frag](/Shaders/FractalFrag.frag) | GLSL | 29 | 0 | 11 | 40 |
| [Shaders/FractalVert.vert](/Shaders/FractalVert.vert) | GLSL | 6 | 0 | 1 | 7 |
| [Shaders/GenerateShadowEdges.comp](/Shaders/GenerateShadowEdges.comp) | GLSL | 118 | 0 | 17 | 135 |
| [Shaders/NoTextureFrag.frag](/Shaders/NoTextureFrag.frag) | GLSL | 8 | 0 | 3 | 11 |
| [Shaders/NoTextureVert.vert](/Shaders/NoTextureVert.vert) | GLSL | 13 | 0 | 4 | 17 |
| [Shaders/SMappingBasicFrag.frag](/Shaders/SMappingBasicFrag.frag) | GLSL | 47 | 0 | 11 | 58 |
| [Shaders/SMappingBasicVert.vert](/Shaders/SMappingBasicVert.vert) | GLSL | 21 | 0 | 5 | 26 |
| [Shaders/SMappingDepthTextureFrag.frag](/Shaders/SMappingDepthTextureFrag.frag) | GLSL | 4 | 0 | 2 | 6 |
| [Shaders/SMappingDepthTextureVert.vert](/Shaders/SMappingDepthTextureVert.vert) | GLSL | 9 | 0 | 2 | 11 |
| [Shaders/SMappingNoTextureFrag.frag](/Shaders/SMappingNoTextureFrag.frag) | GLSL | 43 | 0 | 11 | 54 |
| [Shaders/SMappingNoTextureVert.vert](/Shaders/SMappingNoTextureVert.vert) | GLSL | 18 | 0 | 5 | 23 |
| [Shaders/VShadowingBasicFrag.frag](/Shaders/VShadowingBasicFrag.frag) | GLSL | 12 | 0 | 5 | 17 |
| [Shaders/VShadowingBasicVert.vert](/Shaders/VShadowingBasicVert.vert) | GLSL | 16 | 0 | 4 | 20 |
| [Shaders/VShadowingNoTextureFrag.frag](/Shaders/VShadowingNoTextureFrag.frag) | GLSL | 8 | 0 | 3 | 11 |
| [Shaders/VShadowingNoTextureGeom.geom](/Shaders/VShadowingNoTextureGeom.geom) | GLSL | 54 | 0 | 8 | 62 |
| [Shaders/VShadowingNoTextureVert.vert](/Shaders/VShadowingNoTextureVert.vert) | GLSL | 10 | 0 | 3 | 13 |
| [Shaders/VShadowsFrag.frag](/Shaders/VShadowsFrag.frag) | GLSL | 6 | 0 | 2 | 8 |
| [Shaders/VShadowsVert.vert](/Shaders/VShadowsVert.vert) | GLSL | 9 | 0 | 3 | 12 |

[Summary](results.md) / [Details](details.md) / [Diff Summary](diff.md) / Diff Details