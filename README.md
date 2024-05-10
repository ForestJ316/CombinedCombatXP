## Requirements
* [CMake](https://cmake.org/)
	* Add this to your `PATH`
* [Vcpkg](https://github.com/microsoft/vcpkg)
	* Add the environment variable `VCPKG_ROOT` with the value as the path to the folder containing vcpkg
* [Visual Studio Community 2022](https://visualstudio.microsoft.com/)
	* Desktop development with C++
* [CommonLibF4](https://github.com/powerof3/CommonLibF4)
	* Add the environment variable `CommonLibF4Path` with the value as the path to the folder containing CommonLibF4

## Building
```
git clone https://github.com/ForestJ316/CombinedCombatXP
cd CombinedCombatXP

cmake --preset vs2022-windows-vcpkg
cmake --build build --config Release
```

## Optional
* [Fallout 4](https://store.steampowered.com/app/377160)
	* Add the environment variable `Fallout4Path` to point to the folder where you want the .dll to be copied after it's finished building
