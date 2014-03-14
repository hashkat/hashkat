#ifndef DATAREADWRITE_H_
#define DATAREADWRITE_H_

#include <cstdio>
#include <iostream>

#include <map>

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
    void visit(const smartptr<T>& obj) {
        void* raw_ptr = (void*)obj.get();
        bool added = false;
        if (ptr_map.find(raw_ptr) == ptr_map.end()) {
            added = true;
        }
        *this << raw_ptr << added;
        if (added) {
            ptr_map[raw_ptr] = (smartptr<void*>&)obj;
            obj->visit(*this);
        }
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

    template <typename T>
    void visit_container(T& obj) {
        buffer->write_container(obj);
    }

    template <typename T>
    void visit_objs(std::vector<T>& objs) {
        *this << objs.size();
        for (T& obj : objs) {
            obj.visit(*this);
        }
    }

    bool is_reading() {
        return false;
    }

    bool is_writing() {
        return true;
    }

    AnalysisState& state;
private:
    std::map<void*, smartptr<void*>> ptr_map;
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
    void visit(smartptr<T>& obj) {
        void* raw_ptr = NULL;
        bool added = false;
        (*this) << raw_ptr << added;
        if (added) {
            T data;
            data.visit(*this);
        }
        if (added) {
            T val;
            val->visit(this);
            obj.set(new T(val));
            ptr_map[raw_ptr] = (smartptr<void*>&)obj;
        } else {
            obj = ptr_map[raw_ptr];
        }
    }

    template <typename T>
    void visit(std::vector<T>& obj) {
        buffer->read_container(obj);
    }

    template <typename T>
    void visit_objs(std::vector<T>& objs) {
        size_t size;
        *this << size;
        objs.resize(size);
        for (T& obj : objs) {
            obj.visit(*this);
        }
    }

    template <typename T>
    void write_container(T& obj) {
        throw "Not intended for writing!";
    }

    template <typename T>
    void visit_container(T& obj) {
        buffer->read_container(obj);
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
    std::map<void*, smartptr<void*>> ptr_map;
    smartptr<SerializeBuffer> buffer;
};

#define VISIT(rw, context) \
    template <typename Visitor, typename Context> \
    void visit(Visitor& rw, Context& context)

#define VISIT0(rw) \
    template <typename Visitor> \
    void visit(Visitor& rw)

#define PREVISIT0(rw) \
    template <typename Visitor> \
    void previsit(Visitor& rw)


#endif
