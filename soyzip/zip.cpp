// PKZIP version 2.0: https://pkware.cachefly.net/webdocs/APPNOTE/APPNOTE-2.0.txt
// Deflate: https://tools.ietf.org/html/rfc1951
#include <bits/stdc++.h>
#define DEBUGMSG(format, ...) printf(format, ##__VA_ARGS__); fflush(stdout)
using namespace std;

const unsigned LOCAL_HEADER_SIZE = 30;

const unsigned MAX_LIT_LEN_CODE = 285; // max literal/distance code
const unsigned MAX_DIST_CODE = 32768; // max distance code
const unsigned MAX_CODE_LEN = 15; // max bits in a code

const size_t MAX_BUFFER_SIZE = 1048576; // 1M
const size_t MAX_FIELD_LEN = 65535;
const size_t MAX_FILE_COUNT = 1048576;

enum ReturnState {
    FILE_CONTENT_ERROR,
    FILE_FORMAT_ERROR,
    FILE_UNEXPECTED_CLOSE,
    SUCCESS
};

struct Huffman {
    unsigned code, len;
};

struct FileInfo {
    char filename[MAX_FIELD_LEN],
         extrafield[MAX_FIELD_LEN],
         comment[MAX_FIELD_LEN];
    unsigned made_version, required_version,
             purpose, method,
             modtime, moddate,
             inter_attr, exter_attr,
             compressed_size, uncompressed_size,
             crc32, local_header_offset, start_disk,
             filename_len, extrafield_len, comment_len,
             index;
};

struct ZipInfo {
    char comment[MAX_FIELD_LEN];
    unsigned this_disk, start_disk,
             count_entry_total, count_entry_here,
             dir_size, dir_offset;
};


inline struct tm dosdatetime(uint16_t date, uint16_t time) {
    struct tm datetime;
    datetime.tm_mday = date & 0x1f;
    datetime.tm_mon = ((date >> 5) & 0xf) - 1;
    datetime.tm_year = ((date >> 9) & 0x7f) + 80;
    datetime.tm_sec = time & 0x1f;
    datetime.tm_min = (time >> 5) & 0x3f;
    datetime.tm_hour = (time >> 11) & 0x1f;
    return datetime;
}

inline uint16_t read16(void* ptr) {
    return *(uint16_t*)ptr;
}

inline uint32_t read32(void* ptr) {
    return *(uint32_t*)ptr;
}


class Decoder {
public:
    Decoder(istream* input, ostream* output, char* buf, size_t buf_size) {
        this->input = input;
        this->output = output;
        this->buf = buf;
        this->buf_size = buf_size;
        this->cBit = this->cByte = 0;

        this->input->read(this->buf, buf_size);
    }
    unsigned readbits(size_t count) {
        // beware of overflow
        unsigned bits = 0;
        for (unsigned i=0; i<count; i++)
            bits |= nextbit() << i;
        return bits;
    }
    ReturnState decode_deflate(const FileInfo& file) {
        DEBUGMSG("[decode_deflate] begin: %llu size: %u\n", (size_t)input->tellg(), file.compressed_size);

        char bFinal, bType;
        do {
            bFinal = readbits(1);
            bType = readbits(2);
            DEBUGMSG("header: %hx %hx\n", bFinal, bType);
            if (bType == 0) {
                // stored
                readbits(5); // ignore
                read_stored(readbits(16));
            }
            else if (bType == 1) {
                // fixed huffman code
                DEBUGMSG("fixed huffman\n");
                read_fixed();
            }
            else if (bType == 2) {
                // dynamic huffman code
                DEBUGMSG("dynamic huffman\n");
                read_dynamic();
                input->seekg(file.local_header_offset + LOCAL_HEADER_SIZE + file.compressed_size);
            }
            else {
                DEBUGMSG("file content error!\n");
                return FILE_CONTENT_ERROR;
            }
        } while (!bFinal);
        return SUCCESS;
    }
    ReturnState read_stored(size_t block_size) {
        DEBUGMSG("read_stored block_size: %u\n", block_size);
        copy_n(istream_iterator<char>(*input),
               block_size,
               ostream_iterator<char>(*output));
        cout << output->rdbuf();
    }
    ReturnState read_fixed() {
        unsigned symbol;
        do {
            symbol = 0xfffffff;
            unsigned bits = 0;
            for (int bits_count=1; bits_count<=9; bits_count++) {
                bits = (bits << 1) | readbits(1);
                if (bits_count == 8 && 48 <= bits && bits <= 191) {
                    symbol = bits - 48;
                    break;
                } else if (bits_count == 9 && 400 <= bits && bits <= 511) {
                    symbol = bits - 256;
                    break;
                } else if (bits_count == 7 && bits <= 23) {
                    symbol = bits + 256;
                    break;
                } else if (bits_count == 8 && 192 <= bits && bits <= 199) {
                    symbol = bits + 88;
                    break;
                }
            }
            DEBUGMSG("symbol: %u\n", symbol);
        } while (symbol != 256);
        return SUCCESS;
    }
    ReturnState read_dynamic() {
        unsigned hLit = readbits(5) + 257;
        unsigned hDist = readbits(5) + 1;
        unsigned hCLen = readbits(4) + 4;

        unsigned alpha[19] = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5,
                              11, 4, 12, 3, 13, 2, 14, 1, 15};
        Huffman alpha_tree[19];
        unsigned alpha_codelen[19] = {};
        for (int i = 0; i < hCLen; ++i)
            alpha_codelen[alpha[i]] = readbits(3);
        construct(tree, alpha_codelen);
    }
    void construct(Huffman tree[], int codelen[]) {
        unsigned len_count[MAX_CODE_LEN] = {};
        for (unsigned symbol = 0; symbol <= MAX_LIT_LEN_CODE; ++symbol)
            ++len_count[codelen[symbol]];

        unsigned c = 0;
        unsigned begin_code[MAX_CODE_LEN + 1];
        for (unsigned len = 1; len <= MAX_CODE_LEN; ++len) {
            c = (c + len_count[len-1]) << 1;
            begin_code[len] = c;
        }

        for (unsigned symbol = 0; symbol <= MAX_LIT_LEN_CODE; ++symbol) {
            unsigned c = begin_code[codelen[symbol]]++;
            tree[c].len = codelen[symbol];
            tree[c].symbol = symbol;
        }
    }

