#include "mp3_ID3v2.h"

void mp3_ID3v2::mp3_header::readFromFile(std::ifstream& file) {
    if (!file.is_open()) {
        throw std::invalid_argument("mp3 file didn't opened, probably it doesn't exist.");
    }
    file.read(header, kHeaderSizeInBytes);

    const uint8_t kFlagsByte = 5;
    const uint8_t kUnsynchronisationBit = 0b10000000;
    const uint8_t kExtendedHeaderBit = 0b01000000;
    const uint8_t kExperimentalIndicatorBit = 0b00100000;
    const uint8_t kFooterPresentBit = 0b00010000;
    const uint8_t kIncorrectBits = 0b00001111;


    kUnsynchronisationFlag_ = header[kFlagsByte] & kUnsynchronisationBit;
    kExtendedHeaderFlag_ = header[kFlagsByte] & kExtendedHeaderBit;
    kExperimentalIndicatorFlag_ = header[kFlagsByte] & kExperimentalIndicatorBit;
    FooterPresentFlag_ = header[kFlagsByte] & kFooterPresentBit;

    correct_ = (header[kFlagsByte] & kIncorrectBits) == 0;

    const uint8_t kSyncsafeByte = 7;

    const uint8_t kStartByteSize = 6;
    for (uint8_t i = kStartByteSize; i < kStartByteSize + kByteSize; i++) {
        size_header <<= kSyncsafeByte;
        size_header |= (header[i] );
    }


    if (kExtendedHeaderFlag_) {
        file.read(extended_header_size, kExtendedHeaderSizeInBytes);
        for (char i : extended_header_size) {
            size_eheader <<= kSyncsafeByte;
            size_eheader |= (i & 0b01111111);
        }
        const int64_t kByteStartingFrames = kHeaderSizeInBytes + kExtendedHeaderSizeInBytes + size_eheader + 1;
        file.seekg(kByteStartingFrames);

    }
}

std::vector<std::string> mp3_ID3v2::get_info() {
    return frames.get_info();
}
