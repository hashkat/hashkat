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
#include <sstream>

#include "dependencies/cereal/archives/json.hpp"
#include "dependencies/cereal/archives/binary.hpp"
#include "dependencies/cereal/types/vector.hpp"
#include "dependencies/cereal/types/memory.hpp"

#define NVP CEREAL_NVP

struct AnalysisState;

template <typename Archive>
struct CerealAdapter : public Archive {
    template <typename Stream>
    CerealAdapter(AnalysisState& state, Stream& stream) : Archive(stream), state(state) {
    }
    AnalysisState& state;
};

typedef CerealAdapter<cereal::JSONOutputArchive> JsonWriter;
typedef CerealAdapter<cereal::BinaryOutputArchive> BinaryWriter;
typedef CerealAdapter<cereal::JSONInputArchive> JsonReader;
typedef CerealAdapter<cereal::BinaryInputArchive> BinaryReader;

template <typename Archive>
inline void check_magic(Archive& ar, int check_num) {
    int old_num = check_num;
    ar(NVP(check_num));
    assert(old_num == check_num);
}

template <typename Archive>
inline AnalysisState& get_state(Archive& ar) {
    return dynamic_cast<CerealAdapter<Archive>&>(ar).state;
}

template <typename Archive>
inline AnalysisState& get_state(CerealAdapter<Archive>& ar) {
    return ar.state;
}

template <typename T>
inline std::string to_string(AnalysisState& state, const T& data) {
    std::stringstream stream;
    {
        JsonWriter writer { state, stream };
        writer(NVP(data));
    }
    std::string result = stream.str();
    return result.substr(14, result.size() - 16); // Remove extra object and field specifier
}

#endif
