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

#ifndef JSONWRITER_H_
#define JSONWRITER_H_

/*
 * Class to aid serialization of C++ classes to JSON.
 * The macro NVP expands to a JsonPair object, eg NVP(a) -> JsonPair("a", a)
 * allowing fields to be described succintly.
 */

#include <cassert>
#include <cstdio>
#include <iostream>
#include <string>

#include "dependencies/cereal/archives/json.hpp"
#include "dependencies/cereal/archives/binary.hpp"
#include "dependencies/cereal/types/vector.hpp"

#define NVP CEREAL_NVP

struct AnalysisState;

template <typename Archive>
struct CerealWriter : public Archive {
    CerealWriter(AnalysisState& state, std::ostream& stream) : Archive(stream), state(state) {
    }
    void check_magic(int num) {
        (*this)(num);
    }
    bool is_reading() { return false; }
    bool is_writing() { return true; }
    void raw(T& value) {
        raw((const char*)&value, sizeof(value));
    }
    void raw(const char* data, size_t n) {
        this->saveBinaryValue(data, n);
    }
    template <typename T>
    CerealWriter<Archive>& operator<<(T& value) {
        (*this)(value);
        return *this;
    }
    AnalysisState& state;
};

typedef CerealWriter<cereal::JSONOutputArchive> JsonWriter;
typedef CerealWriter<cereal::BinaryOutputArchive> BinaryWriter;

template <typename Archive>
struct CerealReader : public Archive {
    CerealReader(AnalysisState& state, std::istream& stream) 
        : Archive(stream), state(state) {
    }
    void check_magic(int check_num) {
        int num = 0;
        (*this)(num);
        assert(check_num == num);
    }
    bool is_reading() { return true; }
    bool is_writing() { return false; }
    template <typename T>
    void raw(T& value) {
        raw((const char*)&value, sizeof(value));
    }
    void raw(const char* data, size_t n) {
        loadBinaryValue(data, n);
    }
    template <typename T>
    CerealReader<Archive>& operator<<(T& value) {
        ar(value);
    }
    AnalysisState& state;
};

typedef CerealWriter<cereal::JSONInputArchive> JsonReader;
typedef CerealWriter<cereal::BinaryInputArchive> BinaryReader;

#endif
