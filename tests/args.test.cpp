#include <catch.hpp>
#include "../include/kit/args/args.h"
using namespace std;

TEST_CASE("Args","[args]") {

    SECTION("empty") {
        Args args;
        REQUIRE(args.empty());
        REQUIRE(args.size() == 0);
    }
    
    SECTION("has") {
        // empty
        Args args;
        REQUIRE(not args.has("foobar"));
        
        // single arg
        args = Args(vector<string>{"foobar"});
        REQUIRE(args.has("foobar"));
        REQUIRE(not args.has("foo"));
        
        // multiple args
        args = Args(vector<string>{"foo", "bar"});
        REQUIRE(args.has("foo"));
        REQUIRE(args.has("bar"));
        REQUIRE(not args.has("baz"));

        // switches
        args = Args();
        REQUIRE(not args.has('v', "verbose"));
        args = Args(vector<string>{"--verbose"});
        REQUIRE(args.has('v', "verbose"));
        REQUIRE(not args.has('n', "nope"));
        args = Args(vector<string>{"-v"});
    }
}

