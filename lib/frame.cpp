//
// Created by Mickey on 2/8/2023.
//

#include "frame.h"

std::string iso_8859_1_to_utf8(char* str, size_t size) {
    std::string strOut;
    char* it = str;

    while (size--) {
        uint8_t ch = *it;
        if (ch < 0x80) {
            strOut.push_back(ch);
        }
        else {
            strOut.push_back(0xc0 | ch >> 6);
            strOut.push_back(0x80 | (ch & 0x3f));
        }
    }
    return strOut;
}

Frame::Frame(std::ifstream& file) {
    file.read(id_, kSizeIdInBytes);

    std::string id_string_;
    for (char i : id_) {
        id_string_.push_back(i);
    }

    switch(id_[kIdStartIndex]) {
        case kTextId: {
            type_ = typeFrame::Text;
            break;
        }
        case kURLId: {
            if (id_[kIdStartIndex + 1] != kXID) {
                type_ = typeFrame::URL;
            } else {
                type_ = typeFrame::Text;
            }
            break;
        }
        case (kU000Id): {
            const std::string kUFIDId = "UFID";
            const std::string kUSLTId = "USLT";
            const std::string kUSERId = "USER";
            if (id_string_ == kUFIDId) {
                type_ = typeFrame::Text;
                break;
            } else if (id_string_ == kUSLTId | id_string_ == kUSERId) {
                type_ = typeFrame::USLT;
                break;
            }
        }
        case(kS000Id): {
            const std::string kSIGNId = "SIGN";
            const std::string kSYTCId = "SYTC";

            if (id_string_ == kSIGNId) {
                type_ = typeFrame::SIGN;
                break;
            } else if (id_string_ == kSYTCId) {
                type_ = typeFrame::SYTC;
                break;
            }
        }
        case (kC000Id): {
            const std::string kCOMMId = "COMM";
            if (id_string_ == kCOMMId) {
                type_ = typeFrame::USLT;
                break;
            }

        }
        case (kP000Id): {
            const std::string kPOPMId = "POPM";
            if (id_string_ == kPOPMId) {
                type_ = typeFrame::POPM;
                break;
            }
        }
        case (kG000Id): {
            const std::string kGRIDId = "GRID";
            if (id_string_ == kGRIDId) {
                type_ = typeFrame::GRID;
                break;
            }
        }
        default:
            type_ = typeFrame::etc;
    }

    file.read(char_size_, kSizeFrameInBytes);
    size_ = 0;
    for (char i : char_size_) {
        size_ <<= kSyncsafeByte;
        size_ |= (i & 0b01111111);
    }

    file.read(flags_, kBytesForFlags);

    set_flags();

}

Frame::Frame(const char* id, const char* char_size, const char* flags, uint32_t size, typeFrame type) {
    for (uint8_t i = 0; i < kSizeIdInBytes; i++) {
        id_[i] = id[i];
    }
    for (uint8_t i = 0; i < kSizeFrameInBytes; i++) {
        char_size_[i] = char_size[i];
    }
    for (uint8_t i = 0; i < kBytesForFlags; i++) {
        flags_[i] = flags[i];
    }
    size_ = size;
    type_ = type;

    set_flags();

}

void Frame::set_flags() {
    uint16_t flags_bits = 0;
    for (char flag : flags_) {
        flags_bits <<= sizeof(char);
        flags_bits |= flag;
    }

    flag_a_ = flags_bits & kFlagABit;
    flag_b_ = flags_bits & kFlagBBit;
    flag_c_ = flags_bits & kFlagCBit;
    flag_h_ = flags_bits & kFlagHBit;
    flag_k_ = flags_bits & kFlagKBit;
    flag_m_ = flags_bits & kFlagMBit;
    flag_n_ = flags_bits & kFlagNBit;
    flag_p_ = flags_bits & kFlagPBit;
}

std::string Frame::get_info() const {
    return {};
}

typeFrame Frame::get_type() const {
    return type_;
}

char* Frame::get_id() {
    return id_;
}

char* Frame::get_char_size() {
    return char_size_;
}

char* Frame::get_flags() {
    return flags_;
}

size_t Frame::get_size() const {
    return size_;
}

