#ifndef   WIRING_EX_FILEIO_H_
#define   WIRING_EX_FILEIO_H_

#include "wiring_ex_process.h"

#define FILE_READ 0
#define FILE_WRITE 1
#define FILE_APPEND 2

#define SP_FILES        "FL"
#define FL_OPEN         'O'
#define FL_CLOSE        'C'
#define FL_READ         'R'
#define FL_WRITE        'W'
#define FL_ISDIRECTORY  'D'
#define FL_SEEK         'S'
#define FL_TELL         'T'
#define FL_SIZE         'Z'

class File : public Stream
{
    public:
        File(BridgeClass &b = Bridge);
        File(const char *_filename, uint8_t _mode, BridgeClass &b = Bridge);
        ~File();

        virtual size_t write(uint8_t);
        virtual size_t write(const uint8_t *buf, size_t size);
        virtual int read(void);
        virtual int peek(void);
        virtual int available(void);
        virtual void flush(void);
        int read(void *buf, uint16_t nbyte);
        boolean seek(uint32_t pos);
        uint32_t position(void);
        uint32_t size(void);
        void close(void);
        operator bool(void);
        const char * name(void);
        boolean isDirectory(void);
        File openNextFile(uint8_t mode = FILE_READ);
        void rewindDirectory(void);

        //using Print::write;

    private:
        void doBuffer(void);
        uint8_t buffered;
        uint8_t readPos;
        uint16_t dirPosition;
        static const int BUFFER_SIZE = 64;
        uint8_t buffer[BUFFER_SIZE];


        private:
        uint8_t mode;
        BridgeClass &bridge;
        String filename;
        uint8_t handle;

};

class FileSystemClass
{
    public:
        FileSystemClass() : bridge(Bridge) { }
        FileSystemClass(BridgeClass &_b) : bridge(_b) { }

        boolean begin(void);

        // Open the specified file/directory with the supplied mode (e.g. read or
        // write, etc). Returns a File object for interacting with the file.
        // Note that currently only one file can be open at a time.
        File open(const char *filename, uint8_t mode = FILE_READ);

        // Methods to determine if the requested file path exists.
        boolean exists(const char *filepath);

        // Create the requested directory hierarchy--if intermediate directories
        // do not exist they will be created.
        boolean mkdir(const char *filepath);

        // Delete the file.
        boolean remove(const char *filepath);

        boolean rmdir(const char *filepath);

    private:
        friend class File;

        BridgeClass &bridge;
};

extern FileSystemClass FileSystem;

#endif /*FILEIO_H_*/

