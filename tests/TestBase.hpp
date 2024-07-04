// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef TEST_BASE_HPP_DSTRUCT
#define TEST_BASE_HPP_DSTRUCT

namespace dstruct {

class BigFiveTest {
public:

    struct Obj {
        Obj() : moved { false } { get_test_data_e().mConstructor++; }

        Obj(const Obj &obj) {
            get_test_data_e().mCopyConstructor++;
            moved = obj.moved;
        }

        Obj & operator=(const Obj &obj) {
            if (this == &obj) get_test_data_e().mSelfAssignment++;

            if (this != &obj) {
                this->~Obj();
                get_test_data_e().mCopyAssignment++;
                moved = obj.moved;
            }

            return *this;
        }

        Obj(Obj &&obj) {
            get_test_data_e().mMoveConstructor++;
            moved = obj.moved;

            // set move flag
            obj.moved = true;
        }

        Obj & operator=(Obj &&obj) {
            if (this == &obj) get_test_data_e().mSelfAssignment++;

            if (this != &obj) {
                this->~Obj();
                get_test_data_e().mMoveAssignment++;
                moved = obj.moved;

                // set obj move flag
                obj.moved = true;
            }

            return *this;
        }

        ~Obj() {
            if (!moved) {
                get_test_data_e().mDestructor++;
            }
        }

        bool moved;
    };

public: // checker
    static int constructor() {
        return get_test_data_e().mConstructor;
    }

    static int destructor() {
        return get_test_data_e().mDestructor;
    }

    static int copy_constructor() {
        return get_test_data_e().mCopyConstructor;
    }

    static int copy_assignment() {
        return get_test_data_e().mCopyAssignment;
    }

    static int move_constructor() {
        return get_test_data_e().mMoveConstructor;
    }

    static int move_assignment() {
        return get_test_data_e().mMoveAssignment;
    }

    static int self_assignment() {
        return get_test_data_e().mSelfAssignment;
    }

public:

    static void clear_status() {
        get_test_data_e().mConstructor = 0;
        get_test_data_e().mDestructor = 0;
        get_test_data_e().mCopyConstructor = 0;
        get_test_data_e().mCopyAssignment = 0;
        get_test_data_e().mMoveConstructor = 0;
        get_test_data_e().mMoveAssignment = 0;
    }

private:
    struct TestData {
        int mConstructor = 0;
        int mDestructor = 0;
        int mCopyConstructor = 0;
        int mCopyAssignment = 0;
        int mMoveConstructor = 0;
        int mMoveAssignment = 0;
        int mSelfAssignment = 0;
    };

    static TestData & get_test_data_e() {
        static TestData data;
        return data;
    }
};

};

#endif