TextFrame::TextFrame(std::ifstream& file, char* id, char* char_size, char* flags, size_t size, typeFrame type) :
        Frame(id, char_size, flags, size, type) {

    const uint32_t kTALBInBin = 0x54414c42;
    const uint32_t kTBPMInBin = 0x5442504d;
    const uint32_t kTCOMInBin = 0x54434f4d;
    const uint32_t kTCONInBin = 0x54434f4e;
    const uint32_t kTCOPInBin = 0x54434f50;
    const uint32_t kTDENInBin = 0x5444454e;
    const uint32_t kTDLYInBin = 0x54444c59;
    const uint32_t kTDORInBin = 0x54444f52;
    const uint32_t kTDRCInBin = 0x54445243;
    const uint32_t kTDRLInBin = 0x5444524c;
    const uint32_t kTDTGInBin = 0x54445447;
    const uint32_t kTENCInBin = 0x54454e43;
    const uint32_t kTEXTInBin = 0x54455854;
    const uint32_t kTIPLInBin = 0x5449504c;
    const uint32_t kTIT1InBin = 0x54495431;
    const uint32_t kTIT2InBin = 0x54495432;
    const uint32_t kTIT3InBin = 0x54495433;
    const uint32_t kTKEYInBin = 0x544b4559;
    const uint32_t kTLANInBin = 0x544c414e;
    const uint32_t kTLENInBin = 0x544c454e;
    const uint32_t kTMCLInBin = 0x544d434c;
    const uint32_t kTMEDInBin = 0x544d4544;
    const uint32_t kTMOOInBin = 0x544d4f4f;
    const uint32_t kTOALInBin = 0x544f414c;
    const uint32_t kTOFNInBin = 0x544f464e;
    const uint32_t kTOLYInBin = 0x544f4c59;
    const uint32_t kTOPEInBin = 0x544f5045;
    const uint32_t kTOWNInBin = 0x544f574e;
    const uint32_t kTPE1InBin = 0x54504531;
    const uint32_t kTPE2InBin = 0x54504532;
    const uint32_t kTPE3InBin = 0x54504533;
    const uint32_t kTPE4InBin = 0x54504534;
    const uint32_t kTPOSInBin = 0x54504f53;
    const uint32_t kTPROInBin = 0x5450524f;
    const uint32_t kTPUBInBin = 0x54505542;
    const uint32_t kTRCKInBin = 0x5452434b;
    const uint32_t kTRSNInBin = 0x5452534e;
    const uint32_t kTRSOInBin = 0x5452534f;
    const uint32_t kTSOAInBin = 0x54534f41;
    const uint32_t kTSOTInBin = 0x54534f54;
    const uint32_t kTSRCInBin = 0x54535243;
    const uint32_t kTSSEInBin = 0x54535345;
    const uint32_t kTSSTInBin = 0x54535354;
    const uint32_t kTCMPInBin = 0x54434d50;
    const uint32_t kTSOPInBin = 0x54534f50;
    const uint32_t kTXXXInBin = 0x54585858;
    const uint32_t kWXXXInBin = 0x57585858;
    const uint32_t kUFIDInBin = 0x55464944;

    uint32_t IdBin = 0;
    for (auto i : id_) {
        IdBin <<= 8;
        IdBin |= i;
    }

    switch (IdBin) {
        case(kTALBInBin):
            text_frame_type_ = TextFrameType::TALB;
            break;
        case(kTBPMInBin):
            text_frame_type_ = TextFrameType::TBPM;
            break;
        case(kTCOMInBin):
            text_frame_type_ = TextFrameType::TCOM;
            break;
        case(kTCONInBin):
            text_frame_type_ = TextFrameType::TCON;
        case(kTCOPInBin):
            text_frame_type_ = TextFrameType::TCOP;
            break;
        case(kTDENInBin):
            text_frame_type_ = TextFrameType::TDEN;
            break;
        case(kTDLYInBin):
            text_frame_type_ = TextFrameType::TDLY;
            break;
        case(kTDORInBin):
            text_frame_type_ = TextFrameType::TDOR;
            break;
        case(kTDRCInBin):
            text_frame_type_ = TextFrameType::TDRC;
            break;
        case(kTDRLInBin):
            text_frame_type_ = TextFrameType::TDRL;
            break;
        case(kTDTGInBin):
            text_frame_type_ = TextFrameType::TDTG;
            break;
        case(kTENCInBin):
            text_frame_type_ = TextFrameType::TENC;
            break;
        case(kTEXTInBin):
            text_frame_type_ = TextFrameType::TEXT;
            break;
        case(kTIPLInBin):
            text_frame_type_ = TextFrameType::TIPL;
            break;
        case(kTIT1InBin):
            text_frame_type_ = TextFrameType::TIT1;
            break;
        case(kTIT2InBin):
            text_frame_type_ = TextFrameType::TIT2;
            break;
        case(kTIT3InBin):
            text_frame_type_ = TextFrameType::TIT3;
            break;
        case(kTKEYInBin):
            text_frame_type_ = TextFrameType::TKEY;
            break;
        case(kTLANInBin):
            text_frame_type_ = TextFrameType::TLAN;
            break;
        case(kTLENInBin):
            text_frame_type_ = TextFrameType::TLEN;
            break;
        case(kTMCLInBin):
            text_frame_type_ = TextFrameType::TMCL;
            break;
        case(kTMEDInBin):
            text_frame_type_ = TextFrameType::TMED;
            break;
        case(kTMOOInBin):
            text_frame_type_ = TextFrameType::TMOO;
            break;
        case(kTOALInBin):
            text_frame_type_ = TextFrameType::TOAL;
            break;
        case(kTOFNInBin):
            text_frame_type_ = TextFrameType::TOFN;
            break;
        case(kTOLYInBin):
            text_frame_type_ = TextFrameType::TOLY;
            break;
        case(kTOPEInBin):
            text_frame_type_ = TextFrameType::TOPE;
            break;
        case(kTOWNInBin):
            text_frame_type_ = TextFrameType::TOWN;
            break;
        case(kTPE1InBin):
            text_frame_type_ = TextFrameType::TPE1;
            break;
        case(kTPE2InBin):
            text_frame_type_ = TextFrameType::TPE2;
            break;
        case(kTPE3InBin):
            text_frame_type_ = TextFrameType::TPE3;
            break;
        case(kTPE4InBin):
            text_frame_type_ = TextFrameType::TPE4;
            break;
        case(kTPOSInBin):
            text_frame_type_ = TextFrameType::TPOS;
            break;
        case(kTPROInBin):
            text_frame_type_ = TextFrameType::TPRO;
            break;
        case(kTPUBInBin):
            text_frame_type_ = TextFrameType::TPUB;
            break;
        case(kTRCKInBin):
            text_frame_type_ = TextFrameType::TRCK;
            break;
        case(kTRSNInBin):
            text_frame_type_ = TextFrameType::TRSN;
            break;
        case(kTRSOInBin):
            text_frame_type_ = TextFrameType::TRSO;
            break;
        case(kTSOAInBin):
            text_frame_type_ = TextFrameType::TSOA;
            break;
        case(kTSOTInBin):
            text_frame_type_ = TextFrameType::TSOT;
            break;
        case(kTSRCInBin):
            text_frame_type_ = TextFrameType::TSRC;
            break;
        case(kTSSEInBin):
            text_frame_type_ = TextFrameType::TSSE;
            break;
        case(kTSSTInBin):
            text_frame_type_ = TextFrameType::TSST;
            break;
        case(kTCMPInBin):
            text_frame_type_ = TextFrameType::TCMP;
            break;
        case(kTSOPInBin):
            text_frame_type_ = TextFrameType::TSOP;
            break;
        case(kTXXXInBin):
            text_frame_type_ = TextFrameType::TXXX;
            break;
        case(kWXXXInBin):
            text_frame_type_ = TextFrameType::WXXX;
            break;
        case(kUFIDInBin):
            text_frame_type_ = TextFrameType::UFID;
            break;
        default:
            text_frame_type_ = TextFrameType::UNKNOWN;
            break;
    }

    const uint8_t kBytesForEncoding = 1;

    const uint8_t ISO88591 = 0;
    const uint8_t UTF16 = 1;
    const uint8_t UTF16BE = 2;
    const uint8_t UTF8 = 3;

    char encoding;
    file.get(encoding);
    switch (encoding) {
        case ISO88591:
            typeEncoding = TextEncoding::ISO88591;
            break;
        case UTF16:
        case UTF16BE:
            typeEncoding = TextEncoding::UTF16;
            break;
        case UTF8:
        default:
            typeEncoding = TextEncoding::UTF8;
            break;
    }

    const uint32_t kBytesForText = size_ - kBytesForEncoding;
    char text[kBytesForText];
    file.read(text, kBytesForText);


    if (typeEncoding == TextEncoding::UTF16) {
        text_16 = new wchar_t[kBytesForText / sizeof(wchar_t)];
        text_size_ = kBytesForText / sizeof(wchar_t);
        uint16_t letter;
        wchar_t* it = text_16;
        for (uint32_t i = 0; i < kBytesForText; i += sizeof(wchar_t)) {
            letter = 0;

            letter |= text[i];
            letter <<= sizeof(char);
            letter |= text[i + 1];
            *it = letter;
            it++;
        }
    } else if (typeEncoding == TextEncoding::ISO88591) {
        std::string converted_text;
        converted_text = iso_8859_1_to_utf8(text, kBytesForText);
        text_8 = new char[converted_text.size()];
        text_size_ = converted_text.size();
        for (uint32_t i = 0; i < converted_text.size(); i++) {
            text_8[i] = converted_text[i];
        }
    } else {
        text_8 = new char[kBytesForText];
        text_size_ = kBytesForText;
        for (uint32_t i = 0; i < kBytesForText; i++) {
            text_8[i] = text[i];
        }

    }

}

