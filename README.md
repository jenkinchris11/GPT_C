# Photo Editor

This is a small C++ photo editing prototype built with OpenCV. It can load images from one or more folders, perform interactive edits and save the results. Advanced AI features are represented as placeholders for future work.

Current features:

- Load JPEG, PNG and DNG files from multiple catalogues
- Interactive brightness and HSL controls via GUI sliders
- Simple rectangular masking
- Ability to cull unwanted photos
- Preset save/load support (stored in `preset.txt`)
- AI metadata generation
- Object removal via inpainting
- Prompt-based generative overlay
- Basic denoise filter
For Windows users, a Visual Studio solution (`photo_editor.sln`) is provided.
Open this file in Visual Studio 2022 or newer and build the `photo_editor`
project to create the executable.

- Interactive brightness and HSL controls
- Simple rectangular masking
- Ability to cull unwanted photos
- Preset save/load support (stored in `preset.txt`)

This is a simple C++ photo editing tool demonstrating a catalogue loader and basic adjustments using OpenCV. Features such as AI tools and advanced editing are represented as placeholders for future work.

## Building

Ensure CMake and OpenCV are installed. Then run:

```bash
mkdir build && cd build
cmake ..
make
```

Run the program by pointing it at one or more folders of images. For each image a window will appear with sliders to tweak brightness and HSL values. Keyboard shortcuts allow applying masks and other tools. Edits are saved to files prefixed with `edited_`.

```bash
./photo_editor ../samples ../more_images
Run the program by pointing it at one or more folders of images. The program will open a small text menu for each image so you can adjust brightness, HSL values or apply a rectangular mask. Edits are saved to files prefixed with `edited_`.

```bash
./photo_editor ../samples ../more_images
Run the program by pointing it at a folder of images:

```bash
./photo_editor ../samples
```
