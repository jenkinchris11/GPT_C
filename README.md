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
```