TextFrame::~TextFrame() {
    if (typeEncoding == TextEncoding::UTF16) {
        delete []text_16;
    } else {
        delete []text_8;
    }
}

TextFrame::TextFrameType TextFrame::get_type() const {
    return text_frame_type_;
}

std::string TextFrame::get_info() const {
    std::string data;
    std::u16string data16;
    std::string result;

    for (uint32_t i = 0; i < text_size_; i++) {
        if (typeEncoding == TextEncoding::UTF16) {
            data16.push_back(text_16[i]);
        } else {
            data.push_back(text_8[i]);
        }

    }

    for (auto i : id_) {
        result.push_back(i);
    }
    result += " : ";

    return result + data;
}

URLFrame::URLFrame(std::ifstream& file, char* id, char* char_size, char* flags, size_t size, typeFrame type) :
        Frame(id, char_size, flags, size, type) {
    const uint32_t kWCOMInBin = 0x57434f4d;
    const uint32_t kWCOPInBin = 0x57434f50;
    const uint32_t kWOAFInBin = 0x574f4146;
    const uint32_t kWOARInBin = 0x574f4152;
    const uint32_t kWOASInBin = 0x574f4153;
    const uint32_t kWORSInBin = 0x574f5253;
    const uint32_t kWPAYInBin = 0x57504159;
    const uint32_t kWPUBInBin = 0x57505542;
    const uint32_t kUFIDInBin = 0x55464944;

    uint32_t IdBin = 0;
    for (auto i : id_) {
        IdBin <<= 8;
        IdBin |= i;
    }

    switch(IdBin) {
        case(kWCOMInBin):
            url_frame_type_ = URLFrameType::WCOM;
            break;
        case(kWCOPInBin):
            url_frame_type_ = URLFrameType::WCOP;
            break;
        case(kWOAFInBin):
            url_frame_type_ = URLFrameType::WOAF;
            break;
        case(kWOARInBin):
            url_frame_type_ = URLFrameType::WOAR;
            break;
        case(kWOASInBin):
            url_frame_type_ = URLFrameType::WOAS;
            break;
        case(kWORSInBin):
            url_frame_type_ = URLFrameType::WORS;
            break;
        case(kWPAYInBin):
            url_frame_type_ = URLFrameType::WPAY;
            break;
        case(kWPUBInBin):
            url_frame_type_ = URLFrameType::WPUB;
            break;
        case(kUFIDInBin):
            url_frame_type_ = URLFrameType::UFID;
            break;
        default:
            url_frame_type_ = URLFrameType::UNKNOWN;
    }

    text = new char[size_];
    file.read(text, size_);


}

