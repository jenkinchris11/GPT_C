# User Manual

This project is an early prototype. Current capabilities include:

- Loading one or more folders of images (JPEG/PNG/DNG)
- Interactive brightness and HSL editing via GUI sliders
- Simple rectangular mask adjustment
- Saving adjustments as `preset.txt`
- Culling unwanted images
- Processed images are written as `edited_#.jpg`
- AI metadata generation writes a `.meta` file next to each image
- Object removal uses a simple inpainting tool
- Generative overlay prints the prompt text onto the image
- Denoise filter reduces noise using OpenCV

## Using the GUI

When an image loads a window titled **Photo Editor** appears with sliders for brightness, hue, saturation and lightness. Keyboard shortcuts include:

- `n` or `Esc` – move to the next image
- `q` – quit the session
- `d` – denoise the current photo
- `r` – remove a region by entering coordinates in the console
- `m` – apply a brightness mask to a region
- `i` – write AI metadata to a `.meta` file
- `g` – generate text overlay from a prompt
- `s` – save slider positions as the preset
- `l` – load the preset
- `c` – mark the photo as culled

The code contains placeholders for future features:

- Advanced mask tools

Contributions welcome!
