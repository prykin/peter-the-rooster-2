PisteEngine Installation (for dummies? :)

Sorry if this guide seems a bit like it's made for kids, but to avoid errors,
I decided to make this pretty straightforward. All files must be listed correctly
in FileView, and all paths must be set correctly. I went through this guide once myself
and it worked for me at least. Of course, something might have been forgotten...
If something doesn't work, the most likely reason is that some path in the code
points to the wrong directory. A file might also be missing if I messed up
while packing. And while writing this, I should have been studying for exams...

So, do this:

1. Take a deep breath (not a mandatory step :)
2. Start Visual C++.
3. Select File - New...
4. Select Projects - Win32 Application.
5. Give it a name (something nice).
6. Select "An empty project". Click the Finish button.
7. Extract all PisteEngine package files to your project folder (e.g., Microsoft Visual Studio\MyProjects\gameprogram). Note! A folder named debug should also appear, containing two images.
8. Select the FileView tab in the left-hand window of VC++.
9. Expand all the folders in the tab.
10. Right-click the Source Files folder and select "Add files to folder".
11. Select all .cpp files from your project folder:
- Game.cpp
- PisteDraw.cpp
- PisteFont.cpp
- PisteInput.cpp
- PisteSound.cpp
- PisteWait.cpp
12. Do the same for the Header Files folder, i.e., add the following files from your project folder:
- PisteDraw.h
- PisteFont.h
- PisteInput.h
- PisteSound.h
- PisteWait.h
13. Select File - New - Resource Script. Name it "Resource", for example.
14. Right-click the Resource.rc icon and select import.
15. Select GameIcon.ico. Add cursor1.cur in the same way. Remember to change the file extension to .cur when loading the cursor so that the file can be found... You can, of course, draw your own cursor and icon as well.
16. Hunt down the DirectX header files. Mine are located in a directory like: mssdk\directx7\include So, find the files ddraw.h, dinput.h, and dsound.h and add them to the Header Files folder. Add the resource.h file from your project folder as well.
17. We need .lib files:
- ddraw.lib
- dinput.lib
- dsound.lib
18. These files can be found in my directory: mssdk\directx7\lib You will also need the Winmm.lib file (Windows multimedia or something), which I find in the directory: Microsoft Visual Studio\Vc98\Lib\Winmm.lib
19. The file list should look like this:

Source Files:
- Game.cpp
- PisteDraw.cpp
- PisteFont.cpp
- PisteInput.cpp
- PisteSound.cpp
- PisteWait.cpp
- Resource.rc

Header Files:
- PisteDraw.h
- PisteFont.h
- PisteInput.h
- PisteSound.h

---

*Update from 2024*

You will need Visual Studio 6, DirectX 7 SDK and Midas SDK

After installation, create a new folder named 'libs' and place the SDKs there accordingly. You can find the correct names in the CMakeLists.txt file