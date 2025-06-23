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
void adjustBrightness(Image& img, int delta) {
    cv::Mat tmp;
    img.data.convertTo(tmp, -1, 1, delta);
    img.data = tmp;
    img.edited = true;
}

// Hue saturation lightness simple implementation
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

    if (cat.images.empty()) {
        return 0;
    }

    Preset preset;
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
    return 0;
}