private:
    istream* input;
    ostream* output;
    char* buf;
    size_t buf_size, cByte, cBit;

    unsigned nextbit() {
        unsigned res = (buf[cByte] >> cBit) & 1;
        ++cBit;
        if (cBit > 7) {
            ++cByte;
            cBit = 0;
        }
        if (cByte > buf_size) {
            input->read(buf, buf_size);
            cByte = 0;
        }
        return res;
    }
};

class SoyZip {
public:
    vector<FileInfo> files_info;
    ZipInfo zip_info;

    istream* input;
    stringstream output;

    // void encode(byte* begin, size_t size, size_t block_size) {
    //     size_t dataend = 0;
    //     data[dataend++] = Block(0, 0, *begin);
    //     DEBUGMSG("%lld %lld %c\n", data[dataend-1].offset, data[dataend-1].length, data[dataend-1].next);
    //     byte* cur = begin + 1;
    //     byte* search = begin;
    //     while (cur < begin+size) {
    //         byte *pStart, *pEnd;
    //         pStart = search;
    //         while (*pStart != *cur && pStart+1 < cur)
    //             pStart++;
    //         data[dataend].offset = cur - pStart - 1;
    //         pEnd = pStart;
    //         while (*pEnd == *cur && pEnd < cur)
    //             pEnd++, cur++;
    //         data[dataend].length = pEnd - pStart;
    //         data[dataend].next = *cur;
    //         DEBUGMSG("%d %d %d %d\n", search-begin, pStart-begin, pEnd-begin, cur-begin);
    //         DEBUGMSG("data: %lld %lld %c\n", data[dataend].offset, data[dataend].length, data[dataend].next);
    //         dataend++;
    //         cur++;
    //         if (search < cur - block_size)
    //             search = cur - block_size;
    //     }
    // }

