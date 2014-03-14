#ifndef DATAREADWRITE_H_
#define DATAREADWRITE_H_

#include <cstdio>
#include <iostream>

#include "lcommon/SerializeBuffer.h"
#include "lcommon/smartptr.h"

struct AnalysisState;

/* Passed to objects visit() method during writing: */
struct DataWriter {
    DataWriter(AnalysisState& state, const char* file_name) 
        : state(state) {
            FILE* file = fopen(file_name, "wb"); // write-binary
            buffer.set(new SerializeBuffer(file, SerializeBuffer::OUTPUT, /* Close file: */true));
    }

    template <typename T>
    void visit(const T& obj) {
        printf("WRITING %d\n", *(int*)&obj);
        buffer->write(obj);
    }

    template <typename T>
    void visit(const std::vector<T>& obj) {
        buffer->write_container(obj);
    }

    template <typename T>
    DataWriter& operator<<(const T& obj) {
        visit(obj);
        return *this;
    }

    template <typename T>
    void write_container(T& obj) {
        buffer->write_container(obj);
    }

    bool is_reading() {
        return false;
    }

    bool is_writing() {
        return true;
    }

    AnalysisState& state;
private:
    smartptr<SerializeBuffer> buffer;
};

/* Passed to objects visit() method during reading: */
struct DataReader {
    DataReader(AnalysisState& state, const char* file_name) 
        : state(state) {
            FILE* file = fopen(file_name, "rb"); // read-binary
            buffer.set(new SerializeBuffer(file, SerializeBuffer::INPUT, /* Close file: */true));
    }

    template <typename T>
    DataReader& operator<<(T& obj) {
        visit(obj);
        return *this;
    }

    template <typename T>
    void visit(T& obj) {
        printf("READING %d\n", *(int*)&obj);
        buffer->read(obj);
    }

    template <typename T>
    void visit(std::vector<T>& obj) {
        buffer->read_container(obj);
    }

    template <typename T>
    void write_container(T& obj) {
        ASSERT(false, "Not intended for writing!");
    }

    template <typename T>
    void operator<<(const T& obj) {
        visit(obj);
    }

    bool is_reading() {
        return true;
    }

    bool is_writing() {
        return false;
    }

    AnalysisState& state;
private:
    smartptr<SerializeBuffer> buffer;
};

#define VISIT(rw, context) \
    template <typename Visitor, typename Context> \
    void visit(Visitor& rw, Context& context)

#define VISIT0(rw) \
    template <typename Visitor> \
    void visit(Visitor& rw)

#endif
