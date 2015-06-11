/*
 * This file is part of the #KAT Social Network Simulator.
 *
 * The #KAT Social Network Simulator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The #KAT Social Network Simulator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the #KAT Social Network Simulator.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Addendum:
 *
 * Under this license, derivations of the #KAT Social Network Simulator typically must be provided in source
 * form. The #KAT Social Network Simulator and derivations thereof may be relicensed by decision of 
 * the original authors (Kevin Ryczko & Adam Domurad, Isaac Tamblyn), as well, in the case of a derivation,
 * subsequent authors. 
 */

#ifndef DATAREADWRITE_H_
#define DATAREADWRITE_H_

#include <cstdio>
#include <iostream>

#include <map>

#include "lcommon/SerializeBuffer.h"
#include "lcommon/smartptr.h"

#include "util.h"

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
        buffer->write(obj);
    }

    template <typename T>
    void visit(const std::vector<T>& obj) {
        buffer->write_container(obj);
    }

    /**
     * 'check_visit' saves a value, to be checked up reload.
     * This should be some derived quantity, to be used as a sanity check.
     */
    template <typename T>
    void check_visit(const T& obj) {
        visit(obj);
    }

    template <typename T>
    void visit_smartptr(const smartptr<T>& obj) {
        if (obj.empty()) {
            *this << (void*)NULL << false;
            return;
        }
        void* raw_ptr = (void*)obj.get();
        bool added = false;
        if (ptr_map[raw_ptr].empty()) {
            added = true;
        }
        *this << raw_ptr << added;
//        printf("Placing %p add=%d\n", raw_ptr, added);
        if (added) {
            ptr_map[raw_ptr] = smartptr<void*>(obj);
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

    template <typename T>
    void visit_size(T& obj) {
        size_t size = obj.size();
        (*this) << size;
    }

    bool is_reading() {
        return false;
    }

    bool is_writing() {
        return true;
    }

    SerializeBuffer& get_buffer() {
		return *buffer;
	}

    void checkMagic(int num) {
    	buffer->write_int(num);
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
        buffer->read(obj);
    }

    /**
     * 'check_visit' saves a value, to be checked up reload.
     * This should be some derived quantity, to be used as a sanity check.
     */
    template <typename T>
    void check_visit(const T& obj) {
        T val;
        visit(val);
        ASSERT(val == obj, "Values do not match!");
    }

    template <typename T>
    void visit_smartptr(smartptr<T>& obj) {
        void* old_ptr = NULL;
        bool added = false;
        (*this) << old_ptr << added;
//        printf("Retrieving %p add=%d\n", old_ptr);
        if (added) {
//            printf("Recreating %p\n", old_ptr);
            T val;
            val.visit(*this);
            obj.set(new T(val));
            ptr_map[old_ptr] = smartptr<void*>(obj);
        } else if (old_ptr == NULL) {
            obj.clear();
        } else {
            obj = ptr_map[old_ptr];
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
    void visit_size(T& obj) {
        size_t size = -1;
        (*this) << size;
        obj.resize(size);
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

    void checkMagic(int num) {
    	ASSERT(buffer->read_int() == num, "Serialization magic check failed!");
    }

    AnalysisState& state;

    SerializeBuffer& get_buffer() {
    	return *buffer;
    }
private:
    std::map<void*, smartptr<void*>> ptr_map;
    smartptr<SerializeBuffer> buffer;
};

#define READ_WRITE(rw) \
    template <typename Visitor> \
    void visit(Visitor& rw)

#define PREREAD_WRITE(rw) \
    template <typename Visitor> \
    void previsit(Visitor& rw)


template <typename Visitor, typename Set>
inline void visit_set(Visitor& rw, Set& set) {
    if (rw.is_reading()) {
        size_t size = -1;
        rw << size;
        for (int i = 0 ; i < size; i++) {
            int used;
            rw << used;
            set.insert(used);
        }
    } else {
        // Writing:
        rw << set.size();
        for (int used : set) {
            rw << used;
        }
    }
}

#endif
