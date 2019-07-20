// PKZIP version 2.0
#include <bits/stdc++.h>
#define FOR(i,end) for (int i=0; i<end; ++i)
#define FOR1(i,end) for (int i=1; i<=end; ++i)
#define DEBUGMSG(format, ...) printf(format, ##__VA_ARGS__); fflush(stdout)
using namespace std;

#define FILE_FORMAT_ERROR 66


typedef unsigned char byte;
struct Block {
    size_t offset, length;
    byte next;
    Block() {}
    Block(size_t offset, size_t length, byte next):
        offset(offset),
        length(length),
        next(next) {}
};

class MyZipEncoder {
public:
    static constexpr size_t MAX_BUFFER_SIZE = 1048576; // 1M
    static constexpr size_t MAX_FIELD_LEN = 65535;
    // 32KiB for search block and for string block, 64KiB in total

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
    int decode(istream& src, streampos begin, size_t size) {
        DEBUGMSG("[decode] begin: %llu size: %llu\n\n", (size_t)begin, size);
        src.seekg(size, src.cur);
        return 0;
    }
    int read_header(istream& input) {
        while (input.good()) {
            cout<<"tellg:"<<input.tellg()<<endl;

            input.read((char*)buffer, 4);
            signature = *(uint32_t*)(buffer);
            DEBUGMSG("sign: 0x%x\n\n", signature);
            if (signature == 0x04034b50) {
                // local file header
                input.read((char*)buffer+4, 26);
                uint16_t extract_version = *(uint16_t*)(buffer+4);
                uint16_t purpose = *(uint16_t*)(buffer+6);
                uint16_t method = *(uint16_t*)(buffer+8);
                uint16_t modtime = *(uint16_t*)(buffer+10);
                uint16_t moddate = *(uint16_t*)(buffer+12);
                uint32_t crc32 = *(uint32_t*)(buffer+14);
                uint32_t compressed_size = *(uint32_t*)(buffer+18);
                uint32_t uncompressed_size = *(uint32_t*)(buffer+22);
                uint16_t filename_len = *(uint16_t*)(buffer+26);
                uint16_t extrafield_len = *(uint16_t*)(buffer+28);

                input.read(filename, filename_len);
                filename[filename_len] = '\0';
                input.read(extrafield, extrafield_len);
                extrafield[extrafield_len] = '\0';

                if (purpose & 1) {
                    // [TODO] encrypted
                }
                if (purpose & 2) {
                    // [TODO] compression option
                }
                if (purpose & 4) {
                    // [TODO] compression option
                }
                if (purpose & 8) {
                    // data descriptor
                    input.read(buffer, 12);
                    crc32 = *(uint32_t*)(buffer);
                    compressed_size = *(uint32_t*)(buffer+4);
                    uncompressed_size = *(uint32_t*)(buffer+8);
                }

                struct tm moddt = dosdatetime(moddate, modtime);


                DEBUGMSG("file: %s\n", filename);
                DEBUGMSG("extract_version: %hu\n", extract_version);
                DEBUGMSG("purpose: %hx\n", purpose);
                DEBUGMSG("method: %hx\n", method);
                DEBUGMSG("modtime: %u:%u:%u\n", moddt.tm_hour, moddt.tm_min, moddt.tm_sec);
                DEBUGMSG("moddate: %u.%u.%u\n", 1900+moddt.tm_year, 1+moddt.tm_mon, moddt.tm_mday);
                DEBUGMSG("crc32: %x\n", crc32);
                DEBUGMSG("compressed size: %u\n", compressed_size);
                DEBUGMSG("uncompressed size: %u\n", uncompressed_size);
                DEBUGMSG("filename_len: %hu\n", filename_len);
                DEBUGMSG("extrafield_len: %hu\n", extrafield_len);
                DEBUGMSG("\n");

                // file data
                decode(input, input.tellg(), compressed_size);

            } else if (signature == 0x02014b50) {
                // central directory header
                input.read(buffer+4, 42);
                made_version = *(uint16_t*)(buffer+4);
                extract_version = *(uint16_t*)(buffer+6);
                purpose = *(uint16_t*)(buffer+8);
                method = *(uint16_t*)(buffer+10);
                modtime = *(uint16_t*)(buffer+12);
                moddate = *(uint16_t*)(buffer+14);
                crc32 = *(uint32_t*)(buffer+16);
                compressed_size = *(uint32_t*)(buffer+20);
                uncompressed_size = *(uint32_t*)(buffer+24);
                filename_len = *(uint16_t*)(buffer+28);
                extrafield_len = *(uint16_t*)(buffer+30);
                comment_len = *(uint16_t*)(buffer+32);
                disknum = *(uint16_t*)(buffer+34);
                inter_attr = *(uint16_t*)(buffer+36);
                exter_attr = *(uint32_t*)(buffer+38);
                offset = *(uint32_t*)(buffer+42);

                input.read(filename, filename_len);
                filename[filename_len] = '\0';
                input.read(extrafield, extrafield_len);
                extrafield[extrafield_len] = '\0';
                input.read(comment, comment_len);
                comment[comment_len] = '\0';

                DEBUGMSG("file: %s\n", filename);
                DEBUGMSG("made_version: %hu\n", made_version);
                DEBUGMSG("extract_version: %hu\n", extract_version);
                DEBUGMSG("purpose: %hx\n", purpose);
                DEBUGMSG("method: %hx\n", method);
                DEBUGMSG("modtime: %hx\n", modtime);
                DEBUGMSG("moddate: %hx\n", moddate);
                DEBUGMSG("crc32: %x\n", crc32);
                DEBUGMSG("compressed size: %u\n", compressed_size);
                DEBUGMSG("uncompressed size: %u\n", uncompressed_size);
                DEBUGMSG("filename_len: %hu\n", filename_len);
                DEBUGMSG("extrafield_len: %hu\n", extrafield_len);
                DEBUGMSG("comment_len: %hu\n", comment_len);
                DEBUGMSG("disknum: %hx\n", disknum);
                DEBUGMSG("inter_attr: %hx\n", inter_attr);
                DEBUGMSG("exter_attr: %x\n", exter_attr);
                DEBUGMSG("offset: %u\n", offset);
                DEBUGMSG("\n");
            } else if (signature == 0x05054b50) {
                // digital signature
                input.read(buffer+4, 2);
                uint16_t datasize = *(uint16_t*)(buffer+4);
                input.read(buffer, datasize);
                DEBUGMSG("digital sign: %.*s\n", datasize, buffer);
            } else if (signature == 0x06054b50) {
                // end of central directory record
                // input.read(buffer+4, );
                // disknum = *(uint16_t*)(buffer+4);
                // disknum_with_start = *(uint16_t*)(buffer+6);
                // entry_count = *(uint16_t*)(buffer+8);
            } else {
                DEBUGMSG("unknown signature: %x\n", signature);
            }
        }
        return 0;
    }
private:
    struct FileHeader {
        char filename[MAX_FIELD_LEN],
             extrafield[MAX_FIELD_LEN],
             comment[MAX_FIELD_LEN];
    };

    char buffer[MAX_BUFFER_SIZE];
    // TODO: prevent buffer overflow

    struct tm dosdatetime(uint16_t date, uint16_t time) {
        struct tm datetime;
        datetime.tm_mday = date & 0x1f;
        datetime.tm_mon = ((date & 0x1e0) >> 5) - 1;
        datetime.tm_year = ((date & 0xfe00) >> 9) + 80;
        datetime.tm_sec = time & 0x1f;
        datetime.tm_min = (time & 0x7e0) >> 5;
        datetime.tm_hour = (time & 0xf800) >> 11;
        return datetime;
    }
};

char str[10000];
int main() {
    fstream infile("a.zip", ios::in | ios::binary);
    // file size: 464389
    if (infile.is_open()) {
        MyZipEncoder z;
        int result = z.read_header(infile);
        cout<<result<<endl;
        infile.close();
    }
    // encode((byte*)str, 14, 9);
#ifdef soytw
    system("pause");
#endif
    return 0;
}
