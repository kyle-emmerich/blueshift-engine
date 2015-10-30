cd Assets\Shaders
shaderc -f debug.vert -o debug_vs.win.bin --type vertex --varyingdef varying.def.sc --platform windows -p vs_4_0
shaderc -f debug.frag -o debug_fs.win.bin --type fragment --varyingdef varying.def.sc --platform windows -p ps_4_0

shaderc -f background.vert -o background.vert.win.bin --type vertex --varyingdef background_varying.def.sc --platform windows -p vs_4_0
shaderc -f background.frag -o background.frag.win.bin --type fragment --varyingdef background_varying.def.sc --platform windows -p ps_4_0

echo Built shaders.