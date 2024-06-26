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

#include <iostream>
#include <fstream>
#include <vector>

namespace life {

    class Board {
        public:

            //! Class constructor.
            /*!
            \param filepath Path to the input file.
            \param noLimit Removes board size limits if set to true
            \return The test results
            */
            Board(std::string filepath, bool noLimit=false);

            //! Adds a span of 1 on all board sides.
            /*!
            \return True if successful, false if failed (due to size limit reached)
            */
            bool expand();

            //! Shrinks the board to its minimal size.
            void shrink();

            //! Update the board following Life game rules. Recursive function.
            /*!
            \param nIter Number of iterations to execute.
            \param saveAll Generate an output file for each iteration
            \param currentIter Current iteration being processed, useful as this is a
                recursive function
            */
            void iterate(int nIter=1, bool saveAll=false, int currentIter=0);

            //! Get a string representation of the board
            std::string toString();

            //! Count how many alive adjacent cells there are to a given board position.
            /*!
            \param x Column of the position to analyze. Can be outside of board limits.
            \param y Row of the position to analyze. Can be outside of board limits.
            \return Number of alive neighbours
            */
            uint8_t countAliveNeighbours(size_t x, size_t y);

            //! Save the board to an output file
            /*!
            \param name Name of the output file. Output folder is set in this class
            member variables.
            \return True if save process was successful, false otherwise
            */
            bool save(std::string name);

        private:
            std::string mInputName; ///< Input name reused for output files names
            std::string mOutputDir; ///< Directory where output files will be saved

            bool mNoLimit; ///< Removes board size limits if true

            size_t mW = 0; ///< Width of the board
            size_t mH = 0; ///< Height of the board
            std::vector<std::vector<bool>> mCells; ///< Board array
    };
}