    ReturnState init(istream* input) {
        this->input = input;
        while (input->good()) {
            input->read(buf, 4);
            uint32_t signature = read32(buf);
            if (signature == 0x04034b50) {
                // local file header
                // these information is not read into memory
                // but central directory header instead
                DEBUGMSG("local file header\n");

                FileInfo info;
                input->read(buf, 26);

                info.required_version = read16(buf);
                info.purpose = read16(buf+2);
                info.method = read16(buf+4);
                info.modtime = read16(buf+6);
                info.moddate = read16(buf+8);
                info.crc32 = read32(buf+10);
                info.compressed_size = read32(buf+14);
                info.uncompressed_size = read32(buf+18);
                info.filename_len = read16(buf+22);
                info.extrafield_len = read16(buf+24);

                input->read(info.filename, info.filename_len);
                info.filename[info.filename_len] = '\0';
                input->read(info.extrafield, info.extrafield_len);
                info.extrafield[info.extrafield_len] = '\0';

                if ((info.purpose >> 3) & 1) {
                    // data descriptor
                    DEBUGMSG("data descriptor\n");
                    input->read(buf, 12);
                    info.crc32 = read32(buf);
                    info.compressed_size = read32(buf+4);
                    info.uncompressed_size = read32(buf+8);
                }

                // file data
                input->ignore(info.compressed_size);

            } else if (signature == 0x02014b50) {
                // central directory header
                DEBUGMSG("central directory header\n");

                FileInfo info;
                input->read(buf, 42);

                info.made_version = read16(buf);
                info.required_version = read16(buf+2);
                info.purpose = read16(buf+4);
                info.method = read16(buf+6);
                info.modtime = read16(buf+8);
                info.moddate = read16(buf+10);
                info.crc32 = read32(buf+12);
                info.compressed_size = read32(buf+16);
                info.uncompressed_size = read32(buf+20);
                info.filename_len = read16(buf+24);
                info.extrafield_len = read16(buf+26);
                info.comment_len = read16(buf+28);
                info.start_disk = read16(buf+30);
                info.inter_attr = read16(buf+32);
                info.exter_attr = read32(buf+34);
                info.local_header_offset = read32(buf+38);

                input->read(info.filename, info.filename_len);
                info.filename[info.filename_len] = '\0';
                input->read(info.extrafield, info.extrafield_len);
                info.extrafield[info.extrafield_len] = '\0';
                input->read(info.comment, info.comment_len);
                info.comment[info.comment_len] = '\0';

                info.index = files_info.size();
                files_info.push_back(info);

            } else if (signature == 0x06054b50) {
                // end of central directory record
                DEBUGMSG("end of central directory record\n\n");

                input->read(buf, 18);
                zip_info.this_disk = read16(buf);
                zip_info.start_disk = read16(buf+2);
                zip_info.count_entry_total = read16(buf+4);
                zip_info.count_entry_here = read16(buf+6);
                zip_info.dir_size = read32(buf+8);
                zip_info.dir_offset = read32(buf+12);
                uint16_t comment_len = read16(buf+16);

                input->read(zip_info.comment, comment_len);
                zip_info.comment[comment_len] = '\0';

                break;
            } else {
                DEBUGMSG("unsupported signature: %x\n", signature);
                return FILE_CONTENT_ERROR;
            }
        }
        return (input->eof() ? SUCCESS : FILE_UNEXPECTED_CLOSE);
    }
    void list_files() {
        DEBUGMSG("files in zip:\n");
        for (FileInfo info : files_info) {
            tm moddt = dosdatetime(info.moddate, info.modtime);
            DEBUGMSG("file: %s\n", info.filename);
            DEBUGMSG("made_version: %hu\n", info.made_version);
            DEBUGMSG("required_version: %hu\n", info.required_version);
            DEBUGMSG("purpose: %hx\n", info.purpose);
            DEBUGMSG("method: %hx\n", info.method);
            DEBUGMSG("modtime: %u:%u:%u\n", moddt.tm_hour, moddt.tm_min, moddt.tm_sec);
            DEBUGMSG("moddate: %u.%u.%u\n", 1900+moddt.tm_year, 1+moddt.tm_mon, moddt.tm_mday);
            DEBUGMSG("crc32: %x\n", info.crc32);
            DEBUGMSG("compressed size: %u\n", info.compressed_size);
            DEBUGMSG("uncompressed size: %u\n", info.uncompressed_size);
            DEBUGMSG("start_disk: %hx\n", info.start_disk);
            DEBUGMSG("inter_attr: %hx\n", info.inter_attr);
            DEBUGMSG("exter_attr: %x\n", info.exter_attr);
            DEBUGMSG("offset: %u\n", info.local_header_offset);
            DEBUGMSG("index: %u\n", info.index);
            DEBUGMSG("filename_len: %u\n", info.filename_len);
            DEBUGMSG("extrafield_len: %u\n", info.extrafield_len);
            DEBUGMSG("comment_len: %u\n", info.comment_len);
            DEBUGMSG("extrafield: %*s\n", info.extrafield_len, info.extrafield);
            DEBUGMSG("\n");
        }
    }
    ReturnState decode_file(const FileInfo& file) {
        if (file.method == 0) {
            input->seekg(file.local_header_offset + LOCAL_HEADER_SIZE
                         + file.filename_len); // [TODO] consider extra field
            copy_n(istream_iterator<char>(*input),
                   file.compressed_size,
                   ostream_iterator<char>(output));

            DEBUGMSG("tellg: %d\n", (int)input->tellg());
            DEBUGMSG("stored data: \n");
            cout << output.rdbuf();
            return SUCCESS;

        } else if (file.method == 8) {
            size_t buf_size = MAX_BUFFER_SIZE;
            if (buf_size > file.compressed_size)
                buf_size = file.compressed_size;
            Decoder dc(input, &output, buf, buf_size);
            return dc.decode_deflate(file);

        } else {
            DEBUGMSG("unsupported compression method\n");
            return FILE_FORMAT_ERROR;
        }
    }

private:
    char buf[MAX_BUFFER_SIZE];
    // TODO: prevent buffer overflow
};

char str[10000];
int main() {
    fstream infile("z.zip", ios::in | ios::binary);
    // file size: 470241
    if (infile.is_open()) {
        SoyZip z;
        cout<<"init: "<<z.init(&infile)<<endl;
        z.list_files();
        cout<<endl;
        z.decode_file(z.files_info[0]);
        cout<<endl;
        infile.close();
    }
#ifdef soytw
    system("pause");
#endif
    return 0;
}
