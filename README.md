# Photo Editor

This is a small C++ photo editing prototype built with OpenCV. It can load images from one or more folders, perform interactive edits and save the results. Advanced AI features are represented as placeholders for future work.

Current features:

- Load JPEG, PNG and DNG files from multiple catalogues
- Interactive brightness and HSL controls
- Simple rectangular masking
- Ability to cull unwanted photos
- Preset save/load support (stored in `preset.txt`)

## Building

Ensure CMake and OpenCV are installed. Then run:

```bash
mkdir build && cd build
cmake ..
make
```

Run the program by pointing it at one or more folders of images. The program will open a small text menu for each image so you can adjust brightness, HSL values or apply a rectangular mask. Edits are saved to files prefixed with `edited_`.

```bash
./photo_editor ../samples ../more_images
```
