#include <fstream>
#include <iostream>
#include <sstream>
#include <map>

//#include "tests.h"

//#include "../analyzer.h"
#include "../serialization.h"

using namespace std;

//#define SUITE(x) int main()
//#define TEST(x) 
//
//struct AnalysisState {
//};
//struct TestJsonOut {
//    int foo, bar;
//    template <class Archive>
//    void serialize( Archive & ar ) {
//        ar(NVP(foo));
//        ar(NVP(bar));
//        check_magic(ar, 10);
//    }
//};
//SUITE(JsonWriter) {
//
//    TEST(JsonWriter){
//        AnalysisState state;
//        stringstream out;
//        {
//            JsonWriter writer {state, out};
//            TestJsonOut json_out {10};
//            json_out.serialize(writer);
//        }
//        cout << out.str();
//    }
//}
