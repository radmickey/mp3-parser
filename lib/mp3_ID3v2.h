#pragma once
#include "frame.h"

#include <filesystem>
#include <fstream>
#include <string>

class mp3_ID3v2 {
private:
    struct mp3_header {
        static const uint8_t kHeaderSizeInBytes = 10;
        static const uint8_t kFileIdentifier = 3;
        static const uint8_t kVersion = 2;
        static const uint8_t kFlags = 1;
        static const uint8_t kByteSize = 4;
        static const uint8_t kExtendedHeaderSizeInBytes = 4;

        size_t size_header = 0;
        size_t size_eheader = 0;
        char header[kHeaderSizeInBytes];
        char extended_header_size[kExtendedHeaderSizeInBytes];

        bool kUnsynchronisationFlag_;
        bool kExtendedHeaderFlag_;
        bool kExperimentalIndicatorFlag_;
        bool FooterPresentFlag_;

        bool correct_;

        void readFromFile(std::ifstream& file);
    };


    std::filesystem::path path_file_;
    mp3_header header_;
    std::ifstream mp3_;
    Data frames;
public:
    explicit mp3_ID3v2(const std::filesystem::path& path) {
        path_file_ = path;
        mp3_.open(path, std::ios::binary | std::ios::in);
        header_.readFromFile(mp3_);
        frames.parse(mp3_, header_.size_header);
    }

    explicit mp3_ID3v2(const std::string& path) {
        path_file_ = path;
        mp3_.open(path_file_, std::ios::binary | std::ios::in);
        header_.readFromFile(mp3_);
        frames.parse(mp3_, header_.size_header);

    }

    [[nodiscard]] std::vector<std::string> get_info();

};