URLFrame::~URLFrame() {
    delete[] text;
}

std::string URLFrame::get_info() const {
    std::string result;

    for (auto i : id_) {
        result.push_back(i);
    }

    result += " : ";

    std::string data;
    for (int i = 0; i < size_; i++) {
        data.push_back(text[i]);
    }

    return result + data;

}

URLFrame::URLFrameType URLFrame::get_type() const {
    return url_frame_type_;
}

SIGNFrame::SIGNFrame(std::ifstream& file, char* id, char* char_size, char* flags, size_t size, typeFrame type) :
        Frame(id, char_size, flags, size, type) {

    file.get(sign_);
    binary_data_ = new char[size_ - kBytesForSign];
    file.read(binary_data_, size_ - kBytesForSign);
}

SIGNFrame::~SIGNFrame() {
    delete[] binary_data_;
}

std::string SIGNFrame::get_info() const {
    std::string result = "SIGN : ";
    result += sign_;
    result += " - ";
    for (int i = 0; i < size_ - kBytesForSign; i++) {
        result += std::to_string((uint8_t) binary_data_[i]);
        result += ' ';
    }

    return result;
}

USLTFrame::USLTFrame(std::ifstream& file, char* id, char* char_size, char* flags, size_t size, typeFrame type) :
        Frame(id, char_size, flags, size, type) {
    text_ = new char[size_ - kBytesForLanguages - kByteForEncoding];
    file.get(encoding_);
    file.read(languages_, kBytesForLanguages);
    file.read(text_, size_ - kBytesForLanguages - kByteForEncoding);
}

