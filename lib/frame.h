#pragma once

#include <vector>
#include <cstdint>
#include <fstream>
#include <string>
#include <bitset>
#include <algorithm>


std::string iso_8859_1_to_utf8(char* str, size_t size);

enum typeFrame {
    Text,
    URL,
    SIGN,
    SYTC,
    USLT,
    POPM,
    GRID,
    etc
};


class Frame {
private:
    static const uint16_t kFlagABit = 0b0100000000000000;
    static const uint16_t kFlagBBit = 0b0010000000000000;
    static const uint16_t kFlagCBit = 0b0001000000000000;

    static const uint16_t kFlagHBit = 0b0000000001000000;
    static const uint16_t kFlagKBit = 0b0000000000001000;
    static const uint16_t kFlagMBit = 0b0000000000000100;
    static const uint16_t kFlagNBit = 0b0000000000000010;
    static const uint16_t kFlagPBit = 0b0000000000000001;


    static const char kTextId = 'T';
    static const char kURLId = 'W';
    static const char kU000Id = 'U';
    static const char kS000Id = 'S';
    static const char kC000Id = 'C';
    static const char kP000Id = 'P';
    static const char kG000Id = 'G';
    const char kXID = 'X';

protected:
    static const uint8_t kSyncsafeByte = 7;
    static const uint8_t kIdStartIndex = 0;
    static const uint8_t kSizeFrameInBytes = 4;
    static const uint8_t kSizeIdInBytes = 4;
    static const uint8_t kBytesForFlags = 2;
    static const uint8_t kSizeHeaderFrame = kSizeIdInBytes + kSizeFrameInBytes + kBytesForFlags;

    typeFrame type_;
    char id_[kSizeIdInBytes];
    char char_size_[kSizeFrameInBytes];
    char flags_[kBytesForFlags];

    bool flag_a_;
    bool flag_b_;
    bool flag_c_;
    bool flag_h_;
    bool flag_k_;
    bool flag_m_;
    bool flag_n_;
    bool flag_p_;

    uint32_t size_;

public:

    explicit Frame(std::ifstream& file);

    Frame(const char* id, const char* char_size, const char* flags, uint32_t size, typeFrame type);

    void set_flags();

    [[nodiscard]] virtual std::string get_info() const;

    [[nodiscard]] typeFrame get_type() const;


    [[nodiscard]] char* get_id();

    [[nodiscard]] char* get_char_size();

    [[nodiscard]] char* get_flags();

    [[nodiscard]] size_t get_size() const;

};


class TextFrame : public Frame {
private:
    enum TextFrameType {
        TALB,
        TBPM,
        TCOM,
        TCON,
        TCOP,
        TDEN,
        TDLY,
        TDOR,
        TDRC,
        TDRL,
        TDTG,
        TENC,
        TEXT,
        TIPL,
        TIT1,
        TIT2,
        TIT3,
        TKEY,
        TLAN,
        TLEN,
        TMCL,
        TMED,
        TMOO,
        TOAL,
        TOFN,
        TOLY,
        TOPE,
        TOWN,
        TPE1,
        TPE2,
        TPE3,
        TPE4,
        TPOS,
        TPRO,
        TPUB,
        TRCK,
        TRSN,
        TRSO,
        TSOA,
        TSOT,
        TSRC,
        TSSE,
        TSST,
        TCMP,
        TSOP,
        TXXX,
        WXXX,
        UFID,
        UNKNOWN
    };

    enum TextEncoding {
        ISO88591,
        UTF16,
        UTF8
    };

    TextFrameType text_frame_type_;
    TextEncoding typeEncoding;
    wchar_t* text_16;
    char* text_8;
    size_t text_size_;

public:
    TextFrame(std::ifstream& file, char* id, char* char_size, char* flags, size_t size, typeFrame type);

    ~TextFrame();

    [[nodiscard]] TextFrameType get_type() const;

    [[nodiscard]] std::string get_info() const override;

};

class URLFrame : public Frame {
private:
    enum URLFrameType {
        WCOM,
        WCOP,
        WOAF,
        WOAR,
        WOAS,
        WORS,
        WPAY,
        WPUB,
        UFID,
        UNKNOWN
    };

    URLFrameType url_frame_type_;
    char* text;

public:
    URLFrame(std::ifstream& file, char* id, char* char_size, char* flags, size_t size, typeFrame type);

    ~URLFrame();

    [[nodiscard]] std::string get_info() const override;

    [[nodiscard]] URLFrameType get_type() const;

};

class SIGNFrame : public Frame {
private:
    const uint8_t kBytesForSign = 1;
    char sign_;
    char* binary_data_;
public:
    SIGNFrame(std::ifstream& file, char* id, char* char_size, char* flags, size_t size, typeFrame type);

    ~SIGNFrame();

    [[nodiscard]] std::string get_info() const override;

};


class USLTFrame : public Frame {
private:
    static const uint8_t kBytesForLanguages = 4;
    static const uint8_t kByteForEncoding = 1;

    char encoding_;
    char languages_[kBytesForLanguages];
    char* text_;
public:
    USLTFrame(std::ifstream& file, char* id, char* char_size, char* flags, size_t size, typeFrame type);

    ~USLTFrame();

    [[nodiscard]] std::string get_info() const override;
};

class POPMFrame : public Frame {
private:
    char* email_;
    char* counter_char_;
    char score_;
    int64_t email_size_;
    uint64_t counter_;
public:
    POPMFrame(std::ifstream& file, char* id, char* char_size, char* flags, size_t size, typeFrame type);

    ~POPMFrame();

    [[nodiscard]] std::string get_info() const override;

};

class SYTCFrame : public Frame {
private:
    static const uint8_t kByteForFormat = 1;
    char format_time_;
    char* binary_data_;

public:
    SYTCFrame(std::ifstream& file, char* id, char* char_size, char* flags, size_t size, typeFrame type);

    ~SYTCFrame();

    [[nodiscard]] std::string get_info() const override;

};

class GRIDFrame : public Frame {
private:
    char* text_;
    char* binary_data_;
    char letter;
    size_t size_bin_data_;
    size_t size_text_;
public:
    GRIDFrame(std::ifstream& file, char* id, char* char_size, char* flags, size_t size, typeFrame type);

    ~GRIDFrame();

    [[nodiscard]] std::string get_info() const override;

};

struct Data {
private:
    std::vector<Frame*> frames_;
public:
    ~Data();

    void parse(std::ifstream& file, size_t size);

    std::vector<std::string> get_info();
};
