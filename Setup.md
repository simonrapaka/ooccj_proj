# Compiler Setup
If you already have a GNU compiler installed (like msys2 or MinGW) then skip to SDL Setup

Install the [TDM-GCC](https://jmeubank.github.io/tdm-gcc/download/) compiler to your C: directory

Open Settings and type **Edit Environment Variables for your account** in the settings search bar

In the **User Variables** dialog Click the **Path** row so it is highlighted and click **Edit**

Click the **New** option on the right and then click **Browse**

Go to the **TDM-GCC-64** folder in your C: drive and click **Open**

Click Ok for both the Edit dialog and the Environment Variable dialog

Open command prompt and type `gcc --version` and press Enter. If everything is configured correctly then a message starting with something like `gcc (tdm64-1) 10.3.0
Copyright (C) 2020 Free Software Foundation, Inc` should appear.

# SDL Setup

Download the **SDL Developer Package** from the [SDL Releases](https://github.com/libsdl-org/SDL/releases/latest) page. It's name should start with `SDL-devel`

Unzip it after downloading and open the `SDL2` folder

Navigate to the folder starting with `i686` if you are on a 32-bit system or go to the folder starting with `x86_64` if you're on a 64 bit system

Select all the folders and copy them to your compiler directory

All the folders starting with `bin`, `lib`, `include` etc in your compiler directory should be highlighted once you have pasted the folders

You can now start writing code and develop programs using the SDL library