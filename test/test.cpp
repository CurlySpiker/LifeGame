// Copyright (C) 1997-2024 Free Software Foundation, Inc.
//
// This file is part of the LifeGame program.  This program is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime program Exception, version
// 3.1, as published by the Free Software Foundation.

#include <catch2/catch_test_macros.hpp>

#include "board.h"

TEST_CASE("Parsing") {
    SECTION("Standard parsing") {
        life::Board board("../test/input_files/glider.txt");
    }

    SECTION("Empty parsing"){
        life::Board board("../test/input_files/empty.txt");
    }

    SECTION("File does not exist") {
        bool failed = false;
        try {
            life::Board board("../test/input_files/dummy.txt");
        } catch (std::exception& e) {
            failed = true;
        }

        REQUIRE(failed);
    }

    SECTION("Bad length") {
         bool failed = false;
        try {
            life::Board board("../test/input_files/bad_length.txt");
        } catch (std::logic_error e) {
            failed = true;
        }

        REQUIRE(failed);
    }

    SECTION("Bad character") {
         bool failed = false;
        try {
            life::Board board("../test/input_files/bad_char.txt");
        } catch (std::logic_error e) {
            failed = true;
        }

        REQUIRE(failed);
    }
}

TEST_CASE("Board expansion and shrinking") {
    life::Board board("../test/input_files/glider.txt");
    auto initial = board.toString();
    board.expand();
    REQUIRE(board.toString() == "_____\n___*_\n_*_*_\n__**_\n_____\n");
    board.shrink();
    REQUIRE(board.toString() == initial);
}

TEST_CASE("Count neighbours") {
    SECTION("Glider") {
        life::Board board("../test/input_files/glider.txt");
        REQUIRE(board.countAliveNeighbours(2, 0) == 1);
        REQUIRE(board.countAliveNeighbours(2,2) == 2);
        REQUIRE(board.countAliveNeighbours(1, 1) == 5);
        REQUIRE(board.countAliveNeighbours(3, 3) == 1);
        REQUIRE(board.countAliveNeighbours(5, 5) == 0);
    }

    SECTION("Outside coordinates") {
        life::Board board("../test/input_files/empty.txt");
        REQUIRE(board.countAliveNeighbours(3, 3) == 0);
    }

    SECTION("Empty") {
        life::Board board("../test/input_files/empty.txt");
        REQUIRE(board.countAliveNeighbours(0, 0) == 0);
    }
}

TEST_CASE("To string") {
    life::Board board("../test/input_files/glider.txt");
    REQUIRE(board.toString() == "__*\n*_*\n_**\n");
}

TEST_CASE("Iterate") {
    SECTION("Empty file") {
        life::Board board("../test/input_files/empty.txt");
        board.iterate(4);
        REQUIRE(board.toString() == "");
    }

    SECTION("Simple glider") {
        life::Board board("../test/input_files/glider.txt");
        board.iterate();
        REQUIRE(board.toString() == "*__\n_**\n**_\n");
    }

    SECTION("Glider periodicity check") {
        life::Board board("../test/input_files/glider.txt");
        std::string initial = board.toString();
        board.iterate(4, true);
        REQUIRE(board.toString() == initial);
    }
}

