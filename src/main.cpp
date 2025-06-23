#include <opencv2/opencv.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

namespace fs = std::filesystem;

struct Image {
    cv::Mat data;
    std::string path;
    bool edited = false;
    bool culled = false;

class Image {
public:
    cv::Mat data;
    std::string path;
    Image(const std::string& p) : path(p) {
        data = cv::imread(p, cv::IMREAD_UNCHANGED);
        if (data.empty()) {
            std::cerr << "Failed to load: " << p << std::endl;
        }
    }
};

struct Preset {
    int brightness = 0;
    int hue = 0;
    int saturation = 0;
    int light = 0;
};

class Catalogue {
public:
    std::vector<Image> images;

    void loadFolder(const std::string& folder) {

class Catalogue {
public:
    std::vector<Image> images;
    void load(const std::string& folder) {
        images.clear();
        for (const auto& entry : fs::directory_iterator(folder)) {
            if (entry.is_regular_file()) {
                images.emplace_back(entry.path().string());
            }
        }
        std::cout << "Loaded " << images.size() << " files from " << folder << std::endl;
    }

    void load(const std::vector<std::string>& folders) {
        images.clear();
        for (const auto& f : folders) {
            if (fs::exists(f) && fs::is_directory(f)) {
                loadFolder(f);
            }
        }
    }
};

// Basic brightness adjustment
void adjustBrightness(cv::Mat& mat, int delta) {
    cv::Mat tmp;
    mat.convertTo(tmp, -1, 1, delta);
    mat = tmp;
}

void adjustBrightness(Image& img, int delta) {
    adjustBrightness(img.data, delta);
void adjustBrightness(Image& img, int delta) {
    cv::Mat tmp;
    img.data.convertTo(tmp, -1, 1, delta);
    img.data = tmp;
    img.edited = true;
}

// Hue saturation lightness simple implementation
void adjustHSL(cv::Mat& mat, int hue, int sat, int light) {
    cv::Mat hsv;
    cv::cvtColor(mat, hsv, cv::COLOR_BGR2HSV);
void adjustHSL(Image& img, int hue, int sat, int light) {
    cv::Mat hsv;
    cv::cvtColor(img.data, hsv, cv::COLOR_BGR2HSV);
    for (int y = 0; y < hsv.rows; ++y) {
        for (int x = 0; x < hsv.cols; ++x) {
            cv::Vec3b& pixel = hsv.at<cv::Vec3b>(y, x);
            pixel[0] = cv::saturate_cast<uchar>(pixel[0] + hue);
            pixel[1] = cv::saturate_cast<uchar>(pixel[1] + sat);
            pixel[2] = cv::saturate_cast<uchar>(pixel[2] + light);
        }
    }
    cv::cvtColor(hsv, mat, cv::COLOR_HSV2BGR);
}

void adjustHSL(Image& img, int hue, int sat, int light) {
    adjustHSL(img.data, hue, sat, light);
    cv::cvtColor(hsv, img.data, cv::COLOR_HSV2BGR);
    img.edited = true;
}

// Simple rectangular mask to brighten region
void maskAdjust(Image& img, const cv::Rect& area, int delta) {
    cv::Mat roi = img.data(area);
    cv::Mat tmp;
    roi.convertTo(tmp, -1, 1, delta);
    tmp.copyTo(roi);
    img.edited = true;
}

// AI metadata insertion using simple color analysis
void aiMetadata(Image& img) {
    cv::Scalar mean = cv::mean(img.data);
    std::string outFile = img.path + ".meta";
    std::ofstream out(outFile);
    if (out.is_open()) {
        out << "mean_b=" << mean[0] << "\n";
        out << "mean_g=" << mean[1] << "\n";
        out << "mean_r=" << mean[2] << "\n";
    }
}

// Basic remove tool using inpainting over a user selected rectangle
void aiRemove(Image& img) {
    int x, y, w, h;
    std::cout << "remove x y w h: ";
    std::cin >> x >> y >> w >> h;
    cv::Mat mask = cv::Mat::zeros(img.data.size(), CV_8U);
    cv::rectangle(mask, cv::Rect(x, y, w, h), cv::Scalar(255), cv::FILLED);
    cv::inpaint(img.data, mask, img.data, 3, cv::INPAINT_TELEA);
    img.edited = true;
}

// Generative AI placeholder that overlays the prompt text
void generativeAI(Image& img, const std::string& prompt) {
    cv::putText(img.data, prompt, {10, img.data.rows / 2},
                cv::FONT_HERSHEY_SIMPLEX, 1.0, {255, 255, 255}, 2);
    img.edited = true;
}

// Professional denoise using OpenCV fastNlMeansDenoising
void denoise(Image& img) {
    cv::Mat tmp;
    cv::fastNlMeansDenoisingColored(img.data, tmp, 10, 10, 7, 21);
    img.data = tmp;
    img.edited = true;
// Placeholder for mask adjustments
void maskAdjust(Image& img) {
    // TODO: Implement mask adjustments
}

// Placeholder for AI metadata insertion
void aiMetadata(Image& img) {
    // TODO: Use AI to generate metadata
}

// Placeholder for AI remove tools
void aiRemove(Image& img) {
    // TODO: AI object removal
}

// Placeholder for generative AI
void generativeAI(Image& img, const std::string& prompt) {
    // TODO: Generate new content based on prompt
}

// Placeholder for professional denoise
void denoise(Image& img) {
    // TODO: Advanced denoising
}

bool savePreset(const Preset& p, const std::string& file) {
    std::ofstream out(file);
    if (!out.is_open()) return false;
    out << p.brightness << " " << p.hue << " " << p.saturation << " " << p.light;
    return true;
}

bool loadPreset(Preset& p, const std::string& file) {
    std::ifstream in(file);
    if (!in.is_open()) return false;
    in >> p.brightness >> p.hue >> p.saturation >> p.light;
    return true;
}

struct GUIState {
    cv::Mat original;
    cv::Mat display;
    int bright = 100;
    int hue = 180;
    int sat = 100;
    int light = 100;
};

static void updateDisplay(GUIState* st) {
    st->original.copyTo(st->display);
    adjustBrightness(st->display, st->bright - 100);
    adjustHSL(st->display, st->hue - 180, st->sat - 100, st->light - 100);
    cv::imshow("Photo Editor", st->display);
}

static void onTrack(int, void* userdata) {
    GUIState* st = reinterpret_cast<GUIState*>(userdata);
    updateDisplay(st);
}

// Edit image via simple GUI. Returns false if user quit.
bool editImageGUI(Image& img, Preset& preset) {
    GUIState st;
    st.original = img.data.clone();
    st.display = img.data.clone();
    st.bright = preset.brightness + 100;
    st.hue = preset.hue + 180;
    st.sat = preset.saturation + 100;
    st.light = preset.light + 100;

    cv::namedWindow("Photo Editor", cv::WINDOW_NORMAL);
    cv::createTrackbar("Brightness", "Photo Editor", &st.bright, 200, onTrack, &st);
    cv::createTrackbar("Hue", "Photo Editor", &st.hue, 360, onTrack, &st);
    cv::createTrackbar("Saturation", "Photo Editor", &st.sat, 200, onTrack, &st);
    cv::createTrackbar("Light", "Photo Editor", &st.light, 200, onTrack, &st);

    updateDisplay(&st);

    bool quitAll = false;
    while (true) {
        int k = cv::waitKey(20);
        if (k == 'n' || k == 27) { // next or ESC
            break;
        } else if (k == 'q') {
            quitAll = true;
            break;
        } else if (k == 'd') {
            denoise(img);
            st.original = img.data.clone();
            updateDisplay(&st);
        } else if (k == 'i') {
            aiMetadata(img);
        } else if (k == 'g') {
            std::string prompt; std::cout << "prompt: "; std::getline(std::cin, prompt); generativeAI(img, prompt);
            st.original = img.data.clone();
            updateDisplay(&st);
        } else if (k == 'r') {
            aiRemove(img);
            st.original = img.data.clone();
            updateDisplay(&st);
        } else if (k == 'm') {
            int x,y,w,h,d; std::cout << "x y w h delta: "; std::cin >> x >> y >> w >> h >> d; maskAdjust(img,{x,y,w,h},d); std::cin.ignore();
            st.original = img.data.clone();
            updateDisplay(&st);
        } else if (k == 's') {
            preset.brightness = st.bright - 100;
            preset.hue = st.hue - 180;
            preset.saturation = st.sat - 100;
            preset.light = st.light - 100;
            savePreset(preset, "preset.txt");
        } else if (k == 'l') {
            if (loadPreset(preset, "preset.txt")) {
                st.bright = preset.brightness + 100;
                st.hue = preset.hue + 180;
                st.sat = preset.saturation + 100;
                st.light = preset.light + 100;
                updateDisplay(&st);
            }
        } else if (k == 'c') {
            img.culled = true; break;
        }
    }
    cv::destroyWindow("Photo Editor");

    if (!img.culled) {
        preset.brightness = st.bright - 100;
        preset.hue = st.hue - 180;
        preset.saturation = st.sat - 100;
        preset.light = st.light - 100;
        st.display.copyTo(img.data);
        img.edited = true;
    }
    return !quitAll;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <catalogue folders...>" << std::endl;
        return 0;
    }

    std::vector<std::string> folders;
    for (int i = 1; i < argc; ++i)
        folders.push_back(argv[i]);

    Catalogue cat;
    cat.load(folders);
int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <catalogue folder>" << std::endl;
        return 0;
    }

    Catalogue cat;
    cat.load(argv[1]);

    if (cat.images.empty()) {
        return 0;
    }

    Preset preset;
    loadPreset(preset, "preset.txt"); // ignore result

    bool cont = true;
    for (size_t idx = 0; idx < cat.images.size() && cont; ++idx) {
        Image& img = cat.images[idx];
        std::cout << "Editing " << img.path << std::endl;
        cont = editImageGUI(img, preset);
    std::string presetFile = "preset.txt";
    loadPreset(preset, presetFile); // ignore result

    for (size_t idx = 0; idx < cat.images.size(); ++idx) {
        Image& img = cat.images[idx];
        std::cout << "Editing " << img.path << std::endl;

        char cmd;
        while (true) {
            std::cout << "[b]rightness [h]sl [m]ask [c]ull [s]ave preset [l]oad preset [n]ext [q]uit: ";
            std::cin >> cmd;
            if (cmd == 'b') {
                int d; std::cout << "delta: "; std::cin >> d; adjustBrightness(img, d);
            } else if (cmd == 'h') {
                int hu, sa, li; std::cout << "h s l: "; std::cin >> hu >> sa >> li; adjustHSL(img, hu, sa, li);
            } else if (cmd == 'm') {
                int x,y,w,h,d; std::cout << "x y w h delta: "; std::cin >> x >> y >> w >> h >> d; maskAdjust(img, {x,y,w,h}, d);
            } else if (cmd == 'c') {
                img.culled = true; break;
            } else if (cmd == 's') {
                preset.brightness = preset.hue = preset.saturation = preset.light = 0; // not tracking history
                savePreset(preset, presetFile);
            } else if (cmd == 'l') {
                loadPreset(preset, presetFile);
                adjustBrightness(img, preset.brightness);
                adjustHSL(img, preset.hue, preset.saturation, preset.light);
            } else if (cmd == 'n') {
                break;
            } else if (cmd == 'q') {
                idx = cat.images.size();
                break;
            }
        }

        if (cmd == 'q') break;
    }

    int count = 0;
    for (const auto& img : cat.images) {
        if (!img.culled && img.edited) {
            cv::imwrite("edited_" + std::to_string(count++) + ".jpg", img.data);
        }
    }

    std::cout << "Processed " << count << " images" << std::endl;
    Image& img = cat.images[0];
    adjustBrightness(img, 50);
    adjustHSL(img, 10, 10, 10);
    aiMetadata(img); // placeholder
    denoise(img);    // placeholder
    cv::imwrite("output.jpg", img.data);

    std::cout << "Saved output.jpg" << std::endl;
    return 0;
}
