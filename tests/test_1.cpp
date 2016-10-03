/*
    Copyright (c) 2016 Martin Buck
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation the rights to
    use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
    and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
    DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
    OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#define CATCH_CONFIG_MAIN
#include "../dependencies/Catch.h" //https://github.com/philsquared/Catch

#include <cmath>
#include <vector>

#include "flaggedT.h"

using namespace std;
using namespace flaggedT;


//examples (move these to example files) [also the example test case]
//generator
NonNull<std::unique_ptr<int>> safe_generator() {
    return NonNull<std::unique_ptr<int>>::make_non_null(std::unique_ptr<int>(new int(5)));
}

int no_need_to_check_for_null(NonNull<std::unique_ptr<int>>&& in) {
    return *(in.get().get()) + 1;
}
//examples (move these to example files) [also the example test case]





TEST_CASE("FlaggedT") {
    SECTION("NonNull") {
        int* i = nullptr;
        REQUIRE_THROWS(NonNull<int*>::make_non_null(std::move(i)));

        int* i2 = new int(3);
        auto nn = NonNull<int*>::make_non_null(std::move(i2));
        auto i2s = NonNull<int*>::own(std::move(nn));
        delete i2s;
    }

    SECTION("Sorted") {
        std::vector<int> unsorted({1,7,8,14,3});

        REQUIRE(!std::is_sorted(unsorted.begin(), unsorted.end()));

        auto sorted = Sorted<std::vector<int>>(std::move(unsorted));
        REQUIRE(std::is_sorted(sorted.get().begin(), sorted.get().end()));
    }

    SECTION("UNIQUE") {
        std::vector<int> duped({1,1,2,2,3,3,7,9,11});

        auto unduped = Unique<std::vector<int>>(std::move(duped));
        REQUIRE(unduped.get().size() == 6);
    }

    SECTION("UNIQUESORTED") {
        std::vector<int> dupedUnsorted({11,9,7,3,3,2,2,1,1});

        auto fixed = UniqueAndSorted<std::vector<int>>(std::move(dupedUnsorted));
        REQUIRE(std::is_sorted(fixed.get().begin(), fixed.get().end()));
        REQUIRE(fixed.get().size() == 6);
    }

    SECTION("Move this to an example file") {
        auto tmp = safe_generator();
        auto res = no_need_to_check_for_null(std::move(tmp));
    }
}