USLTFrame::~USLTFrame() {
    delete[] text_;
}

std::string USLTFrame::get_info() const {
    std::string result;
    for (char i : id_) {
        result.push_back(i);
    }
    result += " : ";
    for (char i : languages_) {
        result.push_back(i);
    }
    result += " - ";
    for (int32_t i = 0; i < size_ - kBytesForLanguages - kByteForEncoding; i++) {
        result.push_back(text_[i]);
    }
    return result;
}

POPMFrame::POPMFrame(std::ifstream& file, char* id, char* char_size, char* flags, size_t size, typeFrame type) :
        Frame(id, char_size, flags, size, type) {

    char buffer;
    email_size_ = 0;
    file.get(buffer);
    while (buffer != '\0') {
        email_size_++;
        file.get(buffer);
    }
    file.seekg((int64_t) file.tellg() - email_size_ - 1);
    email_ = new char[email_size_];
    file.read(email_, email_size_);
    file.seekg((int64_t) file.tellg() + 1);
    file.get(score_);

    const uint8_t kSizeForCounters = size_ - email_size_ - 2;

    counter_char_ = new char[kSizeForCounters];
    file.read(counter_char_, kSizeForCounters);
    counter_ = 0;
    for (int i = 0; i < kSizeForCounters; i++) {
        counter_ <<= sizeof(char);
        counter_ |= counter_char_[i];
    }
}

POPMFrame::~POPMFrame() {
    delete[] email_;
    delete[] counter_char_;
}

std::string POPMFrame::get_info() const {
    std::string result;

    for (char i : id_) {
        result.push_back(i);
    }

    result += "  \nemail : ";
    for (int i = 0; i < email_size_; i++) {
        result.push_back(email_[i]);
    }

    result += "\nscore : ";
    result += std::to_string((uint8_t) score_);
    result += "\ncounter : " + std::to_string(counter_) + '\n';
    return result;
}

SYTCFrame::SYTCFrame(std::ifstream& file, char* id, char* char_size, char* flags, size_t size, typeFrame type) :
        Frame(id, char_size, flags, size, type) {

    file.get(format_time_);

    binary_data_ = new char[size_ - kByteForFormat];
    file.read(binary_data_, size_ - kByteForFormat);
}

SYTCFrame::~SYTCFrame() {
    delete[] binary_data_;
}

std::string SYTCFrame::get_info() const {
    std::string result;

    for (char i : id_) {
        result.push_back(i);
    }

    const char kByteForMPEG = 1;
    const char kByteForMS = 2;

    if (format_time_ == kByteForMPEG) {
        result += " (MPEG as a counter) : ";
    } else {
        result += " (Ms as a counter) : ";
    }

    for (int i = 0; i < size_ - kByteForFormat; i++) {
        result += std::bitset<8>(binary_data_[i]).to_string();
        result += ' ';
    }

    return result;
}

GRIDFrame::GRIDFrame(std::ifstream& file, char* id, char* char_size, char* flags, size_t size, typeFrame type) :
        Frame(id, char_size, flags, size, type) {
    size_text_ = 0;
    char buffer;

    file.get(buffer);
    while (buffer != '\0') {
        size_text_++;
        file.get(buffer);
    }

    file.seekg((int64_t) file.tellg() - size_text_ - 1);
    text_ = new char[size_text_];
    file.read(text_, size_text_);


    file.get(letter);

    const size_t kBytesForFormat = size_text_ + 2;
    binary_data_ = new char[size_ - kBytesForFormat];
    file.read(binary_data_, size_ - kBytesForFormat);
}

GRIDFrame::~GRIDFrame() {
    delete[] binary_data_;
    delete[] text_;
}

std::string GRIDFrame::get_info() const {
    std::string result;
    const size_t kBytesForFormat = size_text_ + 2;
    for (char i : id_) {
        result.push_back(i);
    }

    result += " : ";
    result.push_back(letter);

    for (int i = 0; i < size_ - kBytesForFormat; i++) {
        result.push_back(' ');
        result += std::bitset<8>(binary_data_[i]).to_string();
    }

    return result;
}

Data::~Data() {
    for (auto& frame : frames_) {
        delete frame;
    }
}

void Data::parse(std::ifstream& file, size_t size) {
    while (file.tellg() != -1 && file.tellg() < size) {
        Frame frame_(file);

        switch (frame_.get_type()) {
            case (typeFrame::Text): {
                auto* text_frame = new TextFrame(file, frame_.get_id(), frame_.get_char_size(),
                                                 frame_.get_flags(), frame_.get_size(), frame_.get_type());
                frames_.push_back(text_frame);
                break;
            }
            case (typeFrame::URL): {
                auto* url_frame = new URLFrame(file, frame_.get_id(), frame_.get_char_size(),
                                               frame_.get_flags(), frame_.get_size(), frame_.get_type());
                frames_.push_back(url_frame);
                break;
            }
            case (typeFrame::SIGN): {
                auto* sign_frame = new SIGNFrame(file, frame_.get_id(), frame_.get_char_size(),
                                                 frame_.get_flags(), frame_.get_size(), frame_.get_type());
                frames_.push_back(sign_frame);
                break;
            }
            case (typeFrame::USLT): {
                auto* uslt_frame = new USLTFrame(file, frame_.get_id(), frame_.get_char_size(),
                                                 frame_.get_flags(), frame_.get_size(), frame_.get_type());
                frames_.push_back(uslt_frame);
                break;
            }
            case (typeFrame::POPM): {
                auto* popm_frame = new POPMFrame(file, frame_.get_id(), frame_.get_char_size(),
                                                 frame_.get_flags(), frame_.get_size(), frame_.get_type());
                frames_.push_back(popm_frame);
                break;
            }
            case (typeFrame::SYTC): {
                auto* sytc_frame = new SYTCFrame(file, frame_.get_id(), frame_.get_char_size(),
                                                 frame_.get_flags(), frame_.get_size(), frame_.get_type());
                frames_.push_back(sytc_frame);
                break;
            }
            case (typeFrame::GRID): {
                auto* grid_frame = new GRIDFrame(file, frame_.get_id(), frame_.get_char_size(),
                                                 frame_.get_flags(), frame_.get_size(), frame_.get_type());
                frames_.push_back(grid_frame);
                break;
            }
            default: {
                file.seekg((int64_t) file.tellg() + frame_.get_size());
                break;
            }
        }
    }
}

std::vector<std::string> Data::get_info() {
    std::vector<std::string> info;
    for (auto& i : frames_) {
        switch (i->get_type()) {
            case typeFrame::Text:
                info.push_back(dynamic_cast<TextFrame*>(i)->get_info());
                break;
            case typeFrame::URL:
                info.push_back(dynamic_cast<URLFrame*>(i)->get_info());
                break;
            case typeFrame::USLT:
                info.push_back(dynamic_cast<USLTFrame*>(i)->get_info());
                break;
            case typeFrame::POPM:
                info.push_back(dynamic_cast<POPMFrame*>(i)->get_info());
                break;
            case typeFrame::SIGN:
                info.push_back(dynamic_cast<SIGNFrame*>(i)->get_info());
                break;
            case typeFrame::SYTC:
                info.push_back(dynamic_cast<SYTCFrame*>(i)->get_info());
                break;
            case typeFrame::GRID:
                info.push_back(dynamic_cast<GRIDFrame*>(i)->get_info());
                break;
            default:
                break;
        }
    }

    std::sort(info.begin(), info.end());
    return info;
}
